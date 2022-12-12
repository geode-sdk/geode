
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Dirs.hpp>
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
    ghc::filesystem::remove_all(dirs::getModRuntimeDir());
    ghc::filesystem::remove_all(dirs::getTempDir());
}

// Initialization

void Loader::createDirectories() {
#ifdef GEODE_IS_MACOS
    ghc::filesystem::create_directory(dirs::getSaveDir());
#endif

    ghc::filesystem::create_directories(dirs::getGeodeResourcesDir());
    ghc::filesystem::create_directory(dirs::getModConfigDir());
    ghc::filesystem::create_directory(dirs::getModsDir());
    ghc::filesystem::create_directory(dirs::getGeodeLogDir());
    ghc::filesystem::create_directory(dirs::getTempDir());
    ghc::filesystem::create_directory(dirs::getModRuntimeDir());

    if (!ranges::contains(m_modSearchDirectories, dirs::getModsDir())) {
        m_modSearchDirectories.push_back(dirs::getModsDir());
    }
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
    this->refreshModsList();

    this->queueInGDThread([]() {
        Loader::get()->addSearchPaths();
    });

    m_isSetup = true;

    return Ok();
}

void Loader::addSearchPaths() {
    CCFileUtils::get()->addPriorityPath(dirs::getGeodeResourcesDir().string().c_str());
    CCFileUtils::get()->addPriorityPath(dirs::getModRuntimeDir().string().c_str());
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

std::vector<Mod*> Loader::getAllMods() {
    return map::values(m_mods);
}

Mod* Loader::getInternalMod() {
    return InternalMod::get();
}

std::vector<InvalidGeodeFile> Loader::getFailedMods() const {
    return m_invalidMods;
}

// Version info

VersionInfo Loader::getVersion() {
    return LOADER_VERSION;
}

VersionInfo Loader::minModVersion() {
    return VersionInfo { 0, 3, 1 };
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

// Data saving

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

// Mod loading

Result<Mod*> Loader::loadModFromInfo(ModInfo const& info) {
    if (m_mods.count(info.id)) {
        return Err(fmt::format("Mod with ID '{}' already loaded", info.id));
    }

    // create Mod instance
    auto mod = new Mod(info);
    m_mods.insert({ info.id, mod });
    mod->m_enabled = InternalMod::get()->getSavedValue<bool>(
        "should-load-" + info.id, true
    );

    // add mod resources
    this->queueInGDThread([this, mod]() {
        auto searchPath = dirs::getModRuntimeDir() / mod->getID() / "resources";

        CCFileUtils::get()->addSearchPath(searchPath.string().c_str());
        this->updateModResources(mod);
    });

    // this loads the mod if its dependencies are resolved
    GEODE_UNWRAP(mod->updateDependencies());

    return Ok(mod);
}

Result<Mod*> Loader::loadModFromFile(ghc::filesystem::path const& file) {
    auto res = ModInfo::createFromGeodeFile(file);
    if (!res) {
        m_invalidMods.push_back(InvalidGeodeFile {
            .path = file,
            .reason = res.unwrapErr(),
        });
        return Err(res.unwrapErr());
    }
    return this->loadModFromInfo(res.unwrap());
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

void Loader::dispatchScheduledFunctions(Mod* mod) {
    std::lock_guard _(m_scheduledFunctionsMutex);
    for (auto& func : m_scheduledFunctions) {
        func();
    }
    m_scheduledFunctions.clear();
}

void Loader::scheduleOnModLoad(Mod* mod, ScheduledFunction func) {
    std::lock_guard _(m_scheduledFunctionsMutex);
    if (mod) {
        return func();
    }
    m_scheduledFunctions.push_back(func);
}

void Loader::updateModResources(Mod* mod) {
    if (!mod->m_info.spritesheets.size()) {
        return;
    }

    auto searchPath = dirs::getModRuntimeDir() / mod->getID() / "resources";

    log::debug("Adding resources for {}", mod->getID());

    // add spritesheets
    for (auto const& sheet : mod->m_info.spritesheets) {
        log::debug("Adding sheet {}", sheet);
        auto png = sheet + ".png";
        auto plist = sheet + ".plist";
        auto ccfu = CCFileUtils::get();

        if (png == std::string(ccfu->fullPathForFilename(png.c_str(), false)) ||
            plist == std::string(ccfu->fullPathForFilename(plist.c_str(), false))) {
            log::warn(
                "The resource dir of \"{}\" is missing \"{}\" png and/or plist files",
                mod->m_info.id, sheet
            );
        }
        else {
            CCTextureCache::get()->addImage(png.c_str(), false);
            CCSpriteFrameCache::get()->addSpriteFramesWithFile(plist.c_str());
        }
    }
}

// Dependencies and refreshing

void Loader::loadModsFromDirectory(
    ghc::filesystem::path const& dir,
    bool recursive
) {
    log::debug("Searching {}", dir);
    for (auto const& entry : ghc::filesystem::directory_iterator(dir)) {
        // recursively search directories
        if (ghc::filesystem::is_directory(entry) && recursive) {
            this->loadModsFromDirectory(entry.path(), true);
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
            return p->m_info.path == entry.path();
        })) {
            continue;
        }

        // if mods should be loaded immediately, do that
        if (m_earlyLoadFinished) {
            auto load = this->loadModFromFile(entry);
            if (!load) {
                log::error("Unable to load {}: {}", entry, load.unwrapErr());
            }
        }
        // otherwise collect mods to load first to make sure the correct 
        // versions of the mods are loaded and that early-loaded mods are 
        // loaded early
        else {
            auto res = ModInfo::createFromGeodeFile(entry.path());
            if (!res) {
                m_invalidMods.push_back(InvalidGeodeFile {
                    .path = entry.path(),
                    .reason = res.unwrapErr(),
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
}

void Loader::refreshModsList() {
    log::debug("Loading mods...");

    // find mods
    for (auto& dir : m_modSearchDirectories) {
        this->loadModsFromDirectory(dir);
    }
    
    // load early-load mods first
    for (auto& mod : m_modsToLoad) {
        if (mod.needsEarlyLoad) {
            auto load = this->loadModFromInfo(mod);
            if (!load) {
                log::error("Unable to load {}: {}", mod.id, load.unwrapErr());
            }
        }
    }

    // UI can be loaded now
    m_earlyLoadFinished = true;

    // load the rest of the mods
    for (auto& mod : m_modsToLoad) {
        if (!mod.needsEarlyLoad) {
            auto load = this->loadModFromInfo(mod);
            if (!load) {
                log::error("Unable to load {}: {}", mod.id, load.unwrapErr());
            }
        }
    }
    m_modsToLoad.clear();
}

void Loader::updateAllDependencies() {
    for (auto const& [_, mod] : m_mods) {
        (void)mod->updateDependencies();
    }
}

void Loader::waitForModsToBeLoaded() {
    while (!m_earlyLoadFinished) {}
}

// Misc

void Loader::queueInGDThread(ScheduledFunction func) {
    InternalLoader::get()->queueInGDThread(func);
}

bool Loader::didLastLaunchCrash() const {
    return crashlog::didLastLaunchCrash();
}

void Loader::openPlatformConsole() {
    InternalLoader::get()->openPlatformConsole();
}

void Loader::closePlatfromConsole() {
    InternalLoader::get()->closePlatformConsole();
}
