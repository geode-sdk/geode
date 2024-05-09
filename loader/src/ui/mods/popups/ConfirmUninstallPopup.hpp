#pragma once

#include <Geode/ui/Popup.hpp>
#include "../UpdateModListState.hpp"

using namespace geode::prelude;

class ConfirmUninstallPopup : public Popup<Mod*> {
protected:
    Mod* m_mod;
    CCMenuItemToggler* m_deleteDataToggle;

    bool setup(Mod* mod) override;

    void onUninstall(CCObject*);

public:
    static ConfirmUninstallPopup* create(Mod* mod);
};
