#pragma once

#include <Geode/loader/SettingNode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/cocos.hpp>
#include "../GeodeStyle.hpp"

using namespace geode::prelude;

class ModSettingsPopup : public GeodePopup<Mod*>, public SettingNodeDelegate {
protected:
    Mod* m_mod;
    std::vector<SettingNode*> m_settings;
    CCMenuItemSpriteExtra* m_applyBtn;
    ButtonSprite* m_applyBtnSpr;

    void settingValueChanged(SettingNode*) override;
    void settingValueCommitted(SettingNode*) override;

    bool setup(Mod* mod) override;
    bool hasUncommitted() const;
    void onClose(CCObject*) override;
    void onApply(CCObject*);
    void onResetAll(CCObject*);
    void onOpenSaveDirectory(CCObject*);

public:
    static ModSettingsPopup* create(Mod* mod);
};
