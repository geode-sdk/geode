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
#include <iterator>
#include <optional>
#include <resources.hpp>
#include <string>
#include <string_view>
#include <vector>

#include <server/DownloadManager.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

Loader::Impl* LoaderImpl::get() {
    return Loader::get()->m_impl.get();
}

Loader::Impl::Impl() = default;

Loader::Impl::~Impl() = default;

// Initialization

bool Loader::Impl::isForwardCompatMode() {
#ifdef GEODE_IS_ANDROID
    // forward compat mode doesn't really make sense on android
    return false;
#endif

    if (!m_forwardCompatMode.has_value()) {
        m_forwardCompatMode = !this->getGameVersion().empty() &&
            this->getGameVersion() != GEODE_STR(GEODE_GD_VERSION);
    }
    return m_forwardCompatMode.value();
}

void Loader::Impl::createDirectories() {
#ifdef GEODE_IS_MACOS
    std::filesystem::create_directory(dirs::getSaveDir());
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

void Loader::Impl::removeDirectories() {
    // clean up of stale data from Geode v2
    if(std::filesystem::exists(dirs::getGeodeDir() / "index")) {
        std::thread([] {
            std::error_code ec;
            std::filesystem::remove_all(dirs::getGeodeDir() / "index", ec);
        }).detach();
    }
}

Result<> Loader::Impl::setup() {
    if (m_isSetup) {
        return Ok();
    }

    if (this->supportsLaunchArguments()) {
        log::info("Loading launch arguments");
        log::NestScope nest;
        this->initLaunchArguments();
    }

    // on some platforms, using the crash handler overrides more convenient native handlers
    if (!this->getLaunchFlag("disable-crash-handler")) {
        log::info("Setting up crash handler");
        log::NestScope nest;
        if (!crashlog::setupPlatformHandler()) {
            log::debug("Failed to set up crash handler");
        }
    } else {
        log::info("Crash handler setup skipped");
    }

    log::info("Loading hooks");
    if (log::NestScope nest; !this->loadHooks()) {
        return Err("There were errors loading some hooks, see console for details");
    }

    log::info("Setting up directories");
    {
        log::NestScope nest;
        this->createDirectories();
        this->removeDirectories();
        this->addSearchPaths();
    }

    // Trigger on_mod(Loaded) for the internal mod
    // this function is already on the gd thread, so this should be fine
    ModStateEvent(Mod::get(), ModEventType::Loaded).post();

    log::info("Refreshing mod graph");
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
    log::NestScope nest;
    for (auto const& [_, mod] : m_mods) {
        if (!forceReload && ModImpl::getImpl(mod)->m_resourcesLoaded)
            continue;
        this->updateModResources(mod);
        ModImpl::getImpl(mod)->m_resourcesLoaded = true;
    }
    // deduplicate mod resource paths, since they added in both updateModResources and Mod::Impl::setup
    // we have to call it in both places since setup is only called once ever, but updateResources is called
    // on every texture reload
    CCFileUtils::get()->updatePaths();
}

std::vector<Mod*> Loader::Impl::getAllMods() {
    return map::values(m_mods);
}

// Version info

VersionInfo Loader::Impl::getVersion() {
    return about::getLoaderVersion();
}

VersionInfo Loader::Impl::minModVersion() {
    auto ver = this->getVersion();
    return VersionInfo {
        ver.getMajor(), 0, 0, ver.getTag()
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

bool Loader::Impl::isModVersionSupported(VersionInfo const& target) {
    return semverCompare(this->getVersion(), target);
}

// Data saving

void Loader::Impl::saveData() {
    for (auto& [id, mod] : m_mods) {
        log::debug("{}", mod->getID());
        log::NestScope nest;
        auto r = mod->saveData();
        if (!r) {
            log::warn("Unable to save data for mod \"{}\": {}", mod->getID(), r.unwrapErr());
        }
    }
}

void Loader::Impl::loadData() {
    for (auto& [_, mod] : m_mods) {
        log::debug("{}", mod->getID());
        log::NestScope nest;
        auto r = mod->loadData();
        if (!r) {
            log::warn("Unable to load data for mod \"{}\": {}", mod->getID(), r.unwrapErr());
        }
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
    if (!mod->isInternal()) {
        // geode.loader resource is stored somewhere else, which is already added anyway
        auto searchPathRoot = dirs::getModRuntimeDir() / mod->getID() / "resources";
        CCFileUtils::get()->addSearchPath(searchPathRoot.string().c_str());
    }

    // only thing needs previous setup is spritesheets
    if (mod->getMetadataRef().getSpritesheets().empty())
        return;

    log::debug("{}", mod->getID());
    log::NestScope nest;

    for (auto const& sheet : mod->getMetadataRef().getSpritesheets()) {
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
}

void Loader::Impl::addProblem(LoadProblem const& problem) {
    if (std::holds_alternative<Mod*>(problem.cause)) {
        auto mod = std::get<Mod*>(problem.cause);
        ModImpl::getImpl(mod)->m_problems.push_back(problem);
    }
    m_problems.push_back(problem);
}

// Dependencies and refreshing

void Loader::Impl::queueMods(std::vector<ModMetadata>& modQueue) {
    for (auto const& dir : m_modSearchDirectories) {
        log::debug("Searching {}", dir);
        log::NestScope nest;
        for (auto const& entry : std::filesystem::directory_iterator(dir)) {
            if (!std::filesystem::is_regular_file(entry) ||
                entry.path().extension() != GEODE_MOD_EXTENSION)
                continue;

            log::debug("Found {}", entry.path().filename());
            log::NestScope nest;

            auto res = ModMetadata::createFromGeodeFile(entry.path());
            if (!res) {
                this->addProblem({
                    LoadProblem::Type::InvalidFile,
                    entry.path(),
                    res.unwrapErr()
                });
                log::error("Failed to queue: {}", res.unwrapErr());
                continue;
            }
            auto modMetadata = res.unwrap();

            log::debug("id: {}", modMetadata.getID());
            log::debug("version: {}", modMetadata.getVersion());
            log::debug("early: {}", modMetadata.needsEarlyLoad() ? "yes" : "no");

            if (std::find_if(modQueue.begin(), modQueue.end(), [&](auto& item) {
                    return modMetadata.getID() == item.getID();
                }) != modQueue.end()) {
                this->addProblem({
                    LoadProblem::Type::Duplicate,
                    modMetadata,
                    "A mod with the same ID is already present."
                });
                log::error("Failed to queue: a mod with the same ID is already queued");
                continue;
            }

            modQueue.push_back(modMetadata);
        }
    }
}

void Loader::Impl::populateModList(std::vector<ModMetadata>& modQueue) {
    std::vector<std::string> toRemove;
    for (auto& [id, mod] : m_mods) {
        if (mod->isInternal())
            continue;
        delete mod;
        toRemove.push_back(id);
    }
    for (auto const& id : toRemove) {
        m_mods.erase(id);
    }

    for (auto const& metadata : modQueue) {
        log::debug("{} {}", metadata.getID(), metadata.getVersion());
        log::NestScope nest;

        auto mod = new Mod(metadata);

        auto res = mod->m_impl->setup();
        if (!res) {
            this->addProblem({
                LoadProblem::Type::SetupFailed,
                mod,
                res.unwrapErr()
            });
            log::error("Failed to set up: {}", res.unwrapErr());
            continue;
        }

        m_mods.insert({metadata.getID(), mod});
    }
}

void Loader::Impl::buildModGraph() {
    for (auto const& [id, mod] : m_mods) {
        log::debug("{}", mod->getID());
        log::NestScope nest;
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

            if (
                dependency.importance != ModMetadata::Dependency::Importance::Required ||
                dependency.mod == nullptr
            )
                continue;

            dependency.mod->m_impl->m_dependants.push_back(mod);
        }
        for (auto& incompatibility : mod->m_impl->m_metadata.m_impl->m_incompatibilities) {
            incompatibility.mod =
                m_mods.contains(incompatibility.id) ? m_mods[incompatibility.id] : nullptr;
        }
    }
}

void Loader::Impl::loadModGraph(Mod* node, bool early) {
    // Check version first, as it's not worth trying to load a mod with an
    // invalid target version
    // Also this makes it so that when GD updates, outdated mods get shown as
    // "Outdated" in the UI instead of "Missing Dependencies"
    auto res = node->getMetadataRef().checkGameVersion();
    if (!res) {
        this->addProblem({
            LoadProblem::Type::UnsupportedVersion,
            node,
            res.unwrapErr()
        });
        log::error("{}", res.unwrapErr());
        return;
    }

    auto geodeVerRes = node->getMetadataRef().checkGeodeVersion();
    if (!geodeVerRes) {
        this->addProblem({
            node->getMetadataRef().getGeodeVersion() > this->getVersion() ?
                LoadProblem::Type::NeedsNewerGeodeVersion :
                LoadProblem::Type::UnsupportedGeodeVersion,
            node,
            geodeVerRes.unwrapErr()
        });
        log::error("{}", geodeVerRes.unwrapErr());
        return;
    }

    if (node->hasUnresolvedDependencies()) {
        log::warn("{} {} has unresolved dependencies", node->getID(), node->getVersion());
        return;
    }
    if (node->hasUnresolvedIncompatibilities()) {
        log::warn("{} {} has unresolved incompatibilities", node->getID(), node->getVersion());
        return;
    }

    log::NestScope nest;

    if (node->isEnabled()) {
        log::error("Mod {} already loaded, this should never happen", node->getID());
        return;
    }

    m_currentlyLoadingMod = node;
    m_refreshingModCount += 1;
    m_refreshedModCount += 1;
    m_lateRefreshedModCount += early ? 0 : 1;

    auto unzipFunction = [this, node]() {
        log::debug("Unzipping .geode file");
        auto res = node->m_impl->unzipGeodeFile(node->getMetadataRef());
        return res;
    };

    auto loadFunction = [this, node, early]() {
        if (node->shouldLoad()) {
            log::debug("Loading binary");
            auto res = node->m_impl->loadBinary();
            if (!res) {
                this->addProblem({
                    LoadProblem::Type::LoadFailed,
                    node,
                    res.unwrapErr()
                });
                log::error("Failed to load binary: {}", res.unwrapErr());
                m_refreshingModCount -= 1;
                return;
            }
        }

        m_refreshingModCount -= 1;
    };

    {   // version checking
        if (auto reason = node->getMetadataRef().m_impl->m_softInvalidReason) {
            this->addProblem({
                LoadProblem::Type::InvalidFile,
                node,
                reason.value()
            });
            log::error("{}", reason.value());
            m_refreshingModCount -= 1;
            return;
        }
    }

    if (early) {
        auto res = unzipFunction();
        if (!res) {
            this->addProblem({
                LoadProblem::Type::UnzipFailed,
                node,
                res.unwrapErr()
            });
            log::error("Failed to unzip: {}", res.unwrapErr());
            m_refreshingModCount -= 1;
            return;
        }
        loadFunction();
    }
    else {
        auto nest = log::saveNest();
        std::thread([=, this]() {
            thread::setName("Mod Unzip");
            log::loadNest(nest);
            auto res = unzipFunction();
            this->queueInMainThread([=, this, res = std::move(res)]() {
                auto prevNest = log::saveNest();
                log::loadNest(nest);
                if (!res) {
                    this->addProblem({
                        LoadProblem::Type::UnzipFailed,
                        node,
                        res.unwrapErr()
                    });
                    log::error("Failed to unzip: {}", res.unwrapErr());
                    m_refreshingModCount -= 1;
                    log::loadNest(prevNest);
                    return;
                }
                loadFunction();
                log::loadNest(prevNest);
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
        if (mod->targetsOutdatedVersion()) {
            log::warn("{} is outdated", id);
            continue;
        }
        log::debug("{}", id);
        log::NestScope nest;

        for (auto const& dep : mod->getMetadataRef().getDependencies()) {
            if (dep.mod && dep.mod->isEnabled() && dep.version.compare(dep.mod->getVersion()))
                continue;

            auto dismissKey = fmt::format("dismiss-optional-dependency-{}-for-{}", dep.id, id);

            switch(dep.importance) {
                case ModMetadata::Dependency::Importance::Suggested:
                    if (!Mod::get()->getSavedValue<bool>(dismissKey)) {
                        this->addProblem({
                            LoadProblem::Type::Suggestion,
                            mod,
                            fmt::format("{} {}", dep.id, dep.version.toString())
                        });
                        log::info("{} suggests {} {}", id, dep.id, dep.version);
                    }
                    else {
                        log::debug("{} suggests {} {}, but that suggestion was dismissed", id, dep.id, dep.version);
                    }
                    break;
                case ModMetadata::Dependency::Importance::Recommended:
                    if (!Mod::get()->getSavedValue<bool>(dismissKey)) {
                        this->addProblem({
                            LoadProblem::Type::Recommendation,
                            mod,
                            fmt::format("{} {}", dep.id, dep.version.toString())
                        });
                        log::info("{} recommends {} {}", id, dep.id, dep.version);
                    }
                    else {
                        log::debug("{} recommends {} {}, but that suggestion was dismissed", id, dep.id, dep.version);
                    }
                    break;
                case ModMetadata::Dependency::Importance::Required:
                    if(m_mods.find(dep.id) == m_mods.end()) {
                        this->addProblem({
                            LoadProblem::Type::MissingDependency,
                            mod,
                            fmt::format("{}", dep.id)
                        });
                        log::error("{} requires {} {}", id, dep.id, dep.version);
                        break;
                    } else {
                        auto installedDependency = m_mods.at(dep.id);

                        if(!installedDependency->isEnabled()) {
                            this->addProblem({
                                LoadProblem::Type::DisabledDependency,
                                mod,
                                fmt::format("{}", dep.id)
                            });
                            log::error("{} requires {} {}", id, dep.id, dep.version);
                            break;
                        } else if(dep.version.compareWithReason(installedDependency->getVersion()) == VersionCompareResult::TooOld) {
                            this->addProblem({
                                LoadProblem::Type::OutdatedDependency,
                                mod,
                                fmt::format("{}", dep.id)
                            });
                            log::error("{} requires {} {}", id, dep.id, dep.version);
                            break;
                        } else {
                            // fires on major mismatch or too new version of dependency
                            this->addProblem({
                                LoadProblem::Type::MissingDependency,
                                mod,
                                fmt::format("{} {}", dep.id, dep.version)
                            });
                            log::error("{} requires {} {}", id, dep.id, dep.version);
                            break;
                        }
                    }
            }
        }

        for (auto const& dep : mod->getMetadataRef().getIncompatibilities()) {
            if (!dep.mod || !dep.version.compare(dep.mod->getVersion()) || !dep.mod->shouldLoad())
                continue;
            switch(dep.importance) {
                case ModMetadata::Incompatibility::Importance::Conflicting: {
                    this->addProblem({
                        dep.version.toString()[0] == '<' ? LoadProblem::Type::OutdatedConflict : LoadProblem::Type::Conflict,
                        mod,
                        fmt::format("{}", dep.id)
                    });
                    log::warn("{} conflicts with {} {}", id, dep.id, dep.version);
                } break;

                case ModMetadata::Incompatibility::Importance::Breaking: {
                    this->addProblem({
                        dep.version.toString()[0] == '<' ? LoadProblem::Type::OutdatedIncompatibility : LoadProblem::Type::PresentIncompatibility,
                        mod,
                        fmt::format("{}", dep.id)
                    });
                    log::error("{} breaks {} {}", id, dep.id, dep.version);
                } break;

                case ModMetadata::Incompatibility::Importance::Superseded: {
                    this->addProblem({
                        LoadProblem::Type::PresentIncompatibility,
                        mod,
                        fmt::format("{}", dep.id)
                    });
                    log::error("{} supersedes {} {}", id, dep.id, dep.version);
                } break;
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
            this->addProblem({
                LoadProblem::Type::Unknown,
                mod,
                ""
            });
            log::error("{} failed to load for an unknown reason", id);
        }
    }
}

void Loader::Impl::refreshModGraph() {
    log::NestScope nest;

    if (m_isSetup) {
        log::error("Cannot refresh mod graph after startup");
        return;
    }

    auto begin = std::chrono::high_resolution_clock::now();

    m_problems.clear();

    m_loadingState = LoadingState::Queue;
    log::info("Queueing mods");
    std::vector<ModMetadata> modQueue;
    {
        log::NestScope nest;
        this->queueMods(modQueue);
    }

    m_loadingState = LoadingState::List;
    log::info("Populating mod list");
    {
        log::NestScope nest;
        this->populateModList(modQueue);
        modQueue.clear();
    }

    m_loadingState = LoadingState::Graph;
    log::info("Building mod graph");
    {
        log::NestScope nest;
        this->buildModGraph();
    }

    log::info("Ordering mod stack");
    {
        log::NestScope nest;
        this->orderModStack();
    }

    m_loadingState = LoadingState::EarlyMods;
    log::info("Loading early mods");
    {
        log::NestScope nest;
        while (!m_modsToLoad.empty() && m_modsToLoad.front()->needsEarlyLoad()) {
            auto mod = m_modsToLoad.front();
            m_modsToLoad.pop_front();
            log::info("Loading mod {} {}", mod->getID(), mod->getVersion());
            this->loadModGraph(mod, true);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    log::debug("Took {}s. Continuing next frame...", static_cast<float>(time) / 1000.f);

    m_loadingState = LoadingState::Mods;

    queueInMainThread([this]() {
        log::info("Loading non-early mods");
        this->continueRefreshModGraph();
    });
}

void Loader::Impl::orderModStack() {
    std::unordered_set<Mod*> visited;
    visited.insert(Mod::get());
    Mod* selectedMod = nullptr;
    do {
        selectedMod = nullptr;
        for (auto const& mod : ModImpl::get()->m_dependants) {
            if (visited.count(mod) != 0) continue;

            for (auto dep : mod->getMetadataRef().getDependencies()) {
                if (dep.mod && dep.importance == ModMetadata::Dependency::Importance::Required &&
                    visited.count(dep.mod) == 0) {
                    // the dependency is not visited yet
                    // so we cant select this mod
                    goto skip_mod;
                }
            }

            if (selectedMod) {
                if (
                    !selectedMod->m_impl->needsEarlyLoad() &&
                    mod->m_impl->needsEarlyLoad()
                ) {
                    // this mod is implied to be loaded early
                    // so we can override a mod that is not
                    selectedMod = mod;
                }
            }
            else {
                selectedMod = mod;
            }

        skip_mod:
            continue;
        }

        if (selectedMod) {
            m_modsToLoad.push_back(selectedMod);
            visited.insert(selectedMod);
        }
    } while (selectedMod != nullptr);

    for (auto mod : m_modsToLoad) {
        log::debug("{}, early: {}", mod->getID(), mod->needsEarlyLoad());
    }
}

void Loader::Impl::continueRefreshModGraph() {
    if (m_refreshingModCount != 0) {
        queueInMainThread([this]() {
            this->continueRefreshModGraph();
        });
        return;
    }

    if  (m_lateRefreshedModCount > 0) {
        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_timerBegin).count();
        log::debug("Took {}s", static_cast<float>(time) / 1000.f);
    }

    log::debug("Continuing mod graph refresh...");
    log::NestScope nest;

    m_timerBegin = std::chrono::high_resolution_clock::now();

    switch (m_loadingState) {
        case LoadingState::Mods:
            if (!m_modsToLoad.empty()) {
                auto mod = m_modsToLoad.front();
                m_modsToLoad.pop_front();
                log::info("Loading mod {} {}", mod->getID(), mod->getVersion());
                this->loadModGraph(mod, false);
                break;
            }
            m_loadingState = LoadingState::Problems;
            [[fallthrough]];
        case LoadingState::Problems:
            log::info("Finding problems");
            {
                log::NestScope nest;
                this->findProblems();
            }
            m_loadingState = LoadingState::Done;
            {
                auto end = std::chrono::high_resolution_clock::now();
                auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_timerBegin).count();
                log::debug("Took {}s", static_cast<float>(time) / 1000.f);
            }
            break;
        default:
            m_loadingState = LoadingState::Done;
            log::warn("Impossible loading state, resetting to 'Done'! "
                "Was Loader::Impl::continueRefreshModGraph() called from the wrong place?");
            break;
    }

    if (m_loadingState != LoadingState::Done) {
        queueInMainThread([this]() {
            this->continueRefreshModGraph();
        });
    }
}

std::vector<LoadProblem> Loader::Impl::getProblems() const {
    return m_problems;
}

void Loader::Impl::forceReset() {
    for (auto& [_, mod] : m_mods) {
        delete mod;
    }
    m_mods.clear();
    log::Logger::get()->clear();
    std::filesystem::remove_all(dirs::getModRuntimeDir());
    std::filesystem::remove_all(dirs::getTempDir());
}

bool Loader::Impl::isReadyToHook() const {
    return m_readyToHook;
}

void Loader::Impl::addUninitializedHook(Hook* hook, Mod* mod) {
    m_uninitializedHooks.emplace_back(hook, mod);
}

Result<> Loader::Impl::unzipGeodeFile(std::string modId) {
    // Unzip .geode file into temp dir
    auto tempDir = dirs::getModRuntimeDir() / modId;
    auto extractPath = dirs::getModsDir() / (modId + ".geode");
    std::string binaryName = modId + GEODE_PLATFORM_EXTENSION;

    auto datePath = tempDir / "modified-at";
    std::string currentHash = file::readString(datePath).unwrapOr("");

    auto modifiedDate = std::filesystem::last_write_time(extractPath);
    auto modifiedCount = std::chrono::duration_cast<std::chrono::milliseconds>(modifiedDate.time_since_epoch());
    auto modifiedHash = std::to_string(modifiedCount.count());
    if (currentHash == modifiedHash) {
        log::debug("Same hash detected, skipping unzip");
        return Ok();
    }
    log::debug("Hash mismatch detected, unzipping");

    std::error_code ec;
    std::filesystem::remove_all(tempDir, ec);
    if (ec) {
        auto message = ec.message();
        #ifdef GEODE_IS_WINDOWS
            // Force the error message into English
            message = formatSystemError(ec.value());
        #endif
        return Err("Unable to delete temp dir: " + message);
    }

    (void)utils::file::createDirectoryAll(tempDir);

    GEODE_UNWRAP_INTO(auto unzip, file::Unzip::create(extractPath));
    if (!unzip.hasEntry(binaryName)) {
        return Err(
            fmt::format("Unable to find platform binary under the name \"{}\"", binaryName)
        );
    }
    GEODE_UNWRAP(unzip.extractAllTo(tempDir));

    auto res = file::writeString(datePath, modifiedHash);
    if (!res) {
        log::warn("Failed to write modified date of geode zip: {}", res.unwrapErr());
    }

    return Ok();
}

bool Loader::Impl::loadHooks() {
    m_readyToHook = true;
    bool hadErrors = false;
    for (auto const& [hook, mod] : m_uninitializedHooks) {
        auto res = hook->enable();
        if (!res) {
            log::logImpl(Severity::Error, mod, "{}", res.unwrapErr());
            hadErrors = true;
        }
    }
    m_uninitializedHooks.clear();
    return !hadErrors;
}

void Loader::Impl::queueInMainThread(ScheduledFunction&& func) {
    std::lock_guard<std::mutex> lock(m_mainThreadMutex);
    m_mainThreadQueue.push_back(std::forward<ScheduledFunction>(func));
}

void Loader::Impl::executeMainThreadQueue() {
    // copy queue to avoid locking mutex if someone is
    // running addToMainThread inside their function
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

// TODO: Support for quoted launch args w/ spaces (this will be backwards compatible)
// e.g. "--geode:arg=My spaced value"
void Loader::Impl::initLaunchArguments() {
    auto launchStr = this->getLaunchCommand();
    auto args = string::split(launchStr, " ");
    for (const auto& arg : args) {
        if (!arg.starts_with(LAUNCH_ARG_PREFIX)) {
            continue;
        }
        auto pair = arg.substr(LAUNCH_ARG_PREFIX.size());
        auto sep = pair.find('=');
        if (sep == std::string::npos) {
            m_launchArgs.insert({ pair, "true" });
            continue;
        }
        auto key = pair.substr(0, sep);
        auto value = pair.substr(sep + 1);
        m_launchArgs.insert({ key, value });
    }
    for (const auto& pair : m_launchArgs) {
        log::debug("Loaded '{}' as '{}'", pair.first, pair.second);
    }
}

std::vector<std::string> Loader::Impl::getLaunchArgumentNames() const {
    return map::keys(m_launchArgs);
}

bool Loader::Impl::hasLaunchArgument(std::string_view name) const {
    return m_launchArgs.find(std::string(name)) != m_launchArgs.end();
}

std::optional<std::string> Loader::Impl::getLaunchArgument(std::string_view name) const {
    auto value = m_launchArgs.find(std::string(name));
    if (value == m_launchArgs.end()) {
        return std::nullopt;
    }
    return std::optional(value->second);
}

bool Loader::Impl::getLaunchFlag(std::string_view name) const {
    auto arg = this->getLaunchArgument(name);
    return arg.has_value() && arg.value() == "true";
}

Result<tulip::hook::HandlerHandle> Loader::Impl::getHandler(void* address) {
    if (!m_handlerHandles.count(address)) {
        return Err("Handler does not exist at address");
    }
    return Ok(m_handlerHandles[address].first);
}

Result<tulip::hook::HandlerHandle> Loader::Impl::getOrCreateHandler(void* address, tulip::hook::HandlerMetadata const& metadata) {
    if (m_handlerHandles.count(address) && m_handlerHandles[address].second > 0) {
        m_handlerHandles[address].second++;
        return Ok(m_handlerHandles[address].first);
    }
    GEODE_UNWRAP_INTO(auto handle, tulip::hook::createHandler(address, metadata));
    m_handlerHandles[address].first = handle;
    m_handlerHandles[address].second = 1;
    return Ok(handle);
}

Result<tulip::hook::HandlerHandle> Loader::Impl::getAndDecreaseHandler(void* address) {
    if (!m_handlerHandles.count(address)) {
        return Err("Handler does not exist at address");
    }
    auto handle = m_handlerHandles[address].first;
    m_handlerHandles[address].second--;
    return Ok(handle);
}

Result<> Loader::Impl::removeHandlerIfNeeded(void* address) {
    if (!m_handlerHandles.count(address)) {
        return Err("Handler does not exist at address");
    }
    auto handle = m_handlerHandles[address].first;
    if (m_handlerHandles[address].second == 0) {
        GEODE_UNWRAP(tulip::hook::removeHandler(handle));
    }
    return Ok();
}

bool Loader::Impl::isSafeMode() const {
    return m_forceSafeMode || this->getLaunchFlag("safe-mode");
}

void Loader::Impl::forceSafeMode() {
    m_forceSafeMode = true;
}

void Loader::Impl::installModManuallyFromFile(std::filesystem::path const& path, std::function<void()> after) {
    auto res = ModMetadata::createFromGeodeFile(path);
    if (!res) {
        FLAlertLayer::create(
            "Invalid File",
            fmt::format(
                "The path <cy>'{}'</c> is not a valid Geode mod: {}",
                path.string(),
                res.unwrapErr()
            ),
            "OK"
        )->show();
        return;
    }
    auto meta = res.unwrap();

    auto check = meta.checkTargetVersions();
    if (!check) {
        FLAlertLayer::create(
            "Invalid Mod Version",
            fmt::format(
                "The mod <cy>{}</c> can not be installed: {}",
                meta.getID(),
                check.unwrapErr()
            ),
            "OK"
        )->show();
    }

    auto doInstallModFromFile = [this, path, meta, after]() {
        std::error_code ec;

        static size_t MAX_ATTEMPTS = 10;

        // Figure out a free path to install to
        auto installTo = dirs::getModsDir() / fmt::format("{}.geode", meta.getID());
        size_t counter = 0;
        while (std::filesystem::exists(installTo, ec) && counter < MAX_ATTEMPTS) {
            installTo = dirs::getModsDir() / fmt::format("{}-{}.geode", meta.getID(), counter);
            counter += 1;
        }

        // This is incredibly unlikely but theoretically possible
        if (counter >= MAX_ATTEMPTS) {
            FLAlertLayer::create(
                "Unable to Install",
                fmt::format(
                    "Unable to install mod <co>{}</c>: Can't find a free filename!",
                    meta.getID()
                ),
                "OK"
            )->show();
            return;
        }

        // Actually copy the file over to the install directory
        std::filesystem::copy_file(path, installTo, ec);
        if (ec) {
            FLAlertLayer::create(
                "Unable to Install",
                fmt::format(
                    "Unable to install mod <co>{}</c>: {} (Error code <cr>{}</c>)",
                    meta.getID(), ec.message(), ec.value()
                ),
                "OK"
            )->show();
            return;
        }

        // Mark an updated mod as updated or add to the mods list
        if (m_mods.contains(meta.getID())) {
            m_mods.at(meta.getID())->m_impl->m_requestedAction = ModRequestedAction::Update;
        }
        // Otherwise add a new Mod
        // This should be safe as all of the scary stuff in setup() is only relevant
        // for mods that are actually running
        else {
            auto mod = new Mod(meta);
            auto res = mod->m_impl->setup();
            if (!res) {
                log::error("Unable to set up manually installed mod: {}", res.unwrapErr());
            }
            (void)mod->enable();
            m_mods.insert({ meta.getID(), mod });
        }

        if (after) after();

        // No need for the user to go and manually clean up the file
        createQuickPopup(
            "Mod Installed",
            fmt::format(
                "Mod <co>{}</c> has been successfully installed from file! "
                "<cy>Do you want to delete the original file?</c>",
                meta.getName()
            ),
            "OK", "Delete File",
            [path](auto, bool btn2) {
                if (btn2) {
                    std::error_code ec;
                    std::filesystem::remove(path, ec);
                    if (ec) {
                        FLAlertLayer::create(
                            "Unable to Delete",
                            fmt::format(
                                "Unable to delete <cy>{}</c>: {} (Error code <cr>{}</c>)",
                                path, ec.message(), ec.value()
                            ),
                            "OK"
                        )->show();
                    }
                    // No need to show a confirmation popup if successful since that's
                    // to be assumed via pressing the button on the previous popup
                }
            }
        );
    };

    if (auto existing = Loader::get()->getInstalledMod(meta.getID())) {
        createQuickPopup(
            "Already Installed",
            fmt::format(
                "The mod <cy>{}</c> <cj>{}</c> has already been installed "
                "as version <cl>{}</c>. Do you want to <co>replace the "
                "installed version with the file</c>?",
                meta.getID(), meta.getVersion(),
                existing->getVersion()
            ),
            "Cancel", "Replace",
            [doInstallModFromFile, path, existing, meta](auto, bool btn2) mutable {
                std::error_code ec;
                std::filesystem::remove(existing->getPackagePath(), ec);
                if (ec) {
                    FLAlertLayer::create(
                        "Unable to Uninstall",
                        fmt::format(
                            "Unable to uninstall <cy>{}</c>: {} (Error code <cr>{}</c>)",
                            existing->getID(), ec.message(), ec.value()
                        ),
                        "OK"
                    )->show();
                    return;
                }
                doInstallModFromFile();
            }
        );
        return;
    }

    doInstallModFromFile();
}

bool Loader::Impl::isRestartRequired() const {
    for (auto mod : Loader::get()->getAllMods()) {
        if (mod->getRequestedAction() != ModRequestedAction::None) {
            return true;
        }
        if (ModSettingsManager::from(mod)->restartRequired()) {
            return true;
        }
    }
    if (server::ModDownloadManager::get()->wantsRestart()) {
        return true;
    }
    return false;
}
