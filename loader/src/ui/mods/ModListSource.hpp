#pragma once

#include <Geode/utils/cocos.hpp>
#include <Geode/utils/Promise.hpp>
#include "ModItem.hpp"

using namespace geode::prelude;

// Handles loading the entries for the mods list
class ModListSource {
public:
    using Page = std::vector<Ref<BaseModItem>>;
    using PageLoadEventListener = EventListener<PromiseEventFilter<Page>>;

protected:
    std::unordered_map<size_t, Page> m_cachedPages;
    std::optional<size_t> m_cachedPageCount;

    // Load/reload a page. This should also set/update the page count
    virtual Promise<Page> reloadPage(size_t page) = 0;

public:
    // Load page, uses cache if possible unless `update` is true
    Promise<Page> loadPage(size_t page, bool update = false);
    std::optional<size_t> getPageCount() const;
};

class InstalledModsList : public ModListSource {
protected:
    Promise<Page> reloadPage(size_t page) override;

public:
    static InstalledModsList* get();
};

class ModPacksModsList : public ModListSource {
protected:
    Promise<Page> reloadPage(size_t page) override;

public:
    static ModPacksModsList* get();
};
