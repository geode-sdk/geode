#pragma once

#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class ConfirmUninstallPopup : public Popup<Mod*> {
protected:
    Mod* m_mod;
    CCMenuItemToggler* m_deleteDataToggle;
    MiniFunction<void()> m_updateParentState = nullptr;

    bool setup(Mod* mod) override;

    void onUninstall(CCObject*);

public:
    static ConfirmUninstallPopup* create(Mod* mod);

    // todo: replace all of these with a single event
    void onUpdateParentState(MiniFunction<void()> listener);
};
