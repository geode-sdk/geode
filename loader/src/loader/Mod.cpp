#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Dirs.hpp>
#include "ModImpl.hpp"

using namespace geode::prelude;

#pragma warning(suppress : 4996)
Mod::Mod(ModInfo const& info) : m_impl(std::make_unique<Impl>(this, info)) {}
Mod::Mod(ModMetadata const& metadata) : m_impl(std::make_unique<Impl>(this, metadata)) {}

Mod::~Mod() {}

std::string Mod::getID() const {
    return m_impl->getID();
}

std::string Mod::getName() const {
    return m_impl->getName();
}

std::string Mod::getDeveloper() const {
    return m_impl->getDeveloper();
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

json::Value& Mod::getSaveContainer() {
    return m_impl->getSaveContainer();
}

bool Mod::isEnabled() const {
    return m_impl->isEnabled();
}

bool Mod::isLoaded() const {
    return this->isEnabled();
}

bool Mod::supportsDisabling() const {
    return m_impl->supportsDisabling();
}

bool Mod::canDisable() const {
    return true;
}

bool Mod::canEnable() const {
    return true;
}

bool Mod::needsEarlyLoad() const {
    return m_impl->needsEarlyLoad();
}

bool Mod::supportsUnloading() const {
    return false;
}

bool Mod::wasSuccesfullyLoaded() const {
    return this->isEnabled();
}
bool Mod::wasSuccessfullyLoaded() const {
    return this->isEnabled();
}

ModInfo Mod::getModInfo() const {
    return this->getMetadata();
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

bool Mod::hasSetting(std::string const& key) const {
    return m_impl->hasSetting(key);
}

std::optional<Setting> Mod::getSettingDefinition(std::string const& key) const {
    return m_impl->getSettingDefinition(key);
}

SettingValue* Mod::getSetting(std::string const& key) const {
    return m_impl->getSetting(key);
}

void Mod::registerCustomSetting(std::string const& key, std::unique_ptr<SettingValue> value) {
    return m_impl->registerCustomSetting(key, std::move(value));
}

std::vector<Hook*> Mod::getHooks() const {
    return m_impl->getHooks();
}

Result<Hook*> Mod::addHook(Hook* hook) {
    return m_impl->addHook(hook);
}

Result<> Mod::enableHook(Hook* hook) {
    return m_impl->enableHook(hook);
}

Result<> Mod::disableHook(Hook* hook) {
    return m_impl->disableHook(hook);
}

Result<> Mod::removeHook(Hook* hook) {
    return m_impl->removeHook(hook);
}

Result<Patch*> Mod::patch(void* address, ByteVector const& data) {
    return m_impl->patch(address, data);
}

Result<> Mod::unpatch(Patch* patch) {
    return m_impl->unpatch(patch);
}

Result<> Mod::loadBinary() {
    return Err("Load mod binaries after startup is not supported");
}

Result<> Mod::unloadBinary() {
    return Err("Unloading mod binaries is not supported");
}

Result<> Mod::enable() {
    return m_impl->enable();
}

Result<> Mod::disable() {
    return m_impl->disable();
}

Result<> Mod::uninstall() {
    return m_impl->uninstall();
}

bool Mod::isUninstalled() const {
    return m_impl->isUninstalled();
}

ModRequestedAction Mod::getRequestedAction() const {
    return m_impl->getRequestedAction();
}

bool Mod::depends(std::string const& id) const {
    return m_impl->depends(id);
}

Result<> Mod::updateDependencies() {
    return m_impl->updateDependencies();
}

bool Mod::hasUnresolvedDependencies() const {
    return m_impl->hasUnresolvedDependencies();
}

bool Mod::hasUnresolvedIncompatibilities() const {
    return m_impl->hasUnresolvedIncompatibilities();
}

#pragma warning(suppress : 4996)
std::vector<Dependency> Mod::getUnresolvedDependencies() {
    return m_impl->getUnresolvedDependencies();
}

char const* Mod::expandSpriteName(char const* name) {
    return m_impl->expandSpriteName(name);
}

ModJson Mod::getRuntimeInfo() const {
    return m_impl->getRuntimeInfo();
}

bool Mod::hasSavedValue(std::string const& key) {
    return this->getSaveContainer().contains(key);
}
