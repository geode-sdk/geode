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
    CCMenuItemSpriteExtra* m_applyBtn;
    ButtonSprite* m_applyBtnSpr;
    EventListener<EventFilter<SettingNodeValueChangeEventV3>> m_changeListener;

    bool setup(Mod* mod) override;
    void updateState();
    void onChangeEvent(SettingNodeValueChangeEventV3* event);
    bool hasUncommitted() const;
    void onClose(CCObject*) override;
    void onApply(CCObject*);
    void onResetAll(CCObject*);
    void onOpenSaveDirectory(CCObject*);

public:
    static ModSettingsPopup* create(Mod* mod);
};
