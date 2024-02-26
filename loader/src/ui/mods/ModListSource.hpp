#pragma once

#include <Geode/utils/cocos.hpp>
#include <Geode/utils/Promise.hpp>
#include "ModItem.hpp"

using namespace geode::prelude;

// Handles loading the entries for the mods list
class ModListSource {
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

protected:
    std::unordered_map<size_t, Page> m_cachedPages;
    std::optional<size_t> m_cachedItemCount;

    // Load/reload a page. This should also set/update the page count
    virtual PagePromise reloadPage(size_t page) = 0;

public:
    // Load page, uses cache if possible unless `update` is true
    PagePromise loadPage(size_t page, bool update = false);
    std::optional<size_t> getPageCount() const;
    std::optional<size_t> getItemCount() const;
};

class InstalledModsList : public ModListSource {
protected:
    PagePromise reloadPage(size_t page) override;

public:
    static InstalledModsList* get();
};

class FeaturedModsList : public ModListSource {
protected:
    PagePromise reloadPage(size_t page) override;

public:
    static FeaturedModsList* get();
};

class ModPacksModsList : public ModListSource {
protected:
    PagePromise reloadPage(size_t page) override;

public:
    static ModPacksModsList* get();
};
