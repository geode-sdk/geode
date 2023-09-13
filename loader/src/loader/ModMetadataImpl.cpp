#include <Geode/loader/Loader.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <Geode/utils/VersionInfo.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/string.hpp>
#include <about.hpp>
#include <json.hpp>
#include <utility>

#include "ModMetadataImpl.hpp"

using namespace geode::prelude;

ModMetadata::Impl& ModMetadataImpl::getImpl(ModMetadata& info)  {
    return *info.m_impl;
}

bool ModMetadata::Dependency::isResolved() const {
    return this->importance != Importance::Required ||
        this->mod && this->mod->isEnabled() && this->version.compare(this->mod->getVersion());
}

bool ModMetadata::Incompatibility::isResolved() const {
    return this->importance != Importance::Breaking ||
        (!this->mod || !this->version.compare(this->mod->getVersion()));
}

static std::string sanitizeDetailsData(std::string const& str) {
    // delete CRLF
    return utils::string::replace(str, "\r", "");
}

bool ModMetadata::Impl::validateID(std::string const& id) {
    // ids may not be empty
    if (id.empty()) return false;
    for (auto const& c : id) {
        if (!(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') ||
              (c == '-') || (c == '_') || (c == '.')))
            return false;
    }
    return true;
}

Result<ModMetadata> ModMetadata::Impl::createFromSchemaV010(ModJson const& rawJson) {
    ModMetadata info;

    auto impl = info.m_impl.get();

    impl->m_rawJSON = rawJson;

    JsonChecker checker(impl->m_rawJSON);
    auto root = checker.root("[mod.json]").obj();

    root.addKnownKey("geode");

    // don't think its used locally yet
    root.addKnownKey("tags"); 

    root.needs("id").validate(MiniFunction<bool(std::string const&)>(&ModMetadata::validateID)).into(impl->m_id);
    root.needs("version").into(impl->m_version);
    root.needs("name").into(impl->m_name);
    root.needs("developer").into(impl->m_developer);
    root.has("description").into(impl->m_description);
    root.has("repository").into(impl->m_repository);
    root.has("early-load").into(impl->m_needsEarlyLoad);
    // TODO for 2.0.0: fix this lol
    // i think whoever wrote that intended that has would return the value if the key is present and false otherwise
    // but the actual behavior here is false if key not present and true if key is present
    if (root.has("api")) {
        impl->m_isAPI = true;
    }

    if (root.has("toggleable"))
        log::warn("{}: [mod.json].toggleable is deprecated and will be removed in a future update.", impl->m_id);
    if (root.has("unloadable"))
        log::warn("{}: [mod.json].unloadable is deprecated and will be removed in a future update.", impl->m_id);

    // TODO for 2.0.0: specify this in mod.json manually
    if (info.getID() != "geode.loader") {
        impl->m_dependencies.push_back({
            "geode.loader",
            {LOADER_VERSION, VersionCompare::Exact},
            Dependency::Importance::Required,
            Mod::get()
        });
    }

    for (auto& dep : root.has("dependencies").iterate()) {
        auto obj = dep.obj();

        Dependency dependency;
        obj.needs("id").validate(MiniFunction<bool(std::string const&)>(&ModMetadata::validateID)).into(dependency.id);
        obj.needs("version").into(dependency.version);
        auto required = obj.has("required");
        if (required) {
            log::warn("{}: [mod.json].required has been deprecated and will be removed "
                "in a future update. Use importance instead (see TODO: DOCS LINK)", impl->m_id);
            dependency.importance = required.get<bool>() ?
                Dependency::Importance::Required :
                Dependency::Importance::Suggested;
        }
        obj.has("importance").into(dependency.importance);
        obj.checkUnknownKeys();

        impl->m_dependencies.push_back(dependency);
    }

    for (auto& incompat : root.has("incompatibilities").iterate()) {
        auto obj = incompat.obj();

        Incompatibility incompatibility;
        obj.needs("id").validate(MiniFunction<bool(std::string const&)>(&ModMetadata::validateID)).into(incompatibility.id);
        obj.needs("version").into(incompatibility.version);
        obj.has("importance").into(incompatibility.importance);
        obj.checkUnknownKeys();

        impl->m_incompatibilities.push_back(incompatibility);
    }

    for (auto& [key, value] : root.has("settings").items()) {
        GEODE_UNWRAP_INTO(auto sett, Setting::parse(key, impl->m_id, value));
        impl->m_settings.emplace_back(key, sett);
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
            "{}: [mod.json].datastore has been removed. "
            "Use Saved Values instead (see TODO: DOCS LINK)", impl->m_id
        );
    }
    if (root.has("binary")) {
        log::error("{}: [mod.json].binary has been removed.", impl->m_id);
    }

    if (checker.isError()) {
        return Err(checker.getError());
    }
    root.checkUnknownKeys();

    return Ok(info);
}

Result<ModMetadata> ModMetadata::Impl::create(ModJson const& json) {
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
            "specified, or its formatting is invalid (required: \"[v]X.X.X\")!"
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
    if (schema < VersionInfo(0, 1, 0)) {
        return Err(
            "[mod.json] targets a version (" + schema.toString() +
            ") that isn't supported by this version (v" +
            LOADER_VERSION_STR +
            ") of geode. This is probably a bug; report it to "
            "the Geode Development Team."
        );
    }

    return Impl::createFromSchemaV010(json);
}

Result<ModMetadata> ModMetadata::Impl::createFromFile(ghc::filesystem::path const& path) {
    GEODE_UNWRAP_INTO(auto read, utils::file::readString(path));

    try {
        GEODE_UNWRAP_INTO(auto info, ModMetadata::create(json::parse(read)));

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

Result<ModMetadata> ModMetadata::Impl::createFromGeodeFile(ghc::filesystem::path const& path) {
    GEODE_UNWRAP_INTO(auto unzip, file::Unzip::create(path));
    return ModMetadata::createFromGeodeZip(unzip);
}

Result<ModMetadata> ModMetadata::Impl::createFromGeodeZip(file::Unzip& unzip) {
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

    auto res = ModMetadata::create(json);
    if (!res) {
        return Err("\"" + unzip.getPath().string() + "\" - " + res.unwrapErr());
    }
    auto info = res.unwrap();
    auto impl = info.m_impl.get();
    impl->m_path = unzip.getPath();

    GEODE_UNWRAP(info.addSpecialFiles(unzip).expect("Unable to add extra files: {error}"));

    return Ok(info);
}

Result<> ModMetadata::Impl::addSpecialFiles(file::Unzip& unzip) {
    // unzip known MD files
    for (auto& [file, target] : this->getSpecialFiles()) {
        if (unzip.hasEntry(file)) {
            GEODE_UNWRAP_INTO(auto data, unzip.extract(file).expect("Unable to extract \"{}\"", file));
            *target = sanitizeDetailsData(std::string(data.begin(), data.end()));
        }
    }
    return Ok();
}

Result<> ModMetadata::Impl::addSpecialFiles(ghc::filesystem::path const& dir) {
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

std::vector<std::pair<std::string, std::optional<std::string>*>> ModMetadata::Impl::getSpecialFiles() {
    return {
        {"about.md", &this->m_details},
        {"changelog.md", &this->m_changelog},
        {"support.md", &this->m_supportInfo},
    };
}

ModJson ModMetadata::Impl::toJSON() const {
    auto json = m_rawJSON;
    json["path"] = this->m_path.string();
    json["binary"] = this->m_binaryName;
    return json;
}

ModJson ModMetadata::Impl::getRawJSON() const {
    return m_rawJSON;
}

bool ModMetadata::Impl::operator==(ModMetadata::Impl const& other) const {
    return this->m_id == other.m_id;
}

[[maybe_unused]] ghc::filesystem::path ModMetadata::getPath() const {
    return m_impl->m_path;
}

std::string ModMetadata::getBinaryName() const {
    return m_impl->m_binaryName;
}

VersionInfo ModMetadata::getVersion() const {
    return m_impl->m_version;
}

std::string ModMetadata::getID() const {
    return m_impl->m_id;
}

std::string ModMetadata::getName() const {
    return m_impl->m_name;
}

std::string ModMetadata::getDeveloper() const {
    return m_impl->m_developer;
}

std::optional<std::string> ModMetadata::getDescription() const {
    return m_impl->m_description;
}

std::optional<std::string> ModMetadata::getDetails() const {
    return m_impl->m_details;
}

std::optional<std::string> ModMetadata::getChangelog() const {
    return m_impl->m_changelog;
}

std::optional<std::string> ModMetadata::getSupportInfo() const {
    return m_impl->m_supportInfo;
}

std::optional<std::string> ModMetadata::getRepository() const {
    return m_impl->m_repository;
}

std::optional<ModMetadata::IssuesInfo> ModMetadata::getIssues() const {
    return m_impl->m_issues;
}

std::vector<ModMetadata::Dependency> ModMetadata::getDependencies() const {
    return m_impl->m_dependencies;
}

std::vector<ModMetadata::Incompatibility> ModMetadata::getIncompatibilities() const {
    return m_impl->m_incompatibilities;
}

std::vector<std::string> ModMetadata::getSpritesheets() const {
    return m_impl->m_spritesheets;
}

std::vector<std::pair<std::string, Setting>> ModMetadata::getSettings() const {
    return m_impl->m_settings;
}

bool ModMetadata::needsEarlyLoad() const {
    return m_impl->m_needsEarlyLoad;
}

bool ModMetadata::isAPI() const {
    return m_impl->m_isAPI;
}

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE)
void ModMetadata::setPath(ghc::filesystem::path const& value) {
    m_impl->m_path = value;
}

void ModMetadata::setBinaryName(std::string const& value) {
    m_impl->m_binaryName = value;
}

void ModMetadata::setVersion(VersionInfo const& value) {
    m_impl->m_version = value;
}

void ModMetadata::setID(std::string const& value) {
    m_impl->m_id = value;
}

void ModMetadata::setName(std::string const& value) {
    m_impl->m_name = value;
}

void ModMetadata::setDeveloper(std::string const& value) {
    m_impl->m_developer = value;
}

void ModMetadata::setDescription(std::optional<std::string> const& value) {
    m_impl->m_description = value;
}

void ModMetadata::setDetails(std::optional<std::string> const& value) {
    m_impl->m_details = value;
}

void ModMetadata::setChangelog(std::optional<std::string> const& value) {
    m_impl->m_changelog = value;
}

void ModMetadata::setSupportInfo(std::optional<std::string> const& value) {
    m_impl->m_supportInfo = value;
}

void ModMetadata::setRepository(std::optional<std::string> const& value) {
    m_impl->m_repository = value;
}

void ModMetadata::setIssues(std::optional<IssuesInfo> const& value) {
    m_impl->m_issues = value;
}

void ModMetadata::setDependencies(std::vector<Dependency> const& value) {
    m_impl->m_dependencies = value;
}

void ModMetadata::setIncompatibilities(std::vector<Incompatibility> const& value) {
    m_impl->m_incompatibilities = value;
}

void ModMetadata::setSpritesheets(std::vector<std::string> const& value) {
    m_impl->m_spritesheets = value;
}

void ModMetadata::setSettings(std::vector<std::pair<std::string, Setting>> const& value) {
    m_impl->m_settings = value;
}

void ModMetadata::setNeedsEarlyLoad(bool const& value) {
    m_impl->m_needsEarlyLoad = value;
}

void ModMetadata::setIsAPI(bool const& value) {
    m_impl->m_isAPI = value;
}
#endif

Result<ModMetadata> ModMetadata::createFromGeodeZip(utils::file::Unzip& zip) {
    return Impl::createFromGeodeZip(zip);
}

Result<ModMetadata> ModMetadata::createFromGeodeFile(ghc::filesystem::path const& path) {
    return Impl::createFromGeodeFile(path);
}

Result<ModMetadata> ModMetadata::createFromFile(ghc::filesystem::path const& path) {
    return Impl::createFromFile(path);
}

Result<ModMetadata> ModMetadata::create(ModJson const& json) {
    return Impl::create(json);
}

ModJson ModMetadata::toJSON() const {
    return m_impl->toJSON();
}

ModJson ModMetadata::getRawJSON() const {
    return m_impl->getRawJSON();
}

bool ModMetadata::operator==(ModMetadata const& other) const {
    return m_impl->operator==(*other.m_impl);
}

bool ModMetadata::validateID(std::string const& id) {
    return Impl::validateID(id);
}

Result<ModMetadata> ModMetadata::createFromSchemaV010(ModJson const& json) {
    return Impl::createFromSchemaV010(json);
}

Result<> ModMetadata::addSpecialFiles(ghc::filesystem::path const& dir) {
    return m_impl->addSpecialFiles(dir);
}
Result<> ModMetadata::addSpecialFiles(utils::file::Unzip& zip) {
    return m_impl->addSpecialFiles(zip);
}

std::vector<std::pair<std::string, std::optional<std::string>*>> ModMetadata::getSpecialFiles() {
    return m_impl->getSpecialFiles();
}

ModMetadata::ModMetadata() : m_impl(std::make_unique<Impl>()) {}
ModMetadata::ModMetadata(std::string id) : m_impl(std::make_unique<Impl>()) { m_impl->m_id = std::move(id); }
ModMetadata::ModMetadata(ModMetadata const& other) : m_impl(std::make_unique<Impl>(*other.m_impl)) {}
ModMetadata::ModMetadata(ModMetadata&& other) noexcept : m_impl(std::move(other.m_impl)) {}

ModMetadata& ModMetadata::operator=(ModMetadata const& other) {
    m_impl = std::make_unique<Impl>(*other.m_impl);
    return *this;
}

ModMetadata& ModMetadata::operator=(ModMetadata&& other) noexcept {
    m_impl = std::move(other.m_impl);
    return *this;
}

ModMetadata::~ModMetadata() = default;

template <>
struct json::Serialize<geode::ModMetadata::Dependency::Importance> {
    static json::Value GEODE_DLL to_json(geode::ModMetadata::Dependency::Importance const& importance) {
        switch (importance) {
            case geode::ModMetadata::Dependency::Importance::Required: return {"required"};
            case geode::ModMetadata::Dependency::Importance::Recommended: return {"recommended"};
            case geode::ModMetadata::Dependency::Importance::Suggested: return {"suggested"};
            default: return {"unknown"};
        }
    }
    static geode::ModMetadata::Dependency::Importance GEODE_DLL from_json(json::Value const& importance) {
        auto impStr = importance.as_string();
        if (impStr == "required")
            return geode::ModMetadata::Dependency::Importance::Required;
        if (impStr == "recommended")
            return geode::ModMetadata::Dependency::Importance::Recommended;
        if (impStr == "suggested")
            return geode::ModMetadata::Dependency::Importance::Suggested;
        throw json::JsonException(R"(Expected importance to be "required", "recommended" or "suggested")");
    }
};

template <>
struct json::Serialize<geode::ModMetadata::Incompatibility::Importance> {
    static json::Value GEODE_DLL to_json(geode::ModMetadata::Incompatibility::Importance const& importance) {
        switch (importance) {
            case geode::ModMetadata::Incompatibility::Importance::Breaking: return {"breaking"};
            case geode::ModMetadata::Incompatibility::Importance::Conflicting: return {"conflicting"};
            default: return {"unknown"};
        }
    }
    static geode::ModMetadata::Incompatibility::Importance GEODE_DLL from_json(json::Value const& importance) {
        auto impStr = importance.as_string();
        if (impStr == "breaking")
            return geode::ModMetadata::Incompatibility::Importance::Breaking;
        if (impStr == "conflicting")
            return geode::ModMetadata::Incompatibility::Importance::Conflicting;
        throw json::JsonException(R"(Expected importance to be "breaking" or "conflicting")");
    }
};
