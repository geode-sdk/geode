#pragma once

#include <Geode/utils/cocos.hpp>
#include <Geode/utils/Promise.hpp>
#include <server/Server.hpp>
#include "ModItem.hpp"

using namespace geode::prelude;

enum class ModListSourceType {
    Installed,
    Featured,
    Trending,
    ModPacks,
    All,
};

// Handles loading the entries for the mods list
class ModListSource : public CCObject {
public:
    struct LoadPageError {
        std::string message;
        std::optional<std::string> details;

        LoadPageError() = default;
        LoadPageError(auto msg) : message(msg) {}
        LoadPageError(auto msg, auto details) : message(msg), details(details) {}
    };

    using Page = std::vector<Ref<BaseModItem>>;
    using PageLoadEvent = PromiseEvent<Page, LoadPageError, std::optional<uint8_t>>;
    using PageLoadEventFilter = PromiseEventFilter<Page, LoadPageError, std::optional<uint8_t>>;
    using PageLoadEventListener = EventListener<PageLoadEventFilter>;
    using PagePromise = Promise<Page, LoadPageError, std::optional<uint8_t>>;

    using ProviderPromise = Promise<std::pair<Page, size_t>, LoadPageError, std::optional<uint8_t>>;
    using Provider = ProviderPromise(server::ModsQuery&& query);

protected:
    std::unordered_map<size_t, Page> m_cachedPages;
    std::optional<size_t> m_cachedItemCount;
    std::optional<std::string> m_query;
    Provider* m_provider = nullptr;
    bool m_inMemory;

public:
    // Create a new source with an arbitary provider
    static ModListSource* create(Provider* provider, bool inMemory);

    // Get a standard source (lazily created static instance)
    static ModListSource* get(ModListSourceType type);

    // Reset all filters & cache
    void reset();

    // Set a query; clears cache
    void setQuery(std::string const& query);

    // Load page, uses cache if possible unless `update` is true
    PagePromise loadPage(size_t page, bool update = false);
    std::optional<size_t> getPageCount() const;
    std::optional<size_t> getItemCount() const;

    /**
     * True if the source is already fully loaded in memory (doesn't fetch 
     * from a server or filesystem)
     * 
     * Used to determine whether things like searching should update the query 
     * instantaniously or buffer a bit to avoid spamming unnecessary requests
     */
    bool isInMemory() const;
};
