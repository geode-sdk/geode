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
    namespace impl_cache {
        template <class R, class Q>
        struct ExtractServerReqParams {
            using Result = R;
            using Query  = Q;
            ExtractServerReqParams(ServerPromise<R>(*)(Q const&)) {}
        };

        template <auto F>
        class ServerResultCache final {
        public:
            using Extract = decltype(ExtractServerReqParams(F));
            using Result  = Extract::Result;
            using Query   = Extract::Query;

        private:
            std::mutex m_cachedMutex;
            // The result and the query used for cached value
            std::optional<std::pair<Result, Query>> m_cached;

            ServerPromise<Result> fetch(Query&& query) {
                return ServerPromise<Result>([this, query = std::move(query)](auto resolve, auto reject, auto progress, auto cancelled) {
                    F(Query(query))
                    .then([this, resolve, query = std::move(query)](auto res) {
                        std::unique_lock lock(m_cachedMutex);
                        m_cached = { res, query };
                        lock.unlock();

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
            ServerPromise<Result> get(Query&& query) {
                std::unique_lock lock(m_cachedMutex);
                // Return cached value if there is one and the query matches
                if (m_cached && m_cached->second == query) {
                    auto cached = m_cached->first;
                    lock.unlock();
                    return ServerPromise<Result>([cached = std::move(cached)](auto resolve, auto) {
                        resolve(std::move(cached));
                    });
                }
                lock.unlock();
                return this->fetch(std::move(query));
            }

            ServerPromise<Result> refetch(Query&& query) {
                // Clear cache
                std::unique_lock lock(m_cachedMutex);
                m_cached.reset();
                lock.unlock();

                // Fetch new value
                return this->fetch(std::move(query));
            }
        };

        template <auto F>
        class ServerMultiResultCache final {
        public:
            using Extract = decltype(ExtractServerReqParams(F));
            using Result  = Extract::Result;
            using Query   = Extract::Query;

        private:
            std::mutex m_queriesMutex;
            std::map<Query, Result> m_queries;
        
            ServerPromise<Result> fetch(Query const& query) {
                return ServerPromise<Result>([this, query = Query(query)](auto resolve, auto reject, auto progress, auto cancelled) {
                    F(Query(query))
                    .then([this, resolve, query = std::move(query)](auto res) {
                        std::unique_lock lock(m_queriesMutex);
                        m_queries[std::move(query)] = res;
                        lock.unlock();

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
            ServerPromise<Result> get(Query const& query) {
                std::unique_lock lock(m_queriesMutex);
                // Return cached value if there is one and the query matches
                if (m_queries.contains(query)) {
                    auto cached = m_queries.at(query);
                    lock.unlock();
                    return ServerPromise<Result>([cached = std::move(cached)](auto resolve, auto) {
                        resolve(std::move(cached));
                    });
                }
                lock.unlock();
                return this->fetch(std::move(query));
            }

            ServerPromise<Result> refetch(Query const& query) {
                // Clear cache for this query only
                std::unique_lock lock (m_queriesMutex);
                m_queries.erase(query);
                lock.unlock();

                // Fetch new value
                return this->fetch(std::move(query));
            }

            // Clear all caches
            void invalidateAll() {
                std::unique_lock _(m_queriesMutex);
                m_queries.clear();
            }
        };
    }

    // todo: default shared caching for endpoints for all users
    // todo: (so it can be automatically cleared for example after leaving the geode layer)
    // template <auto F>
    // impl_cache::ServerResultCache<F> sharedCache() {}
}
