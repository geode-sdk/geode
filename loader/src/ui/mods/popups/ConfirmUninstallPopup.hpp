#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/ui/Toggler.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

class ConfirmUninstallPopup : public Popup {
protected:
    Mod* m_mod;
    Toggler* m_deleteDataToggle;

    bool init(Mod* mod);

public:
    static ConfirmUninstallPopup* create(Mod* mod);
};