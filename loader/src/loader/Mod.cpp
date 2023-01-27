#include <Geode/loader/Mod.hpp>
#include "ModImpl.hpp"

USE_GEODE_NAMESPACE();

Mod::Mod(ModInfo const& info) : m_impl(std::make_unique<Impl>(this, info)) {}

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
    return m_impl->isLoaded();
}

bool Mod::supportsDisabling() const {
    return m_impl->supportsDisabling();
}

bool Mod::supportsUnloading() const {
    return m_impl->supportsUnloading();
}

bool Mod::wasSuccesfullyLoaded() const {
    return m_impl->wasSuccesfullyLoaded();
}

ModInfo Mod::getModInfo() const {
    return m_impl->getModInfo();
}

ghc::filesystem::path Mod::getTempDir() const {
    return m_impl->getTempDir();
}

ghc::filesystem::path Mod::getBinaryPath() const {
    return m_impl->getBinaryPath();
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
    return m_impl->loadBinary();
}

Result<> Mod::unloadBinary() {
    return m_impl->unloadBinary();
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

bool Mod::depends(std::string const& id) const {
    return m_impl->depends(id);
}

bool Mod::hasUnresolvedDependencies() const {
    return m_impl->hasUnresolvedDependencies();
}

Result<> Mod::updateDependencies() {
    return m_impl->updateDependencies();
}

std::vector<Dependency> Mod::getUnresolvedDependencies() {
    return m_impl->getUnresolvedDependencies();
}

char const* Mod::expandSpriteName(char const* name) {
    return m_impl->expandSpriteName(name);
}

ModJson Mod::getRuntimeInfo() const {
    return m_impl->getRuntimeInfo();
}