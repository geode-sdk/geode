#pragma once

#include "../loader/Mod.hpp"

namespace geode {
    /**
     * Open the Geode mods list
     */
    GEODE_DLL void openModsList();
    /**
     * Open the info popup for a mod
     */
    GEODE_DLL void openInfoPopup(Mod* mod);
    /**
     * Open the store page for a mod (if it exists)
     */
    GEODE_DLL void openIndexPopup(Mod* mod);
    /**
     * Open the settings popup for a mod (if it has any settings)
    */
    GEODE_DLL void openSettingsPopup(Mod* mod);
}

