#include "LoaderImpl.hpp"
#include <cocos2d.h>

#include "ModImpl.hpp"
#include "ModMetadataImpl.hpp"
#include "LogImpl.hpp"

#include <Geode/loader/Dirs.hpp>
#include <Geode/loader/IPC.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/map.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/web.hpp>
#include <about.hpp>
#include <crashlog.hpp>
#include <fmt/format.h>
#include <hash.hpp>
#include <iostream>
#include <resources.hpp>
#include <string>
#include <vector>

using namespace geode::prelude;

Loader::Impl* LoaderImpl::get() {
    return Loader::get()->m_impl.get();
}

Loader::Impl::Impl() = default;

Loader::Impl::~Impl() = default;

// Initialization

void Loader::Impl::createDirectories() {
#ifdef GEODE_IS_MACOS
    ghc::filesystem::create_directory(dirs::getSaveDir());
#endif

    (void) utils::file::createDirectoryAll(dirs::getGeodeResourcesDir());
    (void) utils::file::createDirectoryAll(dirs::getModConfigDir());
    (void) utils::file::createDirectoryAll(dirs::getModsDir());
    (void) utils::file::createDirectoryAll(dirs::getGeodeLogDir());
    (void) utils::file::createDirectoryAll(dirs::getTempDir());
    (void) utils::file::createDirectoryAll(dirs::getModRuntimeDir());

    if (!ranges::contains(m_modSearchDirectories, dirs::getModsDir())) {
        m_modSearchDirectories.push_back(dirs::getModsDir());
    }
}

Result<> Loader::Impl::setup() {
    if (m_isSetup) {
        return Ok();
    }

    log::debug("Setting up crash handler");
    log::pushNest();
    if (!crashlog::setupPlatformHandler()) {
        log::debug("Failed to set up crash handler");
    }
    log::popNest();

    log::debug("Loading hooks");
    log::pushNest();
    if (!this->loadHooks()) {
        return Err("There were errors loading some hooks, see console for details");
    }
    log::popNest();

    log::debug("Setting up IPC");
    log::pushNest();
    this->setupIPC();
    log::popNest();

    log::debug("Setting up directories");
    log::pushNest();
    this->createDirectories();
    this->addSearchPaths();
    log::popNest();

    this->refreshModGraph();

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
    log::pushNest();
    for (auto const& [_, mod] : m_mods) {
        if (!forceReload && ModImpl::getImpl(mod)->m_resourcesLoaded)
            continue;
        this->updateModResources(mod);
        ModImpl::getImpl(mod)->m_resourcesLoaded = true;
    }
    log::popNest();
}

std::vector<Mod*> Loader::Impl::getAllMods() {
    return map::values(m_mods);
}

// Version info

VersionInfo Loader::Impl::getVersion() {
    return LOADER_VERSION;
}

VersionInfo Loader::Impl::minModVersion() {
    return VersionInfo {
        this->getVersion().getMajor(), 0, 0
    };
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
    for (auto& [id, mod] : m_mods) {
        log::debug("{}", mod->getID());
        log::pushNest();
        auto r = mod->saveData();
        if (!r) {
            log::warn("Unable to save data for mod \"{}\": {}", mod->getID(), r.unwrapErr());
        }
        log::popNest();
    }
    return Ok();
}

Result<> Loader::Impl::loadData() {
    for (auto& [_, mod] : m_mods) {
        log::debug("{}", mod->getID());
        log::pushNest();
        auto r = mod->loadData();
        if (!r) {
            log::warn("Unable to load data for mod \"{}\": {}", mod->getID(), r.unwrapErr());
        }
        log::popNest();
    }
    return Ok();
}

// Mod loading

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
    return m_mods.count(id) && m_mods.at(id)->isEnabled();
}

Mod* Loader::Impl::getLoadedMod(std::string const& id) const {
    if (m_mods.count(id)) {
        auto mod = m_mods.at(id);
        if (mod->isEnabled()) {
            return mod;
        }
    }
    return nullptr;
}

void Loader::Impl::updateModResources(Mod* mod) {
    if (mod != Mod::get()) {
        // geode.loader resource is stored somewhere else, which is already added anyway
        auto searchPathRoot = dirs::getModRuntimeDir() / mod->getID() / "resources";
        CCFileUtils::get()->addSearchPath(searchPathRoot.string().c_str());
    }

    // only thing needs previous setup is spritesheets
    if (mod->getMetadata().getSpritesheets().empty())
        return;

    log::debug("{}", mod->getID());
    log::pushNest();

    for (auto const& sheet : mod->getMetadata().getSpritesheets()) {
        log::debug("Adding sheet {}", sheet);
        auto png = sheet + ".png";
        auto plist = sheet + ".plist";
        auto ccfu = CCFileUtils::get();

        if (png == std::string(ccfu->fullPathForFilename(png.c_str(), false)) ||
            plist == std::string(ccfu->fullPathForFilename(plist.c_str(), false))) {
            log::warn(
                R"(The resource dir of "{}" is missing "{}" png and/or plist files)",
                mod->getID(), sheet
            );
        }
        else {
            CCTextureCache::get()->addImage(png.c_str(), false);
            CCSpriteFrameCache::get()->addSpriteFramesWithFile(plist.c_str());
        }
    }

    log::popNest();
}

// Dependencies and refreshing

void Loader::Impl::queueMods(std::vector<ModMetadata>& modQueue) {
    for (auto const& dir : m_modSearchDirectories) {
        log::debug("Searching {}", dir);
        log::pushNest();
        for (auto const& entry : ghc::filesystem::directory_iterator(dir)) {
            if (!ghc::filesystem::is_regular_file(entry) ||
                entry.path().extension() != GEODE_MOD_EXTENSION)
                continue;

            log::debug("Found {}", entry.path().filename());
            log::pushNest();

            auto res = ModMetadata::createFromGeodeFile(entry.path());
            if (!res) {
                m_problems.push_back({
                    LoadProblem::Type::InvalidFile,
                    entry.path(),
                    res.unwrapErr()
                });
                log::error("Failed to queue: {}", res.unwrapErr());
                log::popNest();
                continue;
            }
            auto modMetadata = res.unwrap();

            log::debug("id: {}", modMetadata.getID());
            log::debug("version: {}", modMetadata.getVersion());
            log::debug("early: {}", modMetadata.needsEarlyLoad() ? "yes" : "no");

            if (std::find_if(modQueue.begin(), modQueue.end(), [&](auto& item) {
                    return modMetadata.getID() == item.getID();
                }) != modQueue.end()) {
                m_problems.push_back({
                    LoadProblem::Type::Duplicate,
                    modMetadata,
                    "A mod with the same ID is already present."
                });
                log::error("Failed to queue: a mod with the same ID is already queued");
                log::popNest();
                continue;
            }

            modQueue.push_back(modMetadata);
            log::popNest();
        }
        log::popNest();
    }
}

void Loader::Impl::populateModList(std::vector<ModMetadata>& modQueue) {
    std::vector<std::string> toRemove;
    for (auto& [id, mod] : m_mods) {
        if (id == "geode.loader")
            continue;
        delete mod;
        toRemove.push_back(id);
    }
    for (auto const& id : toRemove) {
        m_mods.erase(id);
    }

    for (auto const& metadata : modQueue) {
        log::debug("{} {}", metadata.getID(), metadata.getVersion());
        log::pushNest();

        auto mod = new Mod(metadata);

        auto res = mod->m_impl->setup();
        if (!res) {
            m_problems.push_back({
                LoadProblem::Type::SetupFailed,
                mod,
                res.unwrapErr()
            });
            log::error("Failed to set up: {}", res.unwrapErr());
            log::popNest();
            continue;
        }

        m_mods.insert({metadata.getID(), mod});

        log::popNest();
    }
}

void Loader::Impl::buildModGraph() {
    for (auto const& [id, mod] : m_mods) {
        log::debug("{}", mod->getID());
        log::pushNest();
        for (auto& dependency : mod->m_impl->m_metadata.m_impl->m_dependencies) {
            log::debug("{}", dependency.id);
            if (!m_mods.contains(dependency.id)) {
                dependency.mod = nullptr;
                continue;
            }

            dependency.mod = m_mods[dependency.id];

            if (!dependency.version.compare(dependency.mod->getVersion())) {
                dependency.mod = nullptr;
                continue;
            }

            if (dependency.importance != ModMetadata::Dependency::Importance::Required || dependency.mod == nullptr)
                continue;

            dependency.mod->m_impl->m_dependants.push_back(mod);
        }
        for (auto& incompatibility : mod->m_impl->m_metadata.m_impl->m_incompatibilities) {
            incompatibility.mod =
                m_mods.contains(incompatibility.id) ? m_mods[incompatibility.id] : nullptr;
        }
        log::popNest();
    }
}

void Loader::Impl::loadModGraph(Mod* node, bool early) {
    if (early && !node->needsEarlyLoad()) {
        m_modsToLoad.push_back(node);
        return;
    }

    if (node->hasUnresolvedDependencies()) {
        log::debug("{} {} has unresolved dependencies", node->getID(), node->getVersion());
        return;
    }
    if (node->hasUnresolvedIncompatibilities()) {
        log::debug("{} {} has unresolved incompatibilities", node->getID(), node->getVersion());
        return;
    }

    log::debug("{} {}", node->getID(), node->getVersion());
    log::pushNest();

    if (node->isEnabled()) {
        for (auto const& dep : node->m_impl->m_dependants) {
            m_modsToLoad.push_front(dep);
        }
        log::popNest();
        return;
    }

    m_currentlyLoadingMod = node;
    m_refreshingModCount += 1;
    m_refreshedModCount += 1;
    m_lateRefreshedModCount += early ? 0 : 1;

    auto unzipFunction = [this, node]() {
        log::debug("Unzip");
        auto res = node->m_impl->unzipGeodeFile(node->getMetadata());
        return res;
    };

    auto loadFunction = [this, node, early]() {
        if (node->shouldLoad()) {
            log::debug("Load");
            auto res = node->m_impl->loadBinary();
            if (!res) {
                m_problems.push_back({
                    LoadProblem::Type::LoadFailed,
                    node,
                    res.unwrapErr()
                });
                log::error("Failed to load binary: {}", res.unwrapErr());
                log::popNest();
                m_refreshingModCount -= 1;
                return;
            }

            if (node->getID() == "absolllute.megahack")
                log::debug("megahack creepypasta");

            for (auto const& dep : node->m_impl->m_dependants) {
                m_modsToLoad.push_front(dep);
            }
        }

        m_refreshingModCount -= 1;

        log::popNest();
    };

    if (early) {
        auto res = unzipFunction();
        if (!res) {
            m_problems.push_back({
                LoadProblem::Type::UnzipFailed,
                node,
                res.unwrapErr()
            });
            log::error("Failed to unzip: {}", res.unwrapErr());
            log::popNest();
            m_refreshingModCount -= 1;
            return;
        }
        loadFunction();
    }
    else {
        std::thread([=]() {
            auto res = unzipFunction();
            queueInMainThread([=, this]() {
                if (!res) {
                    m_problems.push_back({
                        LoadProblem::Type::UnzipFailed,
                        node,
                        res.unwrapErr()
                    });
                    log::error("Failed to unzip: {}", res.unwrapErr());
                    log::popNest();
                    m_refreshingModCount -= 1;
                    return;
                }
                loadFunction();
            });
        }).detach();
    }
}

void Loader::Impl::findProblems() {
    for (auto const& [id, mod] : m_mods) {
        if (!mod->shouldLoad()) {
            log::debug("{} is not enabled", id);
            continue;
        }
        log::debug("{}", id);
        log::pushNest();

        for (auto const& dep : mod->getMetadata().getDependencies()) {
            if (dep.mod && dep.mod->isEnabled() && dep.version.compare(dep.mod->getVersion()))
                continue;
            switch(dep.importance) {
                case ModMetadata::Dependency::Importance::Suggested:
                    m_problems.push_back({
                        LoadProblem::Type::Suggestion,
                        mod,
                        fmt::format("{} {}", dep.id, dep.version.toString())
                    });
                    log::info("{} suggests {} {}", id, dep.id, dep.version);
                    break;
                case ModMetadata::Dependency::Importance::Recommended:
                    m_problems.push_back({
                        LoadProblem::Type::Recommendation,
                        mod,
                        fmt::format("{} {}", dep.id, dep.version.toString())
                    });
                    log::warn("{} recommends {} {}", id, dep.id, dep.version);
                    break;
                case ModMetadata::Dependency::Importance::Required:
                    m_problems.push_back({
                        LoadProblem::Type::MissingDependency,
                        mod,
                        fmt::format("{} {}", dep.id, dep.version.toString())
                    });
                    log::error("{} requires {} {}", id, dep.id, dep.version);
                    break;
            }
        }

        for (auto const& dep : mod->getMetadata().getIncompatibilities()) {
            if (!dep.mod || !dep.version.compare(dep.mod->getVersion()))
                continue;
            switch(dep.importance) {
                case ModMetadata::Incompatibility::Importance::Conflicting:
                    m_problems.push_back({
                        LoadProblem::Type::Conflict,
                        mod,
                        fmt::format("{} {}", dep.id, dep.version.toString())
                    });
                    log::warn("{} conflicts with {} {}", id, dep.id, dep.version);
                    break;
                case ModMetadata::Incompatibility::Importance::Breaking:
                    m_problems.push_back({
                        LoadProblem::Type::PresentIncompatibility,
                        mod,
                        fmt::format("{} {}", dep.id, dep.version.toString())
                    });
                    log::error("{} breaks {} {}", id, dep.id, dep.version);
                    break;
            }
        }

        Mod* myEpicMod = mod; // clang fix
        // if the mod is not loaded but there are no problems related to it
        if (!mod->isEnabled() &&
            mod->shouldLoad() &&
            !std::any_of(m_problems.begin(), m_problems.end(), [myEpicMod](auto& item) {
                return std::holds_alternative<ModMetadata>(item.cause) &&
                    std::get<ModMetadata>(item.cause).getID() == myEpicMod->getID() ||
                    std::holds_alternative<Mod*>(item.cause) &&
                    std::get<Mod*>(item.cause) == myEpicMod;
            })) {
            m_problems.push_back({
                LoadProblem::Type::Unknown,
                mod,
                ""
            });
            log::error("{} failed to load for an unknown reason", id);
        }

        log::popNest();
    }
}

void Loader::Impl::refreshModGraph() {
    log::info("Refreshing mod graph");
    log::pushNest();

    if (m_isSetup) {
        log::error("Cannot refresh mod graph after startup");
        log::popNest();
        return;
    }

    auto begin = std::chrono::high_resolution_clock::now();

    m_problems.clear();

    m_loadingState = LoadingState::Queue;
    log::debug("Queueing mods");
    log::pushNest();
    std::vector<ModMetadata> modQueue;
    this->queueMods(modQueue);
    log::popNest();

    m_loadingState = LoadingState::List;
    log::debug("Populating mod list");
    log::pushNest();
    this->populateModList(modQueue);
    modQueue.clear();
    log::popNest();

    m_loadingState = LoadingState::Graph;
    log::debug("Building mod graph");
    log::pushNest();
    this->buildModGraph();
    log::popNest();

    m_loadingState = LoadingState::EarlyMods;
    log::debug("Loading early mods");
    log::pushNest();
    for (auto const& dep : ModImpl::get()->m_dependants) {
        this->loadModGraph(dep, true);
    }
    log::popNest();

    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    log::info("Took {}s. Continuing next frame...", static_cast<float>(time) / 1000.f);

    log::popNest();

    if (m_modsToLoad.empty())
        m_loadingState = LoadingState::Problems;
    else
        m_loadingState = LoadingState::Mods;

    queueInMainThread([&]() {
        this->continueRefreshModGraph();
    });
}

void Loader::Impl::continueRefreshModGraph() {
    if (m_refreshingModCount != 0) {
        queueInMainThread([&]() {
            this->continueRefreshModGraph();
        });
        return;
    }

    if  (m_lateRefreshedModCount > 0) {
        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_timerBegin).count();
        log::info("Took {}s", static_cast<float>(time) / 1000.f);
    }

    log::info("Continuing mod graph refresh...");
    log::pushNest();

    m_timerBegin = std::chrono::high_resolution_clock::now();

    switch (m_loadingState) {
        case LoadingState::Mods:
            if (!m_modsToLoad.empty()) {
                log::debug("Loading mods");
                log::pushNest();
                this->loadModGraph(m_modsToLoad.front(), false);
                log::popNest();
                m_modsToLoad.pop_front();
                break;
            }
            m_loadingState = LoadingState::Problems;
            [[fallthrough]];
        case LoadingState::Problems:
            log::debug("Finding problems");
            log::pushNest();
            this->findProblems();
            log::popNest();
            m_loadingState = LoadingState::Done;
            {
                auto end = std::chrono::high_resolution_clock::now();
                auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_timerBegin).count();
                log::info("Took {}s", static_cast<float>(time) / 1000.f);
            }
            break;
        default:
            m_loadingState = LoadingState::Done;
            log::warn("Impossible loading state, resetting to 'Done'! "
                "Was Loader::Impl::continueRefreshModGraph() called from the wrong place?");
            break;
    }

    if (m_loadingState != LoadingState::Done) {
        queueInMainThread([&]() {
            this->continueRefreshModGraph();
        });
    }

    log::popNest();
}

std::vector<LoadProblem> Loader::Impl::getProblems() const {
    return m_problems;
}

void Loader::Impl::waitForModsToBeLoaded() {
    log::debug("Waiting for mods to be loaded...");
    // genius
    log::warn("waitForModsToBeLoaded() does not wait for mods to be loaded!");
}

bool Loader::Impl::didLastLaunchCrash() const {
    return crashlog::didLastLaunchCrash();
}

void Loader::Impl::forceReset() {
    this->closePlatformConsole();
    for (auto& [_, mod] : m_mods) {
        delete mod;
    }
    m_mods.clear();
    log::Logger::get()->clear();
    ghc::filesystem::remove_all(dirs::getModRuntimeDir());
    ghc::filesystem::remove_all(dirs::getTempDir());
}

bool Loader::Impl::isReadyToHook() const {
    return m_readyToHook;
}

void Loader::Impl::addUninitializedHook(Hook* hook, Mod* mod) {
    m_uninitializedHooks.emplace_back(hook, mod);
}

bool Loader::Impl::loadHooks() {
    m_readyToHook = true;
    bool hadErrors = false;
    for (auto const& [hook, mod] : m_uninitializedHooks) {
        auto res = mod->addHook(hook);
        if (!res) {
            log::logImpl(Severity::Error, mod, "{}", res.unwrapErr());
            hadErrors = true;
        }
    }
    m_uninitializedHooks.clear();
    return !hadErrors;
}

void Loader::Impl::queueInMainThread(ScheduledFunction func) {
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
        .userAgent("github_api/1.0")
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
            if (code == 404) {
                log::warn("Unable to download resources: {}", info);
            }
            ResourceDownloadEvent(
                UpdateFailed("Unable to download resources: " + info)
            ).post();
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
    auto res = ModMetadataImpl::getImpl(ModImpl::get()->m_metadata).addSpecialFiles(resourcesDir);
    if (res.isErr()) {
        log::warn("Unable to add special files: {}", res.unwrapErr());
    }
}

void Loader::Impl::downloadLoaderResources(bool useLatestRelease) {
    web::AsyncWebRequest()
        .join("loader-tag-exists-check")
        .userAgent("github_api/1.0")
        .fetch(fmt::format(
            "https://api.github.com/repos/geode-sdk/geode/git/ref/tags/{}",
            this->getVersion().toString()
        ))
        .json()
        .then([this](json::Value const& json) {
            this->tryDownloadLoaderResources(fmt::format(
                "https://github.com/geode-sdk/geode/releases/download/{}/resources.zip",
                this->getVersion().toString()
            ), true);  
        })
        .expect([=, this](std::string const& info, int code) {
            if (code == 404) {
                if (useLatestRelease) {
                    log::debug("Loader version {} does not exist on Github, downloading latest resources", this->getVersion().toString());
                    fetchLatestGithubRelease(
                        [this](json::Value const& raw) {
                            auto json = raw;
                            JsonChecker checker(json);
                            auto root = checker.root("[]").obj();

                            // find release asset
                            for (auto asset : root.needs("assets").iterate()) {
                                auto obj = asset.obj();
                                if (obj.needs("name").template get<std::string>() == "resources.zip") {
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
                    return;
                }
                else {
                    log::debug("Loader version {} does not exist on Github, not downloading the resources", this->getVersion().toString());
                }
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
        this->downloadLoaderResources(true);
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
    if (!m_nextMod)
        m_nextMod = this->getInternalMod();
    return m_nextMod;
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
