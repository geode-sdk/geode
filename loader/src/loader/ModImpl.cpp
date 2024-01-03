#include "ModImpl.hpp"
#include "LoaderImpl.hpp"
#include "ModMetadataImpl.hpp"
#include "about.hpp"

#include <hash/hash.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/loader/Hook.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/ModEvent.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <optional>
#include <string>
#include <vector>

using namespace geode::prelude;

Mod::Impl* ModImpl::get() {
    return Mod::get()->m_impl.get();
}

Mod::Impl* ModImpl::getImpl(Mod* mod)  {
    return mod->m_impl.get();
}

Mod::Impl::Impl(Mod* self, ModMetadata const& metadata) : m_self(self), m_metadata(metadata) {
}

Mod::Impl::~Impl() = default;

Result<> Mod::Impl::setup() {
    m_saveDirPath = dirs::getModsSaveDir() / m_metadata.getID();
    (void) utils::file::createDirectoryAll(m_saveDirPath);

    // always create temp dir for all mods, even if disabled, so resources can be loaded
    GEODE_UNWRAP(this->createTempDir().expect("Unable to create temp dir: {error}"));

    this->setupSettings();
    auto loadRes = this->loadData();
    if (!loadRes) {
        log::warn("Unable to load data for \"{}\": {}", m_metadata.getID(), loadRes.unwrapErr());
    }
    if (!m_resourcesLoaded) {
        auto searchPathRoot = dirs::getModRuntimeDir() / m_metadata.getID() / "resources";
        // CCFileUtils::get()->addSearchPath(searchPathRoot.string().c_str());

        m_resourcesLoaded = true;
    }

    return Ok();
}

// Getters

ghc::filesystem::path Mod::Impl::getSaveDir() const {
    return m_saveDirPath;
}

std::string Mod::Impl::getID() const {
    return m_metadata.getID();
}

std::string Mod::Impl::getName() const {
    return m_metadata.getName();
}

std::string Mod::Impl::getDeveloper() const {
    return m_metadata.getDeveloper();
}

std::optional<std::string> Mod::Impl::getDescription() const {
    return m_metadata.getDescription();
}

std::optional<std::string> Mod::Impl::getDetails() const {
    return m_metadata.getDetails();
}

ModMetadata Mod::Impl::getMetadata() const {
    return m_metadata;
}

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE)
void Mod::Impl::setMetadata(ModMetadata const& metadata) {
    m_metadata = metadata;
}
std::vector<Mod*> Mod::Impl::getDependants() const {
    return m_dependants;
}
#endif

ghc::filesystem::path Mod::Impl::getTempDir() const {
    return m_tempDirName;
}

ghc::filesystem::path Mod::Impl::getBinaryPath() const {
    return m_tempDirName / m_metadata.getBinaryName();
}

ghc::filesystem::path Mod::Impl::getPackagePath() const {
    return m_metadata.getPath();
}

VersionInfo Mod::Impl::getVersion() const {
    return m_metadata.getVersion();
}

matjson::Value& Mod::Impl::getSaveContainer() {
    return m_saved;
}

bool Mod::Impl::isEnabled() const {
    return m_enabled;
}

bool Mod::Impl::supportsDisabling() const {
    return m_metadata.getID() != "geode.loader";
}

bool Mod::Impl::needsEarlyLoad() const {
    auto deps = m_dependants;
    return getMetadata().needsEarlyLoad() ||
        !deps.empty() && std::any_of(deps.begin(), deps.end(), [&](auto& item) {
             return item->needsEarlyLoad();
         });
}

std::vector<Hook*> Mod::Impl::getHooks() const {
    return m_hooks;
}

// Settings and saved values

Result<> Mod::Impl::loadData() {
    Loader::get()->queueInMainThread([&]() {
        ModStateEvent(m_self, ModEventType::DataLoaded).post();
    });

    // Settings
    // Check if settings exist
    auto settingPath = m_saveDirPath / "settings.json";
    if (ghc::filesystem::exists(settingPath)) {
        GEODE_UNWRAP_INTO(auto settingData, utils::file::readString(settingPath));
        try {
            // parse settings.json
            auto json = matjson::parse(settingData);

            JsonChecker checker(json);
            auto root = checker.root("[settings.json]");

            m_savedSettingsData = json;

            for (auto& [key, value] : root.items()) {
                // check if this is a known setting
                if (auto setting = this->getSetting(key)) {
                    // load its value
                    if (!setting->load(value.json())) {
                        log::logImpl(
                            Severity::Error,
                            m_self,
                            "{}: Unable to load value for setting \"{}\"",
                            m_metadata.getID(),
                            key
                        );
                    }
                }
                else {
                    log::logImpl(
                        Severity::Warning,
                        m_self,
                        "Encountered unknown setting \"{}\" while loading "
                        "settings",
                        key
                    );
                }
            }
        }
        catch (std::exception& e) {
            return Err(std::string("Unable to parse settings: ") + e.what());
        }
    }

    // Saved values
    auto savedPath = m_saveDirPath / "saved.json";
    if (ghc::filesystem::exists(savedPath)) {
        GEODE_UNWRAP_INTO(auto data, utils::file::readString(savedPath));

        try {
            m_saved = matjson::parse(data);
        } catch (std::exception& err) {
            return Err(std::string("Unable to parse saved values: ") + err.what());
        }
        if (!m_saved.is_object()) {
            log::warn("saved.json was somehow not an object, forcing it to one");
            m_saved = matjson::Object();
        }
    }

    return Ok();
}

Result<> Mod::Impl::saveData() {
    // saveData is expected to be synchronous, and always called from GD thread
    ModStateEvent(m_self, ModEventType::DataSaved).post();

    // Data saving should be fully fail-safe

    std::unordered_set<std::string> coveredSettings;

    // Settings
    matjson::Value json = matjson::Object();
    for (auto& [key, value] : m_settings) {
        coveredSettings.insert(key);
        if (!value->save(json[key])) {
            log::error("Unable to save setting \"{}\"", key);
        }
    }

    // if some settings weren't provided a custom settings handler (for example,
    // the mod was not loaded) then make sure to save their previous state in
    // order to not lose data
    try {
        log::debug("Check covered");
        for (auto& [key, value] : m_savedSettingsData.as_object()) {
            log::debug("Check if {} is saved", key);
            if (!coveredSettings.contains(key)) {
                json[key] = value;
            }
        }
    }
    catch (...) {
    }

    std::string settingsStr = json.dump();
    std::string savedStr = m_saved.dump();

    auto res = utils::file::writeString(m_saveDirPath / "settings.json", settingsStr);
    if (!res) {
        log::error("Unable to save settings: {}", res.unwrapErr());
    }

    auto res2 = utils::file::writeString(m_saveDirPath / "saved.json", savedStr);
    if (!res2) {
        log::error("Unable to save values: {}", res2.unwrapErr());
    }

    return Ok();
}

void Mod::Impl::setupSettings() {
    for (auto& [key, sett] : m_metadata.getSettings()) {
        if (auto value = sett.createDefaultValue()) {
            m_settings.emplace(key, std::move(value));
        }
    }
}

void Mod::Impl::registerCustomSetting(std::string const& key, std::unique_ptr<SettingValue> value) {
    if (!m_settings.count(key)) {
        // load data
        if (m_savedSettingsData.contains(key)) {
            value->load(m_savedSettingsData[key]);
        }
        m_settings.emplace(key, std::move(value));
    }
}

bool Mod::Impl::hasSettings() const {
    return m_metadata.getSettings().size();
}

std::vector<std::string> Mod::Impl::getSettingKeys() const {
    std::vector<std::string> keys;
    for (auto& [key, _] : m_metadata.getSettings()) {
        keys.push_back(key);
    }
    return keys;
}

std::optional<Setting> Mod::Impl::getSettingDefinition(std::string const& key) const {
    for (auto& setting : m_metadata.getSettings()) {
        if (setting.first == key) {
            return setting.second;
        }
    }
    return std::nullopt;
}

SettingValue* Mod::Impl::getSetting(std::string const& key) const {
    if (m_settings.count(key)) {
        return m_settings.at(key).get();
    }
    return nullptr;
}

bool Mod::Impl::hasSetting(std::string const& key) const {
    for (auto& setting : m_metadata.getSettings()) {
        if (setting.first == key) {
            return true;
        }
    }
    return false;
}

// Loading, Toggling, Installing

Result<> Mod::Impl::loadBinary() {
    log::debug("Loading binary for mod {}", m_metadata.getID());
    if (m_enabled)
        return Ok();

    LoaderImpl::get()->provideNextMod(m_self);

    auto res = this->loadPlatformBinary();
    if (!res) {
        // make sure to free up the next mod mutex
        LoaderImpl::get()->releaseNextMod();
        log::error("Failed to load binary for mod {}: {}", m_metadata.getID(), res.unwrapErr());
        return res;
    }

    LoaderImpl::get()->releaseNextMod();

    for (auto const& hook : m_hooks) {
        if (!hook) {
            log::warn("Hook is null in mod \"{}\"", m_metadata.getName());
            continue;
        }
        if (hook->getAutoEnable()) {
            auto res = this->enableHook(hook);
            if (!res) {
                log::error("Can't enable hook {} for mod {}: {}", hook->getDisplayName(), m_metadata.getID(), res.unwrapErr());
            }
        }
    }

    for (auto const& patch : m_patches) {
        if (!patch) {
            log::warn("Patch is null in mod \"{}\"", m_metadata.getName());
            continue;
        }
        if (patch->getAutoEnable()) {
            if (!patch->apply()) {
                log::warn("Unable to apply patch at {}", patch->getAddress());
                continue;
            }
        }
    }

    m_enabled = true;

    ModStateEvent(m_self, ModEventType::Loaded).post();
    ModStateEvent(m_self, ModEventType::Enabled).post();

    return Ok();
}

Result<> Mod::Impl::enable() {
    if (m_requestedAction != ModRequestedAction::None) {
        return Err("Mod already has a requested action");
    }

    m_requestedAction = ModRequestedAction::Enable;
    Mod::get()->setSavedValue("should-load-" + m_metadata.getID(), true);

    return Ok();
}

Result<> Mod::Impl::disable() {
    if (m_requestedAction != ModRequestedAction::None) {
        return Err("Mod already has a requested action");
    }

    m_requestedAction = ModRequestedAction::Disable;
    Mod::get()->setSavedValue("should-load-" + m_metadata.getID(), false);

    return Ok();
}

Result<> Mod::Impl::uninstall(bool deleteSaveData) {
    if (m_requestedAction != ModRequestedAction::None) {
        return Err("Mod already has a requested action");
    }

    if (this->getID() == "geode.loader") {
        utils::game::launchLoaderUninstaller(deleteSaveData);
        utils::game::exit();
        return Ok();
    }

    m_requestedAction = deleteSaveData ?
        ModRequestedAction::UninstallWithSaveData :
        ModRequestedAction::Uninstall;

    std::error_code ec;
    ghc::filesystem::remove(m_metadata.getPath(), ec);
    if (ec) {
        return Err(
            "Unable to delete mod's .geode file: " + ec.message()
        );
    }

    if (deleteSaveData) {
        ghc::filesystem::remove_all(this->getSaveDir(), ec);
        if (ec) {
            return Err(
                "Unable to delete mod's save directory: " + ec.message()
            );
        }
    }

    return Ok();
}

bool Mod::Impl::isUninstalled() const {
    return m_requestedAction == ModRequestedAction::Uninstall ||
        m_requestedAction == ModRequestedAction::UninstallWithSaveData;
}

ModRequestedAction Mod::Impl::getRequestedAction() const {
    return m_requestedAction;
}

// Dependencies

Result<> Mod::Impl::updateDependencies() {
    return Err("Mod::updateDependencies is no longer needed, "
        "as this is handled by Loader::refreshModGraph");
}

bool Mod::Impl::hasUnresolvedDependencies() const {
    for (auto const& dep : m_metadata.getDependencies()) {
        if (!dep.isResolved()) {
            return true;
        }
    }
    return false;
}

bool Mod::Impl::hasUnresolvedIncompatibilities() const {
    for (auto const& dep : m_metadata.getIncompatibilities()) {
        if (!dep.isResolved()) {
            return true;
        }
    }
    return false;
}

bool Mod::Impl::depends(std::string const& id) const {
    return utils::ranges::contains(m_metadata.getDependencies(), [id](ModMetadata::Dependency const& t) {
        return t.id == id;
    });
}

// Hooks

Result<> Mod::Impl::enableHook(Hook* hook) {
    auto res = hook->enable();
    if (!res) {
        log::error("Can't enable hook {} for mod {}: {}", hook->getDisplayName(), m_metadata.getID(), res.unwrapErr());
    }

    return res;
}

Result<> Mod::Impl::disableHook(Hook* hook) {
    return hook->disable();
}

Result<Hook*> Mod::Impl::addHook(Hook* hook) {
    if (!ranges::contains(m_hooks, [&](auto const& h) { return h == hook; }))
        m_hooks.push_back(hook);

    if (!LoaderImpl::get()->isReadyToHook()) {
        LoaderImpl::get()->addUninitializedHook(hook, m_self);
        return Ok(hook);
    }

    if (!this->isEnabled() || !hook->getAutoEnable())
        return Ok(hook);

    auto res = this->enableHook(hook);
    if (!res) {
        delete hook;
        return Err("Can't create hook: " + res.unwrapErr());
    }

    return Ok(hook);
}

Result<> Mod::Impl::removeHook(Hook* hook) {
    auto res = this->disableHook(hook);
    if (res) {
        ranges::remove(m_hooks, hook);
        delete hook;
    }
    return res;
}

// Patches

// TODO: replace this with a safe one
static ByteVector readMemory(void* address, size_t amount) {
    ByteVector ret;
    for (size_t i = 0; i < amount; i++) {
        ret.push_back(*reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(address) + i));
    }
    return ret;
}

Result<Patch*> Mod::Impl::patch(void* address, ByteVector const& data) {
    auto p = new Patch;
    p->m_address = address;
    p->m_original = readMemory(address, data.size());
    p->m_owner = m_self;
    p->m_patch = data;
    if (this->isEnabled() && !p->apply()) {
        delete p;
        return Err("Unable to enable patch at " + std::to_string(reinterpret_cast<uintptr_t>(address)));
    }
    m_patches.push_back(p);
    return Ok(p);
}

Result<> Mod::Impl::unpatch(Patch* patch) {
    if (!patch->restore())
        return Err("Unable to restore patch at " + std::to_string(patch->getAddress()));
    ranges::remove(m_patches, patch);
    delete patch;
    return Ok();
}

// Misc.

Result<> Mod::Impl::createTempDir() {
    // Check if temp dir already exists
    if (!m_tempDirName.string().empty()) {
        return Ok();
    }

    // If the info doesn't specify a path, don't do anything
    if (m_metadata.getPath().string().empty()) {
        return Ok();
    }

    // Create geode/temp
    auto tempDir = dirs::getModRuntimeDir();
    if (!file::createDirectoryAll(tempDir)) {
        return Err("Unable to create mods' runtime directory");
    }

    // Create geode/temp/mod.id
    auto tempPath = tempDir / m_metadata.getID();
    if (!file::createDirectoryAll(tempPath)) {
        return Err("Unable to create mod runtime directory");
    }

    // Mark temp dir creation as succesful
    m_tempDirName = tempPath;

    return Ok();
}

Result<> Mod::Impl::unzipGeodeFile(ModMetadata metadata) {
    // Unzip .geode file into temp dir
    auto tempDir = dirs::getModRuntimeDir() / metadata.getID();

    auto datePath = tempDir / "modified-at";
    std::string currentHash = file::readString(datePath).unwrapOr("");

    auto modifiedDate = ghc::filesystem::last_write_time(metadata.getPath());
    auto modifiedCount = std::chrono::duration_cast<std::chrono::milliseconds>(modifiedDate.time_since_epoch());
    auto modifiedHash = std::to_string(modifiedCount.count());
    if (currentHash == modifiedHash) {
        log::debug("Same hash detected, skipping unzip");
        return Ok();
    }
    log::debug("Hash mismatch detected, unzipping");

    std::error_code ec;
    ghc::filesystem::remove_all(tempDir, ec);
    if (ec) {
        return Err("Unable to delete temp dir: " + ec.message());
    }
    
    (void)utils::file::createDirectoryAll(tempDir);
    auto res = file::writeString(datePath, modifiedHash);
    if (!res) {
        log::warn("Failed to write modified date of geode zip: {}", res.unwrapErr());
    }
    

    GEODE_UNWRAP_INTO(auto unzip, file::Unzip::create(metadata.getPath()));
    if (!unzip.hasEntry(metadata.getBinaryName())) {
        return Err(
            fmt::format("Unable to find platform binary under the name \"{}\"", metadata.getBinaryName())
        );
    }
    GEODE_UNWRAP(unzip.extractAllTo(tempDir));

    return Ok();
}

ghc::filesystem::path Mod::Impl::getConfigDir(bool create) const {
    auto dir = dirs::getModConfigDir() / m_metadata.getID();
    if (create) {
        (void)file::createDirectoryAll(dir);
    }
    return dir;
}

char const* Mod::Impl::expandSpriteName(char const* name) {
    if (m_expandedSprites.count(name)) return m_expandedSprites[name];

    auto exp = new char[strlen(name) + 2 + m_metadata.getID().size()];
    auto exps = m_metadata.getID() + "/" + name;
    memcpy(exp, exps.c_str(), exps.size() + 1);

    m_expandedSprites[name] = exp;

    return exp;
}

ModJson Mod::Impl::getRuntimeInfo() const {
    auto json = m_metadata.toJSON();

    auto obj = matjson::Object();
    obj["hooks"] = matjson::Array();
    for (auto hook : m_hooks) {
        obj["hooks"].as_array().push_back(ModJson(hook->getRuntimeInfo()));
    }
    obj["patches"] = matjson::Array();
    for (auto patch : m_patches) {
        obj["patches"].as_array().push_back(ModJson(patch->getRuntimeInfo()));
    }
    // TODO: so which one is it
    // obj["enabled"] = m_enabled;
    obj["loaded"] = m_enabled;
    obj["temp-dir"] = this->getTempDir();
    obj["save-dir"] = this->getSaveDir();
    obj["config-dir"] = this->getConfigDir(false);
    json["runtime"] = obj;

    return json;
}

bool Mod::Impl::isLoggingEnabled() const {
    return m_loggingEnabled;
}

void Mod::Impl::setLoggingEnabled(bool enabled) {
    m_loggingEnabled = enabled;
}

bool Mod::Impl::shouldLoad() const {
    return Mod::get()->getSavedValue<bool>("should-load-" + m_metadata.getID(), true);
}

static Result<ModMetadata> getModImplInfo() {
    std::string err;
    matjson::Value json;
    try {
        json = matjson::parse(LOADER_MOD_JSON);
    } catch (std::exception& err) {
        return Err("Unable to parse mod.json: " + std::string(err.what()));
    }

    GEODE_UNWRAP_INTO(auto info, ModMetadata::create(json));
    return Ok(info);
}

Mod* Loader::Impl::getInternalMod() {
    auto& mod = Mod::sharedMod<>;
    if (mod)
        return mod;
    if (m_mods.contains("geode.loader")) {
        log::warn("Something went wrong and Mod::sharedMod<> got unset after the internal mod was created! Setting sharedMod back...");
        mod = m_mods["geode.loader"];
        return mod;
    }
    auto infoRes = getModImplInfo();
    if (!infoRes) {
        LoaderImpl::get()->platformMessageBox(
            "Fatal Internal Error",
            "Unable to create internal mod info: \"" + infoRes.unwrapErr() +
                "\"\n"
                "This is a fatal internal error in the loader, please "
                "contact Geode developers immediately!"
        );
        mod = new Mod(ModMetadata("geode.loader"));
    }
    else {
        mod = new Mod(infoRes.unwrap());
    }
    mod->m_impl->m_enabled = true;
    m_mods.insert({ mod->getID(), mod });
    return mod;
}

Result<> Loader::Impl::setupInternalMod() {
    GEODE_UNWRAP(Mod::get()->m_impl->setup());
    auto resourcesDir = dirs::getGeodeResourcesDir() / Mod::get()->getID();
    GEODE_UNWRAP(ModMetadataImpl::getImpl(ModImpl::get()->m_metadata).addSpecialFiles(resourcesDir));
    return Ok();
}
