#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <Geode/utils/VersionInfo.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/string.hpp>
#include <about.hpp>
#include <json.hpp>

#include "ModInfoImpl.hpp"

using namespace geode::prelude;

ModInfo::Impl& ModInfoImpl::getImpl(ModInfo& info)  {
    return *info.m_impl.get();
}

bool Dependency::isResolved() const {
    return !this->required ||
        (this->mod && this->mod->isLoaded() && this->mod->isEnabled() &&
         this->version.compare(this->mod->getVersion()));
}

static std::string sanitizeDetailsData(std::string const& str) {
    // delete CRLF
    return utils::string::replace(str, "\r", "");
}

bool ModInfo::Impl::validateID(std::string const& id) {
    // ids may not be empty
    if (!id.size()) return false;
    for (auto const& c : id) {
        if (!(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') ||
              (c == '-') || (c == '_') || (c == '.')))
            return false;
    }
    return true;
}

Result<ModInfo> ModInfo::Impl::createFromSchemaV010(ModJson const& rawJson) {
    ModInfo info;

    auto impl = info.m_impl.get();

    impl->m_rawJSON = rawJson;

    JsonChecker checker(impl->m_rawJSON);
    auto root = checker.root("[mod.json]").obj();

    root.addKnownKey("geode");

    // don't think its used locally yet
    root.addKnownKey("tags"); 

    root.needs("id").validate(MiniFunction<bool(std::string const&)>(&ModInfo::validateID)).into(impl->m_id);
    root.needs("version").into(impl->m_version);
    root.needs("name").into(impl->m_name);
    root.needs("developer").into(impl->m_developer);
    root.has("description").into(impl->m_description);
    root.has("repository").into(impl->m_repository);
    root.has("toggleable").into(impl->m_supportsDisabling);
    root.has("unloadable").into(impl->m_supportsUnloading);
    root.has("early-load").into(impl->m_needsEarlyLoad);
    if (root.has("api")) {
        impl->m_isAPI = true;
    }

    for (auto& dep : root.has("dependencies").iterate()) {
        auto obj = dep.obj();

        auto depobj = Dependency{};
        obj.needs("id").validate(MiniFunction<bool(std::string const&)>(&ModInfo::validateID)).into(depobj.id);
        obj.needs("version").into(depobj.version);
        obj.has("required").into(depobj.required);
        obj.checkUnknownKeys();

        impl->m_dependencies.push_back(depobj);
    }

    for (auto& [key, value] : root.has("settings").items()) {
        GEODE_UNWRAP_INTO(auto sett, Setting::parse(key, impl->m_id, value));
        impl->m_settings.push_back({key, sett});
    }

    if (auto resources = root.has("resources").obj()) {
        for (auto& [key, _] : resources.has("spritesheets").items()) {
            impl->m_spritesheets.push_back(impl->m_id + "/" + key);
        }
    }

    if (auto issues = root.has("issues").obj()) {
        IssuesInfo issuesInfo;
        issues.needs("info").into(issuesInfo.info);
        issues.has("url").intoAs<std::string>(issuesInfo.url);
        impl->m_issues = issuesInfo;
    }

    // with new cli, binary name is always mod id
    impl->m_binaryName = impl->m_id + GEODE_PLATFORM_EXTENSION;

    // removed keys
    if (root.has("datastore")) {
        log::error(
            "[mod.json].datastore has been deprecated "
            "and removed. Use Saved Values instead (see TODO: DOCS LINK)"
        );
    }
    if (root.has("binary")) {
        log::error("[mod.json].binary has been deprecated and removed.");
    }

    if (checker.isError()) {
        return Err(checker.getError());
    }
    root.checkUnknownKeys();

    return Ok(info);
}

Result<ModInfo> ModInfo::Impl::create(ModJson const& json) {
    // Check mod.json target version
    auto schema = LOADER_VERSION;
    if (json.contains("geode") && json["geode"].is_string()) {
        GEODE_UNWRAP_INTO(
            schema,
            VersionInfo::parse(json["geode"].as_string())
                .expect("[mod.json] has invalid target loader version: {error}")
        );
    }
    else {
        return Err(
            "[mod.json] has no target loader version "
            "specified, or it is invalidally formatted (required: \"[v]X.X.X\")!"
        );
    }
    if (schema < Loader::get()->minModVersion()) {
        return Err(
            "[mod.json] is built for an older version (" + schema.toString() +
            ") of Geode (current: " + Loader::get()->getVersion().toString() +
            "). Please update the mod to the latest version, "
            "and if the problem persists, contact the developer "
            "to update it."
        );
    }
    if (schema > Loader::get()->maxModVersion()) {
        return Err(
            "[mod.json] is built for a newer version (" + schema.toString() +
            ") of Geode (current: " + Loader::get()->getVersion().toString() +
            "). You need to update Geode in order to use "
            "this mod."
        );
    }

    // Handle mod.json data based on target
    if (schema >= VersionInfo(0, 1, 0)) {
        return Impl::createFromSchemaV010(json);
    }

    return Err(
        "[mod.json] targets a version (" + schema.toString() +
        ") that isn't supported by this version (v" +
        LOADER_VERSION_STR +
        ") of geode. This is probably a bug; report it to "
        "the Geode Development Team."
    );
}

Result<ModInfo> ModInfo::Impl::createFromFile(ghc::filesystem::path const& path) {
    GEODE_UNWRAP_INTO(auto read, utils::file::readString(path));

    try {
        GEODE_UNWRAP_INTO(auto info, ModInfo::create(json::parse(read)));

        auto impl = info.m_impl.get();

        impl->m_path = path;
        if (path.has_parent_path()) {
            GEODE_UNWRAP(info.addSpecialFiles(path.parent_path()));
        }
        return Ok(info);
    }
    catch (std::exception& err) {
        return Err(std::string("Unable to parse mod.json: ") + err.what());
    }
}

Result<ModInfo> ModInfo::Impl::createFromGeodeFile(ghc::filesystem::path const& path) {
    GEODE_UNWRAP_INTO(auto unzip, file::Unzip::create(path));
    return ModInfo::createFromGeodeZip(unzip);
}

Result<ModInfo> ModInfo::Impl::createFromGeodeZip(file::Unzip& unzip) {
    // Check if mod.json exists in zip
    if (!unzip.hasEntry("mod.json")) {
        return Err("\"" + unzip.getPath().string() + "\" is missing mod.json");
    }

    // Read mod.json & parse if possible
    GEODE_UNWRAP_INTO(
        auto jsonData, unzip.extract("mod.json").expect("Unable to read mod.json: {error}")
    );

    std::string err;
    ModJson json;
    try {
        json = json::parse(std::string(jsonData.begin(), jsonData.end()));
    }
    catch (std::exception& err) {
        return Err(err.what());
    }

    auto res = ModInfo::create(json);
    if (!res) {
        return Err("\"" + unzip.getPath().string() + "\" - " + res.unwrapErr());
    }
    auto info = res.unwrap();
    auto impl = info.m_impl.get();
    impl->m_path = unzip.getPath();

    GEODE_UNWRAP(info.addSpecialFiles(unzip).expect("Unable to add extra files: {error}"));

    return Ok(info);
}

Result<> ModInfo::Impl::addSpecialFiles(file::Unzip& unzip) {
    // unzip known MD files
    for (auto& [file, target] : this->getSpecialFiles()) {
        if (unzip.hasEntry(file)) {
            GEODE_UNWRAP_INTO(auto data, unzip.extract(file).expect("Unable to extract \"{}\"", file));
            *target = sanitizeDetailsData(std::string(data.begin(), data.end()));
        }
    }
    return Ok();
}

Result<> ModInfo::Impl::addSpecialFiles(ghc::filesystem::path const& dir) {
    // unzip known MD files
    for (auto& [file, target] : this->getSpecialFiles()) {
        if (ghc::filesystem::exists(dir / file)) {
            auto data = file::readString(dir / file);
            if (!data) {
                return Err("Unable to read \"" + file + "\": " + data.unwrapErr());
            }
            *target = sanitizeDetailsData(data.unwrap());
        }
    }
    return Ok();
}

std::vector<std::pair<std::string, std::optional<std::string>*>> ModInfo::Impl::getSpecialFiles() {
    return {
        {"about.md", &this->m_details},
        {"changelog.md", &this->m_changelog},
        {"support.md", &this->m_supportInfo},
    };
}

ModJson ModInfo::Impl::toJSON() const {
    auto json = m_rawJSON;
    json["path"] = this->m_path.string();
    json["binary"] = this->m_binaryName;
    return json;
}

ModJson ModInfo::Impl::getRawJSON() const {
    return m_rawJSON;
}

bool ModInfo::Impl::operator==(ModInfo::Impl const& other) const {
    return this->m_id == other.m_id;
}

ghc::filesystem::path& ModInfo::path() {
    return m_impl->m_path;
}
ghc::filesystem::path const& ModInfo::path() const {
    return m_impl->m_path;
}

std::string& ModInfo::binaryName() {
    return m_impl->m_binaryName;
}
std::string const& ModInfo::binaryName() const {
    return m_impl->m_binaryName;
}

VersionInfo& ModInfo::version() {
    return m_impl->m_version;
}
VersionInfo const& ModInfo::version() const {
    return m_impl->m_version;
}

std::string& ModInfo::id() {
    return m_impl->m_id;
}
std::string const& ModInfo::id() const {
    return m_impl->m_id;
}

std::string& ModInfo::name() {
    return m_impl->m_name;
}
std::string const& ModInfo::name() const {
    return m_impl->m_name;
}

std::string& ModInfo::developer() {
    return m_impl->m_developer;
}
std::string const& ModInfo::developer() const {
    return m_impl->m_developer;
}

std::optional<std::string>& ModInfo::description() {
    return m_impl->m_description;
}
std::optional<std::string> const& ModInfo::description() const {
    return m_impl->m_description;
}

std::optional<std::string>& ModInfo::details() {
    return m_impl->m_details;
}
std::optional<std::string> const& ModInfo::details() const {
    return m_impl->m_details;
}

std::optional<std::string>& ModInfo::changelog() {
    return m_impl->m_changelog;
}
std::optional<std::string> const& ModInfo::changelog() const {
    return m_impl->m_changelog;
}

std::optional<std::string>& ModInfo::supportInfo() {
    return m_impl->m_supportInfo;
}
std::optional<std::string> const& ModInfo::supportInfo() const {
    return m_impl->m_supportInfo;
}

std::optional<std::string>& ModInfo::repository() {
    return m_impl->m_repository;
}
std::optional<std::string> const& ModInfo::repository() const {
    return m_impl->m_repository;
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
    return m_impl->m_spritesheets;
}
std::vector<std::string> const& ModInfo::spritesheets() const {
    return m_impl->m_spritesheets;
}

std::vector<std::pair<std::string, Setting>>& ModInfo::settings() {
    return m_impl->m_settings;
}
std::vector<std::pair<std::string, Setting>> const& ModInfo::settings() const {
    return m_impl->m_settings;
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
    return m_impl->m_needsEarlyLoad;
}
bool const& ModInfo::needsEarlyLoad() const {
    return m_impl->m_needsEarlyLoad;
}

bool& ModInfo::isAPI() {
    return m_impl->m_isAPI;
}
bool const& ModInfo::isAPI() const {
    return m_impl->m_isAPI;
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

bool ModInfo::validateID(std::string const& id) {
    return Impl::validateID(id);
}

ModJson& ModInfo::rawJSON() {
    return m_impl->m_rawJSON;
}
ModJson const& ModInfo::rawJSON() const {
    return m_impl->m_rawJSON;
}

Result<ModInfo> ModInfo::createFromSchemaV010(ModJson const& json) {
    return Impl::createFromSchemaV010(json);
}

Result<> ModInfo::addSpecialFiles(ghc::filesystem::path const& dir) {
    return m_impl->addSpecialFiles(dir);
}
Result<> ModInfo::addSpecialFiles(utils::file::Unzip& zip) {
    return m_impl->addSpecialFiles(zip);
}

std::vector<std::pair<std::string, std::optional<std::string>*>> ModInfo::getSpecialFiles() {
    return m_impl->getSpecialFiles();
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

ModInfo::~ModInfo() {}