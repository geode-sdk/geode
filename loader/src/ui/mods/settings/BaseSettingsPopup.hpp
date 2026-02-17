#pragma once

#include "../GeodeStyle.hpp"
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/ui/TextInput.hpp>
#include <loader/SettingNode.hpp>

using namespace geode::prelude;

class BaseSettingsPopup : public GeodePopup {
protected:
    ScrollLayer* m_list;
    std::vector<Ref<SettingNode>> m_settings;
    CCMenu* m_applyMenu;
    CCMenuItemSpriteExtra* m_applyBtn;
    CCMenuItemSpriteExtra* m_restartBtn;
    ButtonSprite* m_applyBtnSpr;
    TextInput* m_searchInput;
    CCMenuItemSpriteExtra* m_searchClearBtn;
    ListenerHandle m_changeHandle;
    CCSize m_listSize;

    bool init(bool forceDisableTheme, bool spacingForTabs = false);
    virtual void updateState(SettingNode* invoker = nullptr);
    virtual bool shouldShow(SettingNode* node) const;

    bool hasUncommitted() const;
    void onClose(CCObject*) override;
    void onApply(CCObject*);
    void onRestart(CCObject*);
    void onResetAll(CCObject*);
    void onClearSearch(CCObject*);
};
