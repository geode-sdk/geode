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
    using ServerProgress = Promise<>::Progress;
    template <class T>
    using ServerPromise = Promise<T, ServerError, ServerProgress>;

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

    private:
        class Cache final {
            using Pending = ServerPromise<Result>;
            using CachedOrPending = std::variant<Pending, Result>;

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
            std::vector<std::pair<Query, CachedOrPending>> m_values;
            size_t m_sizeLimit = 20;

            template <class As>
            As* find(Query const& query) {
                auto it = std::find_if(m_values.begin(), m_values.end(), [query](auto const& q) {
                    return q.first == query;
                });
                if (it == m_values.end()) {
                    return nullptr;
                }
                if (auto as = std::get_if<As>(&it->second)) {
                    return as;
                }
                return nullptr;
            }

        public:
            std::optional<Result> get(Query const& query) {
                std::unique_lock _(m_mutex);
                if (auto v = this->template find<Result>(query)) {
                    return *v;
                }
                return std::nullopt;
            }
            ServerPromise<Result> pend(Query&& query, auto func) {
                std::unique_lock _(m_mutex);
                ServerPromise<Result> ret;
                if (auto prev = this->template find<Pending>(query)) {
                    return prev;
                }
                else {
                    ret = ServerPromise<Result>([this, func, query = Query(query)](auto resolve, auto reject, auto progress, auto cancelled) {
                        func(Query(query))
                        .then([this, resolve, query = std::move(query)](auto res) {
                            this->add(Query(query), Result(res));
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
                return ret;
            }
            void add(Query&& query, Result&& result) {
                std::unique_lock _(m_mutex);
                auto value = std::make_pair(std::move(query), std::move(result));
                // Shift and replace last element if we're at cache size limit
                if (m_values.size() >= m_sizeLimit) {
                    std::shift_left(m_values.begin(), m_values.end(), 1);
                    m_values.back() = std::move(value);
                }
                // Otherwise append at end
                else {
                    m_values.emplace_back(std::move(value));
                }
            }
            void remove(Query const& query) {
                std::unique_lock _(m_mutex);
                ranges::remove(m_values, [&query](auto const& q) { return q.first == query; });
            }
            void clear() {
                std::unique_lock _(m_mutex);
                m_values.clear();
            }
            // @warning also clears the cache
            void limit(size_t size) {
                std::unique_lock _(m_mutex);
                m_sizeLimit = size;
                m_values.clear();
            }
        };

        Cache m_cache;
    
        ServerPromise<Result> fetch(Query const& query) {
            return F(Query(query))
                .then([this, query = std::move(query)](auto res) {
                    m_cache.add(Query(query), Result(res));
                });
            // return m_cache.pend(Query(query));
        }

    public:
        static ServerResultCache<F>& shared() {
            static auto inst = ServerResultCache();
            static auto _ = listenForSettingChanges<int64_t>("server-cache-size-limit", +[](int64_t size) {
                ServerResultCache::shared().setSizeLimit(size);
            });
            return inst;
        }
    
        ServerPromise<Result> get(Query const& query) {
            // Return cached value if there is one and the query matches
            if (auto cached = m_cache.get(query)) {
                return ServerPromise<Result>([cached = std::move(cached).value()](auto resolve, auto) {
                    resolve(std::move(cached));
                });
            }
            return this->fetch(std::move(query));
        }
        ServerPromise<Result> refetch(Query const& query) {
            // Clear cache for this query only
            m_cache.remove(query);

            // Fetch new value
            return this->fetch(std::move(query));
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
}
