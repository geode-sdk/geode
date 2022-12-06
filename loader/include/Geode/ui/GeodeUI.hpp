#pragma once

#include "../loader/Mod.hpp"
#include "../loader/Index.hpp"

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
     * Open the issue report popup for a mod
     */
    GEODE_DLL void openIssueReportPopup(Mod* mod);
    /**
     * Open the store page for a mod (if it exists)
     */
    GEODE_DLL void openIndexPopup(Mod* mod);
    /**
     * Open the settings popup for a mod (if it has any settings)
     */
    GEODE_DLL void openSettingsPopup(Mod* mod);
    /**
     * Create a default logo sprite
     * @param size Size of the sprite
     */
    GEODE_DLL cocos2d::CCNode* createDefaultLogo(
        cocos2d::CCSize const& size
    );
    /**
     * Create a logo sprite for a mod
     * @param size Size of the sprite
     */
    GEODE_DLL cocos2d::CCNode* createModLogo(
        Mod* mod, cocos2d::CCSize const& size
    );
    /**
     * Create a logo sprite for an index item
     * @param size Size of the sprite
     */
    GEODE_DLL cocos2d::CCNode* createIndexItemLogo(
        IndexItemHandle item, cocos2d::CCSize const& size
    );
}
