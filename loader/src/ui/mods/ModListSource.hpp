#pragma once

#include <Geode/utils/cocos.hpp>
#include <Geode/utils/Promise.hpp>
#include "ModItem.hpp"

using namespace geode::prelude;

// Handles loading the entries for the mods list
struct ModListSource {
    using Page = std::vector<Ref<BaseModItem>>;
 
    // ID of the list source
    virtual std::string_view getID() const = 0;

    // Load a page. Use update to force a reload on existing page. Up to the 
    // source impl to cache this
    virtual Promise<Page> loadNewPage(size_t page, bool update = false) = 0;

    // Get the total number of available pages
    virtual Promise<size_t> loadTotalPageCount() const = 0;

    static std::optional<std::reference_wrapper<ModListSource>> get(std::string_view id);
};
