#pragma once

#include <Geode/ui/General.hpp>
#include <server/Server.hpp>
#include "../sources/ModSource.hpp"

using namespace geode::prelude;

class ModItem : public CCNode {
protected:
    ModSource m_source;
    CCScale9Sprite* m_bg;
    CCNode* m_logo;
    CCNode* m_infoContainer;
    CCNode* m_titleContainer;
    CCLabelBMFont* m_titleLabel;
    CCNode* m_developers;
    CCLabelBMFont* m_developerLabel;
    ButtonSprite* m_restartRequiredLabel = nullptr;
    CCMenu* m_viewMenu;
    MiniFunction<void()> m_updateParentState = nullptr;
    CCMenuItemToggler* m_enableToggle = nullptr;
    CCScale9Sprite* m_checkmark = nullptr;

    /**
     * @warning Make sure `getMetadata` and `createModLogo` are callable 
     * before calling `init`!
    */
    bool init(ModSource&& source);

    // This should never be exposed outside, so the parent can't call this and 
    // cause an infinite loop during state updating
    void updateState();

    void onEnable(CCObject*);
    void onView(CCObject*);

public:
    static ModItem* create(ModSource&& source);

    void updateSize(float width, bool big);

    void onUpdateParentState(MiniFunction<void()> listener);
};
