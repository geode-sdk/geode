#pragma once

#include <Geode/ui/Popup.hpp>
#include "../UpdateModListState.hpp"

class ConfirmUninstallPopup : public Popup {
protected:
    Mod* m_mod;
    CCMenuItemToggler* m_deleteDataToggle;

    bool init(Mod* mod);

public:
    static ConfirmUninstallPopup* create(Mod* mod);
};