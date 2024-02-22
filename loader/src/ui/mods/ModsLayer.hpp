#pragma once

#include <Geode/ui/General.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include "ModItem.hpp"

using namespace geode::prelude;

struct ListCache {
    std::string id;
    std::vector<Ref<BaseModItem>> items;
    float scrollPosition;
};

class ModsLayer : public CCLayer {
protected:
    std::vector<CCMenuItemSpriteExtra*> m_tabs;
    ScrollLayer* m_list;
    ListCache* m_currentList = nullptr;
    std::unordered_map<std::string, ListCache> m_listItemsCache;

    bool init();

    void keyBackClicked() override;
    void onTab(CCObject* sender);
    void gotoTab(std::string const& id);
    void loadList(std::string const& id, bool update = false);

public:
    static ModsLayer* create();
    static ModsLayer* scene();

    void onBack(CCObject*);
    void onRefreshList(CCObject*);
};
