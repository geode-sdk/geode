
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <InternalLoader.hpp>
#include <InternalMod.hpp>
#include <about.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/utils/map.hpp>
#include <crashlog.hpp>

USE_GEODE_NAMESPACE();

Loader* Loader::get() {
    return InternalLoader::get();
}

Loader::~Loader() {
    for (auto& [_, mod] : m_mods) {
        delete mod;
    }
    m_mods.clear();
    log::Logs::clear();
    ghc::filesystem::remove_all(
        this->getGeodeDirectory() / GEODE_TEMP_DIRECTORY
    );
}

VersionInfo Loader::getVersion() {
    return LOADER_VERSION;
}

VersionInfo Loader::minModVersion() {
    return VersionInfo { 0, 7, 0 };
}

VersionInfo Loader::maxModVersion() {
    return VersionInfo {
        Loader::getVersion().getMajor(),
        Loader::getVersion().getMinor(),
        // todo: dynamic version info (vM.M.*)
        99999999,
    };
}

bool Loader::isModVersionSupported(VersionInfo const& version) {
    return
        version >= Loader::minModVersion() &&
        version <= Loader::maxModVersion();
}

void Loader::createDirectories() {
    auto modDir = this->getGeodeDirectory() / GEODE_MOD_DIRECTORY;
    auto logDir = this->getGeodeDirectory() / GEODE_LOG_DIRECTORY;
    auto resDir = this->getGeodeDirectory() / GEODE_RESOURCE_DIRECTORY;
    auto tempDir = this->getGeodeDirectory() / GEODE_TEMP_DIRECTORY;
    auto confDir = this->getGeodeDirectory() / GEODE_CONFIG_DIRECTORY;

#ifdef GEODE_IS_MACOS
    ghc::filesystem::create_directory(this->getSaveDirectory());
#endif

    ghc::filesystem::create_directories(resDir);
    ghc::filesystem::create_directory(confDir);
    ghc::filesystem::create_directory(modDir);
    ghc::filesystem::create_directory(logDir);
    ghc::filesystem::create_directory(tempDir);

    if (!ranges::contains(m_modSearchDirectories, modDir)) {
        m_modSearchDirectories.push_back(modDir);
    }
}

Result<> Loader::saveData() {
    // save mods' data
    for (auto& [_, mod] : m_mods) {
        auto r = mod->saveData();
        if (!r) {
            log::warn("Unable to save data for mod \"{}\": {}", mod->getID(), r.unwrapErr());
        }
    }
    // save loader data
    GEODE_UNWRAP(InternalMod::get()->saveData());
    
    return Ok();
}

Result<> Loader::loadData() {
    auto e = InternalMod::get()->loadData();
    if (!e) {
        log::warn("Unable to load loader settings: {}", e.unwrapErr());
    }
    for (auto& [_, mod] : m_mods) {
        auto r = mod->loadData();
        if (!r) {
            log::warn("Unable to load data for mod \"{}\": {}", mod->getID(), r.unwrapErr());
        }
    }
    return Ok();
}

Result<> Loader::setup() {
    if (m_isSetup) {
        return Ok();
    }

    log::Logs::setup();

    if (crashlog::setupPlatformHandler()) {
        log::debug("Set up platform crash logger");
    }
    else {
        log::debug("Unable to set up platform crash logger");
    }

    log::debug("Setting up Loader...");

    this->createDirectories();
    auto sett = this->loadData();
    if (!sett) {
        log::warn("Unable to load loader settings: {}", sett.unwrapErr());
    }
    GEODE_UNWRAP(this->refreshModsList());

    this->queueInGDThread([]() {
        Loader::get()->addSearchPaths();
    });

    m_isSetup = true;

    return Ok();
}

Result<Mod*> Loader::loadModFromInfo(ModInfo const& info) {
    if (m_mods.count(info.m_id)) {
        return Err(fmt::format("Mod with ID '{}' already loaded", info.m_id));
    }

    // create Mod instance
    auto mod = new Mod(info);
    m_mods.insert({ info.m_id, mod });
    mod->m_enabled = InternalMod::get()->getSavedValue<bool>(
        "should-load-" + info.m_id, true
    );
    // this loads the mod if its dependencies are resolved
    mod->updateDependencyStates();

    // add mod resources
    this->queueInGDThread([this, mod]() {
        auto searchPath = this->getGeodeDirectory() /
            GEODE_TEMP_DIRECTORY / mod->getID() / "resources";

        CCFileUtils::get()->addSearchPath(searchPath.string().c_str());
        this->updateModResources(mod);
    });

    return Ok(mod);
}

Result<Mod*> Loader::loadModFromFile(ghc::filesystem::path const& file) {
    auto res = ModInfo::createFromGeodeFile(file);
    if (!res) {
        m_invalidMods.push_back(InvalidGeodeFile {
            .m_path = file,
            .m_reason = res.unwrapErr(),
        });
        return Err(res.unwrapErr());
    }
    return this->loadModFromInfo(res.unwrap());
}

Result<> Loader::loadModsFromDirectory(
    ghc::filesystem::path const& dir,
    bool recursive
) {
    log::debug("Searching {}", dir);
    for (auto const& entry : ghc::filesystem::directory_iterator(dir)) {
        // recursively search directories
        if (ghc::filesystem::is_directory(entry) && recursive) {
            GEODE_UNWRAP(this->loadModsFromDirectory(entry.path(), true));
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
        if (map::contains<std::string, Mod*>(m_mods, [entry](Mod* p) -> bool {
            return p->m_info.m_path == entry.path();
        })) {
            continue;
        }

        // if mods should be loaded immediately, do that
        if (m_earlyLoadFinished) {
            GEODE_UNWRAP(this->loadModFromFile(entry));
        }
        // otherwise collect mods to load first to make sure the correct 
        // versions of the mods are loaded and that early-loaded mods are 
        // loaded early
        else {
            auto res = ModInfo::createFromGeodeFile(entry.path());
            if (!res) {
                m_invalidMods.push_back(InvalidGeodeFile {
                    .m_path = entry.path(),
                    .m_reason = res.unwrapErr(),
                });
                continue;
            }
            auto info = res.unwrap();

            // skip this entry if it's already set to be loaded
            if (ranges::contains(m_modsToLoad, info)) {
                continue;
            }

            // add to list of mods to load
            m_modsToLoad.push_back(info);
        }
    }
    return Ok();
}

Result<> Loader::refreshModsList() {
    log::debug("Loading mods...");

    // find mods
    for (auto& dir : m_modSearchDirectories) {
        GEODE_UNWRAP(this->loadModsFromDirectory(dir));
    }
    
    // load early-load mods first
    for (auto& mod : m_modsToLoad) {
        if (mod.m_needsEarlyLoad) {
            GEODE_UNWRAP(this->loadModFromInfo(mod));
        }
    }

    // UI can be loaded now
    m_earlyLoadFinished = true;

    // load the rest of the mods
    for (auto& mod : m_modsToLoad) {
        if (!mod.m_needsEarlyLoad) {
            GEODE_UNWRAP(this->loadModFromInfo(mod));
        }
    }
    m_modsToLoad.clear();

    return Ok();
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

std::vector<Mod*> Loader::getAllMods() {
    return map::getValues(m_mods);
}

Mod* Loader::getInternalMod() {
    return InternalMod::get();
}

std::vector<InvalidGeodeFile> Loader::getFailedMods() const {
    return m_invalidMods;
}

void Loader::updateAllDependencies() {
    for (auto const& [_, mod] : m_mods) {
        mod->updateDependencyStates();
    }
}

void Loader::waitForModsToBeLoaded() {
    while (!m_earlyLoadFinished) {}
}

void Loader::dispatchScheduledFunctions(Mod* mod) {
    std::lock_guard _(m_scheduledFunctionsMutex);
    for (auto& func : m_scheduledFunctions) {
        func();
    }
    m_scheduledFunctions.clear();
}

void Loader::queueInGDThread(ScheduledFunction func) {
    InternalLoader::get()->queueInGDThread(func);
}

void Loader::scheduleOnModLoad(Mod* mod, ScheduledFunction func) {
    std::lock_guard _(m_scheduledFunctionsMutex);
    if (mod) {
        return func();
    }
    m_scheduledFunctions.push_back(func);
}

bool Loader::didLastLaunchCrash() const {
    return crashlog::didLastLaunchCrash();
}

ghc::filesystem::path Loader::getCrashLogDirectory() const {
    return crashlog::getCrashLogDirectory();
}

void Loader::openPlatformConsole() {
    InternalLoader::get()->openPlatformConsole();
}

void Loader::closePlatfromConsole() {
    InternalLoader::get()->closePlatformConsole();
}

void Loader::updateModResources(Mod* mod) {
    if (!mod->m_info.m_spritesheets.size()) {
        return;
    }

    auto searchPath = this->getGeodeDirectory() / GEODE_TEMP_DIRECTORY / mod->getID() / "resources";

    log::debug("Adding resources for {}", mod->getID());

    // add spritesheets
    for (auto const& sheet : mod->m_info.m_spritesheets) {
        log::debug("Adding sheet {}", sheet);
        auto png = sheet + ".png";
        auto plist = sheet + ".plist";
        auto ccfu = CCFileUtils::get();

        if (png == std::string(ccfu->fullPathForFilename(png.c_str(), false)) ||
            plist == std::string(ccfu->fullPathForFilename(plist.c_str(), false))) {
            log::warn(
                "The resource dir of \"{}\" is missing \"{}\" png and/or plist files",
                mod->m_info.m_id, sheet
            );
        }
        else {
            CCTextureCache::get()->addImage(png.c_str(), false);
            CCSpriteFrameCache::get()->addSpriteFramesWithFile(plist.c_str());
        }
    }
}

void Loader::addSearchPaths() {
    CCFileUtils::get()->addPriorityPath(
        (this->getGeodeDirectory() / GEODE_RESOURCE_DIRECTORY).string().c_str()
    );
    CCFileUtils::get()->addPriorityPath(
        (this->getGeodeDirectory() / GEODE_TEMP_DIRECTORY).string().c_str()
    );
}

void Loader::updateResources() {
    log::debug("Adding resources");

    // add own spritesheets
    this->updateModResources(InternalMod::get());

    // add mods' spritesheets
    for (auto const& [_, mod] : m_mods) {
        this->updateModResources(mod);
    }
}

ghc::filesystem::path Loader::getGameDirectory() const {
    return ghc::filesystem::path(CCFileUtils::sharedFileUtils()->getWritablePath2().c_str());
}

ghc::filesystem::path Loader::getSaveDirectory() const {
    #ifdef GEODE_IS_MACOS
        // not using ~/Library/Caches
        return ghc::filesystem::path("/Users/Shared/Geode");
    #elif defined(GEODE_IS_WINDOWS)
        return ghc::filesystem::path(
            ghc::filesystem::weakly_canonical(
                CCFileUtils::sharedFileUtils()->getWritablePath().c_str()
            ).string()
        );
    #else
        return ghc::filesystem::path(
            CCFileUtils::sharedFileUtils()->getWritablePath().c_str()
        );
    #endif
}

ghc::filesystem::path Loader::getGeodeDirectory() const {
    return geode::utils::file::geodeRoot() / GEODE_DIRECTORY;
}

ghc::filesystem::path Loader::getGeodeSaveDirectory() const {
    return this->getSaveDirectory() / GEODE_DIRECTORY;
}
