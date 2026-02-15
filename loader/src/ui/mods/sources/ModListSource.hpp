#pragma once

#include <Geode/utils/cocos.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/ZStringView.hpp>
#include <server/Server.hpp>
#include "../list/ModListItem.hpp"
#include "ModSource.hpp"
#include <arc/future/Future.hpp>

using namespace geode::prelude;

class ModListSource;

class InvalidateCacheEvent final : public Event<InvalidateCacheEvent, bool(ModListSource*)> {
public:
    // filter params source
    using Event::Event;
};


// If we want to insert some special item in the middle of the mods list (for
// example, when there are invalid .geode files in the mods folder, a single
// special "You have invalid Geode mods" item is inserted at the top to
// prevent clutter)
struct SpecialModListItemSource final {
    std::string title;
    Function<void()> onDetails;
};

// Handles loading the entries for the mods list
class ModListSource {
public:
    struct LoadPageError {
        std::string message;
        std::optional<std::string> details;

        LoadPageError() = default;
        LoadPageError(std::string msg) : message(std::move(msg)) {}
        LoadPageError(std::string msg, std::optional<std::string> details)
            : message(std::move(msg)), details(std::move(details)) {}
    };

    using Page = std::vector<Ref<ModListItem>>;
    using PageLoadResult = Result<Page, LoadPageError>;
    using PageLoadTask = arc::Future<PageLoadResult>;

    struct ProvidedMods {
        std::vector<std::variant<ModSource, SpecialModListItemSource>> mods;
        size_t totalModCount;
    };
    using ProviderTask = arc::Future<Result<ProvidedMods, LoadPageError>>;

protected:
    std::unordered_map<size_t, Page> m_cachedPages;
    std::optional<size_t> m_cachedItemCount;
    size_t m_pageSize = 10;

    virtual void resetQuery() = 0;
    virtual ProviderTask fetchPage(size_t page, bool forceUpdate) = 0;
    virtual void setSearchQuery(std::string query) = 0;
    // This is literally here just for sorting by recently updated in installed mods...
    virtual std::string getNoModsFoundError() const;

    ModListSource();

public:
    ModListSource(ModListSource const&) = delete;
    ModListSource(ModListSource&&) = delete;

    // Reset all filters & cache
    void reset();
    void clearCache();
    std::optional<Page> getCachedPage(size_t page) const;
    void search(std::string query);
    virtual bool isDefaultQuery() const = 0;

    virtual std::unordered_set<std::string> getModTags() const = 0;
    virtual void setModTags(std::unordered_set<std::string> const& tags) = 0;

    // Load page, uses cache if possible unless `forceUpdate` is true
    ProviderTask loadPage(size_t page, bool forceUpdate = false);
    PageLoadResult processLoadedPage(size_t page, ProvidedMods mods);
    std::optional<size_t> getPageCount() const;
    std::optional<size_t> getItemCount() const;
    void setPageSize(size_t size);

    virtual std::vector<std::pair<size_t, std::string>> getSortingOptions();
    virtual size_t getSort() const;
    virtual void setSort(size_t sortingOptionIndex);

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

enum class InstalledModListSort {
    Alphabetical,
    RecentlyUpdated,
};
enum class InstalledModListType {
    All,
    OnlyUpdates,
    OnlyErrors,
    OnlyOutdated,
};
struct InstalledModsQuery final {
    std::optional<std::string> query;
    std::unordered_set<std::string> tags = {};
    size_t page = 0;
    size_t pageSize = 10;
    InstalledModListType type = InstalledModListType::All;
    InstalledModListSort sort = InstalledModListSort::Alphabetical;
    std::optional<bool> enabledOnly;
    std::optional<bool> enabledFirst;
    
    void filter(ModListSource::ProvidedMods& mods);
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
    void setSearchQuery(std::string query) override;
    std::string getNoModsFoundError() const override;

    InstalledModListSource(InstalledModListType type);

public:
    static InstalledModListSource* get(InstalledModListType type);

    std::unordered_set<std::string> getModTags() const override;
    void setModTags(std::unordered_set<std::string> const& tags) override;

    std::vector<std::pair<size_t, std::string>> getSortingOptions() override;
    size_t getSort() const override;
    void setSort(size_t sortingOptionIndex) override;

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
    Modtober,
};

class ServerModListSource : public ModListSource {
protected:
    ServerModListType m_type;
    server::ModsQuery m_query;

    void resetQuery() override;
    ProviderTask fetchPage(size_t page, bool forceUpdate) override;
    void setSearchQuery(std::string query) override;

    ServerModListSource(ServerModListType type);

public:
    static ServerModListSource* get(ServerModListType type);

    std::unordered_set<std::string> getModTags() const override;
    void setModTags(std::unordered_set<std::string> const& tags) override;

    std::vector<std::pair<size_t, std::string>> getSortingOptions() override;
    size_t getSort() const override;
    void setSort(size_t sortingOptionIndex) override;

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
    void setSearchQuery(std::string query) override;

    ModPackListSource();

public:
    static ModPackListSource* get();

    std::unordered_set<std::string> getModTags() const override;
    void setModTags(std::unordered_set<std::string> const& tags) override;
    bool isDefaultQuery() const override;

    bool isLocalModsOnly() const override;
};

bool weightedFuzzyMatch(ZStringView str, ZStringView kw, double weight, double& out);
bool modFuzzyMatch(ModMetadata const& metadata, ZStringView kw, double& out);
