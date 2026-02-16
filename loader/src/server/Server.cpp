#include "Server.hpp"
#include <Geode/utils/JsonValidation.hpp>
#include <Geode/utils/ranges.hpp>
#include <chrono>
#include <fmt/core.h>
#include <loader/ModMetadataImpl.hpp>
#include <fmt/chrono.h>
#include <arc/sync/Mutex.hpp>
#include <loader/LoaderImpl.hpp>
#include "../internal/about.hpp"
#include "Geode/loader/Loader.hpp"

using namespace server;

#define GEODE_GD_VERSION_STR GEODE_STR(GEODE_GD_VERSION)

template <class K, class V>
    requires std::equality_comparable<K> && std::copy_constructible<K>
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
        auto it = std::find_if(m_values.begin(), m_values.end(), [&key](auto const& q) {
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

template <class F>
struct ExtractFun;

template <class V, class... Args>
struct ExtractFun<ServerFuture<V>(*)(Args...)> {
    using CacheKey = std::tuple<std::remove_cvref_t<Args>...>;
    using Value = V;

    template <class... CArgs>
    static CacheKey key(CArgs&&... args) {
        return std::make_tuple(std::forward<CArgs>(args)..., false);
    }
    template <class... CArgs>
    static ServerFuture<V> invoke(auto&& func, CArgs const&... args) {
        return func(args..., false);
    }
};

template <auto F>
class FunCache final {
public:
    using Extract  = ExtractFun<decltype(F)>;
    using CacheKey = typename Extract::CacheKey;
    using Value    = typename Extract::Value;

private:
    arc::Mutex<CacheMap<CacheKey, Value>> m_cache;

public:
    FunCache() = default;
    FunCache(FunCache const&) = delete;
    FunCache(FunCache&&) = delete;

    template <class... Args>
    arc::Future<Result<Value, ServerError>> get(Args&&... args) {
        ARC_FRAME();
        auto key = Extract::key(args...);

        auto cache = co_await m_cache.lock();
        if (auto v = cache->get(key)) {
            co_return Ok(*v);
        }
        auto f = ARC_CO_UNWRAP(co_await Extract::invoke(F, std::forward<Args>(args)...));
        cache->add(std::move(key), Value{f});
        co_return Ok(f);
    }

    template <class... Args>
    arc::Future<> remove(Args const&... args) {
        auto cache = co_await m_cache.lock();
        cache->remove(Extract::key(args...));
    }

    arc::Future<size_t> size() {
        auto cache = co_await m_cache.lock();
        co_return cache->size();
    }
    arc::Future<> limit(size_t size) {
        auto cache = co_await m_cache.lock();
        cache->limit(size);
    }
    arc::Future<> clear() {
        auto cache = co_await m_cache.lock();
        cache->clear();
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
    if (!json.isObject()) {
        return Err(ServerError(response.code(), "Expected object, got {}", jsonTypeToString(json.type())));
    }
    if (!json.contains("payload")) {
        return Err(ServerError(response.code(), "Object does not contain \"payload\" key - got {}", json.dump()));
    }
    return Ok(std::move(json["payload"]));
}

static ServerError parseServerError(web::WebResponse const& error) {
    // The server should return errors as `{ "error": "...", "payload": "" }`
    if (auto asJson = error.json()) {
        auto& json = asJson.unwrap();
        if (json.isObject() && json.contains("error") && json["error"].isString()) {
            return ServerError(
                error.code(),
                "{}", json["error"].asString().unwrapOr("Unknown (no error message)")
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

Result<ServerTag> ServerTag::parse(matjson::Value raw) {
    auto root = checkJson(std::move(raw), "ServerTag");
    auto res = ServerTag();

    root.needs("id").into(res.id);
    root.needs("name").into(res.name);
    root.needs("display_name").into(res.displayName);

    return root.ok(std::move(res));
}
Result<std::vector<ServerTag>> ServerTag::parseList(matjson::Value raw) {
    auto payload = checkJson(std::move(raw), "ServerTagsList");
    std::vector<ServerTag> list {};
    for (auto& item : payload.items()) {
        auto mod = ServerTag::parse(item.takeJson());
        if (mod) {
            list.push_back(std::move(mod).unwrap());
        }
        else {
            log::error("Unable to parse tag from the server: {}", mod.unwrapErr());
        }
    }
    return payload.ok(std::move(list));
}

Result<ServerDateTime> ServerDateTime::parse(std::string const& str) {
    #ifdef GEODE_IS_WINDOWS
    std::stringstream ss(str);
    std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> seconds;
    if (ss >> std::chrono::parse("%Y-%m-%dT%H:%M:%S%Z", seconds)) {
        return Ok(ServerDateTime {
            .value = seconds
        });
    }
    return Err("Invalid date time format '{}'", str);
    #else
    tm t;
    auto ptr = strptime(str.c_str(), "%Y-%m-%dT%H:%M:%SZ", &t);
    if (ptr != str.data() + str.size()) {
        return Err("Invalid date time format '{}'", str);
    }
    auto time = timegm(&t);
    return Ok(ServerDateTime {
        .value = std::chrono::system_clock::from_time_t(time)
    });
    #endif
}

Result<ServerModVersion> ServerModVersion::parse(matjson::Value raw) {
    auto root = checkJson(std::move(raw), "ServerModVersion");

    auto res = ServerModVersion();

    res.metadata.setGeodeVersion(root.needs("geode").get<VersionInfo>());

    // Verify target GD version
    auto gd_obj = root.needs("gd");
    std::string gd = "0.000";
    if (gd_obj.hasNullable(GEODE_PLATFORM_SHORT_IDENTIFIER)) {
        gd = gd_obj.hasNullable(GEODE_PLATFORM_SHORT_IDENTIFIER). get<std::string>();
    }

    if (gd != "*") {
        res.metadata.setGameVersion(gd);
    }

    // Get server info
    root.needs("download_link").into(res.downloadURL);
    root.needs("download_count").into(res.downloadCount);
    root.needs("hash").into(res.hash);

    // Get mod metadata info
    res.metadata.setID(root.needs("mod_id").get<std::string>());
    res.metadata.setName(root.needs("name").get<std::string>());
    res.metadata.setDescription(root.needs("description").get<std::string>());
    res.metadata.setVersion(root.needs("version").get<VersionInfo>());
    res.metadata.setIsAPI(root.needs("api").get<bool>());

    std::vector<ModMetadata::Dependency> dependencies {};
    for (auto& obj : root.hasNullable("dependencies").items()) {
        // todo: this should probably be generalized to use the same function as mod.json

        bool onThisPlatform = !obj.hasNullable("platforms");
        for (auto& plat : obj.hasNullable("platforms").items()) {
            if (PlatformID::coveredBy(plat.get<std::string>(), GEODE_PLATFORM_TARGET)) {
                onThisPlatform = true;
            }
        }
        if (!onThisPlatform) {
            continue;
        }

        ModMetadata::Dependency dependency;
        std::string modId;
        obj.needs("mod_id").mustBe<std::string>("a valid id", &ModMetadata::validateID).into(modId);
        dependency.setID(modId);
        ComparableVersionInfo version;
        obj.needs("version").into(version);
        dependency.setVersion(version);
        bool required;
        obj.hasNullable("required").into(required);
        dependency.setRequired(required);

        // Check if this dependency is installed, and if so assign the `mod` member to mark that
        auto mod = Loader::get()->getInstalledMod(dependency.getID());
        if (mod && dependency.getVersion().compare(mod->getVersion())) {
            dependency.setMod(mod);
        }

        dependencies.push_back(std::move(dependency));
    }
    res.metadata.setDependencies(std::move(dependencies));

    std::vector<ModMetadata::Incompatibility> incompatibilities {};
    for (auto& obj : root.hasNullable("incompatibilities").items()) {
        ModMetadata::Incompatibility incompatibility;
        bool breaking;
        obj.hasNullable("breaking").into(breaking);
        incompatibility.setBreaking(breaking);

        auto modIdValue = obj.needs("mod_id");
        std::string modId;

        // Do not validate if we have a supersede, maybe the old ID is invalid
        // todo: impl index-based superseding
        // if (incompatibility.getImportance() == ModMetadata::Incompatibility::Importance::Superseded) {
        //     modIdValue.into(modId);
        // } else {
            modIdValue.mustBe<std::string>("a valid id", &ModMetadata::validateID).into(modId);
        // }
        incompatibility.setID(modId);

        ComparableVersionInfo version;
        obj.needs("version").into(version);
        incompatibility.setVersion(version);

        // Check if this incompatibility is installed, and if so assign the `mod` member to mark that
        auto mod = Loader::get()->getInstalledMod(incompatibility.getID());
        if (mod && incompatibility.getVersion().compare(mod->getVersion())) {
            incompatibility.setMod(mod);
        }

        incompatibilities.push_back(std::move(incompatibility));
    }
    res.metadata.setIncompatibilities(std::move(incompatibilities));

    return root.ok(res);
}

Result<ServerModUpdate> ServerModUpdate::parse(matjson::Value raw) {
    auto root = checkJson(std::move(raw), "ServerModUpdate");
    auto res = ServerModUpdate();
    root.needs("id").into(res.id);
    root.needs("version").into(res.version);
    return root.ok(std::move(res));
}

Result<std::vector<ServerModUpdate>> ServerModUpdate::parseList(matjson::Value raw) {
    auto payload = checkJson(std::move(raw), "ServerModUpdateList");
    std::vector<ServerModUpdate> list {};
    for (auto& item : payload.items()) {
        auto mod = ServerModUpdate::parse(item.takeJson());
        if (mod) {
            list.push_back(std::move(mod).unwrap());
        }
        else {
            log::error("Unable to parse mod update from the server: {}", mod.unwrapErr());
        }
    }
    return payload.ok(std::move(list));
}

Mod* ServerModUpdate::hasUpdateForInstalledMod() const {
    if (auto mod = Loader::get()->getInstalledMod(this->id)) {
        return mod->getVersion() < this->version ? mod : nullptr;
    }
    return nullptr;
}

Result<ServerModDeprecation> ServerModDeprecation::parse(matjson::Value json) {
    auto root = checkJson(std::move(json), "ServerModDeprecation");
    auto res = ServerModDeprecation();
    root.needs("id").into(res.id);
    root.needs("by").into(res.by);
    root.needs("reason").into(res.reason);
    return root.ok(std::move(res));
}
Result<std::vector<ServerModDeprecation>> ServerModDeprecation::parseList(matjson::Value json) {
    auto payload = checkJson(std::move(json), "ServerModDeprecationList");
    std::vector<ServerModDeprecation> list {};
    for (auto& item : payload.items()) {
        auto mod = ServerModDeprecation::parse(item.takeJson());
        if (mod) {
            list.push_back(std::move(mod).unwrap());
        }
        else {
            log::error("Unable to parse mod deprecation from the server: {}", mod.unwrapErr());
        }
    }
    return payload.ok(std::move(list));
}

Mod* ServerModDeprecation::hasDeprecationForInstalledMod() const {
    return Loader::get()->getInstalledMod(this->id);
}

Result<ServerModUpdateAllCheck> ServerModUpdateAllCheck::parse(matjson::Value json) {
    // Old v4 format just returned updates as array
    if (json.isArray()) {
        return Ok(ServerModUpdateAllCheck {
            .updates = GEODE_UNWRAP(ServerModUpdate::parseList(std::move(json))),
            .deprecations = {},
        });
    }
    auto root = checkJson(std::move(json), "ServerModUpdateAllCheck");
    auto updates = GEODE_UNWRAP(ServerModUpdate::parseList(root.needs("updates").takeJson()));
    auto deprecations = GEODE_UNWRAP(ServerModDeprecation::parseList(root.needs("deprecations").takeJson()));
    return root.ok(ServerModUpdateAllCheck {
        .updates = updates,
        .deprecations = deprecations,
    });
}

Result<ServerModLinks> ServerModLinks::parse(matjson::Value raw) {
    auto payload = checkJson(std::move(raw), "ServerModLinks");
    auto res = ServerModLinks();

    payload.hasNullable("community").into(res.community);
    payload.hasNullable("homepage").into(res.homepage);
    payload.hasNullable("source").into(res.source);

    return payload.ok(std::move(res));
}

Result<ServerModMetadata> ServerModMetadata::parse(matjson::Value raw) {
    auto root = checkJson(std::move(raw), "ServerModMetadata");

    auto res = ServerModMetadata();
    root.needs("id").into(res.id);
    root.needs("featured").into(res.featured);
    root.needs("download_count").into(res.downloadCount);
    root.hasNullable("about").into(res.about);
    root.hasNullable("changelog").into(res.changelog);
    root.hasNullable("repository").into(res.repository);
    if (root.has("created_at")) {
        GEODE_UNWRAP_INTO(res.createdAt, ServerDateTime::parse(root.has("created_at").get<std::string>()));
    }
    if (root.has("updated_at")) {
        GEODE_UNWRAP_INTO(res.updatedAt, ServerDateTime::parse(root.has("updated_at").get<std::string>()));
    }

    std::vector<std::string> developerNames;
    for (auto& obj : root.needs("developers").items()) {
        auto dev = ServerDeveloper();
        obj.needs("username").into(dev.username);
        obj.needs("display_name").into(dev.displayName);
        obj.needs("is_owner").into(dev.isOwner);
        developerNames.push_back(dev.displayName);
        res.developers.push_back(std::move(dev));
    }
    for (auto& item : root.needs("versions").items()) {
        auto versionRes = ServerModVersion::parse(item.takeJson());
        if (versionRes) {
            auto version = std::move(versionRes).unwrap();
            version.metadata.setDetails(res.about);
            version.metadata.setChangelog(res.changelog);
            version.metadata.setDevelopers(developerNames);
            version.metadata.setRepository(res.repository);
            if (root.hasNullable("links")) {
                auto linkRes = ServerModLinks::parse(root.hasNullable("links").json());
                if (linkRes) {
                    auto links = std::move(linkRes).unwrap();
                    version.metadata.getLinksMut().getImpl()->m_community = std::move(links.community);
                    version.metadata.getLinksMut().getImpl()->m_homepage = std::move(links.homepage);
                    if (links.source.has_value()) version.metadata.setRepository(std::move(links.source));
                }
            }
            res.versions.push_back(std::move(version));
        }
        else {
            log::error("Unable to parse mod '{}' version from the server: {}", res.id, versionRes.unwrapErr());
        }
    }

    // Ensure there's at least one valid version
    if (res.versions.empty()) {
        return Err("Mod '{}' has no (valid) versions", res.id);
    }

    for (auto& item : root.hasNullable("tags").items()) {
        res.tags.insert(item.get<std::string>());
    }

    root.needs("download_count").into(res.downloadCount);

    return root.ok(std::move(res));
}

std::string ServerModMetadata::formatDevelopersToString() const {
    std::optional<ServerDeveloper> owner = ranges::find(developers, [] (auto& item) {
        return item.isOwner;
    });
    switch (developers.size()) {
        case 0: return "Unknown"; break;
        case 1: return developers.front().displayName; break;
        case 2: return developers.front().displayName + " & " + developers.back().displayName; break;
        default: {
            if (owner) {
                return fmt::format("{} + {} More", owner->displayName, developers.size() - 1);
            } else {
                return fmt::format("{} + {} More", developers.front().displayName, developers.size() - 1);
            }
        } break;
    }
}

Result<ServerModsList> ServerModsList::parse(matjson::Value raw) {
    auto payload = checkJson(std::move(raw), "ServerModsList");

    auto list = ServerModsList();
    for (auto& item : payload.needs("data").items()) {
        auto mod = ServerModMetadata::parse(item.takeJson());
        if (mod) {
            list.mods.push_back(std::move(mod).unwrap());
        }
        else {
            log::error("Unable to parse mod from the server: {}", mod.unwrapErr());
        }
    }
    payload.needs("count").into(list.totalModCount);

    return payload.ok(std::move(list));
}

Result<ServerLoaderVersion> ServerLoaderVersion::parse(matjson::Value raw) {
    auto root = checkJson(std::move(raw), "ServerLoaderVersion");

    auto res = ServerLoaderVersion();
    root.needs("version").into(res.version);
    root.needs("tag").into(res.tag);
    root.needs("commit_hash").into(res.commitHash);

    auto gd_obj = root.needs("gd");
    gd_obj.needs(GEODE_PLATFORM_SHORT_IDENTIFIER).into(res.gameVersion);

    return root.ok(std::move(res));
}

ModMetadata const& ServerModMetadata::latestVersion() const {
    return this->versions.front().metadata;
}

Mod* ServerModMetadata::hasUpdateForInstalledMod() const {
    if (auto mod = Loader::get()->getInstalledMod(this->id)) {
        return mod->getVersion() < this->latestVersion().getVersion() ? mod : nullptr;
    }
    return nullptr;
}

std::string server::getServerAPIBaseURL() {
    return "https://api.geode-sdk.org/v1";
}

template <class... Args>
std::string formatServerURL(fmt::format_string<Args...> fmt, Args&&... args) {
    return getServerAPIBaseURL() + fmt::format(fmt, std::forward<Args>(args)...);
}

std::string server::getServerUserAgent() {
    // no need to compute this more than once
    static const auto value = [] {
        // TODO: is this enough info? is it too much?
        return fmt::format("Geode Loader (ver={};commit={};platform={};gd={})",
            Loader::get()->getVersion().toNonVString(),
            about::getLoaderCommitHash(),
            GEODE_PLATFORM_SHORT_IDENTIFIER,
            LoaderImpl::get()->getGameVersion()
        );
    }();
    return value;
}

ServerFuture<ServerModsList> server::getMods(ModsQuery query, bool useCache) {
    ARC_FRAME();
    if (useCache) {
        co_return co_await getCache<getMods>().get(std::move(query));
    }

    auto req = web::WebRequest();
    req.userAgent(getServerUserAgent());

    // Add search params
    if (query.query) {
        req.param("query", *query.query);
    }

    req.param("gd", GEODE_GD_VERSION_STR);
    req.param("geode", Loader::get()->getVersion().toNonVString());

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

    auto response = co_await req.get(formatServerURL("/mods"));

    if (response.ok()) {
        // Parse payload
        auto payload = parseServerPayload(response);
        if (!payload) {
            co_return Err(std::move(payload).unwrapErr());
        }
        // Parse response
        auto list = ServerModsList::parse(std::move(payload).unwrap());
        if (!list) {
            co_return Err(ServerError(response.code(), "Unable to parse response: {}", list.unwrapErr()));
        }
        co_return Ok(std::move(list).unwrap());
    }
    // Treat a 404 as empty mods list
    if (response.code() == 404) {
        co_return Ok(ServerModsList());
    }
    co_return Err(parseServerError(response));
}

ServerFuture<ServerModMetadata> server::getMod(std::string id, bool useCache) {
    ARC_FRAME();
    if (useCache) {
        co_return co_await getCache<getMod>().get(std::move(id));
    }

    auto req = web::WebRequest();
    req.userAgent(getServerUserAgent());
    auto response = co_await req.get(formatServerURL("/mods/{}", id));

    if (response.ok()) {
        // Parse payload
        auto payload = parseServerPayload(response);
        if (!payload) {
            co_return Err(std::move(payload).unwrapErr());
        }
        // Parse response
        auto list = ServerModMetadata::parse(std::move(payload).unwrap());
        if (!list) {
            co_return Err(ServerError(response.code(), "Unable to parse response: {}", list.unwrapErr()));
        }
        co_return Ok(std::move(list).unwrap());
    }

    co_return Err(parseServerError(response));
}

ServerFuture<ServerModVersion> server::getModVersion(std::string id, ModVersion version, bool useCache) {
    ARC_FRAME();
    if (useCache) {
        auto& cache = getCache<getModVersion>();

        auto cachedRequest = co_await cache.get(std::move(id), std::move(version));
        co_return cachedRequest;

        // TODO v5: is this needed

        // // if mod installation was cancelled, remove it from cache and fetch again
        // if (cachedRequest.isCancelled()) {
        //     cache.remove(id, version);
        //     return cache.get(id, version);
        // } else {
        //     return cachedRequest;
        // }
    }

    auto req = web::WebRequest();
    req.userAgent(getServerUserAgent());

    std::string versionURL;
    std::visit(makeVisitor {
        [&](ModVersionLatest const&) {
            versionURL = "latest";
        },
        [&](ModVersionMajor const& ver) {
            versionURL = "latest";
            req.param("major", std::to_string(ver.major));
        },
        [&](ModVersionSpecific const& ver) {
            versionURL = ver.toNonVString();
        },
    }, version);

    auto response = co_await req.get(
        formatServerURL(
            "/mods/{}/versions/{}?gd={}&platforms={}",
            id,
            versionURL,
            Loader::get()->getGameVersion(),
            GEODE_PLATFORM_SHORT_IDENTIFIER
    ));

    if (response.ok()) {
        // Parse payload
        auto payload = parseServerPayload(response);
        if (!payload) {
            co_return Err(payload.unwrapErr());
        }
        // Parse response
        auto list = ServerModVersion::parse(std::move(payload).unwrap());
        if (!list) {
            co_return Err(ServerError(response.code(), "Unable to parse response: {}", list.unwrapErr()));
        }
        co_return Ok(std::move(list).unwrap());
    }
    co_return Err(parseServerError(response));
}

ServerFuture<ByteVector> server::getModLogo(std::string id, bool useCache) {
    ARC_FRAME();
    if (useCache) {
        co_return co_await getCache<getModLogo>().get(std::move(id));
    }

    auto req = web::WebRequest();
    req.userAgent(getServerUserAgent());
    auto response = co_await req.get(formatServerURL("/mods/{}/logo", id));

    if (response.ok()) {
        co_return Ok(std::move(response).data());
    }
    co_return Err(parseServerError(response));
}

ServerFuture<std::vector<ServerTag>> server::getTags(bool useCache) {
    ARC_FRAME();
    if (useCache) {
        co_return co_await getCache<getTags>().get();
    }
    auto req = web::WebRequest();
    req.userAgent(getServerUserAgent());
    auto response = co_await req.get(formatServerURL("/detailed-tags"));

    if (response.ok()) {
        // Parse payload
        auto payload = parseServerPayload(response);
        if (!payload) {
            co_return Err(std::move(payload).unwrapErr());
        }
        auto list = ServerTag::parseList(std::move(payload).unwrap());
        if (!list) {
            co_return Err(ServerError(response.code(), "Unable to parse response: {}", list.unwrapErr()));
        }
        co_return Ok(std::move(list).unwrap());
    }
    co_return Err(parseServerError(response));
}

ServerFuture<ServerModUpdateOneCheck> server::checkUpdates(Mod const* mod) {
    ARC_FRAME();
    auto all = ARC_CO_UNWRAP(co_await checkAllUpdates());
    auto result = ServerModUpdateOneCheck {};
    for (auto&& update : all.updates) {
        if (update.id == mod->getID() && update.version > mod->getVersion()) {
            result.update.emplace(std::move(update));
        }
    }
    for (auto&& dep : all.deprecations) {
        if (dep.id == mod->getID()) {
            result.deprecation.emplace(std::move(dep));
        }
    }
    co_return Ok(std::move(result));
}

ServerFuture<ServerModUpdateAllCheck> server::batchedCheckUpdates(std::vector<std::string> const& batch) {
    ARC_FRAME();
    auto req = web::WebRequest();
    req.userAgent(getServerUserAgent());
    req.param("platform", GEODE_PLATFORM_SHORT_IDENTIFIER);
    req.param("gd", GEODE_GD_VERSION_STR);
    req.param("geode", Loader::get()->getVersion().toNonVString());

    req.param("ids", ranges::join(batch, ";"));
    auto response = co_await req.get(formatServerURL("/mods/updates"));

    if (response.ok()) {
        // Parse payload
        auto payload = parseServerPayload(response);
        if (!payload) {
            co_return Err(payload.unwrapErr());
        }
        // Parse response
        auto list = ServerModUpdateAllCheck::parse(std::move(payload).unwrap());
        if (!list) {
            co_return Err(ServerError(response.code(), "Unable to parse response: {}", list.unwrapErr()));
        }
        co_return Ok(std::move(list).unwrap());
    }
    co_return Err(parseServerError(response));
}

ServerFuture<ServerModUpdateAllCheck> server::checkAllUpdates(bool useCache) {
    ARC_FRAME();
    if (useCache) {
        co_return co_await getCache<checkAllUpdates>().get();
    }

    auto modIDs = ranges::map<std::vector<std::string>>(
        Loader::get()->getAllMods(),
        [](auto mod) { return mod->getID(); }
    );

    // if there's no mods, the request would just be empty anyways
    if (modIDs.empty()) {
        // you would think it could infer like literally anything
        co_return Ok(ServerModUpdateAllCheck {});
    }

    std::vector<std::vector<std::string>> modBatches;
    auto modCount = modIDs.size();
    std::size_t maxMods = 200u; // this affects 0.03% of users

    if (modCount <= maxMods) {
        // no tricks needed
        co_return co_await batchedCheckUpdates(modIDs);
    }

    // even out the mod count, so a request with 230 mods sends two 115 mod requests
    auto batchCount = modCount / maxMods + 1;
    auto maxBatchSize = modCount / batchCount + 1;

    for (std::size_t i = 0u; i < modCount; i += maxBatchSize) {
        auto end = std::min(modCount, i + maxBatchSize);
        modBatches.emplace_back(modIDs.begin() + i, modIDs.begin() + end);
    }

    // chain requests to avoid doing too many large requests at once
    ServerModUpdateAllCheck accum;
    for (auto& batch : modBatches) {
        auto serverValues = ARC_CO_UNWRAP(co_await batchedCheckUpdates(batch));
        accum.updates.reserve(accum.updates.size() + serverValues.updates.size());
        accum.updates.insert(accum.updates.end(), serverValues.updates.begin(), serverValues.updates.end());
        accum.deprecations.reserve(accum.deprecations.size() + serverValues.deprecations.size());
        accum.deprecations.insert(accum.deprecations.end(), serverValues.deprecations.begin(), serverValues.deprecations.end());
    }

    co_return Ok(std::move(accum));
}

ServerFuture<ServerLoaderVersion> server::getLoaderVersion(std::string tag, bool useCache) {
    ARC_FRAME();
    if (useCache) {
        co_return co_await getCache<getLoaderVersion>().get(tag);
    }

    auto req = web::WebRequest();
    req.userAgent(getServerUserAgent());
    auto response = co_await req.get(formatServerURL("/loader/versions/{}", tag));

    if (response.ok()) {
        // Parse payload
        auto payload = parseServerPayload(response);
        if (!payload) {
            co_return Err(payload.unwrapErr());
        }
        auto ver = ServerLoaderVersion::parse(std::move(payload).unwrap());
        if (!ver) {
            co_return Err(ServerError(response.code(), "Unable to parse response: {}", ver.unwrapErr()));
        }
        co_return Ok(std::move(ver).unwrap());
    }
    co_return Err(parseServerError(response));
}

ServerFuture<ServerLoaderVersion> server::getLatestLoaderVersion(bool useCache) {
    ARC_FRAME();
    if (useCache) {
        co_return co_await getCache<getLatestLoaderVersion>().get();
    }

    auto req = web::WebRequest();
    req.userAgent(getServerUserAgent());
    auto response = co_await req.get(
        formatServerURL("/loader/versions/latest?gd={}&platform={}",
            Loader::get()->getGameVersion(),
            GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH
        )
    );

    if (response.ok()) {
        // Parse payload
        auto payload = parseServerPayload(response);
        if (!payload) {
            co_return Err(std::move(payload).unwrapErr());
        }
        auto ver = ServerLoaderVersion::parse(std::move(payload).unwrap());
        if (!ver) {
            co_return Err(ServerError(response.code(), "Unable to parse response: {}", ver.unwrapErr()));
        }
        co_return Ok(std::move(ver).unwrap());
    }
    co_return Err(parseServerError(response));
}

void server::clearServerCaches(bool clearGlobalCaches) {
    async::runtime().spawn([clearGlobalCaches] -> arc::Future<> {
        co_await getCache<&getMods>().clear();
        co_await getCache<&getMod>().clear();
        co_await getCache<&getModLogo>().clear();

        // Only clear global caches if explicitly requested
        if (clearGlobalCaches) {
            co_await getCache<&getTags>().clear();
            co_await getCache<&checkAllUpdates>().clear();
        }
    });
}

$on_mod(Loaded) {
    listenForSettingChanges<int64_t>("server-cache-size-limit", +[](int64_t size) {
        async::runtime().spawn([size] -> arc::Future<> {
            co_await getCache<&server::getMods>().limit(size);
            co_await getCache<&server::getMod>().limit(size);
            co_await getCache<&server::getModLogo>().limit(size);
            co_await getCache<&server::getTags>().limit(size);
            co_await getCache<&server::checkAllUpdates>().limit(size);
        });
    });
}
