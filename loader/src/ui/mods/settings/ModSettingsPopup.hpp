#pragma once

#include <Geode/loader/Setting.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/ui/TextInput.hpp>

#include "../GeodeStyle.hpp"

using namespace geode::prelude;

class ModSettingsPopup : public GeodePopup<Mod*> {
protected:
    Mod* m_mod;
    ScrollLayer* m_list;
    std::vector<Ref<SettingNode>> m_settings;
    CCMenu* m_applyMenu;
    CCMenuItemSpriteExtra* m_applyBtn;
    CCMenuItemSpriteExtra* m_restartBtn;
    ButtonSprite* m_applyBtnSpr;
    TextInput* m_searchInput;
    CCMenuItemSpriteExtra* m_searchClearBtn;
    EventListener<EventFilter<SettingNodeValueChangeEvent>> m_changeListener;

    bool setup(Mod* mod) override;
    void updateState(SettingNode* invoker = nullptr);
    bool hasUncommitted() const;
    void onClose(CCObject*) override;
    void onApply(CCObject*);
    void onRestart(CCObject*);
    void onResetAll(CCObject*);
    void onOpenSaveDirectory(CCObject*);
    void onOpenConfigDirectory(CCObject*);
    void onOpenPersistentDirectory(CCObject*);
    void onClearSearch(CCObject*);

public:
    static ModSettingsPopup* create(Mod* mod, bool forceDisableTheme = false);
};
