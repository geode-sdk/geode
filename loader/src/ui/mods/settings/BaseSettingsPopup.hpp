#pragma once

#include "../../GeodeStyle.hpp"
#include "SettingNode.hpp"

#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

class BaseSettingsPopup : public geode::GeodePopup {
protected:
    ScrollLayer* m_list = nullptr;
    std::vector<Ref<SettingNode>> m_settings;
    CCNode* m_applyMenu = nullptr;
    Button* m_applyBtn = nullptr;
    Button* m_restartBtn = nullptr;
    ButtonSprite* m_applyBtnSpr = nullptr;
    TextInput* m_searchInput = nullptr;
    Button* m_searchClearBtn = nullptr;
    ListenerHandle m_changeHandle;
    CCSize m_listSize;

    bool init(bool forceDisableTheme, bool spacingForTabs = false);
    virtual void updateState(SettingNode* invoker = nullptr);
    virtual bool shouldShow(SettingNode* node) const;

    bool hasUncommitted() const;
    void onClose(CCObject*) override;
};
