#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/Promise.hpp>
#include <Geode/utils/web2.hpp>
#include <Geode/loader/SettingEvent.hpp>

using namespace geode::prelude;

namespace server {
    struct ServerDateTime final {
        using Clock = std::chrono::system_clock;
        using Value = std::chrono::time_point<Clock>;

        Value value;

        std::string toAgoString() const;

        static Result<ServerDateTime> parse(std::string const& str);
    };

    struct ServerDeveloper final {
        std::string username;
        std::string displayName;
    };

    struct ServerModVersion final {
        ModMetadata metadata;
        std::string downloadURL;
        std::string hash;
        size_t downloadCount;

        static Result<ServerModVersion> parse(matjson::Value const& json);
    };

    struct ServerModUpdate final {
        std::string id;
        VersionInfo version;

        static Result<ServerModUpdate> parse(matjson::Value const& json);
        static Result<std::vector<ServerModUpdate>> parseList(matjson::Value const& json);
        
        bool hasUpdateForInstalledMod() const;
    };

    struct ServerModMetadata final {
        std::string id;
        bool featured;
        size_t downloadCount;
        std::vector<ServerDeveloper> developers;
        std::vector<ServerModVersion> versions;
        std::unordered_set<std::string> tags;
        std::optional<std::string> about;
        std::optional<std::string> changelog;
        std::optional<std::string> repository;
        std::optional<ServerDateTime> createdAt;
        std::optional<ServerDateTime> updatedAt;

        static Result<ServerModMetadata> parse(matjson::Value const& json);

        ModMetadata latestVersion() const;
        bool hasUpdateForInstalledMod() const;
    };

    struct ServerModsList final {
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

    struct ModsQuery final {
        std::optional<std::string> query;
        std::unordered_set<PlatformID> platforms = { GEODE_PLATFORM_TARGET };
        std::unordered_set<std::string> tags;
        std::optional<bool> featured;
        ModsSort sorting = ModsSort::Downloads;
        std::optional<std::string> developer;
        size_t page = 0;
        size_t pageSize = 10;
        
        bool operator==(ModsQuery const& other) const = default;
    };

    struct ServerError final {
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
    using ServerProgress = Promise<>::Progress;
    template <class T>
    using ServerPromise = Promise<T, ServerError, ServerProgress>;

    template <class T>
    using ServerRequest = Task<Result<T, ServerError>, ServerProgress>;

    std::string getServerAPIBaseURL();
    std::string getServerUserAgent();
    ServerRequest<ServerModsList> getMods2(ModsQuery const& query, bool useCache = true);
    ServerPromise<ServerModsList> getMods(ModsQuery const& query);
    ServerPromise<ServerModMetadata> getMod(std::string const& id);
    ServerPromise<ByteVector> getModLogo(std::string const& id);
    ServerPromise<std::unordered_set<std::string>> getTags(std::monostate = std::monostate());
    ServerPromise<std::vector<ServerModUpdate>> checkUpdates(std::vector<std::string> const& modIDs);

    // ^^ Note: Any funcs with `std::monostate` parameter is because ServerResultCache expects a single query arg

    // Caching for server endpoints
    namespace detail {
        template <class R, class Q>
        struct ExtractServerReqParams final {
            using Result = R;
            using Query  = Q;
            ExtractServerReqParams(ServerPromise<R>(*)(Q)) {}
            ExtractServerReqParams(ServerPromise<R>(*)(Q const&)) {}
        };

        static std::optional<ServerModUpdate> mapModUpdateList(
            std::string const& modID,
            std::vector<ServerModUpdate>&& result
        ) {
            return ranges::find(result, [modID](auto v) { return v.id == modID; });
        }

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
            size_t limit() const {
                return m_sizeLimit;
            }
        };

        template <class Query, class Result>
        class DefaultCache final {
            std::mutex m_mutex;
            CacheMap<Query, ServerPromise<Result>> m_cache;

        public:
            ServerPromise<Result> get(auto func, Query const& query) {
                std::unique_lock _(m_mutex);

                // If there is already a Promise with this queue (pending or 
                // resolved) just return a copy of that to the caller so they 
                // can listen to it
                if (auto old = m_cache.get(query)) {
                    return *old;
                }
                
                // Create the Promise
                auto promise = func(Query(query));

                // Store the created Promise in the cache
                m_cache.add(Query(query), ServerPromise<Result>(promise));

                // Give a copy of the created Promise to the caller so they 
                // can listen to it
                return promise;
            }
            void clear(Query const& query) {
                std::unique_lock _(m_mutex);
                m_cache.remove(query);
            }
            void clear() {
                std::unique_lock _(m_mutex);
                m_cache.clear();
            }
            // @warning also clears the cache
            void limit(size_t size) {
                std::unique_lock _(m_mutex);
                m_cache.limit(size);
            }
        };

        class ModUpdatesCache final {
        public:
            using UpdatePromise = ServerPromise<std::optional<ServerModUpdate>>;

        private:
            std::mutex m_mutex;
            CacheMap<std::string, UpdatePromise> m_cache;

        public:
            ServerPromise<std::vector<ServerModUpdate>> get(auto, std::vector<std::string> const& modIDs) {
                std::unique_lock _(m_mutex);

                // Limit cache size to installed mods count
                if (m_cache.limit() != Loader::get()->getAllMods().size()) {
                    m_cache.limit(Loader::get()->getAllMods().size());
                }

                std::vector<UpdatePromise> promises;
                std::vector<std::string> idsNotFoundInCache {};
                for (auto const& modID : modIDs) {
                    if (auto old = m_cache.get(modID)) {
                        promises.push_back(*old);
                    }
                    else {
                        idsNotFoundInCache.push_back(modID);
                    }
                }

                if (idsNotFoundInCache.size()) {
                    // Check updates for all IDs not hit in cache
                    auto promise = checkUpdates(idsNotFoundInCache);
                    for (auto modID : std::move(idsNotFoundInCache)) {
                        auto individual = promise.then<std::optional<ServerModUpdate>>([modID](auto result) {
                            return detail::mapModUpdateList(modID, std::move(result));
                        });
                        m_cache.add(std::move(modID), UpdatePromise(individual));
                        promises.push_back(individual);
                    }
                }

                // Wait for all the promises to resolve
                return ServerPromise<std::vector<ServerModUpdate>>([promises = std::move(promises)](auto resolve, auto) {
                    struct Data final {
                        std::vector<ServerModUpdate> results;
                        size_t waitingFor;
                    };
                    std::shared_ptr<Data> data = std::make_shared<Data>();
                    data->waitingFor = promises.size();
                    for (auto promise : promises) {
                        promise.then([data, resolve](auto update) {
                            if (data->waitingFor > 0) {
                                if (update) {
                                    data->results.emplace_back(std::move(std::move(update).value()));
                                }
                                data->waitingFor -= 1;
                                if (data->waitingFor == 0) {
                                    resolve(std::move(data->results));
                                }
                            }
                        }).expect([data, resolve](auto) {
                            if (data->waitingFor > 0) {
                                data->waitingFor -= 1;
                                if (data->waitingFor == 0) {
                                    resolve(std::move(data->results));
                                }
                            }
                        }).cancelled([data, resolve] {
                            if (data->waitingFor > 0) {
                                data->waitingFor -= 1;
                                if (data->waitingFor == 0) {
                                    resolve(std::move(data->results));
                                }
                            }
                        });
                    }
                });
            }
            void clear(std::vector<std::string> const& modIDs) {
                std::unique_lock _(m_mutex);
                for (auto const& modID : modIDs) {
                    m_cache.remove(modID);
                }
            }
            void clear() {
                std::unique_lock _(m_mutex);
                m_cache.clear();
            }
            void limit(size_t) {
                // cache limit is automatically and always set to installed mods count
            }
        };
    
        template <auto F>
        struct CacheFor final {
            using Extract = decltype(detail::ExtractServerReqParams(F));
            using Result  = typename Extract::Result;
            using Query   = typename Extract::Query;
            using Cache   = DefaultCache<Query, Result>;
        };

        template <>
        struct CacheFor<&checkUpdates> {
            using Cache = ModUpdatesCache;
        };
    }

    template <auto F>
    class ServerResultCache final {
    public:
        using Extract = decltype(detail::ExtractServerReqParams(F));
        using Result  = typename Extract::Result;
        using Query   = typename Extract::Query;
        using Cache   = typename detail::CacheFor<F>::Cache;

    private:
        Cache m_cache;
    
    public:
        static ServerResultCache& shared() {
            static auto inst = ServerResultCache();
            static auto _ = listenForSettingChanges<int64_t>("server-cache-size-limit", +[](int64_t size) {
                ServerResultCache::shared().setSizeLimit(size);
            });
            return inst;
        }
        
        ServerPromise<Result> get() requires std::is_same_v<Query, std::monostate> {
            return m_cache.get(F, Query());
        }
        ServerPromise<Result> get(Query const& query) requires (!std::is_same_v<Query, std::monostate>) {
            return m_cache.get(F, query);
        }
        ServerPromise<Result> refetch(Query const& query) {
            // Clear cache for this query only
            m_cache.clear(query);
            // Fetch new value
            return m_cache.get(F, std::move(query));
        }
        // Invalidate the whole cache
        void invalidateAll() {
            m_cache.clear();
        }
        // Limit the size of the cache
        void setSizeLimit(size_t size) {
            m_cache.limit(size);
        }
    };

    // Clear all shared server endpoint caches
    static void clearServerCaches(bool clearGlobalCaches = false) {
        ServerResultCache<&getMods>::shared().invalidateAll();
        ServerResultCache<&getMod>::shared().invalidateAll();
        ServerResultCache<&getModLogo>::shared().invalidateAll();

        // Only clear global caches if explicitly requested
        if (clearGlobalCaches) {
            ServerResultCache<&getTags>::shared().invalidateAll();
            ServerResultCache<&checkUpdates>::shared().invalidateAll();
        }
    }

    void clearServerCaches2(bool clearGlobalCaches = false);
}
