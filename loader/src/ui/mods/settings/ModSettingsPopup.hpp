#pragma once

#include "BaseSettingsPopup.hpp"

using namespace geode::prelude;

class ModSettingsPopup : public BaseSettingsPopup {
protected:
    Mod* m_mod;

    bool init(Mod* mod, bool forceDisableTheme);

    void updateState(SettingNode* invoker = nullptr) override;

public:
    static ModSettingsPopup* create(Mod* mod, bool forceDisableTheme = false);
};
