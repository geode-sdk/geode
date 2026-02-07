#include <Geode/loader/Loader.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <Geode/utils/VersionInfo.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/general.hpp>
#include <Geode/utils/random.hpp>
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
    if (this != &other) {
        m_impl = std::make_unique<Impl>(*other.m_impl);
    }
    return *this;
}
ModMetadataLinks& ModMetadataLinks::operator=(ModMetadataLinks&& other) noexcept = default;
ModMetadataLinks::~ModMetadataLinks() = default;

ModMetadata::Impl& ModMetadataImpl::getImpl(ModMetadata& info) {
    return *info.m_impl;
}

ModMetadata::Impl const& ModMetadataImpl::getImpl(ModMetadata const& info) {
    return *info.m_impl;
}

class ModMetadata::Dependency::Impl {
public:
    std::string id;
    ComparableVersionInfo version;
    bool required = true;
    Mod* mod = nullptr;
    matjson::Value settings;
};

ModMetadata::Dependency::Dependency()
  : m_impl(std::make_unique<Impl>()) {}

ModMetadata::Dependency::Dependency(Dependency const& other)
  : m_impl(std::make_unique<Impl>(*other.m_impl)) {}

ModMetadata::Dependency::Dependency(Dependency&& other) noexcept
  : m_impl(std::move(other.m_impl)) {}

ModMetadata::Dependency& ModMetadata::Dependency::operator=(Dependency const& other) {
    if (this != &other) {
        m_impl = std::make_unique<Impl>(*other.m_impl);
    }
    return *this;
}

ModMetadata::Dependency& ModMetadata::Dependency::operator=(Dependency&& other) noexcept = default;

ModMetadata::Dependency::~Dependency() = default;

std::string const& ModMetadata::Dependency::getID() const {
    return m_impl->id;
}

void ModMetadata::Dependency::setID(std::string value) {
    m_impl->id = std::move(value);
}

ComparableVersionInfo const& ModMetadata::Dependency::getVersion() const {
    return m_impl->version;
}

void ModMetadata::Dependency::setVersion(ComparableVersionInfo value) {
    m_impl->version = std::move(value);
}

bool ModMetadata::Dependency::isRequired() const {
    return m_impl->required;
}

void ModMetadata::Dependency::setRequired(bool value) {
    m_impl->required = value;
}

Mod* ModMetadata::Dependency::getMod() const {
    return m_impl->mod;
}

void ModMetadata::Dependency::setMod(Mod* mod) {
    m_impl->mod = mod;
}

matjson::Value const& ModMetadata::Dependency::getSettings() const {
    return m_impl->settings;
}

void ModMetadata::Dependency::setSettings(matjson::Value value) {
    m_impl->settings = std::move(value);
}

bool ModMetadata::Dependency::isResolved() const {
    return
        !m_impl->required ||
        this->getMod() && this->getMod()->isEnabled() && this->getVersion().compare(this->getMod()->getVersion());
}

class ModMetadata::Incompatibility::Impl {
public:
    std::string id;
    ComparableVersionInfo version;
    bool breaking = true;
    Mod* mod = nullptr;
};

ModMetadata::Incompatibility::Incompatibility()
  : m_impl(std::make_unique<Impl>()) {}

ModMetadata::Incompatibility::Incompatibility(Incompatibility const& other)
  : m_impl(std::make_unique<Impl>(*other.m_impl)) {}

ModMetadata::Incompatibility::Incompatibility(Incompatibility&& other) noexcept
    : m_impl(std::move(other.m_impl)) {}

ModMetadata::Incompatibility& ModMetadata::Incompatibility::operator=(Incompatibility const& other) {
    if (this != &other) {
        m_impl = std::make_unique<Impl>(*other.m_impl);
    }
    return *this;
}

ModMetadata::Incompatibility& ModMetadata::Incompatibility::operator=(Incompatibility&& other) noexcept = default;

ModMetadata::Incompatibility::~Incompatibility() = default;

std::string const& ModMetadata::Incompatibility::getID() const {
    return m_impl->id;
}

void ModMetadata::Incompatibility::setID(std::string value) {
    m_impl->id = std::move(value);
}

ComparableVersionInfo const& ModMetadata::Incompatibility::getVersion() const {
    return m_impl->version;
}

void ModMetadata::Incompatibility::setVersion(ComparableVersionInfo value) {
    m_impl->version = std::move(value);
}

bool ModMetadata::Incompatibility::isBreaking() const {
    return m_impl->breaking;
}

void ModMetadata::Incompatibility::setBreaking(bool value) {
    m_impl->breaking = value;
}

Mod* ModMetadata::Incompatibility::getMod() const {
    return m_impl->mod;
}

void ModMetadata::Incompatibility::setMod(Mod* mod) {
    m_impl->mod = mod;
}

bool ModMetadata::Incompatibility::isResolved() const {
    return !m_impl->breaking ||
        (!this->getMod() || !this->getVersion().compare(this->getMod()->getVersion()) || !this->getMod()->shouldLoad());
}

class ModMetadata::IssuesInfo::Impl {
public:
    std::string info;
    std::optional<std::string> url;
};

ModMetadata::IssuesInfo::IssuesInfo()
  : m_impl(std::make_unique<Impl>()) {}

ModMetadata::IssuesInfo::IssuesInfo(IssuesInfo const& other)
  : m_impl(std::make_unique<Impl>(*other.m_impl)) {}

ModMetadata::IssuesInfo::IssuesInfo(IssuesInfo&& other) noexcept
    : m_impl(std::move(other.m_impl)) {}

ModMetadata::IssuesInfo& ModMetadata::IssuesInfo::operator=(IssuesInfo const& other) {
    if (this != &other) {
        m_impl = std::make_unique<Impl>(*other.m_impl);
    }
    return *this;
}

ModMetadata::IssuesInfo& ModMetadata::IssuesInfo::operator=(IssuesInfo&& other) noexcept = default;

ModMetadata::IssuesInfo::~IssuesInfo() = default;

std::string const& ModMetadata::IssuesInfo::getInfo() const {
    return m_impl->info;
}

void ModMetadata::IssuesInfo::setInfo(std::string value) {
    m_impl->info = std::move(value);
}

std::optional<std::string> const& ModMetadata::IssuesInfo::getURL() const {
    return m_impl->url;
}

void ModMetadata::IssuesInfo::setURL(std::optional<std::string> value) {
    m_impl->url = std::move(value);
}

static std::string sanitizeDetailsData(std::string str) {
    // delete CRLF
    return utils::string::replace(std::move(str), "\r", "");
}

ModMetadata ModMetadata::Impl::parse(ModJson const& rawJson, std::optional<std::string_view> guessedID) {
    ModMetadata info;

    auto impl = info.m_impl.get();

    impl->m_rawJSON = rawJson;

    auto checkerRoot = fmt::format(
        "[{}/{}/mod.json]",
        rawJson["id"].asString().unwrapOr(guessedID ? std::string(*guessedID) : std::string("unknown.mod")),
        rawJson["version"].as<VersionInfo>().map(
            [](VersionInfo const& v) {
                return v.toVString();
            }
        ).unwrapOr("v0.0.0")
    );
    
    auto root = checkJson(impl->m_rawJSON, checkerRoot);
    root.needs("geode").into(impl->m_geodeVersion);

    if (auto gd = root.needs("gd")) {
        gd.assertIsObject();
        if (gd.has(GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH)) {
            gd.needs(GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH)
                .mustBe<std::string>("a valid gd version", [](auto const& str) {
                    return str == "*" || numFromString<double>(str).isOk();
                })
                .into(impl->m_gdVersion);
        }
        else {
            // this will error later on, but try to load the rest of the metadata
            // so that the mod can show up in the mods listing
            impl->m_gdVersion = "0.000";
        }
    }

    constexpr auto ID_REGEX = "[a-z0-9\\-_]+\\.[a-z0-9\\-_]+";
    root.needs("id")
        .mustBe<std::string>(ID_REGEX, &ModMetadata::validateID)
        .into(impl->m_id);
    
    if (impl->m_id.empty() && guessedID) {
        impl->m_id = *guessedID;
    }

    root.needs("version").into(impl->m_version);
    root.needs("name").into(impl->m_name);
    if (root.has("developers")) {
        if (root.has("developer")) {
            impl->m_errors.emplace_back(
                "[mod.json] can not have both \"developer\" and \"developers\" specified"
            );
        }
        else for (auto& dev : root.needs("developers").items()) {
            impl->m_developers.push_back(dev.get<std::string>());
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

    root.has("load-priority").into(impl->m_loadPriority);

    if (info.getID() != "geode.loader") {
        Dependency dep;
        dep.setID("geode.loader");
        dep.setVersion({ about::getLoaderVersion(), VersionCompare::MoreEq });
        dep.setRequired(true);
        dep.setMod(Mod::get());
        impl->m_dependencies.push_back(std::move(dep));
    }

    if (auto deps = root.has("dependencies")) {
        deps.assertIsObject();
        for (auto& [id, dep] : deps.properties()) {
            if (!ModMetadata::validateID(id)) {
                impl->m_errors.emplace_back(fmt::format(
                    "[mod.json].dependencies.\"{}\" is not a valid Mod ID ({})",
                    id, ID_REGEX
                ));
                continue;
            }

            dep.assertIs({ matjson::Type::Object, matjson::Type::String });

            // Check platforms array if we have an object, string syntax allows any platform
            if (dep.isObject()) {
                bool onThisPlatform = !dep.has("platforms");
                for (auto& plat : dep.has("platforms").items()) {
                    if (PlatformID::coveredBy(plat.get<std::string>(), GEODE_PLATFORM_TARGET)) {
                        onThisPlatform = true;
                    }
                }
                if (!onThisPlatform) {
                    continue;
                }
            }

            Dependency dependency;
            dependency.setID(std::move(id));

            ComparableVersionInfo version;
            if (dep.isString()) {
                dep.into(version);
                dependency.setVersion(version);
                dependency.setRequired(true);
            }
            else {
                dep.needs("version").into(version);
                dependency.setVersion(version);

                // Parse "required" field (since v5)
                bool required;
                dep.has("required").into(required);
                dependency.setRequired(required);

                // Compatibility for old "importance" field (although marks the 
                // mod.json as outdated) (I'm not sure if this is needed but I 
                // added it just in case not having it would cause weird load 
                // order issues)
                std::string oldImportance;
                dep.has("importance").into(oldImportance);
                dependency.setRequired(oldImportance == "required");
                if (impl->m_geodeVersion.getMajor() > 5 && oldImportance.size()) {
                    impl->m_errors.emplace_back(fmt::format(
                        "[mod.json].dependencies.\"{}\" is not a valid Mod ID ({})",
                        id, ID_REGEX
                    ));
                }

                matjson::Value dependencySettings;
                dep.has("settings").into(dependencySettings);
                dependency.setSettings(std::move(dependencySettings));
                
                dep.checkUnknownKeys();
            }

            // Check if parsing had errors
            if (!dep) {
                impl->m_errors.emplace_back(dep.ok().unwrapErr());
                continue;
            }

            if (
                dependency.getVersion().getComparison() != VersionCompare::MoreEq &&
                dependency.getVersion().getComparison() != VersionCompare::Any
            ) {
                impl->m_errors.emplace_back(fmt::format(
                    "[mod.json].dependencies.\"{}\".version (\"{}\") must be either a more-than "
                    "comparison for a specific version or a wildcard for any version",
                    dependency.getID(), dependency.getVersion()
                ));
                continue;
            }

            impl->m_dependencies.push_back(dependency);
        }
    }

    if (auto incompats = root.has("incompatibilities")) {
        incompats.assertIsObject();
        for (auto& [id, incompat] : incompats.properties()) {
            if (!ModMetadata::validateID(id)) {
                impl->m_errors.emplace_back(fmt::format(
                    "[mod.json].incompatibilities.\"{}\" is not a valid Mod ID ({})",
                    id, ID_REGEX
                ));
                continue;
            }

            incompat.assertIs({ matjson::Type::Object, matjson::Type::String});

            // Check platforms array if we have an object, string syntax allows any platform
            if (incompat.isObject()) {
                bool onThisPlatform = !incompat.has("platforms");
                for (auto& plat : incompat.has("platforms").items()) {
                    if (PlatformID::coveredBy(plat.get<std::string>(), GEODE_PLATFORM_TARGET)) {
                        onThisPlatform = true;
                    }
                }
                if (!onThisPlatform) {
                    continue;
                }
            }

            Incompatibility incompatibility;
            incompatibility.setID(std::move(id));

            ComparableVersionInfo version;
            if (incompat.isString()) {
                incompat.into(version);
                incompatibility.setVersion(version);
                incompatibility.setBreaking(true);
            }
            else {
                incompat.needs("version").into(version);
                incompatibility.setVersion(version);

                bool breaking;
                incompat.has("breaking").into(breaking);
                incompatibility.setBreaking(breaking);

                incompat.checkUnknownKeys();
            }

            // Check if parsing had errors
            if (!incompat) {
                impl->m_errors.emplace_back(incompat.ok().unwrapErr());
                continue;
            }

            impl->m_incompatibilities.push_back(incompatibility);
        }
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
        std::string rawInfo;
        issues.needs("info").into(rawInfo);
        issuesInfo.setInfo(rawInfo);
        std::optional<std::string> url;
        issues.has("url").into(url);
        issuesInfo.setURL(std::move(url));
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
        impl->m_tags.insert(tag.get<std::string>());
    }

    // with new cli, binary name is always mod id
    impl->m_binaryName = impl->m_id + GEODE_PLATFORM_EXTENSION;

    root.checkUnknownKeys();

    // Check JSON parsing errors
    if (!root) {
        impl->m_errors.emplace_back(root.ok().unwrapErr());
    }
    return info;
}

Result<> ModMetadata::Impl::addSpecialFiles(file::Unzip& unzip) {
    // unzip known MD files
    for (auto& [file, target] : this->getSpecialFiles()) {
        if (unzip.hasEntry(file)) {
            // reference to local binding 'file' declared in enclosing function
            std::string_view fileStr(file);
            GEODE_UNWRAP_INTO(auto data, unzip.extract(fileStr).mapErr([&](auto const& err) {
                return fmt::format("Unable to extract \"{}\": {}", fileStr, err);
            }));
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
            *target = sanitizeDetailsData(std::move(data.unwrap()));
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
    json["path"] = this->m_path;
    json["binary"] = this->m_binaryName;
    return json;
}

ModJson ModMetadata::Impl::getRawJSON() const {
    return m_rawJSON;
}

bool ModMetadata::Impl::operator==(ModMetadata::Impl const& other) const {
    return this->m_id == other.m_id;
}

std::filesystem::path const& ModMetadata::getPath() const {
    return m_impl->m_path;
}

ZStringView ModMetadata::getBinaryName() const {
    return m_impl->m_binaryName;
}

VersionInfo ModMetadata::getVersion() const {
    return m_impl->m_version;
}

ZStringView ModMetadata::getID() const {
    return m_impl->m_id;
}

ZStringView ModMetadata::getName() const {
    return m_impl->m_name;
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

std::vector<std::string> const& ModMetadata::getDevelopers() const {
    return m_impl->m_developers;
}
std::optional<std::string> const& ModMetadata::getDescription() const {
    return m_impl->m_description;
}
std::optional<std::string> const& ModMetadata::getDetails() const {
    return m_impl->m_details;
}
std::optional<std::string> const& ModMetadata::getChangelog() const {
    return m_impl->m_changelog;
}
std::optional<std::string> const& ModMetadata::getSupportInfo() const {
    return m_impl->m_supportInfo;
}
ModMetadataLinks const& ModMetadata::getLinks() const {
    return m_impl->m_links;
}
std::optional<ModMetadata::IssuesInfo> const& ModMetadata::getIssues() const {
    return m_impl->m_issues;
}
std::vector<ModMetadata::Dependency> const& ModMetadata::getDependencies() const {
    return m_impl->m_dependencies;
}
std::vector<ModMetadata::Incompatibility> const& ModMetadata::getIncompatibilities() const {
    return m_impl->m_incompatibilities;
}
std::vector<std::string> const& ModMetadata::getSpritesheets() const {
    return m_impl->m_spritesheets;
}
std::vector<std::pair<std::string, matjson::Value>> const& ModMetadata::getSettings() const {
    return m_impl->m_settings;
}
std::unordered_set<std::string> const& ModMetadata::getTags() const {
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
int ModMetadata::getLoadPriority() const {
    return m_impl->m_loadPriority;
}
bool ModMetadata::hasErrors() const {
    return m_impl->m_errors.size() || m_impl->m_completelyUnparseable;
}
std::vector<std::string> const& ModMetadata::getErrors() const {
    return m_impl->m_errors;
}
bool ModMetadata::wasCompletelyUnparseable() const {
    return m_impl->m_completelyUnparseable;
}
Result<> ModMetadata::checkGameVersion() const {
    if (!m_impl->m_gdVersion.empty() && m_impl->m_gdVersion != "*") {
        auto const ver = m_impl->m_gdVersion;

        auto res = numFromString<double>(ver);
        if (res.isErr()) {
            return Err("This mod ({}) has an invalid target GD version", m_impl->m_id);
        }
        double modTargetVer = res.unwrap();

        if (modTargetVer == 0.0) { // O.o
            return Err(fmt::format("This mod ({}) doesn't support the current platform.", m_impl->m_id));
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
                    "This mod ({}) was created for a different version of Geometry Dash ({}). You currently have version {}.",
                    m_impl->m_id, ver, GEODE_STR(GEODE_GD_VERSION)
                )
            );
        }
    }
    return Ok();
}
Result<> ModMetadata::checkGeodeVersion() const {
    if (!LoaderImpl::get()->isModVersionSupported(m_impl->m_geodeVersion)) {
        auto current = LoaderImpl::get()->getVersion();
        if (m_impl->m_geodeVersion > current) {
            return Err(
                "This mod ({}) was made for a newer version of Geode ({}). You currently have version {}.",
                m_impl->m_id, m_impl->m_geodeVersion, current
            );
        }
        else {
            return Err(
                "This mod ({}) was made for an older version of Geode ({}). You currently have version {}.",
                m_impl->m_id, m_impl->m_geodeVersion, current
            );
        }
    }
    return Ok();
}
Result<> ModMetadata::checkTargetVersions() const {
    GEODE_UNWRAP(this->checkGameVersion());
    GEODE_UNWRAP(this->checkGeodeVersion());
    return Ok();
}

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE)
void ModMetadata::setPath(std::filesystem::path value) {
    m_impl->m_path = value;
}
void ModMetadata::setBinaryName(std::string value) {
    m_impl->m_binaryName = value;
}
void ModMetadata::setVersion(VersionInfo value) {
    m_impl->m_version = value;
}
void ModMetadata::setID(std::string value) {
    m_impl->m_id = value;
}
void ModMetadata::setName(std::string value) {
    m_impl->m_name = value;
}
void ModMetadata::setDeveloper(std::string value) {
    m_impl->m_developers = { std::move(value) };
}
void ModMetadata::setDevelopers(std::vector<std::string> value) {
    m_impl->m_developers = value;
}
void ModMetadata::setDescription(std::optional<std::string> value) {
    m_impl->m_description = value;
}
void ModMetadata::setDetails(std::optional<std::string> value) {
    m_impl->m_details = value;
}
void ModMetadata::setChangelog(std::optional<std::string> value) {
    m_impl->m_changelog = value;
}
void ModMetadata::setSupportInfo(std::optional<std::string> value) {
    m_impl->m_supportInfo = value;
}
void ModMetadata::setRepository(std::optional<std::string> value) {
    this->getLinksMut().getImpl()->m_source = value;
}
void ModMetadata::setIssues(std::optional<IssuesInfo> value) {
    m_impl->m_issues = value;
}
void ModMetadata::setDependencies(std::vector<Dependency> value) {
    m_impl->m_dependencies = value;
}
void ModMetadata::setIncompatibilities(std::vector<Incompatibility> value) {
    m_impl->m_incompatibilities = value;
}
void ModMetadata::setSpritesheets(std::vector<std::string> value) {
    m_impl->m_spritesheets = value;
}
void ModMetadata::setSettings(std::vector<std::pair<std::string, matjson::Value>> value) {
    m_impl->m_settings = value;
}
void ModMetadata::setTags(std::unordered_set<std::string> value) {
    m_impl->m_tags = value;
}
void ModMetadata::setNeedsEarlyLoad(bool value) {
    m_impl->m_needsEarlyLoad = value;
}
void ModMetadata::setIsAPI(bool value) {
    m_impl->m_isAPI = value;
}
void ModMetadata::setGameVersion(std::string value) {
    m_impl->m_gdVersion = value;
}
void ModMetadata::setGeodeVersion(VersionInfo value) {
    m_impl->m_geodeVersion = value;
}
ModMetadataLinks& ModMetadata::getLinksMut() {
    return m_impl->m_links;
}
#endif

ModMetadata ModMetadata::Impl::createInvalidMetadata(
    std::filesystem::path const& path,
    std::string_view error,
    std::optional<std::string_view> guessedID
) {
    ModMetadata v {};
    // Use guessed ID or generate a random ID to prevent conflicts with existing mods
    v.m_impl->m_id = guessedID ? std::string(*guessedID) : fmt::format(
        "geode_invalid-{}",
        utils::random::generateString(16, "abcdefghijklmnopqrstuvwxyz0123456789_-")
    );
    v.m_impl->m_name = utils::string::pathToString(path.filename());
    v.m_impl->m_errors.emplace_back(error);
    v.m_impl->m_developers = { "-" };
    v.m_impl->m_completelyUnparseable = true;
    v.m_impl->m_path = path;
    return v;
}
ModMetadata ModMetadata::createFromGeodeFile(std::filesystem::path const& path) {
    // result code looks weird when you don't want to return a result

    // Try guess ID from filename (since usually Geode mods are named `mod.id.geode`)
    std::optional<std::string> guessedID = utils::string::pathToString(path.stem());
    if (!ModMetadata::validateID(*guessedID)) {
        guessedID = std::nullopt;
    }

    // Attempt to unzip, otherwise return invalid mod with unzip error
    auto r = file::Unzip::create(path);
    if (!r) {
        return Impl::createInvalidMetadata(path, r.unwrapErr(), guessedID);
    }

    auto&& unzip = std::move(r.unwrap());

    /// Extract mod.json from the zip and parse it

    // First check if mod.json exists for a nicer error
    if (!unzip.hasEntry("mod.json")) {
        return Impl::createInvalidMetadata(path, "Geode package is missing \"mod.json\"", guessedID);
    }

    // Extract file
    auto modJsonDataRes = unzip.extract("mod.json").mapErr([](auto const& err) {
        return fmt::format("Unable to extract mod.json: {}", err);
    });
    if (!modJsonDataRes) {
        return Impl::createInvalidMetadata(path, modJsonDataRes.unwrapErr(), guessedID);
    }
    auto&& modJsonData = std::move(modJsonDataRes.unwrap());

    // Parse the JSON
    auto modJsonRes = matjson::parse(std::string(modJsonData.begin(), modJsonData.end()))
        .mapErr([](auto const& err) {
            return fmt::format("Unable to parse mod.json: {}", err);
        });
    if (!modJsonRes) {
        return Impl::createInvalidMetadata(path, modJsonRes.unwrapErr(), guessedID);
    }
    auto&& modJson = std::move(modJsonRes.unwrap());

    return Impl::parse(modJson, guessedID);
}
ModMetadata ModMetadata::create(ModJson const& json) {
    return Impl::parse(json, std::nullopt);
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

bool ModMetadata::validateID(std::string_view id) {
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
    if (this != &other) {
        m_impl = std::make_unique<Impl>(*other.m_impl);
    }
    return *this;
}
ModMetadata& ModMetadata::operator=(ModMetadata&& other) noexcept = default;

ModMetadata::~ModMetadata() = default;
