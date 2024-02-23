#pragma once

#include <Geode/ui/General.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include "ModItem.hpp"
#include "ModListSource.hpp"

using namespace geode::prelude;

// Stores the current page and scroll position for a given list
struct ListPosCache {
    size_t page;
    float scrollPosition;
};

class ModsLayer : public CCLayer {
protected:
    std::vector<CCMenuItemSpriteExtra*> m_tabs;
    ScrollLayer* m_list;
    std::string m_currentListID;
    size_t m_currentPage = 0;
    std::unordered_map<std::string, ListPosCache> m_listPosCaches;

    bool init();

    void keyBackClicked() override;
    void onTab(CCObject* sender);
    void gotoTab(std::string const& id);
    void loadList(std::string const& id);
    void loadPage(size_t page, bool update = false);

public:
    static ModsLayer* create();
    static ModsLayer* scene();

    void onBack(CCObject*);
    void onRefreshList(CCObject*);
};
