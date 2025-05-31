#pragma once

#include <Geode/utils/cocos.hpp>
#include <Geode/utils/string.hpp>
#include <server/Server.hpp>
#include "../list/ModItem.hpp"

using namespace geode::prelude;

class ModListSource;

struct InvalidateCacheEvent : public Event {
    ModListSource* source;
    InvalidateCacheEvent(ModListSource* src);
};

class InvalidateCacheFilter : public EventFilter<InvalidateCacheEvent> {
protected:
    ModListSource* m_source;

public:
    using Callback = void(InvalidateCacheEvent*);

    ListenerResult handle(std::function<Callback> fn, InvalidateCacheEvent* event);

    InvalidateCacheFilter() = default;
    InvalidateCacheFilter(ModListSource* src);
};

// Handles loading the entries for the mods list
class ModListSource {
public:
    struct LoadPageError {
        std::string message;
        std::optional<std::string> details;

        LoadPageError() = default;
        LoadPageError(std::string const& msg) : message(msg) {}
        LoadPageError(auto msg, auto details) : message(msg), details(details) {}
    };

    using Page = std::vector<Ref<ModItem>>;
    using PageLoadTask = Task<Result<Page, LoadPageError>, std::optional<uint8_t>>;

    struct ProvidedMods {
        std::vector<ModSource> mods;
        size_t totalModCount;
    };
    using ProviderTask = Task<Result<ProvidedMods, LoadPageError>, std::optional<uint8_t>>;

protected:
    std::unordered_map<size_t, Page> m_cachedPages;
    std::optional<size_t> m_cachedItemCount;
    size_t m_pageSize = 10;

    virtual void resetQuery() = 0;
    virtual ProviderTask fetchPage(size_t page, bool forceUpdate) = 0;
    virtual void setSearchQuery(std::string const& query) = 0;

    ModListSource();

public:
    ModListSource(ModListSource const&) = delete;
    ModListSource(ModListSource&&) = delete;

    // Reset all filters & cache
    void reset();
    void clearCache();
    void search(std::string const& query);
    virtual bool isDefaultQuery() const = 0;

    virtual std::unordered_set<std::string> getModTags() const = 0;
    virtual void setModTags(std::unordered_set<std::string> const& tags) = 0;

    // Load page, uses cache if possible unless `forceUpdate` is true
    PageLoadTask loadPage(size_t page, bool forceUpdate = false);
    std::optional<size_t> getPageCount() const;
    std::optional<size_t> getItemCount() const;
    void setPageSize(size_t size);

    virtual bool isLocalModsOnly() const = 0;

    static void clearAllCaches();
};

template <class T>
class InvalidateQueryAfter final {
private:
    ModListSource* m_source;
    T& m_ref;

public:
    InvalidateQueryAfter(T& ref, ModListSource* source) : m_ref(ref), m_source(source) {}
    ~InvalidateQueryAfter() {
        m_source->clearCache();
    }
    T* operator->() const {
        return &m_ref;
    }
};

struct LocalModsQueryBase {
    std::optional<std::string> query;
    std::unordered_set<std::string> tags = {};
    size_t page = 0;
    size_t pageSize = 10;
    bool isDefault() const;
};

enum class InstalledModListType {
    All,
    OnlyUpdates,
    OnlyErrors,
    OnlyOutdated,
};
struct InstalledModsQuery final : public LocalModsQueryBase {
    InstalledModListType type = InstalledModListType::All;
    std::optional<bool> enabledOnly;
    std::optional<bool> enabledFirst;
    bool preCheck(ModSource const& src) const;
    bool queryCheck(ModSource const& src, double& weighted) const;
    bool isDefault() const;
    matjson::Value dumpFilters() const;
};

class InstalledModListSource : public ModListSource {
protected:
    InstalledModListType m_type;
    InstalledModsQuery m_query;

    void resetQuery() override;
    ProviderTask fetchPage(size_t page, bool forceUpdate) override;
    void setSearchQuery(std::string const& query) override;

    InstalledModListSource(InstalledModListType type);

public:
    static InstalledModListSource* get(InstalledModListType type);

    std::unordered_set<std::string> getModTags() const override;
    void setModTags(std::unordered_set<std::string> const& tags) override;

    InstalledModsQuery const& getQuery() const;
    InvalidateQueryAfter<InstalledModsQuery> getQueryMut();
    bool isDefaultQuery() const override;

    bool isLocalModsOnly() const override;
};

enum class ServerModListType {
    Download,
    Featured,
    Trending,
    Recent,
};

class ServerModListSource : public ModListSource {
protected:
    ServerModListType m_type;
    server::ModsQuery m_query;

    void resetQuery() override;
    ProviderTask fetchPage(size_t page, bool forceUpdate) override;
    void setSearchQuery(std::string const& query) override;

    ServerModListSource(ServerModListType type);

public:
    static ServerModListSource* get(ServerModListType type);

    std::unordered_set<std::string> getModTags() const override;
    void setModTags(std::unordered_set<std::string> const& tags) override;

    server::ModsQuery const& getQuery() const;
    InvalidateQueryAfter<server::ModsQuery> getQueryMut();
    bool isDefaultQuery() const override;
    server::ModsQuery createDefaultQuery() const;
    ServerModListType getType() const;

    bool isLocalModsOnly() const override;
};

class ModPackListSource : public ModListSource {
protected:
    void resetQuery() override;
    ProviderTask fetchPage(size_t page, bool forceUpdate) override;
    void setSearchQuery(std::string const& query) override;

    ModPackListSource();

public:
    static ModPackListSource* get();

    std::unordered_set<std::string> getModTags() const override;
    void setModTags(std::unordered_set<std::string> const& tags) override;
    bool isDefaultQuery() const override;

    bool isLocalModsOnly() const override;
};

bool weightedFuzzyMatch(std::string const& str, std::string const& kw, double weight, double& out);
bool modFuzzyMatch(ModMetadata const& metadata, std::string const& kw, double& out);

template <std::derived_from<LocalModsQueryBase> Query>
void filterModsWithLocalQuery(ModListSource::ProvidedMods& mods, Query const& query) {
    std::vector<std::pair<ModSource, double>> filtered;

    // Filter installed mods based on query
    // TODO: maybe skip fuzzy matching altogether if query is empty?
    for (auto& src : mods.mods) {
        double weighted = 0;
        bool addToList = true;
        // Do any checks additional this query has to start off with
        if (!query.preCheck(src)) {
            addToList = false;
        }
        // If some tags are provided, only return mods that match
        if (addToList && query.tags.size()) {
            auto compare = src.getMetadata().getTags();
            for (auto& tag : query.tags) {
                if (!compare.contains(tag)) {
                    addToList = false;
                }
            }
        }
        // Don't bother with unnecessary fuzzy match calculations if this mod isn't going to be added anyway
        if (addToList) {
            addToList = query.queryCheck(src, weighted);
        }
        if (addToList) {
            filtered.push_back({ std::move(src), weighted });
        }
    }

    // Sort list based on score
    std::sort(filtered.begin(), filtered.end(), [](auto& a, auto& b) {
        // Sort primarily by score
        if (a.second != b.second) {
            return a.second > b.second;
        }
        // Make sure outdated mods are always last by default
        auto aIsOutdated = a.first.getMetadata().checkTargetVersions().isErr();
        auto bIsOutdated = b.first.getMetadata().checkTargetVersions().isErr();
        if (aIsOutdated != bIsOutdated) {
            return !aIsOutdated;
        }
        // Fallback sort alphabetically
        return utils::string::caseInsensitiveCompare(
            a.first.getMetadata().getName(),
            b.first.getMetadata().getName()
        ) == std::strong_ordering::less;
    });

    mods.mods.clear();
    // Pick out only the mods in the page and page size specified in the query
    for (
        size_t i = query.page * query.pageSize;
        i < filtered.size() && i < (query.page + 1) * query.pageSize;
        i += 1
    ) {
        mods.mods.push_back(std::move(filtered.at(i).first));
    }

    mods.totalModCount = filtered.size();
}
