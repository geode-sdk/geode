#include <Geode/loader/Hook.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/file.hpp>
#include <InternalLoader.hpp>
#include <InternalMod.hpp>
#include <optional>
#include <string>
#include <vector>

USE_GEODE_NAMESPACE();

Mod::Mod(ModInfo const& info) {
    m_info = info;
    m_saveDirPath = Loader::get()->getGeodeSaveDirectory() / GEODE_MOD_DIRECTORY / info.m_id;
    ghc::filesystem::create_directories(m_saveDirPath);
}

Mod::~Mod() {
    (void)this->unloadBinary();
}

// Getters

ghc::filesystem::path Mod::getSaveDir() const {
    return m_saveDirPath;
}

std::string Mod::getID() const {
    return m_info.m_id;
}

std::string Mod::getName() const {
    return m_info.m_name;
}

std::string Mod::getDeveloper() const {
    return m_info.m_developer;
}

std::optional<std::string> Mod::getDescription() const {
    return m_info.m_description;
}

std::optional<std::string> Mod::getDetails() const {
    return m_info.m_details;
}

ModInfo Mod::getModInfo() const {
    return m_info;
}

ghc::filesystem::path Mod::getTempDir() const {
    return m_tempDirName;
}

ghc::filesystem::path Mod::getBinaryPath() const {
    return m_tempDirName / m_info.m_binaryName;
}

ghc::filesystem::path Mod::getPackagePath() const {
    return m_info.m_path;
}

VersionInfo Mod::getVersion() const {
    return m_info.m_version;
}

bool Mod::isEnabled() const {
    return m_enabled;
}

bool Mod::isLoaded() const {
    return m_binaryLoaded;
}

bool Mod::supportsDisabling() const {
    return m_info.m_supportsDisabling;
}

bool Mod::supportsUnloading() const {
    return m_info.m_supportsUnloading;
}

bool Mod::wasSuccesfullyLoaded() const {
    return !this->isEnabled() || this->isLoaded();
}

std::vector<Hook*> Mod::getHooks() const {
    return m_hooks;
}

bool Mod::hasSettings() const {
    return m_info.m_settings.size();
}

decltype(ModInfo::m_settings) Mod::getSettings() const {
    return m_info.m_settings;
}

// Settings and saved values

Result<> Mod::loadData() {
    ModStateEvent(this, ModEventType::DataLoaded).post();

    // Settings
    // Check if settings exist
    auto settingPath = m_saveDirPath / "settings.json";
    if (ghc::filesystem::exists(settingPath)) {
        GEODE_UNWRAP_INTO(auto settingData, utils::file::readString(settingPath));
        try {
            // parse settings.json
            auto json = nlohmann::json::parse(settingData);
            JsonChecker checker(json);
            auto root = checker.root("[settings.json]");

            for (auto& [key, value] : root.items()) {
                // check if this is a known setting
                if (auto setting = this->getSetting(key)) {
                    // load its value
                    if (!setting->load(value.json()))
                        return Err("Unable to load value for setting \"" + key + "\"");
                }
                else {
                    log::log(
                        Severity::Warning, this,
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
            m_saved = nlohmann::json::parse(data);
        }
        catch (std::exception& e) {
            return Err(std::string("Unable to parse saved values: ") + e.what());
        }
    }

    return Ok();
}

Result<> Mod::saveData() {
    ModStateEvent(this, ModEventType::DataSaved).post();

    // Settings
    auto json = nlohmann::json::object();
    for (auto& [key, value] : m_info.m_settings) {
        if (!value->save(json[key])) return Err("Unable to save setting \"" + key + "\"");
    }

    GEODE_UNWRAP(utils::file::writeString(m_saveDirPath / "settings.json", json.dump(4)));

    // Saved values
    GEODE_UNWRAP(utils::file::writeString(m_saveDirPath / "saved.json", m_saved.dump(4)));

    return Ok();
}

std::shared_ptr<Setting> Mod::getSetting(std::string const& key) const {
    for (auto& setting : m_info.m_settings) {
        if (setting.first == key) {
            return setting.second;
        }
    }
    return nullptr;
}

bool Mod::hasSetting(std::string const& key) const {
    for (auto& setting : m_info.m_settings) {
        if (setting.first == key) {
            return true;
        }
    }
    return false;
}

// Loading, Toggling, Installing

Result<> Mod::loadBinary() {
    if (!m_binaryLoaded) {
        GEODE_UNWRAP(this->createTempDir().expect("Unable to create temp directory"));

        if (this->hasUnresolvedDependencies()) return Err("Mod has unresolved dependencies");

        GEODE_UNWRAP(this->loadPlatformBinary());
        m_binaryLoaded = true;

        // Call implicit entry point to place hooks etc.
        m_implicitLoadFunc(this);

        ModStateEvent(this, ModEventType::Loaded).post();

        auto loadRes = this->loadData();
        if (!loadRes) {
            log::warn("Unable to load data for \"{}\": {}", m_info.m_id, loadRes.unwrapErr());
        }

        Loader::get()->updateAllDependencies();

        GEODE_UNWRAP(this->enable());
    }

    return Ok();
}

Result<> Mod::unloadBinary() {
    if (m_binaryLoaded) {
        if (!m_info.m_supportsUnloading) return Err("Mod does not support unloading");

        GEODE_UNWRAP(this->saveData());

        GEODE_UNWRAP(this->disable());
        ModStateEvent(this, ModEventType::Unloaded).post();

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
    }

    return Ok();
}

Result<> Mod::enable() {
    if (!m_binaryLoaded) return this->loadBinary();

    for (auto const& hook : m_hooks) {
        GEODE_UNWRAP(this->enableHook(hook));
    }

    for (auto const& patch : m_patches) {
        if (!patch->apply())
            return Err("Unable to apply patch at " + std::to_string(patch->getAddress()));
    }

    ModStateEvent(this, ModEventType::Enabled).post();
    m_enabled = true;

    return Ok();
}

Result<> Mod::disable() {
    if (m_enabled) {
        if (!m_info.m_supportsDisabling) return Err("Mod does not support disabling");

        ModStateEvent(this, ModEventType::Disabled).post();

        for (auto const& hook : m_hooks) {
            GEODE_UNWRAP(this->disableHook(hook));
        }
        for (auto const& patch : m_patches) {
            if (!patch->restore())
                return Err("Unable to restore patch at " + std::to_string(patch->getAddress()));
        }

        m_enabled = false;
    }

    return Ok();
}

Result<> Mod::uninstall() {
    if (m_info.m_supportsDisabling) {
        GEODE_UNWRAP(this->disable());

        if (m_info.m_supportsUnloading) GEODE_UNWRAP(this->unloadBinary());
    }

    if (!ghc::filesystem::remove(m_info.m_path)) {
        return Err(
            "Unable to delete mod's .geode file! "
            "This might be due to insufficient permissions - "
            "try running GD as administrator."
        );
    }

    return Ok();
}

bool Mod::isUninstalled() const {
    return this != InternalMod::get() && !ghc::filesystem::exists(m_info.m_path);
}

// Dependencies

bool Dependency::isUnresolved() const {
    return m_required &&
        (m_state == ModResolveState::Unloaded || m_state == ModResolveState::Unresolved ||
         m_state == ModResolveState::Disabled);
}

bool Mod::updateDependencyStates() {
    bool hasUnresolved = false;
    for (auto& dep : m_info.m_dependencies) {
        if (!dep.m_mod) dep.m_mod = Loader::get()->getLoadedMod(dep.m_id);

        if (dep.m_mod) {
            dep.m_mod->updateDependencyStates();

            if (dep.m_mod->hasUnresolvedDependencies()) {
                dep.m_state = ModResolveState::Unresolved;
            }
            else {
                if (!dep.m_mod->m_resolved) {
                    dep.m_mod->m_resolved = true;
                    dep.m_state = ModResolveState::Resolved;
                    auto r = dep.m_mod->loadBinary();
                    if (!r) {
                        dep.m_state = ModResolveState::Unloaded;
                        log::log(Severity::Error, dep.m_mod, "{}", r.unwrapErr());
                    }
                }
                else {
                    if (dep.m_mod->isEnabled()) {
                        dep.m_state = ModResolveState::Loaded;
                    }
                    else {
                        dep.m_state = ModResolveState::Disabled;
                    }
                }
            }
        }
        else {
            dep.m_state = ModResolveState::Unloaded;
        }
        if (dep.isUnresolved()) {
            m_resolved = false;
            (void)this->unloadBinary();
            hasUnresolved = true;
        }
    }

    if (!hasUnresolved && !m_resolved) {
        log::debug("All dependencies for {} found", m_info.m_id);
        m_resolved = true;
        if (m_enabled) {
            log::debug("Resolved & loading {}", m_info.m_id);
            auto r = this->loadBinary();
            if (!r) {
                log::error("{} Error loading: {}", this, r.unwrapErr());
            }
        }
        else {
            log::debug("Resolved {}, however not loading it as it is disabled", m_info.m_id);
        }
    }
    return hasUnresolved;
}

bool Mod::hasUnresolvedDependencies() const {
    for (auto const& dep : m_info.m_dependencies) {
        if (dep.isUnresolved()) return true;
    }
    return false;
}

std::vector<Dependency> Mod::getUnresolvedDependencies() {
    std::vector<Dependency> unresolved;
    for (auto const& dep : m_info.m_dependencies) {
        if (dep.isUnresolved()) unresolved.push_back(dep);
    }
    return unresolved;
}

bool Mod::depends(std::string const& id) const {
    return utils::ranges::contains(m_info.m_dependencies, [id](Dependency const& t) {
        return t.m_id == id;
    });
}

// Hooks

Result<> Mod::enableHook(Hook* hook) {
    auto res = hook->enable();
    if (res) m_hooks.push_back(hook);

    return res;
}

Result<> Mod::disableHook(Hook* hook) {
    return hook->disable();
}

Result<Hook*> Mod::addHook(Hook* hook) {
    if (InternalLoader::get()->isReadyToHook()) {
        auto res = this->enableHook(hook);
        if (!res) {
            delete hook;
            return Err("Can't create hook");
        }
    }
    else {
        InternalLoader::get()->addInternalHook(hook, this);
    }

    return Ok(hook);
}

Result<> Mod::removeHook(Hook* hook) {
    auto res = this->disableHook(hook);
    if (res) {
        ranges::remove(m_hooks, hook);
        delete hook;
    }
    return res;
}

// Patches

byte_array readMemory(void* address, size_t amount) {
    byte_array ret;
    for (size_t i = 0; i < amount; i++) {
        ret.push_back(*reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(address) + i));
    }
    return ret;
}

Result<Patch*> Mod::patch(void* address, byte_array data) {
    auto p = new Patch;
    p->m_address = address;
    p->m_original = readMemory(address, data.size());
    p->m_owner = this;
    p->m_patch = data;
    if (!p->apply()) {
        delete p;
        return Err("Unable to enable patch at " + std::to_string(p->getAddress()));
    }
    m_patches.push_back(p);
    return Ok(p);
}

Result<> Mod::unpatch(Patch* patch) {
    if (patch->restore()) {
        ranges::remove(m_patches, patch);
        delete patch;
        return Ok();
    }
    return Err("Unable to restore patch!");
}

// Misc.

Result<> Mod::createTempDir() {
    // Check if temp dir already exists
    if (m_tempDirName.string().empty()) {
        // Create geode/temp
        auto tempDir = Loader::get()->getGeodeDirectory() / GEODE_TEMP_DIRECTORY;
        if (!file::createDirectoryAll(tempDir).isOk()) {
            return Err("Unable to create Geode temp directory");
        }

        // Create geode/temp/mod.id
        auto tempPath = tempDir / m_info.m_id;
        if (!file::createDirectoryAll(tempPath).isOk()) {
            return Err("Unable to create mod temp directory");
        }

        // Unzip .geode file into temp dir
        GEODE_UNWRAP_INTO(auto unzip, file::Unzip::create(m_info.m_path));
        if (!unzip.hasEntry(m_info.m_binaryName)) {
            return Err(fmt::format(
                "Unable to find platform binary under the name \"{}\"", m_info.m_binaryName
            ));
        }
        GEODE_UNWRAP(unzip.extractAllTo(tempPath));

        // Mark temp dir creation as succesful
        m_tempDirName = tempPath;
    }

    return Ok();
}

ghc::filesystem::path Mod::getConfigDir(bool create) const {
    auto dir = Loader::get()->getGeodeDirectory() / GEODE_CONFIG_DIRECTORY / m_info.m_id;
    if (create && !ghc::filesystem::exists(dir)) {
        ghc::filesystem::create_directories(dir);
    }

    return dir;
}

char const* Mod::expandSpriteName(char const* name) {
    static std::unordered_map<std::string, char const*> expanded = {};
    if (expanded.count(name)) return expanded[name];

    auto exp = new char[strlen(name) + 2 + m_info.m_id.size()];
    auto exps = m_info.m_id + "/" + name;
    memcpy(exp, exps.c_str(), exps.size() + 1);

    expanded[name] = exp;

    return exp;
}

ModJson Mod::getRuntimeInfo() const {
    auto json = m_info.toJSON();

    auto obj = ModJson::object();
    obj["hooks"] = ModJson::array();
    for (auto hook : m_hooks) {
        obj["hooks"].push_back(ModJson(hook->getRuntimeInfo()));
    }
    obj["patches"] = ModJson::array();
    for (auto patch : m_patches) {
        obj["patches"].push_back(ModJson(patch->getRuntimeInfo()));
    }
    obj["enabled"] = m_enabled;
    obj["loaded"] = m_binaryLoaded;
    obj["temp-dir"] = this->getTempDir();
    obj["save-dir"] = this->getSaveDir();
    obj["config-dir"] = this->getConfigDir(false);
    json["runtime"] = obj;

    return json;
}
