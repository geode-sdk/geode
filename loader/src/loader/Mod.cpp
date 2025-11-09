#include "ModImpl.hpp"

#include <Geode/loader/Dirs.hpp>
#include <Geode/loader/Mod.hpp>
#include <loader/ModMetadataImpl.hpp>
#include <optional>
#include <string_view>
#include <server/Server.hpp>

using namespace geode::prelude;

Mod::Mod(ModMetadata const& metadata) : m_impl(std::make_unique<Impl>(this, metadata)) {}

Mod::~Mod() {}

std::string Mod::getID() const {
    return m_impl->getID();
}

std::string Mod::getName() const {
    return m_impl->getName();
}

std::vector<std::string> Mod::getDevelopers() const {
    return m_impl->getDevelopers();
}

std::optional<std::string> Mod::getDescription() const {
    return m_impl->getDescription();
}

std::optional<std::string> Mod::getDetails() const {
    return m_impl->getDetails();
}

std::filesystem::path Mod::getPackagePath() const {
    return m_impl->getPackagePath();
}

VersionInfo Mod::getVersion() const {
    return m_impl->getVersion();
}

matjson::Value& Mod::getSaveContainer() {
    return m_impl->getSaveContainer();
}

matjson::Value& Mod::getSavedSettingsData() {
    return m_impl->m_settings->getSaveData();
}

bool Mod::isEnabled() const {
    return m_impl->isEnabled();
}

bool Mod::isOrWillBeEnabled() const {
    bool enabled = m_impl->isEnabled();
    if (m_impl->m_requestedAction == ModRequestedAction::Enable) {
        enabled = true;
    }
    else if (m_impl->m_requestedAction == ModRequestedAction::Disable) {
        enabled = false;
    }
    return enabled;
}

bool Mod::isInternal() const {
    return m_impl->isInternal();
}

bool Mod::needsEarlyLoad() const {
    return m_impl->needsEarlyLoad();
}

ModMetadata Mod::getMetadata() const {
    return m_impl->getMetadata();
}

ModMetadata const& Mod::getMetadataRef() const {
    return m_impl->getMetadata();
}

std::filesystem::path Mod::getTempDir() const {
    return m_impl->getTempDir();
}

std::filesystem::path Mod::getBinaryPath() const {
    return m_impl->getBinaryPath();
}

std::filesystem::path Mod::getResourcesDir() const {
    return dirs::getModRuntimeDir() / this->getID() / "resources" / this->getID();
}

matjson::Value Mod::getDependencySettingsFor(std::string_view dependencyID) const {
    auto id = std::string(dependencyID);
    auto const& settings = ModMetadataImpl::getImpl(m_impl->m_metadata).m_dependencySettings;
    return settings.contains(id) ? settings.at(id) : matjson::Value();
}

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE)
void Mod::setMetadata(ModMetadata const& metadata) {
    m_impl->setMetadata(metadata);
}

std::vector<Mod*> Mod::getDependants() const {
    return m_impl->getDependants();
}
#endif

Mod::CheckUpdatesTask Mod::checkUpdates() const {
    return server::checkUpdates(this).map(
        [](auto* result) -> Mod::CheckUpdatesTask::Value {
            if (result->isOk()) {
                if (auto value = result->unwrap()) {
                    if (value->replacement) {
                        return Err(
                            "Mod has been replaced by {} - please visit the Geode "
                            "menu to install the replacement",
                            value->replacement->id
                        );
                    }
                    return Ok(value->version);
                }
                return Ok(std::nullopt);
            }
            auto err = result->unwrapErr();
            return Err("{} (code {})", err.details, err.code);
        },
        [](auto*) { return std::monostate(); }
    );
}

Result<> Mod::saveData() {
    return m_impl->saveData();
}

Result<> Mod::loadData() {
    return m_impl->loadData();
}

std::filesystem::path Mod::getSaveDir() const {
    return m_impl->getSaveDir();
}

std::filesystem::path Mod::getConfigDir(bool create) const {
    return m_impl->getConfigDir(create);
}

std::filesystem::path Mod::getPersistentDir(bool create) const {
    return m_impl->getPersistentDir(create);
}

bool Mod::hasSettings() const {
    return m_impl->hasSettings();
}

std::vector<std::string> Mod::getSettingKeys() const {
    return m_impl->getSettingKeys();
}

bool Mod::hasSetting(std::string_view key) const {
    return m_impl->hasSetting(key);
}

std::shared_ptr<Setting> Mod::getSetting(std::string_view key) const {
    return m_impl->m_settings->get(std::string(key));
}

Result<> Mod::registerCustomSettingType(std::string_view type, SettingGenerator generator) {
    return m_impl->m_settings->registerCustomSettingType(type, generator);
}

std::string Mod::getLaunchArgumentName(std::string_view name) const {
    return m_impl->getLaunchArgumentName(name);
}

std::vector<std::string> Mod::getLaunchArgumentNames() const {
    return m_impl->getLaunchArgumentNames();
}

bool Mod::hasLaunchArgument(std::string_view name) const {
    return m_impl->hasLaunchArgument(name);
}

std::optional<std::string> Mod::getLaunchArgument(std::string_view name) const {
    return m_impl->getLaunchArgument(name);
}

bool Mod::getLaunchFlag(std::string_view name) const {
    return m_impl->getLaunchFlag(name);
}

Result<Hook*> Mod::claimHook(std::shared_ptr<Hook> hook) {
    return m_impl->claimHook(hook);
}

Result<> Mod::disownHook(Hook* hook) {
    return m_impl->disownHook(hook);
}

std::vector<Hook*> Mod::getHooks() const {
    return m_impl->getHooks();
}

Result<Patch*> Mod::claimPatch(std::shared_ptr<Patch> patch) {
    return m_impl->claimPatch(patch);
}

Result<> Mod::disownPatch(Patch* patch) {
    return m_impl->disownPatch(patch);
}

std::vector<Patch*> Mod::getPatches() const {
    return m_impl->getPatches();
}

Result<> Mod::enable() {
    return m_impl->enable();
}

Result<> Mod::disable() {
    return m_impl->disable();
}

Result<> Mod::uninstall(bool deleteSaveData) {
    return m_impl->uninstall(deleteSaveData);
}

bool Mod::isUninstalled() const {
    return m_impl->isUninstalled();
}

ModRequestedAction Mod::getRequestedAction() const {
    return m_impl->getRequestedAction();
}

bool Mod::depends(std::string_view id) const {
    return m_impl->depends(id);
}

bool Mod::hasUnresolvedDependencies() const {
    return m_impl->hasUnresolvedDependencies();
}

bool Mod::hasUnresolvedIncompatibilities() const {
    return m_impl->hasUnresolvedIncompatibilities();
}

std::string_view Mod::expandSpriteName(std::string_view name) {
    return m_impl->expandSpriteName(name);
}

ModJson Mod::getRuntimeInfo() const {
    return m_impl->getRuntimeInfo();
}

bool Mod::isLoggingEnabled() const {
    return m_impl->isLoggingEnabled();
}

void Mod::setLoggingEnabled(bool enabled) {
    m_impl->setLoggingEnabled(enabled);
}

Severity Mod::getLogLevel() const {
    return m_impl->getLogLevel();
}

void Mod::setLogLevel(Severity level) {
    m_impl->setLogLevel(level);
}

bool Mod::hasSavedValue(std::string_view key) {
    return this->getSaveContainer().contains(key);
}

bool Mod::hasLoadProblems() const {
    return m_impl->hasLoadProblems();
}
std::optional<LoadProblem> Mod::targetsOutdatedVersion() const {
    for (auto problem : this->getAllProblems()) {
        if (problem.isOutdated()) {
            return problem;
        }
    }
    return std::nullopt;
}
std::vector<LoadProblem> Mod::getAllProblems() const {
    return m_impl->getProblems();
}
std::vector<LoadProblem> Mod::getProblems() const {
    std::vector<LoadProblem> result;
    for (auto problem : this->getAllProblems()) {
        if (problem.isProblem()) {
            result.push_back(problem);
        }
    }
    return result;
}
std::vector<LoadProblem> Mod::getRecommendations() const {
    std::vector<LoadProblem> result;
    for (auto problem : this->getAllProblems()) {
        if (problem.isSuggestion()) {
            result.push_back(problem);
        }
    }
    return result;
}
bool Mod::shouldLoad() const {
    return m_impl->shouldLoad();
}
bool Mod::isCurrentlyLoading() const {
    return m_impl->isCurrentlyLoading();
}

int Mod::getLoadPriority() const {
    return m_impl->getLoadPriority();
}
