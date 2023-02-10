#include "ModImpl.hpp"
#include "LoaderImpl.hpp"
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

USE_GEODE_NAMESPACE();

Mod::Impl* ModImpl::getImpl(Mod* mod)  {
    return mod->m_impl.get();
}

Mod::Impl::Impl(Mod* self, ModInfo const& info) : m_self(self), m_info(info) {
}

Mod::Impl::~Impl() {
    (void)this->unloadBinary();
}

Result<> Mod::Impl::setup() {
    m_saveDirPath = dirs::getModsSaveDir() / m_info.id();
    ghc::filesystem::create_directories(m_saveDirPath);
    
    this->setupSettings();
    auto loadRes = this->loadData();
    if (!loadRes) {
        log::warn("Unable to load data for \"{}\": {}", m_info.id(), loadRes.unwrapErr());
    }
    return Ok();
}

// Getters

ghc::filesystem::path Mod::Impl::getSaveDir() const {
    return m_saveDirPath;
}

std::string Mod::Impl::getID() const {
    return m_info.id();
}

std::string Mod::Impl::getName() const {
    return m_info.name();
}

std::string Mod::Impl::getDeveloper() const {
    return m_info.developer();
}

std::optional<std::string> Mod::Impl::getDescription() const {
    return m_info.description();
}

std::optional<std::string> Mod::Impl::getDetails() const {
    return m_info.details();
}

ModInfo Mod::Impl::getModInfo() const {
    return m_info;
}

ghc::filesystem::path Mod::Impl::getTempDir() const {
    return m_tempDirName;
}

ghc::filesystem::path Mod::Impl::getBinaryPath() const {
    return m_tempDirName / m_info.binaryName();
}

ghc::filesystem::path Mod::Impl::getPackagePath() const {
    return m_info.path();
}

VersionInfo Mod::Impl::getVersion() const {
    return m_info.version();
}

json::Value& Mod::Impl::getSaveContainer() {
    return m_saved;
}

bool Mod::Impl::isEnabled() const {
    return m_enabled;
}

bool Mod::Impl::isLoaded() const {
    return m_binaryLoaded;
}

bool Mod::Impl::supportsDisabling() const {
    return m_info.supportsDisabling();
}

bool Mod::Impl::supportsUnloading() const {
    return m_info.supportsUnloading();
}

bool Mod::Impl::wasSuccesfullyLoaded() const {
    return !this->isEnabled() || this->isLoaded();
}

std::vector<Hook*> Mod::Impl::getHooks() const {
    return m_hooks;
}

// Settings and saved values

Result<> Mod::Impl::loadData() {
    ModStateEvent(m_self, ModEventType::DataLoaded).post();

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
                            m_info.id(),
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
    for (auto& [key, sett] : m_info.settings()) {
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
    return m_info.settings().size();
}

std::vector<std::string> Mod::Impl::getSettingKeys() const {
    std::vector<std::string> keys;
    for (auto& [key, _] : m_info.settings()) {
        keys.push_back(key);
    }
    return keys;
}

std::optional<Setting> Mod::Impl::getSettingDefinition(std::string const& key) const {
    for (auto& setting : m_info.settings()) {
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
    for (auto& setting : m_info.settings()) {
        if (setting.first == key) {
            return true;
        }
    }
    return false;
}

// Loading, Toggling, Installing

Result<> Mod::Impl::loadBinary() {
    if (m_binaryLoaded) {
        return Ok();
    }

    GEODE_UNWRAP(this->createTempDir());

    if (this->hasUnresolvedDependencies()) {
        return Err("Mod has unresolved dependencies");
    }

    LoaderImpl::get()->provideNextMod(m_self);

    auto res = this->loadPlatformBinary();
    if (!res) {
        // make sure to free up the next mod mutex
        LoaderImpl::get()->releaseNextMod();
        return res;
    }
    m_binaryLoaded = true;

    LoaderImpl::get()->releaseNextMod();

    ModStateEvent(m_self, ModEventType::Loaded).post();

    Loader::get()->updateAllDependencies();

    GEODE_UNWRAP(this->enable());

    return Ok();
}

Result<> Mod::Impl::unloadBinary() {
    if (!m_binaryLoaded) {
        return Ok();
    }

    if (!m_info.supportsUnloading()) {
        return Err("Mod does not support unloading");
    }

    GEODE_UNWRAP(this->saveData());

    GEODE_UNWRAP(this->disable());
    ModStateEvent(m_self, ModEventType::Unloaded).post();

    // Disabling unhooks and unpatches already
    for (auto const& hook : m_hooks) {
        delete hook;
    }
    m_hooks.clear();

    for (auto const& patch : m_patches) {
        delete patch;
    }
    m_patches.clear();

    GEODE_UNWRAP(this->unloadPlatformBinary());
    m_binaryLoaded = false;

    Loader::get()->updateAllDependencies();

    return Ok();
}

Result<> Mod::Impl::enable() {
    if (!m_binaryLoaded) {
        return this->loadBinary();
    }

    for (auto const& hook : m_hooks) {
        GEODE_UNWRAP(this->enableHook(hook));
    }

    for (auto const& patch : m_patches) {
        if (!patch->apply()) {
            return Err("Unable to apply patch at " + std::to_string(patch->getAddress()));
        }
    }

    ModStateEvent(m_self, ModEventType::Enabled).post();
    m_enabled = true;

    return Ok();
}

Result<> Mod::Impl::disable() {
    if (!m_enabled) {
        return Ok();
    }
    if (!m_info.supportsDisabling()) {
        return Err("Mod does not support disabling");
    }

    ModStateEvent(m_self, ModEventType::Disabled).post();

    for (auto const& hook : m_hooks) {
        GEODE_UNWRAP(this->disableHook(hook));
    }
    for (auto const& patch : m_patches) {
        if (!patch->restore()) {
            return Err("Unable to restore patch at " + std::to_string(patch->getAddress()));
        }
    }

    m_enabled = false;

    return Ok();
}

Result<> Mod::Impl::uninstall() {
    if (m_info.supportsDisabling()) {
        GEODE_UNWRAP(this->disable());
        if (m_info.supportsUnloading()) {
            GEODE_UNWRAP(this->unloadBinary());
        }
    }

    try {
        ghc::filesystem::remove(m_info.path());
    }
    catch (std::exception& e) {
        return Err(
            "Unable to delete mod's .geode file! "
            "This might be due to insufficient permissions - "
            "try running GD as administrator."
        );
    }

    return Ok();
}

bool Mod::Impl::isUninstalled() const {
    return m_self != Mod::get() && !ghc::filesystem::exists(m_info.path());
}

// Dependencies

Result<> Mod::Impl::updateDependencies() {
    bool hasUnresolved = false;
    for (auto& dep : m_info.dependencies()) {
        // set the dependency's loaded mod if such exists
        if (!dep.mod) {
            dep.mod = Loader::get()->getLoadedMod(dep.id);
            // verify loaded dependency version
            if (dep.mod && !dep.version.compare(dep.mod->getVersion())) {
                dep.mod = nullptr;
            }
        }

        // check if the dependency is loaded
        if (dep.mod) {
            // update the dependency recursively
            GEODE_UNWRAP(dep.mod->updateDependencies());

            // enable mod if it's resolved & enabled
            if (!dep.mod->hasUnresolvedDependencies()) {
                if (dep.mod->isEnabled()) {
                    GEODE_UNWRAP(dep.mod->loadBinary().expect("Unable to load dependency: {error}"));
                }
            }
        }
        // check if the dependency is resolved now
        if (!dep.isResolved()) {
            GEODE_UNWRAP(this->unloadBinary().expect("Unable to unload mod: {error}"));
            hasUnresolved = true;
        }
    }
    // load if there weren't any unresolved dependencies
    if (!hasUnresolved) {
        log::debug("All dependencies for {} found", m_info.id());
        if (m_enabled) {
            log::debug("Resolved & loading {}", m_info.id());
            GEODE_UNWRAP(this->loadBinary());
        }
        else {
            log::debug("Resolved {}, however not loading it as it is disabled", m_info.id());
        }
    }
    return Ok();
}

bool Mod::Impl::hasUnresolvedDependencies() const {
    for (auto const& dep : m_info.dependencies()) {
        if (!dep.isResolved()) {
            return true;
        }
    }
    return false;
}

std::vector<Dependency> Mod::Impl::getUnresolvedDependencies() {
    std::vector<Dependency> unresolved;
    for (auto const& dep : m_info.dependencies()) {
        if (!dep.isResolved()) {
            unresolved.push_back(dep);
        }
    }
    return unresolved;
}

bool Mod::Impl::depends(std::string const& id) const {
    return utils::ranges::contains(m_info.dependencies(), [id](Dependency const& t) {
        return t.id == id;
    });
}

// Hooks

Result<> Mod::Impl::enableHook(Hook* hook) {
    auto res = hook->enable();
    if (res) m_hooks.push_back(hook);
    else {
        log::error("Can't enable hook {} for mod {}: {}", m_info.id(), res.unwrapErr());
    }

    return res;
}

Result<> Mod::Impl::disableHook(Hook* hook) {
    return hook->disable();
}

Result<Hook*> Mod::Impl::addHook(Hook* hook) {
    if (LoaderImpl::get()->isReadyToHook()) {
        if (hook->getAutoEnable()) {
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
    if (!p->apply()) {
        delete p;
        return Err("Unable to enable patch at " + std::to_string(p->getAddress()));
    }
    m_patches.push_back(p);
    return Ok(p);
}

Result<> Mod::Impl::unpatch(Patch* patch) {
    if (patch->restore()) {
        ranges::remove(m_patches, patch);
        delete patch;
        return Ok();
    }
    return Err("Unable to restore patch!");
}

// Misc.

Result<> Mod::Impl::createTempDir() {
    // Check if temp dir already exists
    if (!m_tempDirName.string().empty()) {
        return Ok();
    }

    // Create geode/temp
    auto tempDir = dirs::getModRuntimeDir();
    if (!file::createDirectoryAll(tempDir)) {
        return Err("Unable to create mods' runtime directory");
    }

    // Create geode/temp/mod.id
    auto tempPath = tempDir / m_info.id();
    if (!file::createDirectoryAll(tempPath)) {
        return Err("Unable to create mod runtime directory");
    }

    // Unzip .geode file into temp dir
    GEODE_UNWRAP_INTO(auto unzip, file::Unzip::create(m_info.path()));
    if (!unzip.hasEntry(m_info.binaryName())) {
        return Err(
            fmt::format("Unable to find platform binary under the name \"{}\"", m_info.binaryName())
        );
    }
    GEODE_UNWRAP(unzip.extractAllTo(tempPath));

    // Mark temp dir creation as succesful
    m_tempDirName = tempPath;

    return Ok();
}

ghc::filesystem::path Mod::Impl::getConfigDir(bool create) const {
    auto dir = dirs::getModConfigDir() / m_info.id();
    if (create) {
        (void)file::createDirectoryAll(dir);
    }
    return dir;
}

char const* Mod::Impl::expandSpriteName(char const* name) {
    static std::unordered_map<std::string, char const*> expanded = {};
    if (expanded.count(name)) return expanded[name];

    auto exp = new char[strlen(name) + 2 + m_info.id().size()];
    auto exps = m_info.id() + "/" + name;
    memcpy(exp, exps.c_str(), exps.size() + 1);

    expanded[name] = exp;

    return exp;
}

ModJson Mod::Impl::getRuntimeInfo() const {
    auto json = m_info.toJSON();

    auto obj = json::Object();
    obj["hooks"] = json::Array();
    for (auto hook : m_hooks) {
        obj["hooks"].as_array().push_back(ModJson(hook->getRuntimeInfo()));
    }
    obj["patches"] = json::Array();
    for (auto patch : m_patches) {
        obj["patches"].as_array().push_back(ModJson(patch->getRuntimeInfo()));
    }
    obj["enabled"] = m_enabled;
    obj["loaded"] = m_binaryLoaded;
    obj["temp-dir"] = this->getTempDir();
    obj["save-dir"] = this->getSaveDir();
    obj["config-dir"] = this->getConfigDir(false);
    json["runtime"] = obj;

    return json;
}

static constexpr char const* SUPPORT_INFO = R"MD(
**Geode** is funded through your gracious <cy>**donations**</c>!
You can support our work by sending <cp>**catgirl pictures**</c> to [HJfod](https://youtu.be/LOHSF9MmBDw) :))
)MD";

static ModInfo getModImplInfo() {
    std::string err;
    json::Value json;
    try {
        json = json::parse(LOADER_MOD_JSON);
    } catch (std::exception& err) {
        LoaderImpl::get()->platformMessageBox(
            "Fatal Internal Error",
            "Unable to parse loader mod.json: \"" + std::string(err.what()) +
                "\"\n"
                "This is a fatal internal error in the loader, please "
                "contact Geode developers immediately!"
        );
        return ModInfo();
    }

    auto infoRes = ModInfo::create(json);
    if (infoRes.isErr()) {
        LoaderImpl::get()->platformMessageBox(
            "Fatal Internal Error",
            "Unable to parse loader mod.json: \"" + infoRes.unwrapErr() +
                "\"\n"
                "This is a fatal internal error in the loader, please "
                "contact Geode developers immediately!"
        );
        return ModInfo();
    }
    auto info = infoRes.unwrap();
    info.details() = LOADER_ABOUT_MD;
    info.supportInfo() = SUPPORT_INFO;
    info.supportsDisabling() = false;
    return info;
}

Mod* Loader::Impl::createInternalMod() {
    auto& mod = Mod::sharedMod<>;
    if (!mod) {
        mod = new Mod(getModImplInfo());
    }
    return mod;
}

Result<> Loader::Impl::setupInternalMod() {
    return Mod::get()->m_impl->setup();
}
