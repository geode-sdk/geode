#include <Geode/loader/Hook.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Loader.hpp>
#include <InternalLoader.hpp>
#include <InternalMod.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/conststring.hpp>
#include <Geode/utils/vector.hpp>
#include <Geode/utils/map.hpp>
#include <Geode/utils/types.hpp>
#include <mutex>
#include <about.hpp>
#include <crashlog.hpp>

USE_GEODE_NAMESPACE();

bool Loader::s_unloading = false;
std::mutex g_unloadMutex;

VersionInfo Loader::getVersion() {
    return LOADER_VERSION;
}

std::string Loader::getVersionType() {
    return LOADER_VERSION_TYPE;
}

Loader* Loader::get() {
    return InternalLoader::get();
}

void Loader::createDirectories() {
    auto modDir = this->getGeodeDirectory() / GEODE_MOD_DIRECTORY;
    auto logDir = this->getGeodeDirectory() / GEODE_LOG_DIRECTORY;
    auto resDir = this->getGeodeDirectory() / GEODE_RESOURCE_DIRECTORY;
    auto tempDir = this->getGeodeDirectory() / GEODE_TEMP_DIRECTORY;

    ghc::filesystem::create_directories(resDir);
    ghc::filesystem::create_directory(modDir);
    ghc::filesystem::create_directory(logDir);
    ghc::filesystem::create_directory(tempDir);

    if (!utils::vector::contains(m_modDirectories, modDir)) {
        m_modDirectories.push_back(modDir);
    }

    // files too
    m_logStream = std::ofstream(logDir / log::generateLogName());
}

void Loader::updateResourcePaths() {
    // add own geode/resources directory
    CCFileUtils::sharedFileUtils()->addSearchPath(
        (this->getGeodeDirectory() / GEODE_RESOURCE_DIRECTORY).string().c_str()
    );

    // add geode/temp for accessing root resources in mods
    auto tempDir = this->getGeodeDirectory() / GEODE_TEMP_DIRECTORY;
    CCFileUtils::sharedFileUtils()->addSearchPath(tempDir.string().c_str());

    // add geode/temp/mod.id/resources for accessing additional resources in mods
    for (auto& [_, mod] : m_mods) {
        if (mod->m_addResourcesToSearchPath) {
            CCFileUtils::sharedFileUtils()->addSearchPath(
                (tempDir / mod->getID() / "resources").string().c_str()
            );
        }
    }
}

void Loader::updateModResources(Mod* mod) {
    for (auto const& sheet : mod->m_info.m_spritesheets) {
        auto png = sheet + ".png";
        auto plist = sheet + ".plist";
        auto ccfu = CCFileUtils::sharedFileUtils();

        if (
            png == std::string(
                ccfu->fullPathForFilename(png.c_str(), false)
            ) ||
            plist == std::string(
                ccfu->fullPathForFilename(plist.c_str(), false)
            )
        ) {
            log::warn("The resource dir of \"{}\" is missing \"{}\" png and/or plist files", mod->m_info.m_id, sheet);
        } else {
            CCTextureCache::sharedTextureCache()->addImage(png.c_str(), false);
            CCSpriteFrameCache::sharedSpriteFrameCache()
                ->addSpriteFramesWithFile(plist.c_str());
        }
    }
}

void Loader::updateResources() {
    // add own spritesheets
    this->updateModResources(InternalMod::get());

    // add mods' spritesheets
    for (auto const& [_, mod] : m_mods) {
        this->updateModResources(mod);
    }
}

size_t Loader::loadModsFromDirectory(
    ghc::filesystem::path const& dir, bool recursive
) {
    log::debug("Searching {}", dir);
        
    size_t loadedCount = 0;
    for (auto const& entry : ghc::filesystem::directory_iterator(dir)) {
        // recursively search directories
        if (ghc::filesystem::is_directory(entry) && recursive) {
            loadedCount += this->loadModsFromDirectory(entry.path(), true);
            continue;
        }

        // skip this entry if it's not a file
        if (!ghc::filesystem::is_regular_file(entry)) {
            continue;
        }

        // skip this entry if its extension is not .geode 
        if (entry.path().extension() != GEODE_MOD_EXTENSION) {
            continue;
        }

        // skip this entry if it's already loaded
        if (utils::map::contains<std::string, Mod*>(
            m_mods,
            [entry](Mod* p) -> bool {
                return p->m_info.m_path == entry.path();
            }
        )) continue;

        // load mod

        log::debug("Loading {}", entry.path().string());

        auto res = this->loadModFromFile(entry.path().string());
        if (res && res.value()) {
            // succesfully loaded
            loadedCount++;

            // check for dependencies
            if (!res.value()->hasUnresolvedDependencies()) {
                log::debug("Successfully loaded {}", res.value());
            } else {
                log::error("{} has unresolved dependencies", res.value());
            }
        } else {
            // something went wrong
            log::error("{}", res.error());
            m_erroredMods.push_back({ entry.path().string(), res.error() });
        }
    }
    return loadedCount;
}

size_t Loader::refreshMods() {
    log::debug("Loading mods...");
    
    // clear errored mods since that list will be 
    // reconstructed from scratch
    m_erroredMods.clear();

    // make sure mod directory exists
    this->createDirectories();

    size_t loadedCount = 0;

    // find all mods in search directories
    for (auto const& dir : m_modDirectories) {
        // find all mods in this search directory
        loadedCount += loadModsFromDirectory(dir, true);
    }

    log::debug("Loaded {} mods", loadedCount);
    return loadedCount;
}

Result<> Loader::saveSettings() {
    auto json = nlohmann::json::object();

    // save mod enabled / disabled states
    json["mods"] = nlohmann::json::object();
    for (auto [id, mod] : m_mods) {
        if (mod->isUninstalled()) continue;
        auto value = nlohmann::json::object();
        value["enabled"] = mod->m_enabled;

        // save mod's settings
        auto saveSett = mod->saveSettings();
        if (!saveSett) {
            return Err(saveSett.error());
        }

        json["mods"][id] = value;
    }

    // save loader settings
    auto saveIS = InternalMod::get()->saveSettings();
    if (!saveIS) {
        return Err(saveIS.error());
    }

    // save info alerts
    InternalLoader::get()->saveInfoAlerts(json);

    // mark the game as not having crashed
    json["succesfully-closed"] = true;

    return utils::file::writeString(
        this->getGeodeSaveDirectory() / "mods.json",
        json.dump(4)
    );
}

Result<> Loader::loadSettings() {
    auto path = this->getGeodeSaveDirectory() / "mods.json";
    if (!ghc::filesystem::exists(path)) {
        return Ok();
    }

    // read mods.json
    auto read = utils::file::readString(path);
    if (!read) {
        return read;
    }
    try {
        auto json = nlohmann::json::parse(read.value());
        if (json.contains("mods")) {
            auto mods = json["mods"];
            if (!mods.is_object()) {
                return Err("[loader settings].mods is not an object");
            }
            for (auto [key, val] : mods.items()) {
                if (!val.is_object()) {
                    return Err(
                        "[loader settings].mods.\"" + key +
                        "\" is not an object"
                    );
                }
                LoaderSettings::ModSettings mod;
                if (val.contains("enabled")) {
                    if (!val["enabled"].is_boolean()) {
                        return Err(
                            "[loader settings].mods.\"" + key +
                            "\".enabled is not a boolean"
                        );
                    }
                    mod.m_enabled = val["enabled"];
                }
                m_loadedSettings.m_mods.insert({ key, mod });
            }
        }
        InternalLoader::get()->loadInfoAlerts(json);
        return Ok();
    } catch(std::exception const& e) {
        return Err(e.what());
    }
}

bool Loader::didLastLaunchCrash() const {
    return crashlog::didLastLaunchCrash();
}

ghc::filesystem::path Loader::getCrashLogDirectory() const {
    return crashlog::getCrashLogDirectory();
}

bool Loader::shouldLoadMod(std::string const& id) const {
    if (m_loadedSettings.m_mods.count(id)) {
        return m_loadedSettings.m_mods.at(id).m_enabled;
    }
    return true;
}

bool Loader::isModInstalled(std::string const& id) const {
    return m_mods.count(id) && !m_mods.at(id)->isUninstalled();
}

Mod* Loader::getInstalledMod(std::string const& id) const {
    if (m_mods.count(id) && !m_mods.at(id)->isUninstalled()) {
        return m_mods.at(id);
    }
    return nullptr;
}

bool Loader::isModLoaded(std::string const& id) const {
    return m_mods.count(id) && m_mods.at(id)->isLoaded();
}

Mod* Loader::getLoadedMod(std::string const& id) const {
    if (m_mods.count(id)) {
        auto mod = m_mods.at(id);
        if (mod->isLoaded()) {
            return mod;
        }
    }
    return nullptr;
}

std::vector<Mod*> Loader::getAllMods() const {
    return utils::map::getValues(m_mods);
}

std::vector<Loader::FailedModInfo> Loader::getFailedMods() const {
    return m_erroredMods;
}

void Loader::updateAllDependencies() {
    for (auto const& [_, mod] : m_mods) {
        mod->updateDependencyStates();
    }
}

void Loader::unloadMod(Mod* mod) {
    m_mods.erase(mod->m_info.m_id);
    // ~Mod will call FreeLibrary 
    // automatically
    delete mod;
}

bool Loader::setup() {
    if (m_isSetup)
        return true;

    log::debug("Setting up Loader...");

    this->createDirectories();
    this->loadSettings();
    this->refreshMods();

    // add resources on startup
    this->queueInGDThread([]() {
        Loader::get()->updateResourcePaths();
    });

    if (crashlog::setupPlatformHandler()) {
        log::debug("Set up platform crash logger");
    } else {
        log::debug("Unable to set up platform crash logger");
    }

    m_isSetup = true;

    return true;
}

Loader::~Loader() {
    g_unloadMutex.lock();
    s_unloading = true;
    g_unloadMutex.unlock();
    for (auto const& [_, mod] : m_mods) {
        delete mod;
    }
    m_mods.clear();
    m_logs.clear();

    auto tempDir = this->getGeodeDirectory() / GEODE_TEMP_DIRECTORY;
    ghc::filesystem::remove_all(tempDir);
}

void Loader::pushLog(log::Log&& log) {
    std::string logStr = log.toString(true);

    InternalLoader::get()->logConsoleMessage(logStr);
    m_logStream << logStr << std::endl;

    m_logs.push_back(std::move(log));
}

void Loader::popLog(log::Log* log) {
    /*for (auto i = m_logs.begin(); i < m_logs.end(); ++i) {
        if (i == log) {
            m_logs.erase(i);
        }
    }*/
    utils::vector::erase(m_logs, *log);
}

std::vector<log::Log*> Loader::getLogs(
    std::initializer_list<Severity> severityFilter
) {
    std::vector<log::Log*> logs;

    for (auto& log : m_logs) {
        if (utils::vector::contains<Severity>(
            severityFilter, log.getSeverity()
        ) || !severityFilter.size()) {
            logs.push_back(&log);
        }
    }

    return logs;
}

void Loader::clearLogs() {
    m_logs.clear();
}

void Loader::queueInGDThread(ScheduledFunction func) {
    InternalLoader::get()->queueInGDThread(func);
}

Mod* Loader::getInternalMod() {
    return InternalMod::get();
}

bool Loader::isUnloading() {
    return Loader::s_unloading;
}

ghc::filesystem::path Loader::getGameDirectory() const {
    return ghc::filesystem::path(
        CCFileUtils::sharedFileUtils()->getWritablePath2().c_str()
    );
}

ghc::filesystem::path Loader::getSaveDirectory() const {
    return ghc::filesystem::path(
        CCFileUtils::sharedFileUtils()->getWritablePath().c_str()
    );
}

ghc::filesystem::path Loader::getGeodeDirectory() const {
    return geode::utils::file::geodeRoot() / GEODE_DIRECTORY;
}

ghc::filesystem::path Loader::getGeodeSaveDirectory() const {
    return this->getSaveDirectory() / GEODE_DIRECTORY;
}

size_t Loader::getFieldIndexForClass(size_t hash) {
	static std::unordered_map<size_t, size_t> nextIndex;
	return nextIndex[hash]++;
}

VersionInfo Loader::minModVersion() {
    // patches are always backwards-compatible. if not, we have failed
    return VersionInfo {
        Loader::getVersion().getMajor(),
        Loader::getVersion().getMinor(),
        0,
    };
}

VersionInfo Loader::maxModVersion() {
    // patches are always backwards-compatible. if not, we have failed
    return VersionInfo {
        Loader::getVersion().getMajor(),
        Loader::getVersion().getMinor(),
        99999999,
    };
}

bool Loader::supportedModVersion(VersionInfo const& version) {
    return 
        version >= Loader::minModVersion() &&
        version <= Loader::maxModVersion();
}

void Loader::openPlatformConsole() {
    InternalLoader::get()->openPlatformConsole();
}

void Loader::closePlatfromConsole() {
    InternalLoader::get()->closePlatformConsole();
}

void Loader::scheduleOnModLoad(Mod* m, ScheduledFunction func) {
    if (m) return func();
    m_scheduledFunctions.push_back(func);
}

void Loader::releaseScheduledFunctions(Mod* mod) {
    for (auto& func : m_scheduledFunctions) {
        func();
    }
    m_scheduledFunctions.clear();
}
