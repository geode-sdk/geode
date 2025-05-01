#pragma once

#include <Geode/loader/Mod.hpp>

#include "ui/mods/GeodeStyle.hpp"
#include "ui/mods/list/ModProblemList.hpp"

using namespace geode::prelude;

class ModErrorPopup : public GeodePopup<Mod*> {
protected:
    Mod* m_mod;
    ModProblemList* m_list;
public:
    static ModErrorPopup* create(Mod* mod);

private:
    bool setup(Mod* mod) override;

    void createList();
};
