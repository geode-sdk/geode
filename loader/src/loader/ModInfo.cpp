#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/string.hpp>
#include <about.hpp>

USE_GEODE_NAMESPACE();

static std::string sanitizeDetailsData(std::string const& str) {
    // delete CRLF
    return utils::string::replace(str, "\r", "");
}

bool ModInfo::validateID(std::string const& id) {
    // ids may not be empty
    if (!id.size()) return false;
    for (auto const& c : id) {
        if (!(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') ||
              (c == '-') || (c == '_') || (c == '.')))
            return false;
    }
    return true;
}

Result<ModInfo> ModInfo::createFromSchemaV010(ModJson const& rawJson) {
    ModInfo info;

    auto json = rawJson;
    info.m_rawJSON = rawJson;

    JsonChecker checker(json);
    auto root = checker.root("[mod.json]").obj();

    root.addKnownKey("geode");
    root.addKnownKey("binary");

    using nlohmann::detail::value_t;

    root.needs("id").validate(&ModInfo::validateID).into(info.m_id);
    root.needs("version").validate(&VersionInfo::validate).intoAs<std::string>(info.m_version);
    root.needs("name").into(info.m_name);
    root.needs("developer").into(info.m_developer);
    root.has("description").into(info.m_description);
    root.has("repository").into(info.m_repository);
    root.has("toggleable").into(info.m_supportsDisabling);
    root.has("unloadable").into(info.m_supportsUnloading);
    root.has("early-load").into(info.m_needsEarlyLoad);

    for (auto& dep : root.has("dependencies").iterate()) {
        auto obj = dep.obj();

        auto depobj = Dependency {};
        obj.needs("id").validate(&ModInfo::validateID).into(depobj.m_id);
        obj.needs("version").validate(&VersionInfo::validate).intoAs<std::string>(depobj.m_version);
        obj.has("required").into(depobj.m_required);
        obj.checkUnknownKeys();

        info.m_dependencies.push_back(depobj);
    }

    for (auto& [key, value] : root.has("settings").items()) {
        GEODE_UNWRAP_INTO(auto sett, Setting::parse(key, value.json()));
        sett->m_modID = info.m_id;
        info.m_settings.push_back({ key, sett });
    }

    if (auto resources = root.has("resources").obj()) {
        for (auto& [key, _] : resources.has("spritesheets").items()) {
            info.m_spritesheets.push_back(info.m_id + "/" + key);
        }
    }

    if (auto issues = root.has("issues").obj()) {
        IssuesInfo issuesInfo;
        issues.needs("info").into(issuesInfo.m_info);
        issues.has("url").intoAs<std::string>(issuesInfo.m_url);
        info.m_issues = issuesInfo;
    }

    // with new cli, binary name is always mod id
    info.m_binaryName = info.m_id + GEODE_PLATFORM_EXTENSION;

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

Result<ModInfo> ModInfo::create(ModJson const& json) {
    // Check mod.json target version
    auto schema = LOADER_VERSION;
    if (json.contains("geode") && json["geode"].is_string()) {
        auto ver = json["geode"];
        if (VersionInfo::validate(ver)) {
            schema = VersionInfo(ver);
        }
        else {
            return Err(
                "[mod.json] has no target loader version "
                "specified, or it is invalidally formatted (required: \"[v]X.X.X\")!"
            );
        }
    }
    else {
        return Err(
            "[mod.json] has no target loader version "
            "specified, or it is invalidally formatted (required: \"[v]X.X.X\")!"
        );
    }
    if (schema < Loader::minModVersion()) {
        return Err(
            "[mod.json] is built for an older version (" + schema.toString() +
            ") of Geode (current: " + Loader::getVersion().toString() +
            "). Please update the mod to the latest version, "
            "and if the problem persists, contact the developer "
            "to update it."
        );
    }
    if (schema > Loader::maxModVersion()) {
        return Err(
            "[mod.json] is built for a newer version (" + schema.toString() +
            ") of Geode (current: " + Loader::getVersion().toString() +
            "). You need to update Geode in order to use "
            "this mod."
        );
    }

    // Handle mod.json data based on target
    if (schema >= VersionInfo(0, 1, 0)) {
        return ModInfo::createFromSchemaV010(json);
    }

    return Err(
        "[mod.json] targets a version (" + schema.toString() +
        ") that isn't "
        "supported by this version (v" +
        LOADER_VERSION_STR +
        ") of geode. "
        "This is probably a bug; report it to "
        "the Geode Development Team."
    );
}

Result<ModInfo> ModInfo::createFromFile(ghc::filesystem::path const& path) {
    GEODE_UNWRAP_INTO(auto read, utils::file::readString(path));
    try {
        GEODE_UNWRAP_INTO(auto info, ModInfo::create(ModJson::parse(read)));
        info.m_path = path;
        if (path.has_parent_path()) {
            GEODE_UNWRAP(info.addSpecialFiles(path.parent_path()));
        }
        return Ok(info);
    }
    catch (std::exception& e) {
        return Err("Unable to parse mod.json: " + std::string(e.what()));
    }
}

Result<ModInfo> ModInfo::createFromGeodeFile(ghc::filesystem::path const& path) {
    GEODE_UNWRAP_INTO(auto unzip, file::Unzip::create(path));
    return ModInfo::createFromGeodeZip(unzip);
}

Result<ModInfo> ModInfo::createFromGeodeZip(file::Unzip& unzip) {
    // Check if mod.json exists in zip
    if (!unzip.hasEntry("mod.json")) {
        return Err("\"" + unzip.getPath().string() + "\" is missing mod.json");
    }

    // Read mod.json & parse if possible
    GEODE_UNWRAP_INTO(
        auto jsonData,
        unzip.extract("mod.json").expect("Unable to read mod.json: {error}")
    );
    ModJson json;
    try {
        json = ModJson::parse(std::string(jsonData.begin(), jsonData.end()));
    }
    catch (std::exception const& e) {
        return Err(e.what());
    }

    auto res = ModInfo::create(json);
    if (!res) {
        return Err("\"" + unzip.getPath().string() + "\" - " + res.unwrapErr());
    }
    auto info = res.unwrap();
    info.m_path = unzip.getPath();

    GEODE_UNWRAP(
        info.addSpecialFiles(unzip)
            .expect("Unable to add extra files: {error}")
    );

    return Ok(info);
}

Result<> ModInfo::addSpecialFiles(file::Unzip& unzip) {
    // unzip known MD files
    for (auto& [file, target] : getSpecialFiles()) {
        if (unzip.hasEntry(file)) {
            GEODE_UNWRAP_INTO(auto data, unzip.extract(file).expect(
                "Unable to extract \"{}\"", file
            ));
            *target = sanitizeDetailsData(std::string(data.begin(), data.end()));
        }
    }
    return Ok();
}

Result<> ModInfo::addSpecialFiles(ghc::filesystem::path const& dir) {
    // unzip known MD files
    for (auto& [file, target] : getSpecialFiles()) {
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

std::vector<std::pair<std::string, std::optional<std::string>*>> ModInfo::getSpecialFiles() {
    return {
        { "about.md", &m_details },
        { "changelog.md", &m_changelog },
        { "support.md", &m_supportInfo },
    };
}

ModJson ModInfo::toJSON() const {
    auto json = m_rawJSON;
    json["path"] = m_path;
    json["binary"] = m_binaryName;
    return json;
}

ModJson ModInfo::getRawJSON() const {
    return m_rawJSON;
}

bool ModInfo::operator==(ModInfo const& other) const {
    return m_id == other.m_id;
}

void geode::to_json(nlohmann::json& json, ModInfo const& info) {
    json = info.toJSON();
}
