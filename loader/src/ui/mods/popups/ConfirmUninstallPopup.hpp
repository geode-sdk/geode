#pragma once

#include <Geode/ui/Popup.hpp>
#include "../UpdateModListState.hpp"

using namespace geode::prelude;

class ConfirmUninstallPopup : public Popup {
protected:
    Mod* m_mod;
    CCMenuItemToggler* m_deleteDataToggle;

    bool init(Mod* mod);

    void onUninstall(CCObject*);

public:
    static ConfirmUninstallPopup* create(Mod* mod);
};
