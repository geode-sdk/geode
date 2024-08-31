#pragma once

#include <Geode/loader/SettingV3.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/cocos.hpp>
#include "../GeodeStyle.hpp"

using namespace geode::prelude;

class ModSettingsPopup : public GeodePopup<Mod*> {
protected:
    Mod* m_mod;
    std::vector<SettingNodeV3*> m_settings;
    CCMenu* m_applyMenu;
    CCMenuItemSpriteExtra* m_applyBtn;
    CCMenuItemSpriteExtra* m_restartBtn;
    ButtonSprite* m_applyBtnSpr;
    IconButtonSprite* m_openConfigDirBtnSpr;
    EventListener<EventFilter<SettingNodeValueChangeEventV3>> m_changeListener;

    bool setup(Mod* mod) override;
    void updateState(SettingNodeV3* invoker = nullptr);
    bool hasUncommitted() const;
    void onClose(CCObject*) override;
    void onApply(CCObject*);
    void onRestart(CCObject*);
    void onResetAll(CCObject*);
    void onOpenSaveDirectory(CCObject*);
    void onOpenConfigDirectory(CCObject*);

public:
    static ModSettingsPopup* create(Mod* mod);
};
