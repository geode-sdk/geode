#include "LoaderImpl.hpp"
#include <cocos2d.h>

#include "ModImpl.hpp"
#include "ModMetadataImpl.hpp"
#include "LogImpl.hpp"
#include "console.hpp"

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

bool Loader::Impl::isForwardCompatMode() {
    if (!m_forwardCompatModeSet) {
        m_forwardCompatMode = !this->getGameVersion().empty() &&
            this->getGameVersion() != GEODE_STR(GEODE_GD_VERSION);
        m_forwardCompatModeSet = true;
    }
    return m_forwardCompatMode;
}

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

void Loader::Impl::saveData() {
    for (auto& [id, mod] : m_mods) {
        log::debug("{}", mod->getID());
        log::pushNest();
        auto r = mod->saveData();
        if (!r) {
            log::warn("Unable to save data for mod \"{}\": {}", mod->getID(), r.unwrapErr());
        }
        log::popNest();
    }
}

void Loader::Impl::loadData() {
    for (auto& [_, mod] : m_mods) {
        log::debug("{}", mod->getID());
        log::pushNest();
        auto r = mod->loadData();
        if (!r) {
            log::warn("Unable to load data for mod \"{}\": {}", mod->getID(), r.unwrapErr());
        }
        log::popNest();
    }
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

void Loader::Impl::forceReset() {
    console::close();
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
    std::lock_guard<std::mutex> lock(m_mainThreadMutex);
    m_mainThreadQueue.push_back(func);
}

void Loader::Impl::executeGDThreadQueue() {
    // copy queue to avoid locking mutex if someone is
    // running addToGDThread inside their function
    m_mainThreadMutex.lock();
    auto queue = m_mainThreadQueue;
    m_mainThreadQueue.clear();
    m_mainThreadMutex.unlock();

    // call queue
    for (auto const& func : queue) {
        func();
    }
}

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
