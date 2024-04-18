#include "ModImpl.hpp"

#include <Geode/loader/Dirs.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Index.hpp>
#include <optional>
#include <string_view>

using namespace geode::prelude;

Mod::Mod(ModMetadata const& metadata) : m_impl(std::make_unique<Impl>(this, metadata)) {}

Mod::~Mod() {}

std::string Mod::getID() const {
    return m_impl->getID();
}

std::string Mod::getName() const {
    return m_impl->getName();
}

std::string Mod::getDeveloper() const {
    return m_impl->getDevelopers().empty() ? "" : m_impl->getDevelopers().front();
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

ghc::filesystem::path Mod::getPackagePath() const {
    return m_impl->getPackagePath();
}

VersionInfo Mod::getVersion() const {
    return m_impl->getVersion();
}

matjson::Value& Mod::getSaveContainer() {
    return m_impl->getSaveContainer();
}

matjson::Value& Mod::getSavedSettingsData() {
    return m_impl->getSavedSettingsData();
}

bool Mod::isEnabled() const {
    return m_impl->isEnabled();
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

ghc::filesystem::path Mod::getTempDir() const {
    return m_impl->getTempDir();
}

ghc::filesystem::path Mod::getBinaryPath() const {
    return m_impl->getBinaryPath();
}

ghc::filesystem::path Mod::getResourcesDir() const {
    return dirs::getModRuntimeDir() / this->getID() / "resources" / this->getID();
}

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE)
void Mod::setMetadata(ModMetadata const& metadata) {
    m_impl->setMetadata(metadata);
}

std::vector<Mod*> Mod::getDependants() const {
    return m_impl->getDependants();
}
#endif

std::optional<VersionInfo> Mod::hasAvailableUpdate() const {
    if (auto item = Index::get()->getItem(this->getID(), std::nullopt)) {
        if (
            item->getMetadata().getVersion() > this->getVersion() &&
            item->getAvailablePlatforms().contains(GEODE_PLATFORM_TARGET)
        ) {
            return item->getMetadata().getVersion();
        }
    }
    return std::nullopt;
}

Result<> Mod::saveData() {
    return m_impl->saveData();
}

Result<> Mod::loadData() {
    return m_impl->loadData();
}

ghc::filesystem::path Mod::getSaveDir() const {
    return m_impl->getSaveDir();
}

ghc::filesystem::path Mod::getConfigDir(bool create) const {
    return m_impl->getConfigDir(create);
}

bool Mod::hasSettings() const {
    return m_impl->hasSettings();
}

std::vector<std::string> Mod::getSettingKeys() const {
    return m_impl->getSettingKeys();
}

bool Mod::hasSetting(std::string_view const key) const {
    return m_impl->hasSetting(key);
}

std::optional<Setting> Mod::getSettingDefinition(std::string_view const key) const {
    return m_impl->getSettingDefinition(key);
}

SettingValue* Mod::getSetting(std::string_view const key) const {
    return m_impl->getSetting(key);
}

void Mod::registerCustomSetting(std::string_view const key, std::unique_ptr<SettingValue> value) {
    return m_impl->registerCustomSetting(key, std::move(value));
}

std::vector<std::string> Mod::getLaunchArgumentNames() const {
    return m_impl->getLaunchArgumentNames();
}

bool Mod::hasLaunchArgument(std::string_view const name) const {
    return m_impl->hasLaunchArgument(name);
}

std::optional<std::string> Mod::getLaunchArgument(std::string_view const name) const {
    return m_impl->getLaunchArgument(name);
}

bool Mod::getLaunchFlag(std::string_view const name) const {
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

bool Mod::depends(std::string_view const id) const {
    return m_impl->depends(id);
}

bool Mod::hasUnresolvedDependencies() const {
    return m_impl->hasUnresolvedDependencies();
}

bool Mod::hasUnresolvedIncompatibilities() const {
    return m_impl->hasUnresolvedIncompatibilities();
}

char const* Mod::expandSpriteName(char const* name) {
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

bool Mod::hasSavedValue(std::string_view const key) {
    return this->getSaveContainer().contains(key);
}

bool Mod::hasProblems() const {
    return m_impl->hasProblems();
}

bool Mod::shouldLoad() const {
    return m_impl->shouldLoad();
}

bool Mod::isCurrentlyLoading() const {
    return m_impl->isCurrentlyLoading();
}