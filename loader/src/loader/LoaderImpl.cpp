
#include "LoaderImpl.hpp"
#include <cocos2d.h>
#include <Geode/loader/Dirs.hpp>
#include <Geode/loader/IPC.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/ModJsonTest.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/map.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include "ModImpl.hpp"
#include "ModInfoImpl.hpp"
#include <about.hpp>
#include <crashlog.hpp>
#include <fmt/format.h>
#include <hash.hpp>
#include <iostream>
#include <resources.hpp>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

using namespace geode::prelude;

Loader::Impl* LoaderImpl::get() {
    return Loader::get()->m_impl.get();
}

Loader::Impl::Impl() {}

Loader::Impl::~Impl() {}

// Initialization

void Loader::Impl::createDirectories() {
#ifdef GEODE_IS_MACOS
    ghc::filesystem::create_directory(dirs::getSaveDir());
#endif

    // try deleting geode/unzipped if it already exists
    try { ghc::filesystem::remove_all(dirs::getModRuntimeDir()); } catch(...) {}

    (void) utils::file::createDirectoryAll(dirs::getGeodeResourcesDir());
    (void) utils::file::createDirectory(dirs::getModConfigDir());
    (void) utils::file::createDirectory(dirs::getModsDir());
    (void) utils::file::createDirectory(dirs::getGeodeLogDir());
    (void) utils::file::createDirectory(dirs::getTempDir());
    (void) utils::file::createDirectory(dirs::getModRuntimeDir());

    if (!ranges::contains(m_modSearchDirectories, dirs::getModsDir())) {
        m_modSearchDirectories.push_back(dirs::getModsDir());
    }
}

Result<> Loader::Impl::setup() {
    if (m_isSetup) {
        return Ok();
    }

    log::Logger::setup();

    if (crashlog::setupPlatformHandler()) {
        log::debug("Set up platform crash logger");
    }
    else {
        log::debug("Unable to set up platform crash logger");
    }

    log::debug("Setting up Loader...");

    log::debug("Set up internal mod representation");
    log::debug("Loading hooks... ");

    if (!this->loadHooks()) {
        return Err("There were errors loading some hooks, see console for details");
    }

    log::debug("Loaded hooks");

    log::debug("Setting up IPC...");

    this->setupIPC();

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

void Loader::Impl::addSearchPaths() {
    CCFileUtils::get()->addPriorityPath(dirs::getGeodeResourcesDir().string().c_str());
    CCFileUtils::get()->addPriorityPath(dirs::getModRuntimeDir().string().c_str());
}

void Loader::Impl::updateResources() {
    this->updateResources(true);
}

void Loader::Impl::updateResources(bool forceReload) {
    log::debug("Adding resources");

    // add mods' spritesheets
    for (auto const& [_, mod] : m_mods) {
        if (forceReload || !ModImpl::getImpl(mod)->m_resourcesLoaded) {
            this->updateModResources(mod);
            ModImpl::getImpl(mod)->m_resourcesLoaded = true;
        }
    }
}

std::vector<Mod*> Loader::Impl::getAllMods() {
    return map::values(m_mods);
}

Mod* Loader::Impl::getModImpl() {
    return Mod::get();
}

std::vector<InvalidGeodeFile> Loader::Impl::getFailedMods() const {
    return m_invalidMods;
}

// Version info

VersionInfo Loader::Impl::getVersion() {
    return LOADER_VERSION;
}

VersionInfo Loader::Impl::minModVersion() {
    return VersionInfo { 1, 0, 0, VersionTag(VersionTag::Beta, 5) };
}

VersionInfo Loader::Impl::maxModVersion() {
    return VersionInfo {
        this->getVersion().getMajor(),
        this->getVersion().getMinor(),
        // todo: dynamic version info (vM.M.*)
        99999999,
    };
}

bool Loader::Impl::isModVersionSupported(VersionInfo const& version) {
    return
        version >= this->minModVersion() &&
        version <= this->maxModVersion();
}

// Data saving

Result<> Loader::Impl::saveData() {
    // save mods' data
    for (auto& [id, mod] : m_mods) {
        Mod::get()->setSavedValue("should-load-" + id, mod->isEnabled());
        auto r = mod->saveData();
        if (!r) {
            log::warn("Unable to save data for mod \"{}\": {}", mod->getID(), r.unwrapErr());
        }
    }
    // save loader data
    GEODE_UNWRAP(Mod::get()->saveData());
    
    return Ok();
}

Result<> Loader::Impl::loadData() {
    auto e = Mod::get()->loadData();
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

Result<Mod*> Loader::Impl::loadModFromInfo(ModInfo const& info) {
    if (m_mods.count(info.id())) {
        return Err(fmt::format("Mod with ID '{}' already loaded", info.id()));
    }

    // create Mod instance
    auto mod = new Mod(info);
    auto setupRes = mod->m_impl->setup();
    if (!setupRes) {
        // old code artifcat, idk why we are not using unique_ptr TBH
        delete mod;
        return Err(fmt::format(
            "Unable to setup mod '{}': {}",
            info.id(), setupRes.unwrapErr()
        ));
    }

    m_mods.insert({ info.id(), mod });

    mod->m_impl->m_enabled = Mod::get()->getSavedValue<bool>(
        "should-load-" + info.id(), true
    );

    // this loads the mod if its dependencies are resolved
    auto dependenciesRes = mod->updateDependencies();
    if (!dependenciesRes) {
        delete mod;
        m_mods.erase(info.id());
        return Err(dependenciesRes.unwrapErr());
    }

    // add mod resources
    this->queueInGDThread([this, mod]() {
        auto searchPath = dirs::getModRuntimeDir() / mod->getID() / "resources";

        CCFileUtils::get()->addSearchPath(searchPath.string().c_str());
        this->updateModResources(mod);
    });

    return Ok(mod);
}

Result<Mod*> Loader::Impl::loadModFromFile(ghc::filesystem::path const& file) {
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

bool Loader::Impl::isModInstalled(std::string const& id) const {
    return m_mods.count(id) && !m_mods.at(id)->isUninstalled();
}

Mod* Loader::Impl::getInstalledMod(std::string const& id) const {
    if (m_mods.count(id) && !m_mods.at(id)->isUninstalled()) {
        return m_mods.at(id);
    }
    return nullptr;
}

bool Loader::Impl::isModLoaded(std::string const& id) const {
    return m_mods.count(id) && m_mods.at(id)->isLoaded() && m_mods.at(id)->isEnabled();
}

Mod* Loader::Impl::getLoadedMod(std::string const& id) const {
    if (m_mods.count(id)) {
        auto mod = m_mods.at(id);
        if (mod->isLoaded() && mod->isEnabled()) {
            return mod;
        }
    }
    return nullptr;
}

void Loader::Impl::updateModResources(Mod* mod) {
    if (!mod->m_impl->m_info.spritesheets().size()) {
        return;
    }

    auto searchPath = mod->getResourcesDir();

    log::debug("Adding resources for {}", mod->getID());

    // add spritesheets
    for (auto const& sheet : mod->m_impl->m_info.spritesheets()) {
        log::debug("Adding sheet {}", sheet);
        auto png = sheet + ".png";
        auto plist = sheet + ".plist";
        auto ccfu = CCFileUtils::get();

        if (png == std::string(ccfu->fullPathForFilename(png.c_str(), false)) ||
            plist == std::string(ccfu->fullPathForFilename(plist.c_str(), false))) {
            log::warn(
                "The resource dir of \"{}\" is missing \"{}\" png and/or plist files",
                mod->m_impl->m_info.id(), sheet
            );
        }
        else {
            CCTextureCache::get()->addImage(png.c_str(), false);
            CCSpriteFrameCache::get()->addSpriteFramesWithFile(plist.c_str());
        }
    }
}

// Dependencies and refreshing

void Loader::Impl::loadModsFromDirectory(
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
            return p->m_impl->m_info.path() == entry.path();
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

void Loader::Impl::refreshModsList() {
    log::debug("Loading mods...");

    // find mods
    for (auto& dir : m_modSearchDirectories) {
        this->loadModsFromDirectory(dir);
    }
    
    // load early-load mods first
    for (auto& mod : m_modsToLoad) {
        if (mod.needsEarlyLoad()) {
            auto load = this->loadModFromInfo(mod);
            if (!load) {
                log::error("Unable to load {}: {}", mod.id(), load.unwrapErr());

                m_invalidMods.push_back(InvalidGeodeFile {
                    .path = mod.path(),
                    .reason = load.unwrapErr(),
                });
            }
        }
    }

    // UI can be loaded now
    m_earlyLoadFinished = true;
    m_earlyLoadFinishedCV.notify_all();

    // load the rest of the mods
    for (auto& mod : m_modsToLoad) {
        if (!mod.needsEarlyLoad()) {
            auto load = this->loadModFromInfo(mod);
            if (!load) {
                log::error("Unable to load {}: {}", mod.id(), load.unwrapErr());

                m_invalidMods.push_back(InvalidGeodeFile {
                    .path = mod.path(),
                    .reason = load.unwrapErr(),
                });
            }
        }
    }
    m_modsToLoad.clear();
}

void Loader::Impl::updateAllDependencies() {
    for (auto const& [_, mod] : m_mods) {
        (void)mod->updateDependencies();
    }
}

void Loader::Impl::waitForModsToBeLoaded() {
    auto lock = std::unique_lock(m_earlyLoadFinishedMutex);
    log::debug("Waiting for mods to be loaded... {}", bool(m_earlyLoadFinished));
    m_earlyLoadFinishedCV.wait(lock, [this] {
        return bool(m_earlyLoadFinished);
    });
}

bool Loader::Impl::didLastLaunchCrash() const {
    return crashlog::didLastLaunchCrash();
}

void Loader::Impl::reset() {
    this->closePlatformConsole();

    for (auto& [_, mod] : m_mods) {
        delete mod;
    }
    m_mods.clear();
    log::Logger::clear();
    ghc::filesystem::remove_all(dirs::getModRuntimeDir());
    ghc::filesystem::remove_all(dirs::getTempDir());
}

bool Loader::Impl::isReadyToHook() const {
    return m_readyToHook;
}

void Loader::Impl::addInternalHook(Hook* hook, Mod* mod) {
    m_internalHooks.push_back({hook, mod});
}

bool Loader::Impl::loadHooks() {
    m_readyToHook = true;
    auto thereWereErrors = false;
    for (auto const& hook : m_internalHooks) {
        auto res = hook.second->addHook(hook.first);
        if (!res) {
            log::internalLog(Severity::Error, hook.second, "{}", res.unwrapErr());
            thereWereErrors = true;
        }
    }
    // free up memory
    m_internalHooks.clear();
    return !thereWereErrors;
}

void Loader::Impl::queueInGDThread(ScheduledFunction func) {
    std::lock_guard<std::mutex> lock(m_gdThreadMutex);
    m_gdThreadQueue.push_back(func);
}

void Loader::Impl::executeGDThreadQueue() {
    // copy queue to avoid locking mutex if someone is
    // running addToGDThread inside their function
    m_gdThreadMutex.lock();
    auto queue = m_gdThreadQueue;
    m_gdThreadQueue.clear();
    m_gdThreadMutex.unlock();

    // call queue
    for (auto const& func : queue) {
        func();
    }
}

void Loader::Impl::logConsoleMessage(std::string const& msg) {
    if (m_platformConsoleOpen) {
        // TODO: make flushing optional
        std::cout << msg << '\n' << std::flush;
    }
}

bool Loader::Impl::platformConsoleOpen() const {
    return m_platformConsoleOpen;
}

void Loader::Impl::fetchLatestGithubRelease(
    utils::MiniFunction<void(json::Value const&)> then,
    utils::MiniFunction<void(std::string const&)> expect
) {
    if (m_latestGithubRelease) {
        return then(m_latestGithubRelease.value());
    }
    // TODO: add header to not get rate limited
    web::AsyncWebRequest()
        .join("loader-auto-update-check")
        .fetch("https://api.github.com/repos/geode-sdk/geode/releases/latest")
        .json()
        .then([this, then](json::Value const& json) {
            m_latestGithubRelease = json;
            then(json);
        })
        .expect(expect);
}

void Loader::Impl::tryDownloadLoaderResources(
    std::string const& url,
    bool tryLatestOnError
) {
    auto tempResourcesZip = dirs::getTempDir() / "new.zip";
    auto resourcesDir = dirs::getGeodeResourcesDir() / Mod::get()->getID();

    web::AsyncWebRequest()
        // use the url as a join handle
        .join(url)
        .fetch(url)
        .into(tempResourcesZip)
        .then([tempResourcesZip, resourcesDir, this](auto) {
            // unzip resources zip
            auto unzip = file::Unzip::intoDir(tempResourcesZip, resourcesDir, true);
            if (!unzip) {
                ResourceDownloadEvent(
                    UpdateFailed("Unable to unzip new resources: " + unzip.unwrapErr())
                ).post();
                return;
            }
            this->updateSpecialFiles();

            ResourceDownloadEvent(UpdateFinished()).post();
        })
        .expect([this, tryLatestOnError](std::string const& info, int code) {
            // if the url was not found, try downloading latest release instead
            // (for development versions)
            if (code == 404 && tryLatestOnError) {
                this->downloadLoaderResources(true);
            }
            else {
                ResourceDownloadEvent(
                    UpdateFailed("Unable to download resources: " + info)
                ).post();
            }
        })
        .progress([](auto&, double now, double total) {
            ResourceDownloadEvent(
                UpdateProgress(
                    static_cast<uint8_t>(now / total * 100.0),
                    "Downloading resources"
                )
            ).post();
        });
}

void Loader::Impl::updateSpecialFiles() {
    auto resourcesDir = dirs::getGeodeResourcesDir() / Mod::get()->getID();
    auto res = ModInfoImpl::getImpl(ModImpl::get()->m_info).addSpecialFiles(resourcesDir);
    if (res.isErr()) {
        log::warn("Unable to add special files: {}", res.unwrapErr());
    }
}

void Loader::Impl::downloadLoaderResources(bool useLatestRelease) {
    if (!useLatestRelease) {
        web::AsyncWebRequest()
            .join("loader-tag-exists-check")
            .fetch(fmt::format(
                "https://api.github.com/repos/geode-sdk/geode/git/ref/tags/{}",
                this->getVersion().toString()
            ))
            .json()
            .then([this](json::Value const& json) {
                this->tryDownloadLoaderResources(fmt::format(
                    "https://github.com/geode-sdk/geode/releases/download/{}/resources-" GEODE_PLATFORM_SHORT_IDENTIFIER ".zip",
                    this->getVersion().toString()
                ), true);  
            })
            .expect([this](std::string const& info, int code) {
                if (code == 404) {
                    log::debug("Loader version {} does not exist on Github, not downloading the resources", this->getVersion().toString());
                    ResourceDownloadEvent(
                        UpdateFinished()
                    ).post();
                }
                else {
                    ResourceDownloadEvent(
                        UpdateFailed("Unable to check if tag exists: " + info)
                    ).post();
                }
            });
    }
    else {
        fetchLatestGithubRelease(
            [this](json::Value const& raw) {
                auto json = raw;
                JsonChecker checker(json);
                auto root = checker.root("[]").obj();

                // find release asset
                for (auto asset : root.needs("assets").iterate()) {
                    auto obj = asset.obj();
                    if (obj.needs("name").template get<std::string>() == "resources-" GEODE_PLATFORM_SHORT_IDENTIFIER ".zip") {
                        this->tryDownloadLoaderResources(
                            obj.needs("browser_download_url").template get<std::string>(),
                            false
                        );
                        return;
                    }
                }

                ResourceDownloadEvent(
                    UpdateFailed("Unable to find resources in latest GitHub release")
                ).post();
            },
            [this](std::string const& info) {
                ResourceDownloadEvent(
                    UpdateFailed("Unable to download resources: " + info)
                ).post();
            }
        );
    }
}

bool Loader::Impl::verifyLoaderResources() {
    static std::optional<bool> CACHED = std::nullopt;
    if (CACHED.has_value()) {
        return CACHED.value();
    }

    // geode/resources/geode.loader
    auto resourcesDir = dirs::getGeodeResourcesDir() / Mod::get()->getID();

    // if the resources dir doesn't exist, then it's probably incorrect
    if (!(
        ghc::filesystem::exists(resourcesDir) &&
        ghc::filesystem::is_directory(resourcesDir)
    )) {
        log::debug("Resources directory does not exist");
        this->downloadLoaderResources();
        return false;
    }

    // TODO: actually have a proper way to disable checking resources
    // for development builds
    if (ghc::filesystem::exists(resourcesDir / "dont-update.txt")) {
        // this is kind of a hack, but it's the easiest way to prevent
        // auto update while developing
        log::debug("Not updating resources since dont-update.txt exists");
        return true;
    }

    // make sure every file was covered
    size_t coverage = 0;

    // verify hashes
    for (auto& file : ghc::filesystem::directory_iterator(resourcesDir)) {
        auto name = file.path().filename().string();
        // skip unknown files
        if (!LOADER_RESOURCE_HASHES.count(name)) {
            continue;
        }
        // verify hash
        auto hash = calculateSHA256(file.path());
        auto expected = LOADER_RESOURCE_HASHES.at(name);
        if (hash != expected) {
            log::debug("Resource hash mismatch: {} ({}, {})", name, hash.substr(0, 7), expected.substr(0, 7));
            this->downloadLoaderResources();
            return false;
        }
        coverage += 1;
    }

    // make sure every file was found
    if (coverage != LOADER_RESOURCE_HASHES.size()) {
        log::debug("Resource coverage mismatch");
        this->downloadLoaderResources();
        return false;
    }

    return true;
}

void Loader::Impl::downloadLoaderUpdate(std::string const& url) {
    auto updateZip = dirs::getTempDir() / "loader-update.zip";
    auto targetDir = dirs::getGeodeDir() / "update";

    web::AsyncWebRequest()
        .join("loader-update-download")
        .fetch(url)
        .into(updateZip)
        .then([this, updateZip, targetDir](auto) {
            // unzip resources zip
            auto unzip = file::Unzip::intoDir(updateZip, targetDir, true);
            if (!unzip) {
                LoaderUpdateEvent(
                    UpdateFailed("Unable to unzip update: " + unzip.unwrapErr())
                ).post();
                return;
            }
            m_isNewUpdateDownloaded = true;
            LoaderUpdateEvent(UpdateFinished()).post();
        })
        .expect([](std::string const& info) {
            LoaderUpdateEvent(
                UpdateFailed("Unable to download update: " + info)
            ).post();
        })
        .progress([](auto&, double now, double total) {
            LoaderUpdateEvent(
                UpdateProgress(
                    static_cast<uint8_t>(now / total * 100.0),
                    "Downloading update"
                )
            ).post();
        });
}

void Loader::Impl::checkForLoaderUpdates() {
    // Check for updates in the background
    fetchLatestGithubRelease(
        [this](json::Value const& raw) {
            auto json = raw;
            JsonChecker checker(json);
            auto root = checker.root("[]").obj();

            VersionInfo ver { 0, 0, 0 };
            root.needs("tag_name").into(ver);

            // make sure release is newer
            if (ver <= this->getVersion()) {
                return;
            }

            // don't auto-update major versions
            if (ver.getMajor() > this->getVersion().getMajor()) {
                return;
            }

            // find release asset
            for (auto asset : root.needs("assets").iterate()) {
                auto obj = asset.obj();
                if (string::endsWith(
                    obj.needs("name").template get<std::string>(),
                    GEODE_PLATFORM_SHORT_IDENTIFIER ".zip"
                )) {
                    this->downloadLoaderUpdate(
                        obj.needs("browser_download_url").template get<std::string>()
                    );
                    return;
                }
            }

            LoaderUpdateEvent(
                UpdateFailed("Unable to find release asset for " GEODE_PLATFORM_NAME)
            ).post();
        },
        [](std::string const& info) {
            LoaderUpdateEvent(
                UpdateFailed("Unable to check for updates: " + info)
            ).post();
        }
    );
}

bool Loader::Impl::isNewUpdateDownloaded() const {
    return m_isNewUpdateDownloaded;
}
    
json::Value Loader::Impl::processRawIPC(void* rawHandle, std::string const& buffer) {
    json::Value reply;

    json::Value json;
    try {
        json = json::parse(buffer);
    } catch (...) {
        log::warn("Received IPC message that isn't valid JSON");
        return reply;
    }

    if (!json.contains("mod") || !json["mod"].is_string()) {
        log::warn("Received IPC message without 'mod' field");
        return reply;
    }
    if (!json.contains("message") || !json["message"].is_string()) {
        log::warn("Received IPC message without 'message' field");
        return reply;
    }
    json::Value data;
    if (json.contains("data")) {
        data = json["data"];
    }
    // log::debug("Posting IPC event");
    // ! warning: if the event system is ever made asynchronous this will break!
    IPCEvent(rawHandle, json["mod"].as_string(), json["message"].as_string(), data, reply).post();
    return reply;
}

ResourceDownloadEvent::ResourceDownloadEvent(
    UpdateStatus const& status
) : status(status) {}

ListenerResult ResourceDownloadFilter::handle(
    utils::MiniFunction<Callback> fn,
    ResourceDownloadEvent* event
) {
    fn(event);
    return ListenerResult::Propagate;
}

ResourceDownloadFilter::ResourceDownloadFilter() {}

LoaderUpdateEvent::LoaderUpdateEvent(
    UpdateStatus const& status
) : status(status) {}

ListenerResult LoaderUpdateFilter::handle(
    utils::MiniFunction<Callback> fn,
    LoaderUpdateEvent* event
) {
    fn(event);
    return ListenerResult::Propagate;
}

LoaderUpdateFilter::LoaderUpdateFilter() {}

void Loader::Impl::provideNextMod(Mod* mod) {
    m_nextModLock.lock();
    if (mod) {
        m_nextMod = mod;
    }
}

Mod* Loader::Impl::takeNextMod() {
    if (!m_nextMod) {
        m_nextMod = this->createInternalMod();
        log::debug("Created internal mod {}", m_nextMod->getName());
    }
    auto ret = m_nextMod;
    return ret;
}

void Loader::Impl::releaseNextMod() {
    m_nextMod = nullptr;

    m_nextModLock.unlock();
}


Result<> Loader::Impl::createHandler(void* address, tulip::hook::HandlerMetadata const& metadata) {
    if (m_handlerHandles.count(address)) {
        return Err("Handler already exists at address");
    }

    GEODE_UNWRAP_INTO(auto handle, tulip::hook::createHandler(address, metadata));
    m_handlerHandles[address] = handle;
    return Ok();
}

bool Loader::Impl::hasHandler(void* address) {
    return m_handlerHandles.count(address) > 0;
}

Result<tulip::hook::HandlerHandle> Loader::Impl::getHandler(void* address) {
    if (!m_handlerHandles.count(address)) {
        return Err("Handler does not exist at address");
    }

    return Ok(m_handlerHandles[address]);
}

Result<> Loader::Impl::removeHandler(void* address) {
    if (!m_handlerHandles.count(address)) {
        return Err("Handler does not exist at address");
    }

    auto handle = m_handlerHandles[address];
    GEODE_UNWRAP(tulip::hook::removeHandler(handle));
    m_handlerHandles.erase(address);
    return Ok();
}
