#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/Promise.hpp>
#include <Geode/utils/web2.hpp>

using namespace geode::prelude;

namespace server {
    struct ServerDateTime final {
        using Clock = std::chrono::system_clock;
        using Value = std::chrono::time_point<Clock>;

        Value value;

        std::string toAgoString() const;

        static Result<ServerDateTime> parse(std::string const& str);
    };

    struct ServerDeveloper {
        std::string username;
        std::string displayName;
    };

    struct ServerModVersion {
        ModMetadata metadata;
        std::string downloadURL;
        std::string hash;
        size_t downloadCount;

        static Result<ServerModVersion> parse(matjson::Value const& json);
    };

    struct ServerModMetadata {
        std::string id;
        bool featured;
        size_t downloadCount;
        std::vector<ServerDeveloper> developers;
        std::vector<ServerModVersion> versions;
        std::unordered_set<std::string> tags;
        std::optional<std::string> about;
        std::optional<std::string> changelog;
        std::optional<ServerDateTime> createdAt;
        std::optional<ServerDateTime> updatedAt;

        static Result<ServerModMetadata> parse(matjson::Value const& json);

        ModMetadata latestVersion() const;
        bool hasUpdateForInstalledMod() const;
    };

    struct ServerModsList {
        std::vector<ServerModMetadata> mods;
        size_t totalModCount = 0;

        static Result<ServerModsList> parse(matjson::Value const& json);
    };

    enum class ModsSort {
        Downloads,
        RecentlyUpdated,
        RecentlyPublished,
    };

    static const char* sortToString(ModsSort sorting);

    struct ModsQuery {
        std::optional<std::string> query;
        std::unordered_set<PlatformID> platforms = { GEODE_PLATFORM_TARGET };
        std::unordered_set<std::string> tags;
        std::optional<bool> featured;
        ModsSort sorting = ModsSort::Downloads;
        std::optional<std::string> developer;
        size_t page = 0;
        size_t pageSize = 10;
    };

    struct ServerError {
        int code;
        std::string details;

        ServerError() = default;

        template <class... Args>
        ServerError(
            int code,
            fmt::string_view format,
            Args&&... args
        ) : code(code), details(fmt::vformat(format, fmt::make_format_args(args...))) {}
    };
    template <class T>
    using ServerPromise = Promise<T, ServerError>;

    std::string getServerAPIBaseURL();
    std::string getServerUserAgent();
    ServerPromise<ServerModsList> getMods(ModsQuery const& query);
    ServerPromise<ServerModMetadata> getMod(std::string const& id);
    ServerPromise<ByteVector> getModLogo(std::string const& id);

    // Caching for server endpoints
    namespace detail {
        template <class R, class Q>
        struct ExtractServerReqParams {
            using Result = R;
            using Query  = Q;
            ExtractServerReqParams(ServerPromise<R>(*)(Q const&)) {}
        };
    }

    template <auto F>
    class ServerResultCache final {
    public:
        using Extract = decltype(detail::ExtractServerReqParams(F));
        using Result  = Extract::Result;
        using Query   = Extract::Query;
        using Cached  = std::variant<ServerPromise<Result>, Result>;

    private:
        class Cache final {
            std::mutex m_mutex;
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
            std::vector<std::pair<Query, Result>> m_values;

        public:
            std::optional<Result> get(Query const& query) {
                std::unique_lock _(m_mutex);
                // mfw no std::optional::map
                if (auto found = ranges::find(m_values, [&query](auto const& q) { return q.first == query; })) {
                    return found->second;
                }
                return std::nullopt;
            }
            void add(Query&& query, Result&& result) {
                std::unique_lock _(m_mutex);
                m_values.emplace_back(std::make_pair(std::move(query), std::move(result)));
            }
            void remove(Query const& query) {
                std::unique_lock _(m_mutex);
                ranges::remove(m_values, [&query](auto const& q) { return q.first == query; });
            }
            void shrink(size_t size) {
                std::unique_lock _(m_mutex);
                size_t removeCount = size < m_values.size() ? m_values.size() - size : 0;
                m_values.erase(m_values.begin(), m_values.begin() + removeCount);
            }
            void clear() {
                std::unique_lock _(m_mutex);
                m_values.clear();
            }
        };

        Cache m_cache;
        // todo: loader setting to customize this
        static constexpr size_t CACHE_SIZE_LIMIT = 20;
    
        ServerPromise<Result> fetch(Query const& query) {
            return ServerPromise<Result>([this, query = Query(query)](auto resolve, auto reject, auto progress, auto cancelled) {
                F(Query(query))
                .then([this, resolve, query = std::move(query)](auto res) {
                    m_cache.add(Query(query), Result(res));
                    resolve(res);
                })
                .expect([reject](auto err) {
                    reject(err);
                })
                .progress([progress](auto prog) {
                    progress(prog);
                })
                .link(cancelled);
            });
        }

    public:
        static ServerResultCache<F>& shared() {
            static auto inst = ServerResultCache<F>();
            return inst;
        }
    
        ServerPromise<Result> get(Query const& query) {
            // Return cached value if there is one and the query matches
            if (auto cached = m_cache.get(query)) {
                return ServerPromise<Result>([cached = std::move(cached).value()](auto resolve, auto) {
                    resolve(std::move(cached));
                });
            }
            // Shrink to fit size limit + 1 new item
            m_cache.shrink(CACHE_SIZE_LIMIT - 1);
            return this->fetch(std::move(query));
        }

        ServerPromise<Result> refetch(Query const& query) {
            // Clear cache for this query only
            m_cache.remove(query);

            // Fetch new value
            return this->fetch(std::move(query));
        }

        // Clear all caches
        void invalidateAll() {
            m_cache.clear();
        }
    };
}
