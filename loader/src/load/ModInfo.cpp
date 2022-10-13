#include <about.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/file.hpp>

USE_GEODE_NAMESPACE();

static std::string sanitizeDetailsData(std::string const& str) {
    // delete CRLF
    return utils::string::replace(str, "\r", "");
}

Result<ModInfo> ModInfo::createFromSchemaV010(ModJson const& rawJson) {
    ModInfo info;

    auto json = rawJson;

    #define PROPAGATE(err) \
        { auto err__ = err; if (!err__) return Err(err__.error()); }

    JsonChecker checker(json);
    auto root = checker.root("[mod.json]").obj();

    root.addKnownKey("geode");
    root.addKnownKey("binary");

    using nlohmann::detail::value_t;

    root
        .needs("id")
        .validate(&Mod::validateID)
        .into(info.m_id);
    root
        .needs("version")
        .validate(&VersionInfo::validate)
        .intoAs<std::string>(info.m_version);
    root.needs("name").into(info.m_name);
    root.needs("developer").into(info.m_developer);
    root.has("description").into(info.m_description);
    root.has("repository").into(info.m_repository);
    root.has("datastore").intoRaw(info.m_defaultDataStore);
    root.has("toggleable").into(info.m_supportsDisabling);
    root.has("unloadable").into(info.m_supportsUnloading);

    for (auto& dep : root.has("dependencies").iterate()) {
        auto obj = dep.obj();

        auto depobj = Dependency {};
        obj
            .needs("id")
            .validate(&Mod::validateID)
            .into(depobj.m_id);
        obj
            .needs("version")
            .validate(&VersionInfo::validate)
            .intoAs<std::string>(depobj.m_version);
        obj
            .has("required")
            .into(depobj.m_required);
        obj.checkUnknownKeys();

        info.m_dependencies.push_back(depobj);
    }

    for (auto& [key, value] : root.has("settings").items()) {
        auto settRes = Setting::parse(key, value.json());
        PROPAGATE(settRes);
        auto sett = settRes.value();
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

    if (root.has("binary")) {
        log::warn(
            "[mod.json].binary is deprecated "
            "and will be removed in the future."
        );
    }

    root.has("binary").asOneOf<value_t::string, value_t::object>();

    bool autoEndBinaryName = true;

    root.has("binary").is<value_t::string>().into(info.m_binaryName);
    
    if (auto bin = root.has("binary").is<value_t::object>().obj()) {
        bin.has("*").into(info.m_binaryName);
        bin.has("auto").into(autoEndBinaryName);

    #if defined(GEODE_IS_WINDOWS)
        bin.has("windows").into(info.m_binaryName);
    #elif defined(GEODE_IS_MACOS)
        bin.has("macos").into(info.m_binaryName);
    #elif defined(GEODE_IS_ANDROID)
        bin.has("android").into(info.m_binaryName);
    #elif defined(GEODE_IS_IOS)
        bin.has("ios").into(info.m_binaryName);
    #endif
    }

    if (
        root.has("binary") &&
        autoEndBinaryName &&
        !utils::string::endsWith(info.m_binaryName, GEODE_PLATFORM_EXTENSION)
    ) {
        info.m_binaryName += GEODE_PLATFORM_EXTENSION;
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
        } else {
            return Err(
                "[mod.json] has no target loader version "
                "specified, or it is invalidally formatted (required: \"[v]X.X.X\")!"
            );
        }
    } else {
        return Err(
            "[mod.json] has no target loader version "
            "specified, or it is invalidally formatted (required: \"[v]X.X.X\")!"
        );
    }
    if (schema < Loader::minModVersion()) {
        return Err(
            "[mod.json] is built for an older version (" + 
            schema.toString() + ") of Geode (current: " + 
            Loader::minModVersion().toString() +
            "). Please update the mod to the latest version, "
            "and if the problem persists, contact the developer "
            "to update it."
        );
    }
    if (schema > Loader::maxModVersion()) {
        return Err(
            "[mod.json] is built for a newer version (" + 
            schema.toString() + ") of Geode (current: " +
            Loader::maxModVersion().toString() +
            "). You need to update Geode in order to use "
            "this mod."
        );
    }

    // Handle mod.json data based on target
    if (schema >= VersionInfo(0, 1, 0)) {
        return ModInfo::createFromSchemaV010(json);
    }

    return Err(
        "[mod.json] targets a version (" +
        schema.toString() + ") that isn't "
        "supported by this version (v" + 
        LOADER_VERSION_STR + ") of geode. "
        "This is probably a bug; report it to "
        "the Geode Development Team."
    );
}

Result<ModInfo> ModInfo::createFromFile(ghc::filesystem::path const& path) {
    try {
        auto read = utils::file::readString(path);
        if (!read) return Err(read.error());
        try {
            auto res = ModInfo::create(ModJson::parse(read.value()));
            if (!res) return res;
            auto info = res.value();
            info.m_path = path;
            if (path.has_parent_path()) {
                info.addSpecialFiles(path.parent_path());
            }
            return Ok(info);
        } catch(std::exception& e) {
            return Err("Unable to parse mod.json: " + std::string(e.what()));
        }
    } catch(std::exception const& e) {
        return Err(e.what());
    }
}

Result<ModInfo> ModInfo::createFromGeodeFile(ghc::filesystem::path const& path) {
    ZipFile unzip(path.string());
    if (!unzip.isLoaded()) {
        return Err<>("\"" + path.string() + "\": Unable to unzip");
    }
    // Check if mod.json exists in zip
    if (!unzip.fileExists("mod.json")) {
        return Err<>("\"" + path.string() + "\" is missing mod.json");
    }
    // Read mod.json & parse if possible
    unsigned long readSize = 0;
    auto read = unzip.getFileData("mod.json", &readSize);
    if (!read || !readSize) {
        return Err("\"" + path.string() + "\": Unable to read mod.json");
    }
    ModJson json;
    try {
        json = ModJson::parse(std::string(read, read + readSize));
    } catch(std::exception const& e) {
        delete[] read;
        return Err<>(e.what());
    }

    delete[] read;

    if (!json.is_object()) {
        return Err(
            "\"" + path.string() + "/mod.json\" does not have an "
            "object at root despite expected"
        );
    }

    auto res = ModInfo::create(json);
    if (!res) {
        return Err("\"" + path.string() + "\" - " + res.error());
    }
    auto info = res.value();
    info.m_path = path;

    info.addSpecialFiles(unzip);
    
    return Ok(info);
}

Result<> ModInfo::addSpecialFiles(ZipFile& unzip) {
    // unzip known MD files
    for (auto& [file, target] : getSpecialFiles()) {
        if (unzip.fileExists(file)) {
            unsigned long readSize = 0;
            auto fileData = unzip.getFileData(file, &readSize);
            if (!fileData || !readSize) {
                return Err("Unable to read \"" + file + "\"");
            } else {
                *target = sanitizeDetailsData(
                    std::string(fileData, fileData + readSize)
                );
            }
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
                return Err("Unable to read \"" + file + "\": " + data.error());
            }
            *target = sanitizeDetailsData(data.value());
        }
    }
    return Ok();
}

std::vector<std::pair<
    std::string,
    std::optional<std::string>*
>> ModInfo::getSpecialFiles() {
    return {
        { "about.md",     &m_details },
        { "changelog.md", &m_changelog },
        { "support.md",   &m_supportInfo },
    };
}
