#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/utils/Ref.hpp>
#include <Geode/loader/SettingNode.hpp>

USE_GEODE_NAMESPACE();

class ModSettingsPopup : public Popup<Mod*>, public SettingNodeDelegate {
protected:
    Mod* m_mod;
    std::vector<SettingNode*> m_settings;
    CCMenuItemSpriteExtra* m_applyBtn;
    ButtonSprite* m_applyBtnSpr;

    void settingValueChanged(SettingNode* node) override;
    
    bool setup(Mod* mod) override;
    bool hasUncommitted() const;
    void onClose(CCObject*) override;
    
public:
    static ModSettingsPopup* create(Mod* mod);
};

