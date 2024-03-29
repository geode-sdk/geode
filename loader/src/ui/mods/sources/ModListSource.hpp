#pragma once

#include <Geode/utils/cocos.hpp>
#include <Geode/utils/Promise.hpp>
#include <server/Server.hpp>
#include "../list/ModItem.hpp"

using namespace geode::prelude;

struct InstalledModsQuery final {
    std::optional<std::string> query;
    bool onlyUpdates = false;
    size_t page = 0;
    size_t pageSize = 10;
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
    using PageLoadEvent = PromiseEvent<Page, LoadPageError, std::optional<uint8_t>>;
    using PageLoadEventFilter = PromiseEventFilter<Page, LoadPageError, std::optional<uint8_t>>;
    using PageLoadEventListener = EventListener<PageLoadEventFilter>;
    using PagePromise = Promise<Page, LoadPageError, std::optional<uint8_t>>;

    struct ProvidedMods {
        std::vector<ModSource> mods;
        size_t totalModCount;
    };
    using ProviderPromise = Promise<ProvidedMods, LoadPageError, std::optional<uint8_t>>;

protected:
    std::unordered_map<size_t, Page> m_cachedPages;
    std::optional<size_t> m_cachedItemCount;

    virtual void resetQuery() = 0;
    virtual ProviderPromise fetchPage(size_t page, size_t pageSize) = 0;
    virtual void setSearchQuery(std::string const& query) = 0;

    ModListSource();

public:
    ModListSource(ModListSource const&) = delete;
    ModListSource(ModListSource&&) = delete;

    // Reset all filters & cache
    void reset();
    void clearCache();
    void search(std::string const& query);

    // Load page, uses cache if possible unless `update` is true
    PagePromise loadPage(size_t page, bool update = false);
    std::optional<size_t> getPageCount() const;
    std::optional<size_t> getItemCount() const;

    /**
     * True if the source consists only of installed mods
     */
    virtual bool isInstalledMods() const = 0;
    virtual bool wantsRestart() const = 0;
};

class InstalledModListSource : public ModListSource {
protected:
    bool m_onlyUpdates;
    InstalledModsQuery m_query;

    void resetQuery() override;
    ProviderPromise fetchPage(size_t page, size_t pageSize) override;

    InstalledModListSource(bool onlyUpdates);

public:
    static InstalledModListSource* get(bool onlyUpdates);

    void setSearchQuery(std::string const& query) override;

    InstalledModsQuery const& getQuery() const;
    // Get mutable access to the current query; this clears the cache
    InstalledModsQuery& getQueryMut();

    bool isInstalledMods() const override;
    bool wantsRestart() const override;
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
    ProviderPromise fetchPage(size_t page, size_t pageSize) override;

    ServerModListSource(ServerModListType type);

public:
    static ServerModListSource* get(ServerModListType type);

    void setSearchQuery(std::string const& query) override;

    server::ModsQuery const& getQuery() const;
    // Get mutable access to the current query; this clears the cache
    server::ModsQuery& getQueryMut();

    bool isInstalledMods() const override;
    bool wantsRestart() const override;
};

class ModPackListSource : public ModListSource {
protected:
    void resetQuery() override;
    ProviderPromise fetchPage(size_t page, size_t pageSize) override;

    ModPackListSource();

public:
    static ModPackListSource* get();

    void setSearchQuery(std::string const& query) override;

    bool isInstalledMods() const override;
    bool wantsRestart() const override;
};
