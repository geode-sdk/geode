#include <Geode/loader/Loader.hpp>
#include <Geode/utils/VersionInfo.hpp>
#include <Geode/utils/file.hpp>
#include <json.hpp>

#include "ModInfoImpl.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

using namespace geode::prelude;

ModInfo::Impl& ModInfoImpl::getImpl(ModInfo& info)  {
    return *info.m_impl;
}

bool Dependency::isResolved() const {
    return !this->required ||
        (this->mod && this->mod->isLoaded() && this->mod->isEnabled() &&
         this->version.compare(this->mod->getVersion()));
}

bool ModInfo::Impl::validateID(std::string const& id) {
    return ModMetadata::Impl::validateID(id);
}

Result<ModInfo> ModInfo::Impl::create(ModJson const& json) {
    return ModMetadata::Impl::create(json);
}

Result<ModInfo> ModInfo::Impl::createFromFile(ghc::filesystem::path const& path) {
    return ModMetadata::Impl::createFromFile(path);
}

Result<ModInfo> ModInfo::Impl::createFromGeodeFile(ghc::filesystem::path const& path) {
    return ModMetadata::Impl::createFromGeodeFile(path);
}

Result<ModInfo> ModInfo::Impl::createFromGeodeZip(file::Unzip& unzip) {
    return ModMetadata::Impl::createFromGeodeZip(unzip);
}

ModJson ModInfo::Impl::toJSON() const {
    return m_metadata.m_rawJSON;
}

ModJson ModInfo::Impl::getRawJSON() const {
    return m_metadata.m_rawJSON;
}

bool ModInfo::Impl::operator==(ModInfo::Impl const& other) const {
    return this->m_metadata.m_id == other.m_metadata.m_id;
}

ghc::filesystem::path& ModInfo::path() {
    return m_impl->m_metadata.m_path;
}
ghc::filesystem::path const& ModInfo::path() const {
    return m_impl->m_metadata.m_path;
}

std::string& ModInfo::binaryName() {
    return m_impl->m_metadata.m_binaryName;
}
std::string const& ModInfo::binaryName() const {
    return m_impl->m_metadata.m_binaryName;
}

VersionInfo& ModInfo::version() {
    return m_impl->m_metadata.m_version;
}
VersionInfo const& ModInfo::version() const {
    return m_impl->m_metadata.m_version;
}

std::string& ModInfo::id() {
    return m_impl->m_metadata.m_id;
}
std::string const& ModInfo::id() const {
    return m_impl->m_metadata.m_id;
}

std::string& ModInfo::name() {
    return m_impl->m_metadata.m_name;
}
std::string const& ModInfo::name() const {
    return m_impl->m_metadata.m_name;
}

std::string& ModInfo::developer() {
    return m_impl->m_metadata.m_developer;
}
std::string const& ModInfo::developer() const {
    return m_impl->m_metadata.m_developer;
}

std::optional<std::string>& ModInfo::description() {
    return m_impl->m_metadata.m_description;
}
std::optional<std::string> const& ModInfo::description() const {
    return m_impl->m_metadata.m_description;
}

std::optional<std::string>& ModInfo::details() {
    return m_impl->m_metadata.m_details;
}
std::optional<std::string> const& ModInfo::details() const {
    return m_impl->m_metadata.m_details;
}

std::optional<std::string>& ModInfo::changelog() {
    return m_impl->m_metadata.m_changelog;
}
std::optional<std::string> const& ModInfo::changelog() const {
    return m_impl->m_metadata.m_changelog;
}

std::optional<std::string>& ModInfo::supportInfo() {
    return m_impl->m_metadata.m_supportInfo;
}
std::optional<std::string> const& ModInfo::supportInfo() const {
    return m_impl->m_metadata.m_supportInfo;
}

std::optional<std::string>& ModInfo::repository() {
    return m_impl->m_metadata.m_repository;
}
std::optional<std::string> const& ModInfo::repository() const {
    return m_impl->m_metadata.m_repository;
}

std::optional<IssuesInfo>& ModInfo::issues() {
    return m_impl->m_issues;
}
std::optional<IssuesInfo> const& ModInfo::issues() const {
    return m_impl->m_issues;
}

std::vector<Dependency>& ModInfo::dependencies() {
    return m_impl->m_dependencies;
}
std::vector<Dependency> const& ModInfo::dependencies() const {
    return m_impl->m_dependencies;
}

std::vector<std::string>& ModInfo::spritesheets() {
    return m_impl->m_metadata.m_spritesheets;
}
std::vector<std::string> const& ModInfo::spritesheets() const {
    return m_impl->m_metadata.m_spritesheets;
}

std::vector<std::pair<std::string, Setting>>& ModInfo::settings() {
    return m_impl->m_metadata.m_settings;
}
std::vector<std::pair<std::string, Setting>> const& ModInfo::settings() const {
    return m_impl->m_metadata.m_settings;
}

bool& ModInfo::supportsDisabling() {
    return m_impl->m_supportsDisabling;
}
bool const& ModInfo::supportsDisabling() const {
    return m_impl->m_supportsDisabling;
}

bool& ModInfo::supportsUnloading() {
    return m_impl->m_supportsUnloading;
}
bool const& ModInfo::supportsUnloading() const {
    return m_impl->m_supportsUnloading;
}

bool& ModInfo::needsEarlyLoad() {
    return m_impl->m_metadata.m_needsEarlyLoad;
}
bool const& ModInfo::needsEarlyLoad() const {
    return m_impl->m_metadata.m_needsEarlyLoad;
}

bool& ModInfo::isAPI() {
    return m_impl->m_metadata.m_isAPI;
}
bool const& ModInfo::isAPI() const {
    return m_impl->m_metadata.m_isAPI;
}

Result<ModInfo> ModInfo::createFromGeodeZip(utils::file::Unzip& zip) {
    return Impl::createFromGeodeZip(zip);
}

Result<ModInfo> ModInfo::createFromGeodeFile(ghc::filesystem::path const& path) {
    return Impl::createFromGeodeFile(path);
}

Result<ModInfo> ModInfo::createFromFile(ghc::filesystem::path const& path) {
    return Impl::createFromFile(path);
}

Result<ModInfo> ModInfo::create(ModJson const& json) {
    return Impl::create(json);
}

ModJson ModInfo::toJSON() const {
    return m_impl->toJSON();
}

ModJson ModInfo::getRawJSON() const {
    return m_impl->getRawJSON();
}

bool ModInfo::operator==(ModInfo const& other) const {
    return m_impl->operator==(*other.m_impl);
}

ModInfo::ModInfo() : m_impl(std::make_unique<Impl>()) {}

ModInfo::ModInfo(ModInfo const& other) : m_impl(std::make_unique<Impl>(*other.m_impl)) {}

ModInfo::ModInfo(ModInfo&& other) noexcept : m_impl(std::move(other.m_impl)) {}

ModInfo& ModInfo::operator=(ModInfo const& other) {
    m_impl = std::make_unique<Impl>(*other.m_impl);
    return *this;
}

ModInfo& ModInfo::operator=(ModInfo&& other) noexcept {
    m_impl = std::move(other.m_impl);
    return *this;
}

ModInfo::operator ModMetadata() {
    ModMetadata metadata;
    ModMetadataImpl::getImpl(metadata) = std::move(m_impl->m_metadata);
    auto& metadataImpl = ModMetadataImpl::getImpl(metadata);
    metadataImpl.m_issues = m_impl->m_issues ?
        ModMetadata::IssuesInfo::fromDeprecated(m_impl->m_issues.value()) :
        std::optional<ModMetadata::IssuesInfo>();
    for (auto& dep : m_impl->m_dependencies)
        metadataImpl.m_dependencies.push_back(ModMetadata::Dependency::fromDeprecated(dep));
    return metadata;
}

ModInfo::operator ModMetadata() const {
    ModMetadata metadata;
    ModMetadataImpl::getImpl(metadata) = std::move(m_impl->m_metadata);
    return metadata;
}

ModInfo::~ModInfo() = default;

#pragma clang diagnostic pop
