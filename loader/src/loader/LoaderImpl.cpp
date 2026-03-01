#include "LoaderImpl.hpp"
#include <cocos2d.h>

#include "ModImpl.hpp"
#include "ModMetadataImpl.hpp"
#include "LogImpl.hpp"
#include "console.hpp"

#include <Geode/loader/Event.hpp>
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
#include <unordered_set>
#include <vector>

#include <server/DownloadManager.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

comm::EventCenter* geode::comm::EventCenter::get() {
    static auto s_instance = new EventCenter();
    return s_instance;
}

Loader::Impl* LoaderImpl::get() {
    return Loader::get()->m_impl.get();
}

Loader::Impl::Impl() = default;

Loader::Impl::~Impl() = default;

// Initialization

void Loader::Impl::createDirectories() {
    log::debug("Creating necessary directories");
    (void) utils::file::createDirectoryAll(dirs::getSaveDir());
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
    log::debug("Removing unnecessary directories");
    // clean up of stale data from Geode v2
    if(std::filesystem::exists(dirs::getGeodeDir() / "index")) {
        async::runtime().spawnBlocking<void>([] {
            std::error_code ec;
            std::filesystem::remove_all(dirs::getGeodeDir() / "index", ec);
        });
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

    if (auto value = this->getLaunchArgument("use-common-handler-offset")) {
        log::info("Using common handler offset: {}", value.value());
        log::NestScope nest;
        auto offset = numFromString<size_t>(value.value(), 16);
        if (offset.isErr()) {
            log::error("Could not parse common handler offset, falling back to default");
        } else {
            log::info("Disabling runtime intervening");
            auto res = tulip::hook::disableRuntimeIntervening((void*)(base::get() + offset.unwrap()));
            if (res.isErr()) {
                log::error("Failed to disable runtime intervening: {}", res.unwrapErr());
            } else {
                log::info("Runtime intervening disabled successfully");
                m_isPatchless = true;
            }
        }
    }

    if (auto value = this->getLaunchArgument("binary-dir")) {
        log::info("Using custom binary directory: {}", value.value());
        m_binaryPath = value.value();
    }

    if (this->getLaunchFlag("enable-tulip-hook-logs")) {
        log::info("Enabling TulipHook logs");
        tulip::hook::setLogCallback([](std::string_view msg) {
            log::debug("TulipHook: {}", msg);
        });
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
    ModStateEvent(ModEventType::Loaded, Mod::get()).send();

    log::info("Refreshing mod graph");
    this->refreshModGraph();

    m_isSetup = true;

    return Ok();
}

void Loader::Impl::addSearchPaths() {
    log::debug("Adding search paths");
    CCFileUtils::get()->addPriorityPath(utils::string::pathToString(dirs::getGeodeResourcesDir()).c_str());
    CCFileUtils::get()->addPriorityPath(utils::string::pathToString(dirs::getModRuntimeDir()).c_str());
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

bool Loader::Impl::isModInstalled(std::string_view id) const {
    return this->getInstalledMod(id) != nullptr;
}

Mod* Loader::Impl::getInstalledMod(std::string_view id) const {
    auto it = m_mods.find(id);
    if (it != m_mods.end() && !it->second->isUninstalled()) {
        return it->second;
    }
    return nullptr;
}

bool Loader::Impl::isModLoaded(std::string_view id) const {
    return this->getLoadedMod(id) != nullptr;
}

Mod* Loader::Impl::getLoadedMod(std::string_view id) const {
    auto it = m_mods.find(id);
    if (it != m_mods.end() && it->second->isLoaded()) {
        return it->second;
    }
    return nullptr;
}

void Loader::Impl::updateModResources(Mod* mod) {
    if (!mod->isInternal()) {
        // geode.loader resource is stored somewhere else, which is already added anyway
        auto searchPathRoot = dirs::getModRuntimeDir() / mod->getID() / "resources";
        CCFileUtils::get()->addSearchPath(utils::string::pathToString(searchPathRoot).c_str());
    }

    // only thing needs previous setup is spritesheets
    auto& sheets = mod->getMetadata().getSpritesheets();
    if (sheets.empty())
        return;

    log::debug("{}", mod->getID());
    log::NestScope nest;

    for (auto const& sheet : sheets) {
        log::debug("Adding sheet {}", sheet);

        auto ccfu = CCFileUtils::get();

        std::string tmp;
        tmp.reserve(sheet.size() + 6);
        tmp.append(sheet);
        tmp.append(".png");
        auto pngPath = ccfu->fullPathForFilename(tmp.c_str(), false);
        bool missingPng = std::string_view{pngPath} == tmp;
        tmp.resize(sheet.size());
        tmp.append(".plist");
        auto plistPath = ccfu->fullPathForFilename(tmp.c_str(), false);
        bool missingPlist = std::string_view{plistPath} == tmp;

        if (missingPng || missingPlist) {
            log::warn(
                R"(The resource dir of "{}" is missing "{}" png and/or plist files)",
                mod->getID(), sheet
            );
        }
        else {
            CCTextureCache::get()->addImage(pngPath.c_str(), false);
            CCSpriteFrameCache::get()->addSpriteFramesWithFile(plistPath.c_str());
        }
    }
}

void Loader::Impl::addProblem(LoadProblem const& problem) {
    if (auto mod = std::get_if<Mod*>(&problem.cause)) {
        auto impl = ModImpl::getImpl(*mod);
        if (impl->m_problem) {
            log::error(
                "Mod {} already has a LoadProblem (message: {}) set? This should not happen, overriding with {}",
                impl->m_metadata.getID(),
                impl->m_problem->message,
                problem.message
            );
        }
        impl->m_problem = problem;
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

            auto modMetadata = ModMetadata::createFromGeodeFile(entry.path());

            log::debug("id: {}", modMetadata.getID());
            log::debug("version: {}", modMetadata.getVersion());
            log::debug("early: {}", modMetadata.needsEarlyLoad() ? "yes" : "no");

            if (std::find_if(modQueue.begin(), modQueue.end(), [&](auto& item) {
                    return modMetadata.getID() == item.getID();
                }) != modQueue.end()) {
                log::error("Failed to queue: a mod with the same ID is already queued");

                auto modMetadata = ModMetadataImpl::createInvalidMetadata(
                    entry.path(),
                    "A mod with the same ID is already present.",
                    // Passing `nullopt` to `createInvalidMetadata` generates a
                    // random non-conflicting ID
                    std::nullopt
                );
                modQueue.push_back(modMetadata);

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
                LoadProblem::Type::Unknown,
                mod,
                res.unwrapErr()
            });
            log::error("Failed to set up: {}", res.unwrapErr());
            continue;
        }

        m_mods.insert({metadata.getID(), mod});
    }

    if (!m_keybindSettings.empty()) {
        KeyboardInputEvent().listen([this](KeyboardInputData& data) {
            Keybind keybind(data.key, data.modifiers);
            bool down = data.action != KeyboardInputData::Action::Release;
            bool repeat = data.action == KeyboardInputData::Action::Repeat;

            auto it = m_keybindSettings.find(keybind);
            if (it == m_keybindSettings.end()) {
                return ListenerResult::Propagate;
            }

            if (down) {
                for (auto& setting : it->second) {
                    if (!repeat) m_activeKeybinds[setting.get()] = keybind;
                    if (KeybindSettingPressedEventV3(setting->getModID(), setting->getKey()).send(keybind, down, repeat, data.timestamp)) {
                        return ListenerResult::Stop;
                    }
                }
            } else {
                for (auto& setting : it->second) {
                    auto it2 = m_activeKeybinds.find(setting.get());
                    if (it2 != m_activeKeybinds.end() && it2->second.key == data.key) {
                        KeybindSettingPressedEventV3(setting->getModID(), setting->getKey()).send(keybind, down, repeat, data.timestamp);
                        m_activeKeybinds.erase(it2);
                    }
                }
            }
            return ListenerResult::Propagate;
        }).leak();

        MouseInputEvent().listen([this](MouseInputData& data) {
            auto key = MouseInputData::buttonToKeyCode(data.button);
            if (key == KEY_None) {
                return ListenerResult::Propagate;
            }
            auto keybind = Keybind(key, data.modifiers);
            bool down = data.action == MouseInputData::Action::Press;
            for (auto& setting : m_keybindSettings[keybind]) {
                if (KeybindSettingPressedEventV3(setting->getModID(), setting->getKey()).send(keybind, down, false, data.timestamp)) {
                    return ListenerResult::Stop;
                }
            }
            return ListenerResult::Propagate;
        }).leak();
    }
}

void Loader::Impl::buildModGraph() {
    for (auto const& [id, mod] : m_mods) {
        log::debug("{}", mod->getID());
        log::NestScope nest;
        for (auto& dependency : mod->m_impl->m_metadata.m_impl->m_dependencies) {
            log::debug("{}", dependency.getID());
            if (!m_mods.contains(dependency.getID())) {
                dependency.setMod(nullptr);
                continue;
            }

            dependency.setMod(m_mods[dependency.getID()]);

            if (!dependency.getVersion().compare(dependency.getMod()->getVersion())) {
                dependency.setMod(nullptr);
                continue;
            }

            if (!dependency.isRequired() || dependency.getMod() == nullptr) {
                continue;
            }

            dependency.getMod()->m_impl->m_dependants.push_back(mod);
            dependency.getMod()->m_impl->m_settings->addDependant(mod);
        }
        for (auto& incompatibility : mod->m_impl->m_metadata.m_impl->m_incompatibilities) {
            incompatibility.setMod(
                m_mods.contains(incompatibility.getID()) ? m_mods[incompatibility.getID()] : nullptr
            );
        }
    }
}

void Loader::Impl::loadModGraph(Mod* node, bool early) {
    // Check version first, as it's not worth trying to load a mod with an
    // invalid target version
    // Also this makes it so that when GD updates, outdated mods get shown as
    // "Outdated" in the UI instead of "Missing Dependencies"
    if (!node->getMetadata().checkGameVersion() || !node->getMetadata().checkGeodeVersion()) {
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

    if (node->isLoaded()) {
        log::error("Mod {} already loaded, this should never happen", node->getID());
        return;
    }

    m_currentlyLoadingMod = node;
    m_refreshingModCount += 1;
    m_refreshedModCount += 1;
    m_lateRefreshedModCount += early ? 0 : 1;

    auto unzipFunction = [this, node]() {
        log::debug("Unzipping .geode file");
        auto res = this->unzipGeodeFile(node->getMetadata());
        return res;
    };

    auto loadFunction = [this, node, early]() {
        if (node->shouldLoad()) {
            log::debug("Loading binary");
            auto res = node->m_impl->loadBinary();
            if (!res) {
                this->addProblem({
                    LoadProblem::Type::Unknown,
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

    if (early) {
        auto res = unzipFunction();
        if (!res) {
            this->addProblem({ LoadProblem::Type::Unknown, node, res.unwrapErr() });
            log::error("Failed to unzip: {}", res.unwrapErr());
            m_refreshingModCount -= 1;
            return;
        }
        loadFunction();
    }
    else {
        auto nest = log::saveNest();

        async::runtime().spawnBlocking<void>([=, this]() {
            log::loadNest(nest);
            auto res = unzipFunction();
            this->queueInMainThread([=, this, res = std::move(res)]() {
                auto prevNest = log::saveNest();
                log::loadNest(nest);
                if (!res) {
                    this->addProblem({ LoadProblem::Type::Unknown, node, res.unwrapErr() });
                    log::error("Failed to unzip: {}", res.unwrapErr());
                    m_refreshingModCount -= 1;
                    log::loadNest(prevNest);
                    return;
                }
                loadFunction();
                log::loadNest(prevNest);
            });
        });
    }
}

void Loader::Impl::findProblems() {
    for (auto const& [id, mod] : m_mods) {
        // If this mod already has a problem, continue as usual
        if (mod->getLoadProblem()) {
            continue;
        }

        // Check version first, as it's not worth trying to load a mod with an
        // invalid target version
        // Also this makes it so that when GD updates, outdated mods get shown as
        // "Outdated" in the UI instead of "Missing Dependencies"
        // btw hi if you ever change this line please update LoaderImpl and ModMetadataImpl as well
        auto res = mod->getMetadata().checkGameVersion();
        if (!res) {
            this->addProblem({ LoadProblem::Type::Outdated, mod, res.unwrapErr() });
            log::error("{}", res.unwrapErr());
            continue;
        }

        if (m_isPatchless && mod->getMetadata().needsPatching()) {
            this->addProblem({ LoadProblem::Type::Unknown, mod, "This mod requires JIT" });
            log::error("{} requires JIT, but loader is JIT-less", id);
            continue;
        }

        auto geodeVerRes = mod->getMetadata().checkGeodeVersion();
        if (!geodeVerRes) {
            this->addProblem({ LoadProblem::Type::Outdated, mod, geodeVerRes.unwrapErr() });
            log::error("{}", geodeVerRes.unwrapErr());
            continue;
        }

        if (mod->getMetadata().hasErrors()) {
            auto message = ranges::join(mod->getMetadata().getErrors(), ", ");
            this->addProblem({ LoadProblem::Type::InvalidGeodeFile, mod, message });
            log::error("{} failed to load: {}", id, message);
            continue;
        }

        // Don't check dependencies or incompatibilities for disabled mods
        if (!mod->shouldLoad()) {
            log::debug("{} is not enabled", id);
            continue;
        }

        log::debug("{}", id);
        log::NestScope nest;

        // These are all for collecting up a nice error message that has all the
        // same categories of errors bunched up
        std::vector<std::string> noninstalledDependencies;
        std::vector<std::string> disabledDependencies;
        std::vector<std::string> outdatedDependencies;
        std::vector<std::string> breakingIncompatibilities;

        // Collect breaking incompatibilities
        for (auto const& dep : mod->getMetadata().getIncompatibilities()) {
            if (!dep.getMod() || !dep.getVersion().compare(dep.getMod()->getVersion()) || !dep.getMod()->shouldLoad() || !dep.getMod()->getMetadata().checkGameVersion()) {
                continue;
            }
            if (dep.isBreaking()) {
                // todo: which direction is this relationship in?
                // todo: if mod A marks B as breaking, is B the one that shouldn't be loaded?
                breakingIncompatibilities.push_back(dep.getMod()->getName());
                log::warn("{} breaks {} {}", id, dep.getID(), dep.getVersion());
            }
            else {
                // todo: add warning to UI
                log::warn("{} conflicts with {} {}", id, dep.getID(), dep.getVersion());
            }
        }
        // Collect missing required dependencies
        for (auto const& dep : mod->getMetadata().getDependencies()) {
            if (
                !dep.isRequired() ||
                (dep.getMod() && dep.getMod()->isLoaded() && dep.getVersion().compare(dep.getMod()->getVersion()))
            ) {
                continue;
            }
            log::error("{} requires {} ({})", id, dep.getID(), dep.getVersion());
            if (!m_mods.contains(dep.getID())) {
                noninstalledDependencies.push_back(fmt::format("{} ({})", dep.getID(), dep.getVersion()));
            }
            else {
                auto installedDependency = m_mods.at(dep.getID());
                if (!installedDependency->isLoaded()) {
                    disabledDependencies.push_back(installedDependency->getName());
                }
                else if (dep.getVersion().compareWithReason(installedDependency->getVersion()) == VersionCompareResult::TooOld) {
                    outdatedDependencies.push_back(fmt::format(
                        "{} ({} -> {})",
                        installedDependency->getName(),
                        installedDependency->getVersion(),
                        dep.getVersion()
                    ));
                }
                else {
                    // fires on major mismatch or too new version of dependency
                    noninstalledDependencies.push_back(fmt::format("{} ({})", dep.getID(), dep.getVersion()));
                }
            }
        }

        // Add single LoadProblem for this mod's incompatibilities and missing dependencies
        if (
            breakingIncompatibilities.size() ||
            noninstalledDependencies.size() ||
            disabledDependencies.size() ||
            outdatedDependencies.size()
        ) {
            std::string message;
            bool lastWasIncompatible = false;
            for (auto const& [whatToDo, mods] : std::initializer_list<std::pair<std::string_view, std::vector<std::string> const&>> {
                std::make_pair("incompatible", breakingIncompatibilities),
                std::make_pair("installed", noninstalledDependencies),
                std::make_pair("enabled", disabledDependencies),
                std::make_pair("updated", outdatedDependencies),
            }) {
                if (mods.empty()) continue;
                if (message.empty()) {
                    // Incompatibilities have a different message because
                    // they're not missing dependencies
                    if (whatToDo == "incompatible") {
                        message = fmt::format(
                            "{} is incompatible with the following mod{}: {}",
                            mod->getName(), (mods.size() == 1 ? "" : "s"), ranges::join(mods, ", ")
                        );
                        lastWasIncompatible = true;
                    }
                    // Everything else is missing dependency-related
                    else {
                        message = fmt::format(
                            "{} requires the following mod{} to be {}: {}",
                            mod->getName(), (mods.size() == 1 ? "" : "s"), whatToDo, ranges::join(mods, ", ")
                        );
                    }
                }
                else {
                    message += "\n";

                    // Enclose missing dependencies after incompatibilities in
                    // parentheses since those aren't the main point of the
                    // error message
                    message += (breakingIncompatibilities.size() ? "(" : "");

                    // If the first sentence was about an incompatibility, we
                    // need to have the next sentence specify that we are now
                    // listing dependencies
                    if (lastWasIncompatible) {
                        message += fmt::format(
                            "And requires these mod{} to be {}: {}",
                            (mods.size() == 1 ? "" : "s"), whatToDo, ranges::join(mods, ", ")
                        );
                    }
                    else {
                        message += fmt::format(
                            "And these mod{} to be {}: {}",
                            (mods.size() == 1 ? "" : "s"), whatToDo, ranges::join(mods, ", ")
                        );
                    }
                    message += breakingIncompatibilities.size() ? ")" : "";
                    lastWasIncompatible = false;
                }
            }
            this->addProblem({
                // Incompatibilities take precedence since the mod won't ever
                // be loadable even if you get the dependencies
                breakingIncompatibilities.size() ?
                    LoadProblem::Type::HasIncompatibilities :
                    LoadProblem::Type::MissingDependencies,
                mod, message
            });
        }

        Mod* myEpicMod = mod; // clang fix
        // if the mod is not loaded but there are no problems related to it
        if (
            !mod->isLoaded() &&
            mod->shouldLoad() &&
            !std::any_of(m_problems.begin(), m_problems.end(), [myEpicMod](auto& item) {
                return std::holds_alternative<ModMetadata>(item.cause) &&
                    std::get<ModMetadata>(item.cause).getID() == myEpicMod->getID() ||
                    std::holds_alternative<Mod*>(item.cause) &&
                    std::get<Mod*>(item.cause) == myEpicMod;
            })
        ) {
            this->addProblem({
                LoadProblem::Type::Unknown,
                mod,
                fmt::format("Unknown error loading mod {}", id)
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
        utils::thread::setName("Main");

        log::info("Loading non-early mods");
        this->continueRefreshModGraph();
    });
}

void Loader::Impl::orderModStack() {
    std::unordered_set<Mod*> visited;

    auto& dependants = ModImpl::get()->m_dependants;
    std::sort(dependants.begin(), dependants.end(), [](Mod* a, Mod* b) {
        // early load check (early loads go first)
        auto aEarly = a->needsEarlyLoad();
        auto bEarly = b->needsEarlyLoad();
        if (aEarly != bEarly) {
            return aEarly > bEarly;
        }

        // load priority check (higher priority/lower number goes first)
        auto aPriority = a->getLoadPriority();
        auto bPriority = b->getLoadPriority();
        if (aPriority != bPriority) {
            return aPriority < bPriority;
        }

        // fallback to alphabetical id order
        return a->getID() < b->getID();
    });

    auto visit = [&](Mod* mod, auto&& visit) -> void {
        if (mod == nullptr || mod == Mod::get()) return;
        if (visited.contains(mod))
            return;
        visited.insert(mod);
        for (auto dep : mod->m_impl->m_metadata.m_impl->m_dependencies) {
            if (!dep.isRequired()) {
                continue;
            }
            visit(dep.getMod(), visit);
        }
        m_modsToLoad.push_back(mod);
        log::debug("{} [{}]{}", mod->getID(), mod->getLoadPriority(), mod->needsEarlyLoad() ? " (early)" : "");
    };

    for (auto mod : dependants) {
        visit(mod, visit);
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
    else {
        GameEvent(GameEventType::ModsLoaded).send();
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

static bool isPlatformBinary(std::string_view modID, std::string_view filename) {
    if (!filename.starts_with(modID)) {
        return false;
    }

    return filename.ends_with(".dll")
        || filename.ends_with(".dylib")
        || filename.ends_with(".android32.so")
        || filename.ends_with(".android64.so")
        || filename.ends_with(".ios.dylib");
}

Result<> Loader::Impl::unzipGeodeFile(ModMetadata metadata) {
    // Unzip .geode file into temp dir
    auto tempDir = dirs::getModRuntimeDir() / metadata.getID();

    auto datePath = tempDir / "modified-at";
    std::string currentHash = file::readString(datePath).unwrapOr("");

    std::error_code ec;
    auto modifiedDate = std::filesystem::last_write_time(metadata.getPath(), ec);
    if (ec) {
        auto message = formatSystemError(ec.value());
        return Err("Unable to get last modified time of zip: " + message);
    }
    auto modifiedCount = std::chrono::duration_cast<std::chrono::milliseconds>(modifiedDate.time_since_epoch());
    auto modifiedHash = std::to_string(modifiedCount.count());
    if (currentHash == modifiedHash) {
        log::debug("Same hash detected, skipping unzip");
        return Ok();
    }
    log::debug("Hash mismatch detected, unzipping");

    std::filesystem::remove_all(tempDir, ec);
    if (ec) {
        auto message = formatSystemError(ec.value());
        return Err("Unable to delete temp dir: " + message);
    }

    (void)utils::file::createDirectoryAll(tempDir);

    GEODE_UNWRAP_INTO(auto unzip, file::Unzip::create(metadata.getPath()));
    if (!unzip.hasEntry(metadata.getBinaryName())) {
        return Err(
            fmt::format("Unable to find platform binary under the name \"{}\"", metadata.getBinaryName())
        );
    }
    GEODE_UNWRAP(unzip.extractAllTo(tempDir));

    // Delete binaries for other platforms since they're pointless
    // The if should never fail, but you never know
    for (auto& entry : std::filesystem::directory_iterator(tempDir)) {
        if (entry.is_directory()) {
            continue;
        }

        const std::string filename = utils::string::pathToString(entry.path().filename());
        if (metadata.getBinaryName() == filename || !isPlatformBinary(metadata.getID(), filename)) {
            continue;
        }

        // The binary is not for our platform, delete!
        // We don't really care if the deletion succeeds though.
        std::error_code ec;
        std::filesystem::remove(entry.path(), ec);
    }

    // Check if there is a binary that we need to move over from the unzipped binaries dir
    if (this->isPatchless()) {
        // TODO: enable in 4.7.0
        // auto src = dirs::getModBinariesDir() / metadata.getBinaryName();
        auto src = dirs::getModRuntimeDir() / "binaries" / metadata.getBinaryName();
        auto dst = tempDir / metadata.getBinaryName();
        if (std::filesystem::exists(src)) {
            std::error_code ec;
            std::filesystem::rename(src, dst, ec);
            if (ec) {
                auto message = formatSystemError(ec.value());
                return Err(fmt::format("Failed to move binary from {} to {}: {}",
                    src, dst, message
                ));
            }
        }
    }

    auto res = file::writeString(datePath, modifiedHash);
    if (!res) {
        log::warn("Failed to write modified date of geode zip, will try to unzip next launch: {}", res.unwrapErr());
    }

    return Ok();
}

Result<> Loader::Impl::extractBinary(ModMetadata metadata) {
    if (!this->isPatchless()) {
        // If we are not patchless, there is no need to extract the binary separately
        return Ok();
    }

    // Extract the binary from the .geode file
    GEODE_UNWRAP_INTO(auto unzip, file::Unzip::create(metadata.getPath()));
    if (!unzip.hasEntry(metadata.getBinaryName())) {
        return Err(
            fmt::format("Unable to find platform binary under the name \"{}\"", metadata.getBinaryName())
        );
    }
    // TODO: enable in 4.7.0
    // GEODE_UNWRAP(unzip.extractTo(metadata.getBinaryName(), dirs::getModBinariesDir() / metadata.getBinaryName()));
    GEODE_UNWRAP(unzip.extractTo(metadata.getBinaryName(), dirs::getModRuntimeDir() / "binaries" / metadata.getBinaryName()));

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
    m_mainThreadQueue.push_back(std::move(func));
}

void Loader::Impl::executeMainThreadQueue() {
    m_mainThreadMutex.lock();

    // to prevent allocating an extra vector every frame we have a separate temp queue,
    // where we first move all functions before executing them.
    // this means there are no allocations in the common case, and we maintain deadlock safety
    // since we do not call any functions while holding the mutex

    auto& queue = m_mainThreadQueue;
    auto& execQueue = m_mainThreadQueueExec;
    execQueue.reserve(queue.size());
    std::move(queue.begin(), queue.end(), std::back_inserter(execQueue));
    queue.clear();

    m_mainThreadMutex.unlock();

    // call all functions
    for (auto& func : execQueue) {
        func();
    }

    execQueue.clear();
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

    std::vector<std::string> arguments;
    bool inQuotes = false;
    std::string currentArg;
    for (auto const c : launchStr) {
        if (c == ' ' && !inQuotes) {
            arguments.emplace_back(std::move(currentArg));
            currentArg.clear();
            continue;
        }
        if (c == '"') {
            inQuotes = !inQuotes;
            continue;
        }
        currentArg.push_back(c);
    }
    arguments.emplace_back(std::move(currentArg));

    for (const auto& argstr : arguments) {
        std::string_view arg{argstr};
        if (!arg.starts_with(LAUNCH_ARG_PREFIX)) {
            continue;
        }
        arg.remove_prefix(LAUNCH_ARG_PREFIX.size());
        auto sep = arg.find('=');
        if (sep == std::string::npos) {
            m_launchArgs.insert({ std::string{arg}, "true" });
            continue;
        }

        m_launchArgs.insert({
            std::string{arg.substr(0, sep)},
            std::string{arg.substr(sep + 1)}
        });
    }
    for (const auto& pair : m_launchArgs) {
        log::debug("Loaded '{}' as '{}'", pair.first, pair.second);
    }
}

std::vector<std::string> Loader::Impl::getLaunchArgumentNames() const {
    return map::keys(m_launchArgs);
}

bool Loader::Impl::hasLaunchArgument(std::string_view name) const {
    return m_launchArgs.find(name) != m_launchArgs.end();
}

std::optional<std::string> Loader::Impl::getLaunchArgument(std::string_view name) const {
    auto value = m_launchArgs.find(name);
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
    tulip::hook::HandlerHandle handle;
    GEODE_UNWRAP_INTO(handle, tulip::hook::createHandler(address, metadata));

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

void Loader::Impl::installModManuallyFromFile(std::filesystem::path const& path, geode::Function<void()> after) {
    auto meta = ModMetadata::createFromGeodeFile(path);
    if (meta.hasErrors()) {
        for (auto const& error : meta.getErrors()) {
            log::error("Error installing mod from file: {}", error);
        }
        FLAlertLayer::create(
            "Invalid File",
            fmt::format(
                "The path <cy>'{}'</c> is not a valid Geode mod!\n"
                "(Developers, check console for more)",
                path
            ),
            "OK"
        )->show();
        return;
    }

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

    auto doInstallModFromFile = [this, path, meta, after = std::move(after)]() mutable {
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
            },
            true,
            false
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
            [doInstallModFromFile = std::move(doInstallModFromFile), path, existing, meta](auto, bool btn2) mutable {
                if (btn2) {
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
            },
            true,
            false
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

bool Loader::Impl::isPatchless() const {
    return m_isPatchless;
}

std::optional<std::string> Loader::Impl::getBinaryPath() const {
    return m_binaryPath;
}

void Loader::Impl::onKeybindSettingChanged(std::shared_ptr<KeybindSettingV3> setting, std::vector<Keybind> const& keybinds) {
    for (auto& keybind : setting->getValue()) {
        if (!std::ranges::contains(keybinds, keybind)) {
            if (auto it = m_keybindSettings.find(keybind); it != m_keybindSettings.end()) {
                auto& vec = it->second;
                vec.erase(std::remove(vec.begin(), vec.end(), setting), vec.end());
                if (vec.empty()) {
                    m_keybindSettings.erase(it);
                }
            }
        }
    }

    for (auto& keybind : keybinds) {
        auto& settings = m_keybindSettings[keybind];
        if (!std::ranges::contains(settings, setting)) {
            settings.insert(std::ranges::find_if(settings, [&setting](auto& s) {
                return setting->getPriority() < s->getPriority();
            }), setting);
        }
    }
}
