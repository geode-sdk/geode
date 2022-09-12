#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/utils/Ref.hpp>

USE_GEODE_NAMESPACE();

class ModSettingsPopup : public Popup<Mod*> {
protected:
    Mod* m_mod;
    std::vector<SettingNode*> m_settings;
    CCMenuItemSpriteExtra* m_applyBtn;
    ButtonSprite* m_applyBtnSpr;

    bool setup(Mod* mod) override;
    bool hasUncommitted() const;
    void onClose(CCObject*) override;
    
public:
    static ModSettingsPopup* create(Mod* mod);
};

