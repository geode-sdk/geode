#include <about.hpp>
#include <Geode/loader/Hook.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Interface.hpp>
#include <Geode/utils/conststring.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/json_check.hpp>
#include <Geode/utils/map.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/vector.hpp>
//#include <InternalLoader.hpp>
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
    this->m_info = info;
}

Mod::~Mod() {
    this->unload();
}

Result<> Mod::loadDataStore() {
    auto dsPath = this->m_saveDirPath / "ds.json";
    if (!ghc::filesystem::exists(dsPath)) {
        this->m_dataStore = this->m_info.m_defaultDataStore;
    } else {
        auto dsData = file_utils::readString(dsPath);
        if (!dsData) return dsData;

        this->m_dataStore = nlohmann::json::parse(dsData.value());
    }
    return Ok<>();
}

Result<> Mod::saveDataStore() {
    auto dsPath = this->m_saveDirPath / "ds.json";
    return file_utils::writeString(dsPath, m_dataStore.dump(4));
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
    ZipFile unzip(this->m_info.m_path.string());

    if (!unzip.isLoaded()) {
        return Err<>("Unable to unzip " + this->m_info.m_path.string());
    }

    if (!unzip.fileExists(this->m_info.m_binaryName)) {
        return Err<>(
            "Unable to find platform binary under the name \"" +
            this->m_info.m_binaryName + "\""
        );
    }

    auto tempDir = Loader::get()->getGeodeDirectory() / GEODE_TEMP_DIRECTORY;
    if (!ghc::filesystem::exists(tempDir)) {
        if (!ghc::filesystem::create_directory(tempDir)) {
            return Err<>("Unable to create temp directory for mods!");
        }
    }
    
    auto tempPath = ghc::filesystem::path(tempDir) / this->m_info.m_id;
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
        auto wrt = file_utils::writeBinary(
            tempPath / file,
            byte_array(data, data + size)
        );
        if (!wrt) return Err<>("Unable to write \"" + file + "\": " + wrt.error());
    }

    this->m_addResourcesToSearchPath = true;
    Loader::get()->addModResourcesPath(this);

    return Ok<>(tempPath);
}

Result<> Mod::load() {
	if (this->m_loaded) {
        return Ok<>();
    }
    #define RETURN_LOAD_ERR(str) \
        {m_loadErrorInfo = str; \
        return Err<>(m_loadErrorInfo);}

    if (!this->m_tempDirName.string().size()) {
        auto err = this->createTempDir();
        if (!err) RETURN_LOAD_ERR("Unable to create temp directory: " + err.error());
    }

    if (this->hasUnresolvedDependencies()) {
        RETURN_LOAD_ERR("Mod has unresolved dependencies");
    }
    auto err = this->loadPlatformBinary();
    if (!err) RETURN_LOAD_ERR(err.error());
    if (this->m_implicitLoadFunc) {
        auto r = this->m_implicitLoadFunc(this);
        if (!r) {
            this->unloadPlatformBinary();
            RETURN_LOAD_ERR("Implicit mod entry point returned an error");
        }
    }
    if (this->m_loadFunc) {
        auto r = this->m_loadFunc(this);
        if (!r) {
            this->unloadPlatformBinary();
            RETURN_LOAD_ERR("Mod entry point returned an error");
        }
    }
    this->m_loaded = true;
    if (this->m_loadDataFunc) {
        if (!this->m_loadDataFunc(this->m_saveDirPath.string().c_str())) {
            this->logInfo("Mod load data function returned false", Severity::Error);
        }
    }
    m_loadErrorInfo = "";
    Loader::get()->updateAllDependencies();
    return Ok<>();
}

Result<> Mod::unload() {
    if (!this->m_loaded) {
        return Ok<>();
    }

    if (!m_info.m_supportsUnloading) {
        return Err<>("Mod does not support unloading");
    }
    
    if (this->m_saveDataFunc) {
        if (!this->m_saveDataFunc(this->m_saveDirPath.string().c_str())) {
            this->logInfo("Mod save data function returned false", Severity::Error);
        }
    }

    if (this->m_unloadFunc) {
        this->m_unloadFunc();
    }

    for (auto const& hook : this->m_hooks) {
        auto d = this->disableHook(hook);
        if (!d) return d;
        delete hook;
    }
    this->m_hooks.clear();

    for (auto const& patch : this->m_patches) {
        if (!patch->restore()) {
            return Err<>("Unable to restore patch at " + std::to_string(patch->getAddress()));
        }
        delete patch;
    }
    this->m_patches.clear();

    auto res = this->unloadPlatformBinary();
    if (!res) {
        return res;
    }
    this->m_loaded = false;
    Loader::get()->updateAllDependencies();
    return Ok<>();
}

Result<> Mod::enable() {
    if (!this->m_loaded) {
        return Err<>("Mod is not loaded");
    }
    
    if (this->m_enableFunc) {
        if (!this->m_enableFunc()) {
            return Err<>("Mod enable function returned false");
        }
    }

    for (auto const& hook : this->m_hooks) {
        auto d = this->enableHook(hook);
        if (!d) return d;
    }

    for (auto const& patch : this->m_patches) {
        if (!patch->apply()) {
            return Err<>("Unable to apply patch at " + std::to_string(patch->getAddress()));
        }
    }

    this->m_enabled = true;

    return Ok<>();
}

Result<> Mod::disable() {
    if (!m_info.m_supportsDisabling) {
        return Err<>("Mod does not support disabling");
    }

    if (this->m_disableFunc) {
        if (!this->m_disableFunc()) {
            return Err<>("Mod disable function returned false");
        }
    }

    for (auto const& hook : this->m_hooks) {
        auto d = this->disableHook(hook);
        if (!d) return d;
    }

    for (auto const& patch : this->m_patches) {
        if (!patch->restore()) {
            return Err<>("Unable to restore patch at " + std::to_string(patch->getAddress()));
        }
    }

    this->m_enabled = false;

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
    return this->m_required &&
           (this->m_state == ModResolveState::Unloaded ||
           this->m_state == ModResolveState::Unresolved ||
           this->m_state == ModResolveState::Disabled);
}

bool Mod::updateDependencyStates() {
    bool hasUnresolved = false;
	for (auto & dep : this->m_info.m_dependencies) {
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
                        dep.m_mod->logInfo(r.error(), Severity::Error);
                    }
                    else {
                    	auto r = dep.m_mod->enable();
                    	if (!r) {
	                        dep.m_state = ModResolveState::Disabled;
	                        dep.m_mod->logInfo(r.error(), Severity::Error);
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
			this->m_resolved = false;
            this->unload();
            hasUnresolved = true;
		}
	}
    if (!hasUnresolved && !this->m_resolved) {
        Log::get() << Severity::Debug << "All dependencies for " << m_info.m_id << " found";
        this->m_resolved = true;
        if (this->m_enabled) {
            Log::get() << Severity::Debug << "Resolved & loading " << m_info.m_id;
            auto r = this->load();
            if (!r) {
                Log::get() << Severity::Error << this << "Error loading: " << r.error();
            }
            else {
            	auto r = this->enable();
	            if (!r) {
	                Log::get() << Severity::Error << this << "Error enabling: " << r.error();
	            }
            }
        } else {
            Log::get() << Severity::Debug << "Resolved " << m_info.m_id << ", however not loading it as it is disabled";
        }
    }
    return hasUnresolved;
}

bool Mod::hasUnresolvedDependencies() const {
	for (auto const& dep : this->m_info.m_dependencies) {
		if (dep.isUnresolved()) {
			return true;
		}
	}
	return false;
}

std::vector<Dependency> Mod::getUnresolvedDependencies() {
    std::vector<Dependency> res;
	for (auto const& dep : this->m_info.m_dependencies) {
		if (dep.isUnresolved()) {
			res.push_back(dep);
		}
	}
    return res;
}

ghc::filesystem::path Mod::getSaveDir() const {
    return this->m_saveDirPath;
}

decltype(ModInfo::m_id) Mod::getID() const {
    return this->m_info.m_id;
}

decltype(ModInfo::m_name) Mod::getName() const {
    return this->m_info.m_name;
}

decltype(ModInfo::m_developer) Mod::getDeveloper() const {
    return this->m_info.m_developer;
}

decltype(ModInfo::m_description) Mod::getDescription() const {
    return this->m_info.m_description;
}

decltype(ModInfo::m_details) Mod::getDetails() const {
    return this->m_info.m_details;
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
    return this->m_info.m_path.string();
}

VersionInfo Mod::getVersion() const {
    return this->m_info.m_version;
}

bool Mod::isEnabled() const {
    return this->m_enabled;
}

bool Mod::isLoaded() const {
    return this->m_loaded;
}

bool Mod::supportsDisabling() const {
    return this->m_info.m_supportsDisabling;
}

bool Mod::supportsUnloading() const {
    return this->m_info.m_supportsUnloading;
}

bool Mod::wasSuccesfullyLoaded() const {
    return !this->isEnabled() || this->isLoaded();
}

std::vector<Hook*> Mod::getHooks() const {
    return this->m_hooks;
}

Log Mod::log() {
    return Log(this);
}

void Mod::logInfo(
    std::string const& info,
    Severity severity
) {
    Log l(this);
    l << severity << info;
}

bool Mod::depends(std::string const& id) const {
    return vector_utils::contains<Dependency>(
        this->m_info.m_dependencies,
        [id](Dependency t) -> bool { return t.m_id == id; }
    );
}

const char* Mod::expandSpriteName(const char* name) {
    static std::unordered_map<std::string, const char*> expanded = {};
    if (expanded.count(name)) {
        return expanded[name];
    }
    auto exp = new char[strlen(name) + 2 + this->m_info.m_id.size()];
    auto exps = this->m_info.m_id + "_" + name;
    memcpy(exp, exps.c_str(), exps.size() + 1);
    expanded[name] = exp;
    return exp;
}

std::string Mod::getLoadErrorInfo() const {
    return m_loadErrorInfo;
}

std::string sanitizeDetailsData(unsigned char* start, unsigned char* end) {
    // delete CRLF
    return string_utils::replace(std::string(start, end), "\r", "");
}

Result<ModInfo> ModInfo::createFromSchemaV010(
    nlohmann::json const& rawJson
) {
    ModInfo info;

    auto json = rawJson;

    try {

    std::set<std::string_view> knownKeys;

    json_check(knownKeys, json)
        .needs("id")
        .as<std::string>()
        .validate([](auto t) -> bool { return Mod::validateID(t.template get<std::string>()); })
        .into(info.m_id);

    json_check(knownKeys, json)
        .needs("version")
        .as<std::string>()
        .validate([](auto t) -> bool { return VersionInfo::validate(t.template get<std::string>()); })
        .into([&info](auto json) -> void { info.m_version = VersionInfo(json.template get<std::string>()); });

    json_assign_required(knownKeys, json, "name", info.m_name);
    json_assign_required(knownKeys, json, "developer", info.m_developer);
    json_assign_optional(knownKeys, json, "description", info.m_description);
    json_assign_optional(knownKeys, json, "repository", info.m_repository);

    json_check(knownKeys, json)
        .has("dependencies")
        .as<nlohmann::json::array_t>()
        .each([&info](json_check dep) -> void {
            dep.as<nlohmann::json::object_t>();
            auto depobj = Dependency {};
            std::set<std::string_view> knownKeys;
            json_check(knownKeys, dep)
                .needs("id")
                .as<std::string>()
                .into(depobj.m_id);
            json_check(knownKeys, dep)
                .has("version")
                .as<std::string>()
                .validate([&](auto t) -> bool { return VersionInfo::validate(t.template get<std::string>()); })
                .into([&info](auto json) -> void { info.m_version = VersionInfo(json.template get<std::string>()); });
            json_check(knownKeys, dep).has("required").as<bool>().into(depobj.m_required);
            json_check_unknown(knownKeys, dep.m_json, dep.m_hierarchy);
            info.m_dependencies.push_back(depobj);
        });
    
    json_check(knownKeys, json)
        .has("datastore")
        .as<nlohmann::json::object_t>()
        .into(info.m_defaultDataStore);
    
    json_check(knownKeys, json)
        .has("resources")
        .as<nlohmann::json::object_t>()
        .step()
        .has("spritesheets")
        .as<nlohmann::json::object_t>()
        .each([&info](auto key, auto) -> void {
            info.m_spritesheets.push_back(info.m_id + "_" + key);
        });
    
    json_assign_optional(knownKeys, json, "toggleable", info.m_supportsDisabling);
    json_assign_optional(knownKeys, json, "unloadable", info.m_supportsUnloading);

    knownKeys.insert("geode");
    knownKeys.insert("binary");
    knownKeys.insert("userdata");
    json_check_unknown(knownKeys, json, "");

    } catch(std::exception& e) {
        return Err<>(e.what());
    }

    if (json.contains("binary")) {
        bool autoEnd = true;
        if (json["binary"].is_string()) {
            info.m_binaryName = json["binary"];
        } else if (json["binary"].is_object()) {
            auto bo = json["binary"];
            if (bo.contains("*") && bo["*"].is_string()) {
                info.m_binaryName = bo["*"];
            }
            if (bo.contains("auto") && bo["auto"].is_boolean()) {
                autoEnd = bo["auto"];
            }
            #if defined(GEODE_IS_WINDOWS)
            if (bo.contains("windows") && bo["windows"].is_string()) {
                info.m_binaryName = bo["windows"];
            }
            #elif defined(GEODE_IS_MACOS)
            if (bo.contains("macos") && bo["macos"].is_string()) {
                info.m_binaryName = bo["macos"];
            }
            #elif defined(GEODE_IS_ANDROID)
            if (bo.contains("android") && bo["android"].is_string()) {
                info.m_binaryName = bo["android"];
            }
            #elif defined(GEODE_IS_IOS)
            if (bo.contains("ios") && bo["ios"].is_string()) {
                info.m_binaryName = bo["ios"];
            }
            #endif
        } else goto skip_binary_check;
        if (autoEnd && !string_utils::endsWith(info.m_binaryName, GEODE_PLATFORM_EXTENSION)) {
            info.m_binaryName += GEODE_PLATFORM_EXTENSION;
        }
    }
    skip_binary_check:

    return Ok(info);
}

Result<ModInfo> ModInfo::create(nlohmann::json const& json) {
    // Check mod.json target version
    auto schema = LOADER_VERSION;
    if (json.contains("geode") && json["geode"].is_string()) {
        auto ver = json["geode"];
        if (VersionInfo::validate(ver)) {
            schema = VersionInfo(ver);
        } else {
            return Err(
                "[mod.json] has no target loader version "
                "specified, or it is invalidally formatted (required: \"[v]X.X.X\")!"
            );
        }
    } else {
        return Err(
            "[mod.json] has no target loader version "
            "specified, or it is invalidally formatted (required: \"[v]X.X.X\")!"
        );
    }
    if (schema < Loader::s_supportedVersionMin) {
        return Err(
            "[mod.json] is built for an older version (" + 
            schema.toString() + ") of Geode (current: " + 
            Loader::s_supportedVersionMin.toString() +
            "). Please update the mod to the latest version, "
            "and if the problem persists, contact the developer "
            "to update it."
        );
    }
    if (schema > Loader::s_supportedVersionMax) {
        return Err(
            "[mod.json] is built for a newer version (" + 
            schema.toString() + ") of Geode (current: " +
            Loader::s_supportedVersionMax.toString() +
            "). You need to update Geode in order to use "
            "this mod."
        );
    }

    // Handle mod.json data based on target
    if (schema <= VersionInfo(0, 2, 0)) {
        return ModInfo::createFromSchemaV010(json);
    }

    return Err(
        "[mod.json] targets a version (" +
        schema.toString() + ") that isn't "
        "supported by this version (v" + 
        LOADER_VERSION_STR + ") of geode. "
        "This is probably a bug; report it to "
        "the Geode Development Team."
    );
}

Result<ModInfo> ModInfo::createFromFile(ghc::filesystem::path const& path) {
    try {
        auto read = file_utils::readString(path);
        if (!read) return Err(read.error());
        auto res = ModInfo::create(nlohmann::json::parse(read.value()));
        if (!res) return res;
        auto info = res.value();
        info.m_path = path;
        return Ok(info);
    } catch(std::exception const& e) {
        return Err(e.what());
    }
}

Result<ModInfo> ModInfo::createFromGeodeFile(ghc::filesystem::path const& path) {
    ZipFile unzip(path.string());
    if (!unzip.isLoaded()) {
        return Err<>("\"" + path.string() + "\": Unable to unzip");
    }
    // Check if mod.json exists in zip
    if (!unzip.fileExists("mod.json")) {
        return Err<>("\"" + path.string() + "\" is missing mod.json");
    }
    // Read mod.json & parse if possible
    unsigned long readSize = 0;
    auto read = unzip.getFileData("mod.json", &readSize);
    if (!read || !readSize) {
        return Err("\"" + path.string() + "\": Unable to read mod.json");
    }
    nlohmann::json json;
    try {
        json = nlohmann::json::parse(std::string(read, read + readSize));
    } catch(std::exception const& e) {
        delete[] read;
        return Err<>(e.what());
    }

    delete[] read;

    if (!json.is_object()) {
        return Err(
            "\"" + path.string() + "/mod.json\" does not have an "
            "object at root despite expected"
        );
    }

    auto res = ModInfo::create(json);
    if (!res) {
        return Err("\"" + path.string() + "\" - " + res.error());
    }
    auto info = res.value();
    info.m_path = path;
    
    // unzip known MD files
    using God = std::initializer_list<std::pair<std::string, std::string*>>;
    for (auto [file, target] : God {
        { "about.md", &info.m_details },
        { "changelog.md", &info.m_changelog },
    }) {
        if (unzip.fileExists(file)) {
            unsigned long readSize = 0;
            auto fileData = unzip.getFileData(file, &readSize);
            if (!fileData || !readSize) {
                return Err("Unable to read \"" + path.string() + "\"/" + file);
            } else {
                *target = sanitizeDetailsData(fileData, fileData + readSize);
            }
        }
    }

    return Ok(info);
}
