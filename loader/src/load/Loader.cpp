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
#include <Geode/Geode.hpp>
#include <about.hpp>
#include <crashlog.hpp>

USE_GEODE_NAMESPACE();

bool Loader::s_unloading = false;
std::mutex g_unloadMutex;

VersionInfo Loader::getVersion() const {
    return LOADER_VERSION;
}

std::string Loader::getVersionType() const {
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

    if (!vector_utils::contains(m_modDirectories, modDir)) {
        m_modDirectories.push_back(modDir);
    }

    // files too
    m_logStream = std::ofstream(logDir / generateLogName());
}

void Loader::addModResourcesPath(Mod* mod) {
    if (mod->m_addResourcesToSearchPath) {
        CCFileUtils::sharedFileUtils()->addSearchPath(
            mod->m_tempDirName.string().c_str()
        );
        CCFileUtils::sharedFileUtils()->addSearchPath(
            (mod->m_tempDirName / "resources").string().c_str()
        );
    }
}

void Loader::updateResourcePaths() {
	auto resDir = this->getGeodeDirectory() / GEODE_RESOURCE_DIRECTORY;
    CCFileUtils::sharedFileUtils()->addSearchPath(resDir.string().c_str());
    for (auto const& [_, mod] : m_mods) {
        this->addModResourcesPath(mod);
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
            InternalMod::get()->logInfo(
                "The resource dir of \"" + mod->m_info.m_id +
                "\" is missing \"" + sheet + "\" png and/or plist files",
                Severity::Warning
            );
        } else {
            CCTextureCache::sharedTextureCache()->addImage(png.c_str(), false);
            CCSpriteFrameCache::sharedSpriteFrameCache()
                ->addSpriteFramesWithFile(plist.c_str());
        }
    }
}

void Loader::updateResources() {
    for (auto const& [_, mod] : m_mods) {
        this->updateModResources(mod);
    }
}

size_t Loader::loadModsFromDirectory(
    ghc::filesystem::path const& dir, bool recursive
) {
    InternalMod::get()->log() << Severity::Debug << "Searching " << dir;
        
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
        if (map_utils::contains<std::string, Mod*>(
            m_mods,
            [entry](Mod* p) -> bool {
                return p->m_info.m_path == entry.path();
            }
        )) continue;

        // load mod

        InternalMod::get()->log()
            << Severity::Debug
            << "Loading " << entry.path().string();

        auto res = this->loadModFromFile(entry.path().string());
        if (res && res.value()) {
            // succesfully loaded
            loadedCount++;

            // check for dependencies
            if (!res.value()->hasUnresolvedDependencies()) {
                InternalMod::get()->log()
                    << "Succesfully loaded " << res.value();
            } else {
                InternalMod::get()->log()
                    << res.value() << " has unresolved dependencies";
            }
        } else {
            // something went wrong
            InternalMod::get()->logInfo(res.error(), Severity::Error);
            m_erroredMods.push_back({ entry.path().string(), res.error() });
        }
    }
    return loadedCount;
}

size_t Loader::refreshMods() {
    InternalMod::get()->log() << Severity::Debug << "Loading mods...";
    
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

    InternalMod::get()->log()
        << Severity::Debug
        << "Loaded " << loadedCount << " mods";
    return loadedCount;
}

Result<> Loader::saveSettings() {
    auto json = nlohmann::json::object();
    json["mods"] = nlohmann::json::object();
    for (auto [id, mod] : m_mods) {
        if (mod->isUninstalled()) continue;
        auto value = nlohmann::json::object();
        value["enabled"] = mod->m_enabled;
        mod->saveDataStore();
        json["mods"][id] = value;
    }
    json["succesfully-closed"] = true;
    auto path = this->getGeodeSaveDirectory() / "mods.json";
    return file_utils::writeString(path, json.dump(4));
}

Result<> Loader::loadSettings() {
    auto path = this->getGeodeSaveDirectory() / "mods.json";
    if (!ghc::filesystem::exists(path)) {
        return Ok();
    }

    auto read = file_utils::readString(path);
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
    return map_utils::getValues(m_mods);
}

std::vector<Loader::FailedModInfo> const& Loader::getFailedMods() const {
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

    InternalMod::get()->log()
        << Severity::Debug
        << "Setting up Loader...";

    this->createDirectories();
    this->loadSettings();
    this->refreshMods();

    if (crashlog::setupPlatformHandler()) {
        InternalMod::get()->log()
            << Severity::Debug
            << "Set up platform crash logger";
    } else {
        InternalMod::get()->log()
            << Severity::Debug
            << "Unable to set up platform crash logger";
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
    for (auto const& log : m_logs) {
        delete log;
    }
    m_logs.clear();

    auto tempDir = this->getGeodeDirectory() / GEODE_TEMP_DIRECTORY;
    ghc::filesystem::remove_all(tempDir);
}

void Loader::pushLog(LogPtr* logptr) {
    m_logs.push_back(logptr);

    #ifdef GEODE_PLATFORM_CONSOLE
    if (InternalLoader::get()->platformConsoleReady()) {
        std::cout << logptr->toString(true);
    } else {
        InternalLoader::get()->queueConsoleMessage(logptr);
    }
    #endif

    m_logStream << logptr->toString(true) << std::endl;
}

void Loader::popLog(LogPtr* log) {
    vector_utils::erase(m_logs, log);
    delete log;
}

std::vector<LogPtr*> const& Loader::getLogs() const {
    return m_logs;
}

std::vector<LogPtr*> Loader::getLogs(
    std::initializer_list<Severity> severityFilter
) {
    if (!severityFilter.size()) {
        return m_logs;
    }

    std::vector<LogPtr*> logs;

    for (auto const& log : m_logs) {
        if (vector_utils::contains<Severity>(
            severityFilter, log->getSeverity()
        )) {
            logs.push_back(log);
        }
    }

    return logs;
}

void Loader::queueInGDThread(std::function<void GEODE_CALL()> func) {
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
    return geode::utils::dirs::geodeRoot() / GEODE_DIRECTORY;
}

ghc::filesystem::path Loader::getGeodeSaveDirectory() const {
    return this->getSaveDirectory() / GEODE_DIRECTORY;
}

size_t Loader::getFieldIndexForClass(size_t hash) {
	static std::unordered_map<size_t, size_t> nextIndex;
	return nextIndex[hash]++;
}
