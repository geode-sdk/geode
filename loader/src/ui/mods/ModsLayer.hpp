#pragma once

#include <Geode/ui/General.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/ui/TextArea.hpp>
#include "ModItem.hpp"
#include "ModListSource.hpp"
#include "ModList.hpp"

using namespace geode::prelude;

class ModsLayer : public CCLayer {
protected:
    CCNode* m_frame;
    std::vector<CCMenuItemSpriteExtra*> m_tabs;
    ModListSource* m_currentSource = nullptr;
    std::unordered_map<ModListSource*, Ref<ModList>> m_lists;

    bool init();

    void keyBackClicked() override;
    void onTab(CCObject* sender);

public:
    static ModsLayer* create();
    static ModsLayer* scene();

    void onBack(CCObject*);
    void onRefreshList(CCObject*);

    void gotoTab(ModListSourceType type);
};
