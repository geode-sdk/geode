#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/loader/Mod.hpp>

USE_GEODE_NAMESPACE();

class AdvancedSettingsPopup : public Popup<Mod*> {
protected:
    Mod* m_mod;

    bool setup(Mod* mod) override;

public:
    static AdvancedSettingsPopup* create(Mod* mod);
};
