#include <Geode/loader/Loader.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <Geode/utils/VersionInfo.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/general.hpp>
#include <about.hpp>
#include <matjson.hpp>
#include <utility>
#include <clocale>

#include "ModMetadataImpl.hpp"
#include "LoaderImpl.hpp"

using namespace geode::prelude;

std::optional<std::string> ModMetadataLinks::getHomepageURL() const {
    return m_impl->m_homepage;
}
std::optional<std::string> ModMetadataLinks::getSourceURL() const {
    return m_impl->m_source;
}
std::optional<std::string> ModMetadataLinks::getCommunityURL() const {
    return m_impl->m_community;
}

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE)
ModMetadataLinks::Impl* ModMetadataLinks::getImpl() {
    return m_impl.get();
}
#endif

ModMetadataLinks::ModMetadataLinks() : m_impl(std::make_unique<Impl>()) {}
ModMetadataLinks::ModMetadataLinks(ModMetadataLinks const& other)
  : m_impl(std::make_unique<Impl>(*other.m_impl)) {}
ModMetadataLinks::ModMetadataLinks(ModMetadataLinks&& other) noexcept
  : m_impl(std::move(other.m_impl)) {}
ModMetadataLinks& ModMetadataLinks::operator=(ModMetadataLinks const& other) {
    m_impl = std::make_unique<Impl>(*other.m_impl);
    return *this;
}
ModMetadataLinks& ModMetadataLinks::operator=(ModMetadataLinks&& other) noexcept {
    m_impl = std::move(other.m_impl);
    return *this;
}
ModMetadataLinks::~ModMetadataLinks() = default;

ModMetadata::Impl& ModMetadataImpl::getImpl(ModMetadata& info)  {
    return *info.m_impl;
}

bool ModMetadata::Dependency::isResolved() const {
    return
        this->importance != Importance::Required ||
        this->mod && this->mod->isEnabled() && this->version.compare(this->mod->getVersion());
}

bool ModMetadata::Incompatibility::isResolved() const {
    return this->importance != Importance::Breaking ||
        (!this->mod || !this->mod->isEnabled() || !this->version.compare(this->mod->getVersion()));
}

static std::string sanitizeDetailsData(std::string const& str) {
    // delete CRLF
    return utils::string::replace(str, "\r", "");
}

bool ModMetadata::Impl::validateOldID(std::string const& id) {
    // Old IDs may not be empty
    if (id.empty()) return false;
    for (auto const& c : id) {
        if (!(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') ||
              (c == '-') || (c == '_') || (c == '.')))
            return false;
    }
    return true;
}

bool ModMetadata::Impl::validateID(std::string const& id) {
    // IDs may not be empty nor exceed 64 characters
    if (id.size() == 0 || id.size() > 64) {
        return false;
    }

    // Only one dot permitted
    bool foundDot = false;
    for (auto const& c : id) {
        if (!(
            ('a' <= c && c <= 'z') ||
            ('0' <= c && c <= '9') ||
            (c == '-' || c == '_') ||
            (c == '.' && !foundDot)
        )) {
            if (c == '.') {
                foundDot = true;
            }
            return false;
        }
    }
    return true;
}

bool ModMetadata::Impl::isDeprecatedIDForm(std::string const& id) {
    return !validateID(id) && validateOldID(id);
}

Result<ModMetadata> ModMetadata::Impl::createFromSchemaV010(ModJson const& rawJson) {
    ModMetadata info;

    auto impl = info.m_impl.get();

    impl->m_rawJSON = rawJson;

    auto checkerRoot = fmt::format(
        "[{}/v0.0.0/mod.json]",
        rawJson.contains("id") ? rawJson["id"].as_string() : "unknown.mod"
    );
    // JsonChecker did it this way too
    try {
        checkerRoot = fmt::format(
            "[{}/{}/mod.json]",
            rawJson.contains("id") ? rawJson["id"].as_string() : "unknown.mod",
            rawJson.contains("version") ? rawJson["version"].as<VersionInfo>().toVString() : "v0.0.0"
        );
    }
    catch (...) { }

    auto root = checkJson(impl->m_rawJSON, checkerRoot);
    root.needs("geode").into(impl->m_geodeVersion);
    
    if (auto gd = root.needs("gd")) {
        // In the future when we get rid of support for string format just 
        // change all of this to the gd.needs(...) stuff
        gd.assertIs({ matjson::Type::Object, matjson::Type::String });
        if (gd.isObject()) {
            if (gd.has(GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH)) {
                gd.needs(GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH)
                    .mustBe<std::string>("a valid gd version", [](auto const& str) {
                        return str == "*" || numFromString<double>(str).isOk();
                    })
                    .into(impl->m_gdVersion);
            } else {
                // this will error later on, but try to load the rest of the metadata
                // so that the mod can show up in the mods listing
                impl->m_gdVersion = "0.000";
            }
        }
        else if (gd.isString()) {
            impl->m_softInvalidReason = "mod.json uses old syntax";
        }
    }

    constexpr auto ID_REGEX = "[a-z0-9\\-_]+\\.[a-z0-9\\-_]+";
    root.needs("id")
        .mustBe<std::string>(ID_REGEX, &ModMetadata::Impl::validateOldID)
        .into(impl->m_id);

    // if (!isDeprecatedIDForm(impl->m_id)) {
    //     log::warn(
    //         "Mod ID '{}' will be rejected in the future - "
    //         "IDs must match the regex `[a-z0-9\\-_]+\\.[a-z0-9\\-_]+`",
    //         impl->m_id
    //     );
    // }

    root.needs("version").into(impl->m_version);
    root.needs("name").into(impl->m_name);
    if (root.has("developers")) {
        if (root.has("developer")) {
            return Err("[mod.json] can not have both \"developer\" and \"developers\" specified");
        }
        for (auto& dev : root.needs("developers").items()) {
            impl->m_developers.push_back(dev.template get<std::string>());
        }
    }
    else {
        std::string dev;
        root.needs("developer").into(dev);
        impl->m_developers = { dev };
    }
    root.has("description").into(impl->m_description);
    root.has("repository").into(info.getLinksMut().getImpl()->m_source);
    root.has("early-load").into(impl->m_needsEarlyLoad);
    if (root.has("api")) {
        impl->m_isAPI = true;
    }

    if (info.getID() != "geode.loader") {
        impl->m_dependencies.push_back({
            "geode.loader",
            {about::getLoaderVersion(), VersionCompare::Exact},
            Dependency::Importance::Required,
            Mod::get()
        });
    }

    for (auto& dep : root.has("dependencies").items()) {
        bool onThisPlatform = !dep.has("platforms");
        for (auto& plat : dep.has("platforms").items()) {
            if (PlatformID::coveredBy(plat.get<std::string>(), GEODE_PLATFORM_TARGET)) {
                onThisPlatform = true;
            }
        }
        if (!onThisPlatform) {
            continue;
        }

        Dependency dependency;
        dep.needs("id").mustBe<std::string>(ID_REGEX, &ModMetadata::Impl::validateOldID).into(dependency.id);
        dep.needs("version").into(dependency.version);
        dep.has("importance").into(dependency.importance);
        dep.checkUnknownKeys();

        if (
            dependency.version.getComparison() != VersionCompare::MoreEq &&
            dependency.version.getComparison() != VersionCompare::Any
        ) {
            return Err(
                "[mod.json].dependencies.{}.version must be either a more-than "
                "comparison for a specific version or a wildcard for any version",
                dependency.id
            );
        }

        // if (isDeprecatedIDForm(dependency.id)) {
        //     log::warn(
        //         "Dependency ID '{}' will be rejected in the future - "
        //         "IDs must match the regex `[a-z0-9\\-_]+\\.[a-z0-9\\-_]+`",
        //         impl->m_id
        //     );
        // }

        impl->m_dependencies.push_back(dependency);
    }

    for (auto& incompat : root.has("incompatibilities").items()) {
        bool onThisPlatform = !incompat.has("platforms");
        for (auto& plat : incompat.has("platforms").items()) {
            if (PlatformID::coveredBy(plat.get<std::string>(), GEODE_PLATFORM_TARGET)) {
                onThisPlatform = true;
            }
        }
        if (!onThisPlatform) {
            continue;
        }

        Incompatibility incompatibility;
        incompat.needs("id").mustBe<std::string>(ID_REGEX, &ModMetadata::Impl::validateOldID).into(incompatibility.id);
        incompat.needs("version").into(incompatibility.version);
        incompat.has("importance").into(incompatibility.importance);
        incompat.checkUnknownKeys();
        impl->m_incompatibilities.push_back(incompatibility);
    }

    for (auto& [key, value] : root.has("settings").properties()) {
        // Skip settings not on this platform
        if (value.is(matjson::Type::Object)) {
            bool onThisPlatform = !value.has("platforms");
            for (auto& plat : value.has("platforms").items()) {
                if (PlatformID::coveredBy(plat.get<std::string>(), GEODE_PLATFORM_TARGET)) {
                    onThisPlatform = true;
                }
            }
            if (!onThisPlatform) {
                continue;
            }
        }
        impl->m_settings.emplace_back(key, value.json());
    }

    if (auto resources = root.has("resources")) {
        for (auto& [key, _] : resources.has("spritesheets").properties()) {
            impl->m_spritesheets.push_back(impl->m_id + "/" + key);
        }
    }

    if (auto issues = root.has("issues")) {
        IssuesInfo issuesInfo;
        issues.needs("info").into(issuesInfo.info);
        issues.has("url").into(issuesInfo.url);
        impl->m_issues = issuesInfo;
    }

    if (auto links = root.has("links")) {
        links.has("homepage").into(info.getLinksMut().getImpl()->m_homepage);
        links.has("source").into(info.getLinksMut().getImpl()->m_source);
        links.has("community").into(info.getLinksMut().getImpl()->m_community);
        // do not check unknown for future compat
    }

    // Tags. Actual validation is done when interacting with the server in the UI
    for (auto& tag : root.has("tags").items()) {
        impl->m_tags.insert(tag.template get<std::string>());
    }

    // with new cli, binary name is always mod id
    impl->m_binaryName = impl->m_id + GEODE_PLATFORM_EXTENSION;

    root.checkUnknownKeys();

    return root.ok(info);
}

Result<ModMetadata> ModMetadata::Impl::create(ModJson const& json) {
    // Check mod.json target version
    auto schema = about::getLoaderVersion();
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
    // if (schema < Loader::get()->minModVersion()) {
    //     return Err(
    //         "[mod.json] is built for an older version (" + schema.toString() +
    //         ") of Geode (current: " + Loader::get()->getVersion().toString() +
    //         "). Please update the mod to the latest version, "
    //         "and if the problem persists, contact the developer "
    //         "to update it."
    //     );
    // }
    // if (schema > Loader::get()->maxModVersion()) {
    //     return Err(
    //         "[mod.json] is built for a newer version (" + schema.toString() +
    //         ") of Geode (current: " + Loader::get()->getVersion().toString() +
    //         "). You need to update Geode in order to use "
    //         "this mod."
    //     );
    // }

    // Handle mod.json data based on target
    if (schema < VersionInfo(0, 1, 0)) {
        return Err(
            "[mod.json] targets a version (" + schema.toVString() +
            ") that isn't supported by this version (v" +
            about::getLoaderVersionStr() +
            ") of geode. This is probably a bug; report it to "
            "the Geode Development Team."
        );
    }

    return Impl::createFromSchemaV010(json);
}

Result<ModMetadata> ModMetadata::Impl::createFromFile(std::filesystem::path const& path) {
    GEODE_UNWRAP_INTO(auto read, utils::file::readString(path));

    std::string error;
    auto res = matjson::parse(read, error);
    if (error.size() > 0) {
        return Err(std::string("Unable to parse mod.json: ") + error);
    }

    GEODE_UNWRAP_INTO(auto info, ModMetadata::create(res.value()));

    auto impl = info.m_impl.get();

    impl->m_path = path;
    if (path.has_parent_path()) {
        GEODE_UNWRAP(info.addSpecialFiles(path.parent_path()));
    }
    return Ok(info);
}

Result<ModMetadata> ModMetadata::Impl::createFromGeodeFile(std::filesystem::path const& path) {
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

    std::string error;
    auto res = matjson::parse(std::string(jsonData.begin(), jsonData.end()), error);
    if (error.size() > 0) {
        return Err(std::string("Unable to parse mod.json: ") + error);
    }
    ModJson json = res.value();

    auto res2 = ModMetadata::create(json);
    if (!res2) {
        return Err("\"" + unzip.getPath().string() + "\" - " + res2.unwrapErr());
    }
    auto info = res2.unwrap();
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

Result<> ModMetadata::Impl::addSpecialFiles(std::filesystem::path const& dir) {
    // unzip known MD files
    for (auto& [file, target] : this->getSpecialFiles()) {
        if (std::filesystem::exists(dir / file)) {
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

[[maybe_unused]] std::filesystem::path ModMetadata::getPath() const {
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

bool ModMetadata::usesDeprecatedIDForm() const {
    return Impl::isDeprecatedIDForm(m_impl->m_id);
}

std::string ModMetadata::getName() const {
    return m_impl->m_name;
}

std::string ModMetadata::getDeveloper() const {
    // m_developers should be guaranteed to never be empty, but this is 
    // just in case it is anyway somehow
    return m_impl->m_developers.empty() ? "" : m_impl->m_developers.front();
}

std::string ModMetadata::formatDeveloperDisplayString(std::vector<std::string> const& developers) {
    switch (developers.size()) {
        case 0: return "Unknown"; break;
        case 1: return developers.front(); break;
        case 2: return developers.front() + " & " + developers.back(); break;
        default: {
            return developers.front() + " + " + 
                std::to_string(developers.size() - 1) + " More";
        } break;
    }
}

std::vector<std::string> ModMetadata::getDevelopers() const {
    return m_impl->m_developers;
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
    return m_impl->m_links.getSourceURL();
}
ModMetadataLinks ModMetadata::getLinks() const {
    return m_impl->m_links;
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
    std::vector<std::pair<std::string, Setting>> res;
    for (auto [key, sett] : m_impl->m_settings) {
        auto checker = JsonChecker(sett);
        auto value = checker.root("");
        auto legacy = Setting::parse(key, m_impl->m_id, value);
        if (!checker.isError() && legacy.isOk()) {
            res.push_back(std::make_pair(key, *legacy));
        }
    }
    return res;
}
std::vector<std::pair<std::string, matjson::Value>> ModMetadata::getSettingsV3() const {
    return m_impl->m_settings;
}
std::unordered_set<std::string> ModMetadata::getTags() const {
    return m_impl->m_tags;
}
bool ModMetadata::needsEarlyLoad() const {
    return m_impl->m_needsEarlyLoad;
}
bool ModMetadata::isAPI() const {
    return m_impl->m_isAPI;
}
std::optional<std::string> ModMetadata::getGameVersion() const {
    if (m_impl->m_gdVersion.empty()) return std::nullopt;
    return m_impl->m_gdVersion;
}
VersionInfo ModMetadata::getGeodeVersion() const {
    return m_impl->m_geodeVersion;
}
Result<> ModMetadata::checkGameVersion() const {
    if (!m_impl->m_gdVersion.empty() && m_impl->m_gdVersion != "*") {
        auto const ver = m_impl->m_gdVersion;

        auto res = numFromString<double>(ver);
        if (res.isErr()) {
            return Err("Invalid target GD version");
        }
        double modTargetVer = res.unwrap();

        if (modTargetVer == 0.0) { // O.o
            return Err(fmt::format("This mod doesn't support the current platform."));
        }

        if (LoaderImpl::get()->isForwardCompatMode()) {
            // this means current gd version is > GEODE_GD_VERSION
            if (modTargetVer <= GEODE_GD_VERSION) {
                return Err(fmt::format("This mod doesn't support this version of Geometry Dash ({})", ver));
            }
        } else if (ver != GEODE_STR(GEODE_GD_VERSION)) {
            // we are not in forward compat mode, so GEODE_GD_VERSION is the current gd version
            return Err(
                fmt::format(
                    "This mod was created for a different version of Geometry Dash ({}). You currently have version {}.",
                    ver,
                    GEODE_STR(GEODE_GD_VERSION)
                )
            );
        }
    }
    return Ok();
}

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE)
void ModMetadata::setPath(std::filesystem::path const& value) {
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
    m_impl->m_developers = { value };
}
void ModMetadata::setDevelopers(std::vector<std::string> const& value) {
    m_impl->m_developers = value;
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
    this->getLinksMut().getImpl()->m_source = value;
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
    // intentionally no-op because no one is supposed to be using this 
    // without subscribing to "internals are not stable" mentality
}
void ModMetadata::setSettings(std::vector<std::pair<std::string, matjson::Value>> const& value) {
    m_impl->m_settings = value;
}
void ModMetadata::setTags(std::unordered_set<std::string> const& value) {
    m_impl->m_tags = value;
}
void ModMetadata::setNeedsEarlyLoad(bool const& value) {
    m_impl->m_needsEarlyLoad = value;
}
void ModMetadata::setIsAPI(bool const& value) {
    m_impl->m_isAPI = value;
}
void ModMetadata::setGameVersion(std::string const& value) {
    m_impl->m_gdVersion = value;
}
void ModMetadata::setGeodeVersion(VersionInfo const& value) {
    m_impl->m_geodeVersion = value;
}
ModMetadataLinks& ModMetadata::getLinksMut() {
    return m_impl->m_links;
}
#endif

Result<ModMetadata> ModMetadata::createFromGeodeZip(utils::file::Unzip& zip) {
    return Impl::createFromGeodeZip(zip);
}
Result<ModMetadata> ModMetadata::createFromGeodeFile(std::filesystem::path const& path) {
    return Impl::createFromGeodeFile(path);
}
Result<ModMetadata> ModMetadata::createFromFile(std::filesystem::path const& path) {
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

Result<> ModMetadata::addSpecialFiles(std::filesystem::path const& dir) {
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
ModMetadata::ModMetadata(ModMetadata const& other) : m_impl(other.m_impl ? std::make_unique<Impl>(*other.m_impl) : std::make_unique<Impl>()) {}
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
