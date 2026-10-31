#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/ui/Toggler.hpp>
#include "../UpdateModListState.hpp"

class ConfirmUninstallPopup : public Popup {
protected:
    Mod* m_mod;
    Toggler* m_deleteDataToggle;

    bool init(Mod* mod);

public:
    static ConfirmUninstallPopup* create(Mod* mod);
};