#pragma once

#include "BaseSettingsPopup.hpp"

using namespace geode::prelude;

enum class KeybindsPopupTab {
    All = -1,
    Universal = static_cast<int>(KeybindCategory::Universal),
    Gameplay = static_cast<int>(KeybindCategory::Gameplay),
    Editor = static_cast<int>(KeybindCategory::Editor),
};

class KeybindsPopup : public BaseSettingsPopup {
protected:
    KeybindsPopupTab m_tab = KeybindsPopupTab::All;
    CCMenu* m_tabsMenu;

    bool init(std::optional<KeybindsPopupTab> tab, Mod* mod, bool forceDisableTheme);
    bool shouldShow(SettingNode* node) const override;

    void onSelectTab(CCObject*);
    void focusMod(Mod* mod);

public:
    static KeybindsPopup* create(bool forceDisableTheme = false);
    static KeybindsPopup* create(std::optional<KeybindsPopupTab> tab, Mod* mod, bool forceDisableTheme = false);
};
