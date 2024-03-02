#include "Server.hpp"
#include <Geode/utils/JsonValidation.hpp>
#include <loader/ModMetadataImpl.hpp>
#include <fmt/chrono.h>

using namespace server;

#define GEODE_GD_VERSION_STR GEODE_STR(GEODE_GD_VERSION)

static const char* jsonTypeToString(matjson::Type const& type) {
    switch (type) {
        case matjson::Type::Object: return "object";
        case matjson::Type::Array: return "array";
        case matjson::Type::Bool: return "boolean";
        case matjson::Type::Number: return "number";
        case matjson::Type::String: return "string";
        case matjson::Type::Null: return "null";
        default: return "unknown";
    }
}

static Result<matjson::Value, ServerError> parseServerPayload(web::WebResponse&& response) {
    auto asJson = response.json();
    if (!asJson) {
        return Err(ServerError(response.code(), "Response was not valid JSON: {}", asJson.unwrapErr()));
    }
    auto json = std::move(asJson).unwrap();
    if (!json.is_object()) {
        return Err(ServerError(response.code(), "Expected object, got {}", jsonTypeToString(json.type())));
    }
    auto obj = json.as_object();
    if (!obj.contains("payload")) {
        return Err(ServerError(response.code(), "Object does not contain \"payload\" key - got {}", json.dump()));
    }
    return Ok(obj["payload"]);
}

static void parseServerError(auto reject, auto error) {
    // The server should return errors as `{ "error": "...", "payload": "" }`
    if (auto asJson = error.json()) {
        auto json = asJson.unwrap();
        if (json.is_object() && json.contains("error")) {
            reject(ServerError(
                error.code(),
                "{}", json.template get<std::string>("error")
            ));
        }
        else {
            reject(ServerError(error.code(), "Unknown (not valid JSON)"));
        }
    }
    // But if we get something else for some reason, return that
    else {
        reject(ServerError(
            error.code(),
            "{}", error.string().unwrapOr("Unknown (not a valid string)")
        ));
    }
}

static void parseServerProgress(auto progress, auto prog, auto msg) {
    if (auto per = prog.downloadProgress()) {
        progress({ msg, static_cast<uint8_t>(*per) });
    }
    else {
        progress({ msg });
    }
}

const char* server::sortToString(ModsSort sorting) {
    switch (sorting) {
        default:
        case ModsSort::Downloads: return "downloads";
        case ModsSort::RecentlyUpdated: return "recently_updated";
        case ModsSort::RecentlyPublished: return "recently_published";
    }
}

std::string ServerDateTime::toAgoString() const {
    auto const fmtPlural = [](auto count, auto unit) {
        if (count == 1) {
            return fmt::format("{} {} ago", count, unit);
        }
        return fmt::format("{} {}s ago", count, unit);
    };
    auto now = Clock::now();
    auto len = std::chrono::duration_cast<std::chrono::minutes>(now - value).count();
    if (len < 60) {
        return fmtPlural(len, "minute");
    }
    len = std::chrono::duration_cast<std::chrono::hours>(now - value).count();
    if (len < 24) {
        return fmtPlural(len, "hour");
    }
    len = std::chrono::duration_cast<std::chrono::days>(now - value).count();
    if (len < 31) {
        return fmtPlural(len, "day");
    }
    return fmt::format("{:%b %d %Y}", value);
}

Result<ServerDateTime> ServerDateTime::parse(std::string const& str) {
    std::stringstream ss(str);
    Value value;
    if (std::chrono::from_stream(ss, "%FT%TZ", value)) {
        return Ok(ServerDateTime {
            .value = value,
        });
    }
    return Err("Invalid date time format '{}'", str);
}

Result<ServerModVersion> ServerModVersion::parse(matjson::Value const& raw) {
    auto json = raw;
    JsonChecker checker(json);
    auto root = checker.root("ServerModVersion").obj();

    auto res = ServerModVersion();

    // Verify target Geode version
    auto version = root.needs("geode").template get<VersionInfo>();
    if (!semverCompare(Loader::get()->getVersion(), version)) {
        return Err(
            "Mod targets version {} but Geode is version {}",
            version, Loader::get()->getVersion()
        );
    }

    // Verify target GD version
    auto gd = root.needs("gd").obj().needs(GEODE_PLATFORM_SHORT_IDENTIFIER).template get<std::string>();
    if (gd != GEODE_GD_VERSION_STR && gd != "*") {
        return Err(
            "Mod targets GD version {} but current is version {}",
            gd, GEODE_GD_VERSION_STR
        );
    }

    // Get server info
    root.needs("download_link").into(res.downloadURL);
    root.needs("download_count").into(res.downloadCount);
    root.needs("hash").into(res.hash);

    // Get mod metadata info
    res.metadata.setID(root.needs("mod_id").template get<std::string>());
    res.metadata.setName(root.needs("name").template get<std::string>());
    res.metadata.setDescription(root.needs("description").template get<std::string>());
    res.metadata.setVersion(root.needs("version").template get<VersionInfo>());
    res.metadata.setIsAPI(root.needs("api").template get<bool>());

    std::vector<ModMetadata::Dependency> dependencies {};
    for (auto dep : root.has("dependencies").iterate()) {
        // todo: this should probably be generalized to use the same function as mod.json

        auto obj = dep.obj();

        bool onThisPlatform = !obj.has("platforms");
        for (auto& plat : obj.has("platforms").iterate()) {
            if (PlatformID::from(plat.get<std::string>()) == GEODE_PLATFORM_TARGET) {
                onThisPlatform = true;
            }
        }
        if (!onThisPlatform) {
            continue;
        }

        ModMetadata::Dependency dependency;
        obj.needs("id").validate(MiniFunction<bool(std::string const&)>(&ModMetadata::validateID)).into(dependency.id);
        obj.needs("version").into(dependency.version);
        obj.has("importance").into(dependency.importance);

        dependencies.push_back(dependency);
    }
    res.metadata.setDependencies(dependencies);

    std::vector<ModMetadata::Incompatibility> incompatibilities {};
    for (auto& incompat : root.has("incompatibilities").iterate()) {
        auto obj = incompat.obj();

        ModMetadata::Incompatibility incompatibility;
        obj.needs("id").validate(MiniFunction<bool(std::string const&)>(&ModMetadata::validateID)).into(incompatibility.id);
        obj.needs("version").into(incompatibility.version);
        obj.has("importance").into(incompatibility.importance);

        incompatibilities.push_back(incompatibility);
    }

    // Check for errors and return result
    if (root.isError()) {
        return Err(root.getError());
    }
    return Ok(res);
}

Result<ServerModMetadata> ServerModMetadata::parse(matjson::Value const& raw) {
    auto json = raw;
    JsonChecker checker(json);
    auto root = checker.root("ServerModMetadata").obj();

    auto res = ServerModMetadata();
    root.needs("id").into(res.id);
    root.needs("featured").into(res.featured);
    root.needs("download_count").into(res.downloadCount);
    root.has("about").into(res.about);
    root.has("changelog").into(res.changelog);
    if (root.has("created_at")) {
        GEODE_UNWRAP_INTO(res.createdAt, ServerDateTime::parse(root.has("created_at").template get<std::string>()));
    }
    if (root.has("updated_at")) {
        GEODE_UNWRAP_INTO(res.updatedAt, ServerDateTime::parse(root.has("updated_at").template get<std::string>()));
    }

    std::vector<std::string> developerNames;
    for (auto item : root.needs("developers").iterate()) {
        auto obj = item.obj();
        auto dev = ServerDeveloper();
        obj.needs("username").into(dev.username);
        obj.needs("display_name").into(dev.displayName);
        res.developers.push_back(dev);
        developerNames.push_back(dev.displayName);
    }
    for (auto item : root.needs("versions").iterate()) {
        auto versionRes = ServerModVersion::parse(item.json());
        if (versionRes) {
            auto version = versionRes.unwrap();
            version.metadata.setDetails(res.about);
            version.metadata.setChangelog(res.changelog);
            version.metadata.setDevelopers(developerNames);
            res.versions.push_back(version);
        }
        else {
            log::error("Unable to parse mod '{}' version from the server: {}", res.id, versionRes.unwrapErr());
        }
    }

    // Ensure there's at least one valid version
    if (res.versions.empty()) {
        return Err("Mod '{}' has no (valid) versions", res.id);
    }
    
    for (auto item : root.has("tags").iterate()) {
        res.tags.insert(item.template get<std::string>());
    }

    root.needs("download_count").into(res.downloadCount);

    // Check for errors and return result
    if (root.isError()) {
        return Err(root.getError());
    }
    return Ok(res);
}

Result<ServerModsList> ServerModsList::parse(matjson::Value const& raw) {
    auto json = raw;
    JsonChecker checker(json);
    auto payload = checker.root("ServerModsList").obj();

    auto list = ServerModsList();
    for (auto item : payload.needs("data").iterate()) {
        auto mod = ServerModMetadata::parse(item.json());
        if (mod) {
            list.mods.push_back(mod.unwrap());
        }
        else {
            log::error("Unable to parse mod from the server: {}", mod.unwrapErr());
        }
    }
    payload.needs("count").into(list.totalModCount);

    // Check for errors and return result
    if (payload.isError()) {
        return Err(payload.getError());
    }
    return Ok(list);
}

std::string server::getServerAPIBaseURL() {
    return "https://api.geode-sdk.org/v1";
}

std::string server::getServerUserAgent() {
    // this may change in the future..
    return fmt::format("Geode {}/{}",
        Loader::get()->getVersion().toString(),
        PlatformID::toShortString(GEODE_PLATFORM_TARGET)
    );
}

ServerPromise<ServerModsList> server::getMods(ModsQuery const& query) {
    auto req = web::WebRequest();
    req.userAgent(getServerUserAgent());

    // Always target current GD version and Loader version
    req.param("gd", GEODE_GD_VERSION_STR);
    req.param("geode", Loader::get()->getVersion().toString());

    // Add search params
    if (query.query) {
        req.param("query", *query.query);
    }
    if (query.platforms.size()) {
        std::string plats = "";
        bool first = true;
        for (auto plat : query.platforms) {
            if (!first) plats += ",";
            plats += PlatformID::toShortString(plat.m_value);
            first = false;
        }
        req.param("platforms", plats);
    }
    if (query.tags.size()) {
        req.param("tags", ranges::join(query.tags, ","));
    }
    if (query.featured) {
        req.param("featured", query.featured.value() ? "true" : "false");
    }
    req.param("sort", sortToString(query.sorting));
    if (query.developer) {
        req.param("developer", *query.developer);
    }

    // Paging (1-based on server, 0-based locally)
    req.param("page", std::to_string(query.page + 1));
    req.param("per_page", std::to_string(query.pageSize));

    return ServerPromise<ServerModsList>([req = std::move(req)](auto resolve, auto reject, auto progress, auto cancel) mutable {
        req.get(getServerAPIBaseURL() + "/mods")
            .then([resolve, reject](auto value) {
                // Store the code, since the value is moved afterwards
                auto code = value.code();

                // Parse payload
                auto payload = parseServerPayload(std::move(value));
                if (!payload) {
                    return reject(payload.unwrapErr());
                }
                // Parse response
                auto list = ServerModsList::parse(payload.unwrap());
                if (!list) {
                    return reject(ServerError(code, "Unable to parse response: {}", list.unwrapErr()));
                }
                resolve(list.unwrap());
            })
            .expect([resolve, reject](auto error) {
                // Treat a 404 as empty mods list
                if (error.code() == 404) {
                    return resolve(ServerModsList());
                }
                parseServerError(reject, error);
            })
            .progress([progress](auto prog) {
                parseServerProgress(progress, prog, "Downloading mods");
            })
            .link(cancel);
    });
}

ServerPromise<ServerModMetadata> server::getMod(std::string const& id) {
    auto req = web::WebRequest();
    req.userAgent(getServerUserAgent());
    return ServerPromise<ServerModMetadata>([req = std::move(req), id](auto resolve, auto reject, auto progress, auto cancel) mutable {
        req.get(getServerAPIBaseURL() + "/mods/" + id)
            .then([resolve, reject](auto value) {
                // Store the code, since the value is moved afterwards
                auto code = value.code();

                // Parse payload
                auto payload = parseServerPayload(std::move(value));
                if (!payload) {
                    return reject(payload.unwrapErr());
                }
                // Parse response
                auto list = ServerModMetadata::parse(payload.unwrap());
                if (!list) {
                    return reject(ServerError(code, "Unable to parse response: {}", list.unwrapErr()));
                }
                resolve(list.unwrap());
            })
            .expect([reject](auto error) {
                parseServerError(reject, error);
            })
            .progress([progress, id](auto prog) {
                parseServerProgress(progress, prog, "Downloading logo for " + id);
            })
            .link(cancel);
    });
}

ServerPromise<ByteVector> server::getModLogo(std::string const& id) {
    auto req = web::WebRequest();
    req.userAgent(getServerUserAgent());
    return ServerPromise<ByteVector>([req = std::move(req), id](auto resolve, auto reject, auto progress, auto cancel) mutable {
        req.get(getServerAPIBaseURL() + "/mods/" + id + "/logo")
            .then([resolve](auto response) {
                resolve(response.data());
            })
            .expect([reject](auto error) {
                parseServerError(reject, error);
            })
            .progress([progress, id](auto prog) {
                parseServerProgress(progress, prog, "Downloading logo for " + id);
            })
            .link(cancel);
    });
}
