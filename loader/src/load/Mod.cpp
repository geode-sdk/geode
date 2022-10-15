#include <about.hpp>
#include <Geode/loader/Hook.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Setting.hpp>
#include <Geode/utils/conststring.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <Geode/utils/map.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/vector.hpp>
#include <InternalMod.hpp>
#include <ZipUtils.h>

USE_GEODE_NAMESPACE();

nlohmann::json& DataStore::operator[](std::string const& key) {
    return m_mod->m_dataStore[key];
}

DataStore& DataStore::operator=(nlohmann::json& jsn) {
    m_mod->m_dataStore = jsn;
    return *this;
}

nlohmann::json& DataStore::getJson() const {
    return m_mod->m_dataStore;
}

bool DataStore::contains(std::string const& key) const {
    return m_mod->m_dataStore.contains(key);
}

DataStore::operator nlohmann::json() {
    return m_mod->m_dataStore;
}

DataStore::~DataStore() {
    if (m_store != m_mod->m_dataStore) {
        m_mod->postDSUpdate();
    }
}

Mod::Mod(ModInfo const& info) {
    m_info = info;
}

Mod::~Mod() {
    this->unload();
}

Result<> Mod::loadSettings() {
    // settings

    // Check if settings exist
    auto settPath = m_saveDirPath / "settings.json";
    if (ghc::filesystem::exists(settPath)) {
        auto settData = utils::file::readString(settPath);
        if (!settData) return settData;
        try {
            // parse settings.json
            auto data = nlohmann::json::parse(settData.value());
            JsonChecker checker(data);
            auto root = checker.root("[settings.json]");

            for (auto& [key, value] : root.items()) {
                // check if this is a known setting
                if (auto sett = this->getSetting(key)) {
                    // load its value
                    if (!sett->load(value.json())) {
                        return Err(
                            "Unable to load value for setting \"" +
                            key + "\""
                        );
                    }
                } else {
                    log::log(
                        Severity::Warning,
                        this, 
                        "Encountered unknown setting \"{}\" while loading settings",
                        key
                    );
                }
            }

        } catch(std::exception& e) {
            return Err(std::string("Unable to parse settings: ") + e.what());
        }
    }

    // datastore
    auto dsPath = m_saveDirPath / "ds.json";
    if (!ghc::filesystem::exists(dsPath)) {
        m_dataStore = m_info.m_defaultDataStore;
    } else {
        auto dsData = utils::file::readString(dsPath);
        if (!dsData) return dsData;
        try {
            m_dataStore = nlohmann::json::parse(dsData.value());
        } catch(std::exception& e) {
            return Err(std::string("Unable to parse datastore: ") + e.what());
        }
    }
    return Ok();
}

Result<> Mod::saveSettings() {
    // settings
    auto settPath = m_saveDirPath / "settings.json";
    auto json = nlohmann::json::object();
    for (auto& [key, sett] : m_info.m_settings) {
        if (!sett->save(json[key])) {
            return Err("Unable to save setting \"" + key + "\"");
        }
    }
    auto sw = utils::file::writeString(settPath, json.dump(4));
    if (!sw) {
        return sw;
    }

    // datastore
    auto dsPath = m_saveDirPath / "ds.json";
    auto dw = utils::file::writeString(dsPath, m_dataStore.dump(4));
    if (!dw) {
        return dw;
    }

    return Ok();
}

DataStore Mod::getDataStore() {
    return DataStore(this, m_dataStore);
}

void Mod::postDSUpdate() {
    /*EventCenter::get()->send(Event(
        "datastore-changed",
        this
    ), this);*/
    // FIXME: Dispatch
}

Result<> Mod::createTempDir() {
    ZipFile unzip(m_info.m_path.string());

    if (!unzip.isLoaded()) {
        return Err<>("Unable to unzip " + m_info.m_path.string());
    }

    if (!unzip.fileExists(m_info.m_binaryName)) {
        return Err<>(
            "Unable to find platform binary under the name \"" +
            m_info.m_binaryName + "\""
        );
    }

    auto tempDir = Loader::get()->getGeodeDirectory() / GEODE_TEMP_DIRECTORY;
    if (!ghc::filesystem::exists(tempDir)) {
        if (!ghc::filesystem::create_directory(tempDir)) {
            return Err<>("Unable to create temp directory for mods!");
        }
    }
    
    auto tempPath = ghc::filesystem::path(tempDir) / m_info.m_id;
    if (!ghc::filesystem::exists(tempPath) && !ghc::filesystem::create_directories(tempPath)) {
        return Err<>("Unable to create temp directory");
    }
    m_tempDirName = tempPath;

    for (auto file : unzip.getAllFiles()) {
        auto path = ghc::filesystem::path(file);
        if (path.has_parent_path()) {
            if (
                !ghc::filesystem::exists(tempPath / path.parent_path()) &&
                !ghc::filesystem::create_directories(tempPath / path.parent_path())
            ) {
                return Err<>("Unable to create directories \"" + path.parent_path().string() + "\"");
            }
        }
        unsigned long size;
        auto data = unzip.getFileData(file, &size);
        if (!data || !size) {
            return Err<>("Unable to read \"" + std::string(file) + "\"");
        }
        auto wrt = utils::file::writeBinary(
            tempPath / file,
            byte_array(data, data + size)
        );
        if (!wrt) return Err<>("Unable to write \"" + file + "\": " + wrt.error());
    }

    m_addResourcesToSearchPath = true;

    return Ok<>(tempPath);
}

Result<> Mod::load() {
	if (m_loaded) {
        return Ok<>();
    }
    #define RETURN_LOAD_ERR(str) \
        {m_loadErrorInfo = str; \
        return Err<>(m_loadErrorInfo);}

    if (!m_tempDirName.string().size()) {
        auto err = this->createTempDir();
        if (!err) RETURN_LOAD_ERR("Unable to create temp directory: " + err.error());
    }

    if (this->hasUnresolvedDependencies()) {
        RETURN_LOAD_ERR("Mod has unresolved dependencies");
    }
    auto err = this->loadPlatformBinary();
    if (!err) RETURN_LOAD_ERR(err.error());
    if (m_implicitLoadFunc) {
        auto r = m_implicitLoadFunc(this);
        if (!r) {
            this->unloadPlatformBinary();
            RETURN_LOAD_ERR("Implicit mod entry point returned an error");
        }
    }
    if (m_loadFunc) {
        auto r = m_loadFunc(this);
        if (!r) {
            this->unloadPlatformBinary();
            RETURN_LOAD_ERR("Mod entry point returned an error");
        }
    }
    m_loaded = true;
    if (m_loadDataFunc) {
        if (!m_loadDataFunc(m_saveDirPath.string().c_str())) {
            log::log(Severity::Error, this, "Mod load data function returned false");
        }
    }
    m_loadErrorInfo = "";
    Loader::get()->updateAllDependencies();
    return Ok<>();
}

Result<> Mod::unload() {
    if (!m_loaded) {
        return Ok<>();
    }

    if (!m_info.m_supportsUnloading) {
        return Err<>("Mod does not support unloading");
    }
    
    if (m_saveDataFunc) {
        if (!m_saveDataFunc(m_saveDirPath.string().c_str())) {
            log::log(Severity::Error, this, "Mod save data function returned false");
        }
    }

    if (m_unloadFunc) {
        m_unloadFunc();
    }

    for (auto const& hook : m_hooks) {
        auto d = this->disableHook(hook);
        if (!d) return d;
        delete hook;
    }
    m_hooks.clear();

    for (auto const& patch : m_patches) {
        if (!patch->restore()) {
            return Err<>("Unable to restore patch at " + std::to_string(patch->getAddress()));
        }
        delete patch;
    }
    m_patches.clear();

    auto res = this->unloadPlatformBinary();
    if (!res) {
        return res;
    }
    m_loaded = false;
    Loader::get()->updateAllDependencies();
    return Ok<>();
}

Result<> Mod::enable() {
    if (!m_loaded) {
        return Err<>("Mod is not loaded");
    }
    
    if (m_enableFunc) {
        if (!m_enableFunc()) {
            return Err<>("Mod enable function returned false");
        }
    }

    for (auto const& hook : m_hooks) {
        auto d = this->enableHook(hook);
        if (!d) return d;
    }

    for (auto const& patch : m_patches) {
        if (!patch->apply()) {
            return Err<>("Unable to apply patch at " + std::to_string(patch->getAddress()));
        }
    }

    m_enabled = true;

    return Ok<>();
}

Result<> Mod::disable() {
    if (!m_info.m_supportsDisabling) {
        return Err<>("Mod does not support disabling");
    }

    if (m_disableFunc) {
        if (!m_disableFunc()) {
            return Err<>("Mod disable function returned false");
        }
    }

    for (auto const& hook : m_hooks) {
        auto d = this->disableHook(hook);
        if (!d) return d;
    }

    for (auto const& patch : m_patches) {
        if (!patch->restore()) {
            return Err<>("Unable to restore patch at " + std::to_string(patch->getAddress()));
        }
    }

    m_enabled = false;

    return Ok<>();
}

Result<> Mod::uninstall() {
    if (m_info.m_supportsDisabling) {
        auto r = this->disable();
        if (!r) return r;
        if (m_info.m_supportsUnloading) {
            auto ur = this->unload();
            if (!ur) return ur;
        }
    }
    if (!ghc::filesystem::remove(m_info.m_path)) {
        return Err<>(
            "Unable to delete mod's .geode file! "
            "This might be due to insufficient permissions - "
            "try running GD as administrator."
        );
    }
    return Ok<>();
}

bool Mod::isUninstalled() const {
    return this != InternalMod::get() && !ghc::filesystem::exists(m_info.m_path);
}

bool Dependency::isUnresolved() const {
    return m_required &&
           (m_state == ModResolveState::Unloaded ||
           m_state == ModResolveState::Unresolved ||
           m_state == ModResolveState::Disabled);
}

bool Mod::updateDependencyStates() {
    bool hasUnresolved = false;
	for (auto & dep : m_info.m_dependencies) {
		if (!dep.m_mod) {
			dep.m_mod = Loader::get()->getLoadedMod(dep.m_id);
		}
		if (dep.m_mod) {
			dep.m_mod->updateDependencyStates();

			if (dep.m_mod->hasUnresolvedDependencies()) {
				dep.m_state = ModResolveState::Unresolved;
			} else {
				if (!dep.m_mod->m_resolved) {
					dep.m_mod->m_resolved = true;
					dep.m_state = ModResolveState::Resolved;
                    auto r = dep.m_mod->load();
                    if (!r) {
                        dep.m_state = ModResolveState::Unloaded;
                        log::log(Severity::Error, dep.m_mod, "{}", r.error());
                    }
                    else {
                    	auto r = dep.m_mod->enable();
                    	if (!r) {
	                        dep.m_state = ModResolveState::Disabled;
                            log::log(Severity::Error, dep.m_mod, "{}", r.error());
	                    }
                    }
				} else {
					if (dep.m_mod->isEnabled()) {
						dep.m_state = ModResolveState::Loaded;
					} else {
						dep.m_state = ModResolveState::Disabled;
					}
				}
			}
		} else {
			dep.m_state = ModResolveState::Unloaded;
		}
		if (dep.isUnresolved()) {
			m_resolved = false;
            this->unload();
            hasUnresolved = true;
		}
	}
    if (!hasUnresolved && !m_resolved) {
        log::debug("All dependencies for {} found", m_info.m_id);
        m_resolved = true;
        if (m_enabled) {
            log::debug("Resolved & loading {}", m_info.m_id);
            auto r = this->load();
            if (!r) {
                log::error("{} Error loading: {}", this, r.error());
            }
            else {
            	auto r = this->enable();
	            if (!r) {
	                log::error("{} Error enabling: {}", this, r.error());
	            }
            }
        } else {
            log::debug("Resolved {}, however not loading it as it is disabled", m_info.m_id);
        }
    }
    return hasUnresolved;
}

bool Mod::hasUnresolvedDependencies() const {
	for (auto const& dep : m_info.m_dependencies) {
		if (dep.isUnresolved()) {
			return true;
		}
	}
	return false;
}

std::vector<Dependency> Mod::getUnresolvedDependencies() {
    std::vector<Dependency> res;
	for (auto const& dep : m_info.m_dependencies) {
		if (dep.isUnresolved()) {
			res.push_back(dep);
		}
	}
    return res;
}

ghc::filesystem::path Mod::getSaveDir() const {
    return m_saveDirPath;
}

decltype(ModInfo::m_id) Mod::getID() const {
    return m_info.m_id;
}

decltype(ModInfo::m_name) Mod::getName() const {
    return m_info.m_name;
}

decltype(ModInfo::m_developer) Mod::getDeveloper() const {
    return m_info.m_developer;
}

decltype(ModInfo::m_description) Mod::getDescription() const {
    return m_info.m_description;
}

decltype(ModInfo::m_details) Mod::getDetails() const {
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

std::string Mod::getPath() const {
    return m_info.m_path.string();
}

VersionInfo Mod::getVersion() const {
    return m_info.m_version;
}

bool Mod::isEnabled() const {
    return m_enabled;
}

bool Mod::isLoaded() const {
    return m_loaded;
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

bool Mod::depends(std::string const& id) const {
    return utils::vector::contains<Dependency>(
        m_info.m_dependencies,
        [id](Dependency t) -> bool { return t.m_id == id; }
    );
}

const char* Mod::expandSpriteName(const char* name) {
    static std::unordered_map<std::string, const char*> expanded = {};
    if (expanded.count(name)) {
        return expanded[name];
    }
    auto exp = new char[strlen(name) + 2 + m_info.m_id.size()];
    auto exps = m_info.m_id + "/" + name;
    memcpy(exp, exps.c_str(), exps.size() + 1);
    expanded[name] = exp;
    return exp;
}

bool Mod::hasSettings() const {
    return m_info.m_settings.size();
}

decltype(ModInfo::m_settings) Mod::getSettings() const {
    return m_info.m_settings;
}

std::shared_ptr<Setting> Mod::getSetting(std::string const& key) const {
    for (auto& sett : m_info.m_settings) {
        if (sett.first == key) {
            return sett.second;
        }
    }
    return nullptr;
}

bool Mod::hasSetting(std::string const& key) const {
    for (auto& sett : m_info.m_settings) {
        if (sett.first == key) {
            return true;
        }
    }
    return false;
}

std::string Mod::getLoadErrorInfo() const {
    return m_loadErrorInfo;
}
