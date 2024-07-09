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
     * Open the info popup for a mod on the changelog page
     */
    GEODE_DLL void openChangelogPopup(Mod* mod);
    /**
     * Open the issue report popup for a mod
     */
    GEODE_DLL void openIssueReportPopup(Mod* mod);
    /**
     * Open the support popup for a mod
     */
    GEODE_DLL void openSupportPopup(Mod* mod);
    GEODE_DLL void openSupportPopup(ModMetadata const& metadata);
    /**
     * Open the store page for a mod (if it exists)
     */
    [[deprecated("Will be removed, use openInfoPopup instead")]]
    GEODE_DLL void openIndexPopup(Mod* mod);
    /**
     * Open the settings popup for a mod (if it has any settings)
     */
    GEODE_DLL void openSettingsPopup(Mod* mod);
    /**
     * Create a default logo sprite
     */
    GEODE_DLL cocos2d::CCNode* createDefaultLogo();
    /**
     * Create a logo sprite for a mod
     */
    GEODE_DLL cocos2d::CCNode* createModLogo(Mod* mod);
    /**
     * Create a logo sprite for a mod downloaded from the Geode servers. The 
     * logo is initially a loading circle, with the actual sprite downloaded 
     * asynchronously
     */
    GEODE_DLL cocos2d::CCNode* createServerModLogo(std::string const& id);
}
