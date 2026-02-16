#pragma once

#include "BaseSettingsPopup.hpp"

using namespace geode::prelude;

class KeybindsPopup : public BaseSettingsPopup {
protected:
    bool init(bool forceDisableTheme);

public:
    static KeybindsPopup* create(bool forceDisableTheme = false);
};
