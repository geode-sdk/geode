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

    bool init(bool forceDisableTheme);
    bool shouldShow(SettingNode* node) const override;

    void onSelectTab(CCObject*);

public:
    static KeybindsPopup* create(bool forceDisableTheme = false);
};
