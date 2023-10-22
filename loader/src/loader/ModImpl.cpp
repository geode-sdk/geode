#include "ModImpl.hpp"
#include "LoaderImpl.hpp"
#include "ModMetadataImpl.hpp"
#include "about.hpp"

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
        CCFileUtils::get()->addSearchPath(searchPathRoot.string().c_str());
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

json::Value& Mod::Impl::getSaveContainer() {
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
            auto json = json::parse(settingData);

            JsonChecker checker(json);
            auto root = checker.root("[settings.json]");

            m_savedSettingsData = json;

            for (auto& [key, value] : root.items()) {
                // check if this is a known setting
                if (auto setting = this->getSetting(key)) {
                    // load its value
                    if (!setting->load(value.json())) {
                        log::internalLog(
                            Severity::Error,
                            m_self,
                            "{}: Unable to load value for setting \"{}\"",
                            m_metadata.getID(),
                            key
                        );
                    }
                }
                else {
                    log::internalLog(
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
            m_saved = json::parse(data);
        } catch (std::exception& err) {
            return Err(std::string("Unable to parse saved values: ") + err.what());
        }
        if (!m_saved.is_object()) {
            log::warn("saved.json was somehow not an object, forcing it to one");
            m_saved = json::Object();
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
    json::Value json = json::Object();
    for (auto& [key, value] : m_settings) {
        coveredSettings.insert(key);
        if (!value->save(json[key])) {
            log::error("Unable to save setting \"" + key + "\"");
        }
    }

    // if some settings weren't provided a custom settings handler (for example,
    // the mod was not loaded) then make sure to save their previous state in
    // order to not lose data
    try {
        log::debug("Check covered");
        for (auto& [key, value] : m_savedSettingsData.as_object()) {
            log::debug("Check if {} is saved", key);
            if (!coveredSettings.count(key)) {
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

Result<> Mod::Impl::uninstall() {
    if (m_requestedAction != ModRequestedAction::None) {
        return Err("Mod already has a requested action");
    }

    m_requestedAction = ModRequestedAction::Uninstall;

    std::error_code ec;
    ghc::filesystem::remove(m_metadata.getPath(), ec);
    if (ec) {
        return Err(
            "Unable to delete mod's .geode file: " + ec.message()
        );
    }

    return Ok();
}

bool Mod::Impl::isUninstalled() const {
    return m_requestedAction == ModRequestedAction::Uninstall;
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

// msvc stop fucking screaming please i BEG YOU
#pragma warning(suppress : 4996)
std::vector<Dependency> Mod::Impl::getUnresolvedDependencies() {
#pragma warning(suppress : 4996)
    std::vector<Dependency> unresolved;
    for (auto const& dep : m_metadata.getDependencies()) {
        if (!dep.isResolved()) {
#pragma warning(suppress : 4996)
            unresolved.push_back(dep);
        }
    }
    return unresolved;
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
    m_hooks.push_back(hook);
    if (LoaderImpl::get()->isReadyToHook()) {
        if (this->isEnabled() && hook->getAutoEnable()) {
            auto res = this->enableHook(hook);
            if (!res) {
                delete hook;
                return Err("Can't create hook: "+ res.unwrapErr());
            }
        }
    }
    else {
        LoaderImpl::get()->addInternalHook(hook, m_self);
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

    // Unzip .geode file into temp dir
    GEODE_UNWRAP_INTO(auto unzip, file::Unzip::create(m_metadata.getPath()));
    if (!unzip.hasEntry(m_metadata.getBinaryName())) {
        return Err(
            fmt::format("Unable to find platform binary under the name \"{}\"", m_metadata.getBinaryName())
        );
    }
    GEODE_UNWRAP(unzip.extractAllTo(tempPath));

    // Mark temp dir creation as succesful
    m_tempDirName = tempPath;

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

    auto obj = json::Object();
    obj["hooks"] = json::Array();
    for (auto hook : m_hooks) {
        obj["hooks"].as_array().push_back(ModJson(hook->getRuntimeInfo()));
    }
    obj["patches"] = json::Array();
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

static Result<ModMetadata> getModImplInfo() {
    std::string err;
    json::Value json;
    try {
        json = json::parse(LOADER_MOD_JSON);
    } catch (std::exception& err) {
        return Err("Unable to parse mod.json: " + std::string(err.what()));
    }

    GEODE_UNWRAP_INTO(auto info, ModMetadata::create(json));
    return Ok(info);
}

Mod* Loader::Impl::createInternalMod() {
    auto& mod = Mod::sharedMod<>;
    if (mod) return mod;
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
    return Mod::get()->m_impl->setup();
}
