#include "Server.hpp"
#include <Geode/utils/JsonValidation.hpp>
#include <loader/ModMetadataImpl.hpp>
#include <fmt/chrono.h>

using namespace server;

#define GEODE_GD_VERSION_STR GEODE_STR(GEODE_GD_VERSION)

template <class K, class V> 
class CacheMap final {
private:
    // I know this looks like a goofy choice over just 
    // `std::unordered_map`, but hear me out:
    // 
    // This needs preserved insertion order (so shrinking the cache 
    // to match size limits doesn't just have to erase random 
    // elements) 
    // 
    // If this used a map for values and another vector for storing 
    // insertion order, it would have a pretty big memory footprint 
    // (two copies of Query, one for order, one for map + two heap 
    // allocations on top of that)
    // 
    // In addition, it would be a bad idea to have a cache of 1000s 
    // of items in any case (since that would likely take up a ton 
    // of memory, which we want to avoid since it's likely many 
    // crashes with the old index were due to too much memory 
    // usage)
    // 
    // Linear searching a vector of at most a couple dozen items is 
    // lightning-fast (🚀), and besides the main performance benefit 
    // comes from the lack of a web request - not how many extra 
    // milliseconds we can squeeze out of a map access
    std::vector<std::pair<K, V>> m_values;
    size_t m_sizeLimit = 20;

public:
    std::optional<V> get(K const& key) {
        auto it = std::find_if(m_values.begin(), m_values.end(), [key](auto const& q) {
            return q.first == key;
        });
        if (it != m_values.end()) {
            return it->second;
        }
        return std::nullopt;
    }
    void add(K&& key, V&& value) {
        auto pair = std::make_pair(std::move(key), std::move(value));

        // Shift and replace last element if we're at cache size limit
        if (m_values.size() >= m_sizeLimit) {
            std::shift_left(m_values.begin(), m_values.end(), 1);
            m_values.back() = std::move(pair);
        }
        // Otherwise append at end
        else {
            m_values.emplace_back(std::move(pair));
        }
    }
    void remove(K const& key) {
        ranges::remove(m_values, [&key](auto const& q) { return q.first == key; });
    }
    void clear() {
        m_values.clear();
    }
    void limit(size_t size) {
        m_sizeLimit = size;
        m_values.clear();
    }
    size_t size() const {
        return m_values.size();
    }
    size_t limit() const {
        return m_sizeLimit;
    }
};

template <class Q, class V>
using ServerFuncQ  = V(*)(Q const&, bool);
template <class V>
using ServerFuncNQ = V(*)(bool);

template <class F>
struct ExtractFun;

template <class Q, class V>
struct ExtractFun<ServerRequest<V>(*)(Q const&, bool)> {
    using Query = Q;
    using Value = V;
    static ServerRequest<V> invoke(auto&& func, Query const& query) {
        return func(query, false);
    }
};

template <class V>
struct ExtractFun<ServerRequest<V>(*)(bool)> {
    using Query = std::monostate;
    using Value = V;
    static ServerRequest<V> invoke(auto&& func, Query const&) {
        return func(false);
    }
};

template <auto F>
class FunCache final {
public:
    using Extract = ExtractFun<decltype(F)>;
    using Query   = typename Extract::Query;
    using Value   = typename Extract::Value;

private:
    CacheMap<Query, ServerRequest<Value>> m_cache;

public:
    FunCache() = default;
    FunCache(FunCache const&) = delete;
    FunCache(FunCache&&) = delete;

    ServerRequest<Value> get(Query const& query = Query()) {
        if (auto v = m_cache.get(query)) {
            return *v;
        }
        auto f = Extract::invoke(F, query);
        m_cache.add(Query(query), ServerRequest<Value>(f));
        return f;
    }
    void clear() {
        m_cache.clear();
    }
};

template <auto F>
FunCache<F>& getCache() {
    static auto inst = FunCache<F>();
    return inst;
}

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

static Result<matjson::Value, ServerError> parseServerPayload(web::WebResponse const& response) {
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

static ServerError parseServerError(web::WebResponse const& error) {
    // The server should return errors as `{ "error": "...", "payload": "" }`
    if (auto asJson = error.json()) {
        auto json = asJson.unwrap();
        if (json.is_object() && json.contains("error")) {
            return ServerError(
                error.code(),
                "{}", json.template get<std::string>("error")
            );
        }
        else {
            return ServerError(error.code(), "Unknown (not valid JSON)");
        }
    }
    // But if we get something else for some reason, return that
    else {
        return ServerError(
            error.code(),
            "{}", error.string().unwrapOr("Unknown (not a valid string)")
        );
    }
}

static ServerProgress parseServerProgress(web::WebProgress const& prog, auto msg) {
    if (auto per = prog.downloadProgress()) {
        return ServerProgress(msg, static_cast<uint8_t>(*per));
    }
    else {
        return ServerProgress(msg);
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
    std::tm value;
    if (ss >> std::get_time(&value, "%Y-%m-%dT%H:%M:%SZ")) {
        auto time = std::mktime(&value);
        return Ok(ServerDateTime {
            .value = Clock::from_time_t(time),
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
    res.metadata.setIncompatibilities(incompatibilities);

    // Check for errors and return result
    if (root.isError()) {
        return Err(root.getError());
    }
    return Ok(res);
}

Result<ServerModUpdate> ServerModUpdate::parse(matjson::Value const& raw) {
    auto json = raw;
    JsonChecker checker(json);
    auto root = checker.root("ServerModUpdate").obj();

    auto res = ServerModUpdate();

    root.needs("id").into(res.id);
    root.needs("version").into(res.version);

    // Check for errors and return result
    if (root.isError()) {
        return Err(root.getError());
    }
    return Ok(res);
}

Result<std::vector<ServerModUpdate>> ServerModUpdate::parseList(matjson::Value const& raw) {
    auto json = raw;
    JsonChecker checker(json);
    auto payload = checker.root("ServerModUpdatesList").array();

    std::vector<ServerModUpdate> list {};
    for (auto item : payload.iterate()) {
        auto mod = ServerModUpdate::parse(item.json());
        if (mod) {
            list.push_back(mod.unwrap());
        }
        else {
            log::error("Unable to parse mod update from the server: {}", mod.unwrapErr());
        }
    }

    // Check for errors and return result
    if (payload.isError()) {
        return Err(payload.getError());
    }
    return Ok(list);
}

bool ServerModUpdate::hasUpdateForInstalledMod() const {
    if (auto mod = Loader::get()->getInstalledMod(this->id)) {
        return mod->getVersion() < this->version;
    }
    return false;
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
    root.has("repository").into(res.repository);
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
            version.metadata.setRepository(res.repository);
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

ModMetadata ServerModMetadata::latestVersion() const {
    return this->versions.front().metadata;
}

bool ServerModMetadata::hasUpdateForInstalledMod() const {
    if (auto mod = Loader::get()->getInstalledMod(this->id)) {
        return mod->getVersion() < this->latestVersion().getVersion();
    }
    return false;
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

ServerRequest<ServerModsList> server::getMods2(ModsQuery const& query, bool useCache) {
    if (useCache) {
        return getCache<getMods2>().get(query);
    }

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

    return req.send2("GET", getServerAPIBaseURL() + "/mods").map(
        [](web::WebResponse* response) -> Result<ServerModsList, ServerError> {
            if (response->ok()) {
                // Parse payload
                auto payload = parseServerPayload(*response);
                if (!payload) {
                    return Err(payload.unwrapErr());
                }
                // Parse response
                auto list = ServerModsList::parse(payload.unwrap());
                if (!list) {
                    return Err(ServerError(response->code(), "Unable to parse response: {}", list.unwrapErr()));
                }
                return Ok(list.unwrap());
            }
            else {
                // Treat a 404 as empty mods list
                if (response->code() == 404) {
                    return Ok(ServerModsList());
                }
                return Err(parseServerError(*response));
            }
        },
        [](web::WebProgress* progress) {
            return parseServerProgress(*progress, "Downloading mods");
        }
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

    return req.get(getServerAPIBaseURL() + "/mods")
        .then<ServerModsList, ServerError>([](auto result) -> Result<ServerModsList, ServerError> {
            if (result) {
                auto value = std::move(result).unwrap();

                // Store the code, since the value is moved afterwards
                auto code = value.code();

                // Parse payload
                auto payload = parseServerPayload(std::move(value));
                if (!payload) {
                    return Err(payload.unwrapErr());
                }
                // Parse response
                auto list = ServerModsList::parse(payload.unwrap());
                if (!list) {
                    return Err(ServerError(code, "Unable to parse response: {}", list.unwrapErr()));
                }
                return Ok(list.unwrap());
            }
            else {
                auto error = std::move(result).unwrapErr();
                // Treat a 404 as empty mods list
                if (error.code() == 404) {
                    return Ok(ServerModsList());
                }
                return Err(parseServerError(error));
            }
        })
        .progress<ServerProgress>([](auto prog) {
            return parseServerProgress(prog, "Downloading mods");
        });
}

ServerPromise<ServerModMetadata> server::getMod(std::string const& id) {
    auto req = web::WebRequest();
    req.userAgent(getServerUserAgent());
    return req.get(getServerAPIBaseURL() + "/mods/" + id)
        .then<ServerModMetadata, ServerError>([](auto result) -> Result<ServerModMetadata, ServerError> {
            if (result) {
                auto value = result.unwrap();

                // Store the code, since the value is moved afterwards
                auto code = value.code();

                // Parse payload
                auto payload = parseServerPayload(std::move(value));
                if (!payload) {
                    return Err(payload.unwrapErr());
                }
                // Parse response
                auto list = ServerModMetadata::parse(payload.unwrap());
                if (!list) {
                    return Err(ServerError(code, "Unable to parse response: {}", list.unwrapErr()));
                }
                return Ok(list.unwrap());
            }
            else {
                return Err(parseServerError(result.unwrapErr()));
            }
        })
        .progress<ServerProgress>([id](auto prog) {
            return parseServerProgress(prog, "Downloading metadata for " + id);
        });
}

ServerPromise<ByteVector> server::getModLogo(std::string const& id) {
    auto req = web::WebRequest();
    req.userAgent(getServerUserAgent());
    return req.get(getServerAPIBaseURL() + "/mods/" + id + "/logo")
        .then<ByteVector>([](auto response) {
            return response.data();
        })
        .expect<ServerError>([](auto error) {
            return parseServerError(error);
        })
        .progress<ServerProgress>([id](auto prog) {
            return parseServerProgress(prog, "Downloading logo for " + id);
        });
}

ServerPromise<std::unordered_set<std::string>> server::getTags(std::monostate) {
    auto req = web::WebRequest();
    req.userAgent(getServerUserAgent());
    return req.get(getServerAPIBaseURL() + "/tags")
        .then<std::unordered_set<std::string>, ServerError>([](auto response) -> Result<std::unordered_set<std::string>, ServerError> {
            if (response) {
                auto value = response.unwrap();

                // Store the code, since the value is moved afterwards
                auto code = value.code();
                
                // Parse payload
                auto payload = parseServerPayload(std::move(value));
                if (!payload) {
                    return Err(payload.unwrapErr());
                }
                matjson::Value json = payload.unwrap();
                if (!json.is_array()) {
                    return Err(ServerError(code, "Expected a string array"));
                }

                std::unordered_set<std::string> tags;
                for (auto item : json.as_array()) {
                    if (!item.is_string()) {
                        return Err(ServerError(code, "Expected a string array"));
                    }
                    tags.insert(item.as_string());
                }
                return Ok(tags);
            }
            else {
                return Err(parseServerError(response.unwrapErr()));
            }
        })
        .progress<ServerProgress>([](auto prog) {
            return parseServerProgress(prog, "Downloading valid tags");
        });
}

ServerPromise<std::vector<ServerModUpdate>> server::checkUpdates(std::vector<std::string> const& modIDs) {
    auto req = web::WebRequest();
    req.userAgent(getServerUserAgent());
    req.param("platform", GEODE_PLATFORM_SHORT_IDENTIFIER);
    if (modIDs.size()) {
        req.param("ids", ranges::join(modIDs, ";"));
    }
    return req.get(getServerAPIBaseURL() + "/mods/updates")
        .then<std::vector<ServerModUpdate>, ServerError>([](auto result) -> Result<std::vector<ServerModUpdate>, ServerError> {
            if (result) {
                auto value = result.unwrap();

                // Store the code, since the value is moved afterwards
                auto code = value.code();

                // Parse payload
                auto payload = parseServerPayload(std::move(value));
                if (!payload) {
                    return Err(payload.unwrapErr());
                }
                // Parse response
                auto list = ServerModUpdate::parseList(payload.unwrap());
                if (!list) {
                    return Err(ServerError(code, "Unable to parse response: {}", list.unwrapErr()));
                }
                return Ok(list.unwrap());
            }
            else {
                return Err(parseServerError(result.unwrapErr()));
            }
        })
        .progress<ServerProgress>([](auto prog) {
            return parseServerProgress(prog, "Checking updates for mods");
        });
}

void server::clearServerCaches2(bool clearGlobalCaches) {
    getCache<&getMods2>().clear();
    // getCache<&getMod>().clear();
    // getCache<&getModLogo>().clear();

    // Only clear global caches if explicitly requested
    if (clearGlobalCaches) {
        // getCache<&getTags>().clear();
        // getCache<&checkUpdates>().clear();
    }
}
