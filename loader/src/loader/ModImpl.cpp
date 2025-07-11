#include "ModImpl.hpp"
#include "LoaderImpl.hpp"
#include "ModMetadataImpl.hpp"
#include "HookImpl.hpp"
#include "PatchImpl.hpp"
#include "about.hpp"
#include "console.hpp"

#include <hash/hash.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/loader/Hook.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/ModEvent.hpp>
#include <Geode/platform/cplatform.h>
#include <Geode/utils/file.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <Geode/utils/string.hpp>
#include <filesystem>
#include <optional>
#include <string>
#include <system_error>
#include <vector>
#include <string_view>

using namespace geode::prelude;

static constexpr const char* humanReadableDescForAction(ModRequestedAction action) {
    switch (action) {
        default: return "(Unknown action)";
        case ModRequestedAction::None: return "(No action has been taken)";
        case ModRequestedAction::Enable: return "Mod has been enabled";
        case ModRequestedAction::Disable: return "Mod has been disabled";
        case ModRequestedAction::Uninstall: return "Mod has been uninstalled";
        case ModRequestedAction::UninstallWithSaveData: return "Mod has been uninstalled";
        case ModRequestedAction::Update: return "Mod has been updated";
    }
}

Mod::Impl* ModImpl::get() {
    return Mod::get()->m_impl.get();
}

Mod::Impl* ModImpl::getImpl(Mod* mod)  {
    return mod->m_impl.get();
}

Mod::Impl::Impl(Mod* self, ModMetadata const& metadata) : m_self(self), m_metadata(metadata) {
}

Mod::Impl::~Impl() = default;

Result<> Mod::Impl::setup() {
    m_saveDirPath = dirs::getModsSaveDir() / m_metadata.getID();
    (void) utils::file::createDirectoryAll(m_saveDirPath);

    // always create temp dir for all mods, even if disabled, so resources can be loaded
    GEODE_UNWRAP(this->createTempDir().mapErr([](auto const& err) {
        return fmt::format("Unable to create temp dir: {}", err);
    }));

    m_settings = std::make_unique<ModSettingsManager>(m_metadata);
    auto loadRes = this->loadData();
    if (!loadRes) {
        log::warn("Unable to load data for \"{}\": {}", m_metadata.getID(), loadRes.unwrapErr());
    }
    if (!m_resourcesLoaded && !this->isInternal()) {
        auto searchPathRoot = dirs::getModRuntimeDir() / m_metadata.getID() / "resources";

        // Hi, linux bros!
        Loader::get()->queueInMainThread([searchPathRoot]() {
            CCFileUtils::get()->addSearchPath(searchPathRoot.string().c_str());
        });

        // binaries on macos are merged, so make the platform binaries merged as well
        auto const binaryPlatformId = PlatformID::toShortString(GEODE_PLATFORM_TARGET GEODE_MACOS(, true));

        auto const binariesDir = searchPathRoot / m_metadata.getID() / "binaries" / binaryPlatformId;

        std::error_code code;
        if (std::filesystem::exists(binariesDir, code) && !code) {
            LoaderImpl::get()->addNativeBinariesPath(binariesDir);
        }

        m_resourcesLoaded = true;
    }

    return Ok();
}

// Getters

std::filesystem::path Mod::Impl::getSaveDir() const {
    return m_saveDirPath;
}

std::string Mod::Impl::getID() const {
    return m_metadata.getID();
}

std::string Mod::Impl::getName() const {
    return m_metadata.getName();
}

std::vector<std::string> Mod::Impl::getDevelopers() const {
    return m_metadata.getDevelopers();
}

std::optional<std::string> Mod::Impl::getDescription() const {
    return m_metadata.getDescription();
}

std::optional<std::string> Mod::Impl::getDetails() const {
    return m_metadata.getDetails();
}

ModMetadata const& Mod::Impl::getMetadata() const {
    return m_metadata;
}

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE)
void Mod::Impl::setMetadata(ModMetadata const& metadata) {
    m_metadata = metadata;
}
std::vector<Mod*> Mod::Impl::getDependants() const {
    return m_dependants;
}
#endif

std::filesystem::path Mod::Impl::getTempDir() const {
    return m_tempDirName;
}

std::filesystem::path Mod::Impl::getBinaryPath() const {
    if (auto value = LoaderImpl::get()->getBinaryPath()) {
        return std::filesystem::path(value.value()) / m_metadata.getBinaryName();
    }
    return m_tempDirName / m_metadata.getBinaryName();
}

std::filesystem::path Mod::Impl::getPackagePath() const {
    return m_metadata.getPath();
}

VersionInfo Mod::Impl::getVersion() const {
    return m_metadata.getVersion();
}

matjson::Value& Mod::Impl::getSaveContainer() {
    return m_saved;
}

bool Mod::Impl::isEnabled() const {
    return m_enabled || this->isInternal();
}

bool Mod::Impl::isInternal() const {
    return m_metadata.getID() == "geode.loader";
}

bool Mod::Impl::needsEarlyLoad() const {
    if (this->getMetadata().needsEarlyLoad()) return true;
    for (auto& dep : m_dependants) {
        if (dep->needsEarlyLoad()) return true;
    }
    return false;
}

std::vector<Hook*> Mod::Impl::getHooks() const {
    std::vector<Hook*> ret;
    for (auto& hook : m_hooks) {
        ret.push_back(hook.get());
    }
    return ret;
}

std::vector<Patch*> Mod::Impl::getPatches() const {
    std::vector<Patch*> ret;
    for (auto& patch : m_patches) {
        ret.push_back(patch.get());
    }
    return ret;
}

// Settings and saved values

Result<> Mod::Impl::loadData() {
    // Settings
    // Check if settings exist
    auto settingPath = m_saveDirPath / "settings.json";
    if (std::filesystem::exists(settingPath)) {
        GEODE_UNWRAP_INTO(auto json, utils::file::readJson(settingPath));
        auto load = m_settings->load(json);
        if (!load) {
            log::warn("Unable to load settings: {}", load.unwrapErr());
        }
    }

    // Saved values
    auto savedPath = m_saveDirPath / "saved.json";
    if (std::filesystem::exists(savedPath)) {
        GEODE_UNWRAP_INTO(auto data, utils::file::readString(savedPath));
        m_saved = GEODE_UNWRAP(matjson::parse(data).mapErr([](auto&& err) {
            return fmt::format("Unable to parse saved values: {}", err);
        }));
        if (!m_saved.isObject()) {
            log::warn("saved.json was somehow not an object, forcing it to one");
            m_saved = matjson::Value::object();
        }
    }

    return Ok();
}

Result<> Mod::Impl::saveData() {
    if (this->getRequestedAction() == ModRequestedAction::UninstallWithSaveData) {
        // Don't save data if the mod is being uninstalled with save data
        return Ok();
    }

    // ModSettingsManager keeps track of the whole savedata
    matjson::Value json = m_settings->save();

    // saveData is expected to be synchronous, and always called from GD thread
    ModStateEvent(m_self, ModEventType::DataSaved).post();

    auto res = utils::file::writeStringSafe(m_saveDirPath / "settings.json", json.dump());
    if (!res) {
        log::error("Unable to save settings: {}", res.unwrapErr());
    }
    auto res2 = utils::file::writeStringSafe(m_saveDirPath / "saved.json", m_saved.dump());
    if (!res2) {
        log::error("Unable to save values: {}", res2.unwrapErr());
    }

    return Ok();
}

bool Mod::Impl::hasSettings() const {
    return m_metadata.getSettings().size();
}

std::vector<std::string> Mod::Impl::getSettingKeys() const {
    std::vector<std::string> keys;
    for (auto& [key, _] : m_metadata.getSettings()) {
        keys.push_back(key);
    }
    return keys;
}

bool Mod::Impl::hasSetting(std::string_view key) const {
    for (auto& setting : m_metadata.getSettings()) {
        if (setting.first == key) {
            return true;
        }
    }
    return false;
}

std::string Mod::Impl::getLaunchArgumentName(std::string_view name) const {
    return this->getID() + "." + std::string(name);
}

std::vector<std::string> Mod::Impl::getLaunchArgumentNames() const {
    auto prefix = this->getID() + ".";
    std::vector<std::string> names;
    for (const auto& name : Loader::get()->getLaunchArgumentNames()) {
        if (name.starts_with(prefix)) {
            names.push_back(name.substr(prefix.size()));
        }
    }
    return names;
}

bool Mod::Impl::hasLaunchArgument(std::string_view name) const {
    return Loader::get()->hasLaunchArgument(this->getLaunchArgumentName(name));
}

std::optional<std::string> Mod::Impl::getLaunchArgument(std::string_view name) const {
    return Loader::get()->getLaunchArgument(this->getLaunchArgumentName(name));
}

bool Mod::Impl::getLaunchFlag(std::string_view name) const {
    return Loader::get()->getLaunchFlag(this->getLaunchArgumentName(name));
}

// Loading, Toggling, Installing

Result<> Mod::Impl::loadBinary() {
    if (!this->isInternal() && LoaderImpl::get()->isSafeMode()) {
        // pretend to have loaded the mod, so that it still shows up on the mod list properly,
        // while the user can still toggle/uninstall it
        m_enabled = true;
        return Ok();
    }

    log::debug("Loading binary for mod {}", m_metadata.getID());
    if (m_enabled)
        return Ok();

    if (!std::filesystem::exists(this->getBinaryPath())) {
        std::error_code ec;
        std::filesystem::remove(m_tempDirName / "modified-at", ec);
        return Err(
            fmt::format(
                "Failed to load {}: No binary could be found for current platform.\n"
                "This mod doesn't support this platform" GEODE_WINDOWS(" or something deleted it (like an antivirus)") ".",
                m_metadata.getID()
            )
        );
    }

    LoaderImpl::get()->provideNextMod(m_self);

    m_enabled = true;
    m_isCurrentlyLoading = true;
    auto res = this->loadPlatformBinary();
    if (!res) {
        m_isCurrentlyLoading = false;
        m_enabled = false;
        // make sure to free up the next mod mutex
        LoaderImpl::get()->releaseNextMod();
        log::error("Failed to load binary for mod {}: {}", m_metadata.getID(), res.unwrapErr());
        return res;
    }

    LoaderImpl::get()->releaseNextMod();

    ModStateEvent(m_self, ModEventType::Loaded).post();
    ModStateEvent(m_self, ModEventType::DataLoaded).post();

    // do we not have a function for getting all the dependencies of a mod directly? ok then
    // Anyway this lets all of this mod's dependencies know it has been loaded
    // In case they're API mods and want to know those kinds of things
    for (auto const& dep : ModMetadataImpl::getImpl(m_metadata).m_dependencies) {
        if (auto depMod = Loader::get()->getLoadedMod(dep.id)) {
            DependencyLoadedEvent(depMod, m_self).post();
        }
    }

    m_isCurrentlyLoading = false;

    return Ok();
}

Result<> Mod::Impl::enable() {
    switch (m_requestedAction) {
        // Allow reverting disabling
        case ModRequestedAction::Disable: {
            m_requestedAction = ModRequestedAction::None;
        } break;

        // Only possible to enable otherwise
        case ModRequestedAction::None: {
            m_requestedAction = ModRequestedAction::Enable;
        } break;

        default: {
            return Err(humanReadableDescForAction(m_requestedAction));
        } break;
    }
    Mod::get()->setSavedValue("should-load-" + m_metadata.getID(), true);

    return Ok();
}

Result<> Mod::Impl::disable() {
    switch (m_requestedAction) {
        // Allow reverting enabling
        case ModRequestedAction::Enable: {
            m_requestedAction = ModRequestedAction::None;
        } break;

        // Only possible to enable otherwise
        case ModRequestedAction::None: {
            m_requestedAction = ModRequestedAction::Disable;
        } break;

        default: {
            return Err(humanReadableDescForAction(m_requestedAction));
        } break;
    }
    Mod::get()->setSavedValue("should-load-" + m_metadata.getID(), false);

    return Ok();
}

Result<> Mod::Impl::uninstall(bool deleteSaveData) {
    // Allow uninstalling if the mod has been disabled / enabled
    if (modRequestedActionIsUninstall(m_requestedAction)) {
        return Err(humanReadableDescForAction(m_requestedAction));
    }

    if (this->isInternal()) {
        utils::game::launchLoaderUninstaller(deleteSaveData);
        utils::game::exit(true);
        return Ok();
    }

    m_requestedAction = deleteSaveData ?
        ModRequestedAction::UninstallWithSaveData :
        ModRequestedAction::Uninstall;

    // Make loader forget the mod should be disabled
    Mod::get()->getSaveContainer().erase("should-load-" + m_metadata.getID());

    std::error_code ec;
    std::filesystem::remove(m_metadata.getPath(), ec);
    if (ec) {
        return Err(
            "Unable to delete mod's .geode file: " + ec.message()
        );
    }

    if (deleteSaveData) {
        std::filesystem::remove_all(this->getSaveDir(), ec);
        if (ec) {
            return Err(
                "Unable to delete mod's save directory: " + ec.message()
            );
        }
    }

    return Ok();
}

bool Mod::Impl::isUninstalled() const {
    return modRequestedActionIsUninstall(m_requestedAction);
}

ModRequestedAction Mod::Impl::getRequestedAction() const {
    return m_requestedAction;
}

// Dependencies

Result<> Mod::Impl::updateDependencies() {
    return Err("Mod::updateDependencies is no longer needed, "
        "as this is handled by Loader::refreshModGraph");
}

bool Mod::Impl::hasUnresolvedDependencies() const {
    for (auto const& dep : m_metadata.getDependencies()) {
        if (!dep.isResolved()) {
            return true;
        }
    }
    return false;
}

bool Mod::Impl::hasUnresolvedIncompatibilities() const {
    for (auto const& dep : m_metadata.getIncompatibilities()) {
        if (!dep.isResolved()) {
            return true;
        }
    }
    return false;
}

bool Mod::Impl::depends(std::string_view id) const {
    return utils::ranges::contains(m_metadata.getDependencies(), [id](ModMetadata::Dependency const& t) {
        return t.id == id;
    });
}

// Hooks

Result<Hook*> Mod::Impl::claimHook(std::shared_ptr<Hook> hook) {
    auto res1 = hook->m_impl->setOwner(m_self);
    if (!res1) {
        return Err("Cannot claim hook: {}", res1.unwrapErr());
    }

    m_hooks.push_back(hook);

    auto ptr = hook.get();
    if (!this->isEnabled() || !hook->getAutoEnable())
        return Ok(ptr);

    if (!LoaderImpl::get()->isReadyToHook() && hook->getAutoEnable()) {
        LoaderImpl::get()->addUninitializedHook(ptr, m_self);
        return Ok(ptr);
    }

    auto res2 = ptr->enable();
    if (!res2) {
        return Err("Cannot enable hook: {}", res2.unwrapErr());
    }

    return Ok(ptr);
}

Result<> Mod::Impl::disownHook(Hook* hook) {
    if (hook->getOwner() != m_self) {
        return Err("Cannot disown hook not owned by this mod");
    }

    auto res1 = hook->m_impl->setOwner(nullptr);
    if (!res1) {
        return Err("Cannot disown hook: {}", res1.unwrapErr());
    }

    auto foundIt = std::find_if(m_hooks.begin(), m_hooks.end(), [&](auto& a) {
        return a.get() == hook;
    });
    if (foundIt == m_hooks.end())
        return Err("WEE, WOO !! Something just went horribly wrong! "
                   "A hook that was getting disowned had its owner set but the owner "
                   "didn't have the hook in m_hooks.");

    m_hooks.erase(foundIt);

    if (!this->isEnabled() || !hook->getAutoEnable())
        return Ok();

    auto res2 = hook->disable();
    if (!res2) {
        return Err("Cannot disable hook: {}", res2.unwrapErr());
    }

    return Ok();
}

// Patches

Result<Patch*> Mod::Impl::claimPatch(std::shared_ptr<Patch> patch) {
    auto res1 = patch->m_impl->setOwner(m_self);
    if (!res1) {
        return Err("Cannot claim patch: {}", res1.unwrapErr());
    }

    m_patches.push_back(patch);

    auto ptr = patch.get();
    if (!this->isEnabled() || !patch->getAutoEnable())
        return Ok(ptr);

    auto res2 = ptr->enable();
    if (!res2) {
        return Err("Cannot enable patch: {}", res2.unwrapErr());
    }

    return Ok(ptr);
}

Result<> Mod::Impl::disownPatch(Patch* patch) {
    if (patch->getOwner() != m_self) {
        return Err("Cannot disown patch not owned by this mod");
    }

    auto res1 = patch->m_impl->setOwner(nullptr);
    if (!res1) {
        return Err("Cannot disown patch: {}", res1.unwrapErr());
    }

    auto foundIt = std::find_if(m_patches.begin(), m_patches.end(), [&](auto& a) {
        return a.get() == patch;
    });
    if (foundIt == m_patches.end())
        return Err("WEE, WOO !! Something just went horribly wrong! "
                   "A patch that was getting disowned had its owner set but the owner "
                   "didn't have the patch in m_patches.");


    if (this->isEnabled() && patch->getAutoEnable()) {
        auto res2 = patch->disable();
        if (!res2) {
            return Err("Cannot disable patch: {}", res2.unwrapErr());
        }
    }

    m_patches.erase(foundIt);

    return Ok();
}

// Misc.

Result<> Mod::Impl::createTempDir() {
    // Check if temp dir already exists
    if (!m_tempDirName.string().empty()) {
        return Ok();
    }

    // If the info doesn't specify a path, don't do anything
    if (m_metadata.getPath().string().empty()) {
        return Ok();
    }

    // Create geode/temp
    auto tempDir = dirs::getModRuntimeDir();
    if (!file::createDirectoryAll(tempDir)) {
        return Err("Unable to create mods' runtime directory");
    }

    // Create geode/temp/mod.id
    auto tempPath = tempDir / m_metadata.getID();
    if (!file::createDirectoryAll(tempPath)) {
        return Err("Unable to create mod runtime directory");
    }

    // Mark temp dir creation as succesful
    m_tempDirName = tempPath;

    return Ok();
}

std::filesystem::path Mod::Impl::getConfigDir(bool create) const {
    auto dir = dirs::getModConfigDir() / m_metadata.getID();
    if (create) {
        (void)file::createDirectoryAll(dir);
    }
    return dir;
}

std::filesystem::path Mod::Impl::getPersistentDir(bool create) const {
    auto dir = dirs::getModPersistentDir() / m_metadata.getID();
    if (create) {
        (void)file::createDirectoryAll(dir);
    }
    return dir;
}

std::string_view Mod::Impl::expandSpriteName(std::string_view name) {
    std::string nameKey(name);
    if (m_expandedSprites.contains(nameKey)) return m_expandedSprites[nameKey];

    auto exp = new char[name.size() + 2 + m_metadata.getID().size()];
    auto exps = (m_metadata.getID() + "/") + name.data();
    memcpy(exp, exps.c_str(), exps.size() + 1);

    m_expandedSprites[nameKey] = exp;

    return exp;
}

ModJson Mod::Impl::getRuntimeInfo() const {
    auto json = m_metadata.toJSON();

    auto obj = matjson::Value::object();
    obj["hooks"] = matjson::Value::array();
    for (auto hook : m_hooks) {
        obj["hooks"].push(ModJson(hook->getRuntimeInfo()));
    }
    obj["patches"] = matjson::Value::array();
    for (auto patch : m_patches) {
        obj["patches"].push(ModJson(patch->getRuntimeInfo()));
    }
    obj["loaded"] = m_enabled;
    obj["temp-dir"] = this->getTempDir();
    obj["save-dir"] = this->getSaveDir();
    obj["config-dir"] = this->getConfigDir(false);
    json["runtime"] = obj;

    return json;
}

bool Mod::Impl::isLoggingEnabled() const {
    return m_loggingEnabled;
}

void Mod::Impl::setLoggingEnabled(bool enabled) {
    m_loggingEnabled = enabled;
}

Severity Mod::Impl::getLogLevel() const {
    return m_logLevel;
}

void Mod::Impl::setLogLevel(Severity level) {
    m_logLevel = level;
}

bool Mod::Impl::shouldLoad() const {
    return Mod::get()->getSavedValue<bool>("should-load-" + m_metadata.getID(), true) || this->isInternal();
}

bool Mod::Impl::isCurrentlyLoading() const {
    return m_isCurrentlyLoading;
}

bool Mod::Impl::hasLoadProblems() const {
    for (auto const& problem : m_problems) {
        if (problem.isProblem()) {
            return true;
        }
    }
    return false;
}

std::vector<LoadProblem> Mod::Impl::getProblems() const {
    return m_problems;
}

static Result<ModMetadata> getModImplInfo() {
    auto json = GEODE_UNWRAP(matjson::parse(about::getLoaderModJson()).mapErr([](auto&& err) {
        return fmt::format("Unable to parse mod.json: {}", err);
    }));

    GEODE_UNWRAP_INTO(auto info, ModMetadata::create(json));
    return Ok(info);
}

Mod* Loader::Impl::getInternalMod() {
    auto& mod = Mod::sharedMod<>;
    if (mod)
        return mod;
    if (m_mods.contains("geode.loader")) {
        log::warn("Something went wrong and Mod::sharedMod<> got unset after the internal mod was created! Setting sharedMod back...");
        mod = m_mods["geode.loader"];
        return mod;
    }
    auto infoRes = getModImplInfo();
    if (!infoRes) {
        console::messageBox(
            "Fatal Internal Error",
            "Unable to create internal mod info: \"" + infoRes.unwrapErr() +
                "\"\n"
                "This is a fatal internal error in the loader, please "
                "contact Geode developers immediately!"
        );
        mod = new Mod(ModMetadata("geode.loader"));
    }
    else {
        mod = new Mod(infoRes.unwrap());
    }
    mod->m_impl->m_enabled = true;
    m_mods.insert({ mod->getID(), mod });
    return mod;
}

Result<> Loader::Impl::setupInternalMod() {
    GEODE_UNWRAP(Mod::get()->m_impl->setup());
    auto resourcesDir = dirs::getGeodeResourcesDir() / Mod::get()->getID();
    GEODE_UNWRAP(ModMetadataImpl::getImpl(ModImpl::get()->m_metadata).addSpecialFiles(resourcesDir));
    return Ok();
}
