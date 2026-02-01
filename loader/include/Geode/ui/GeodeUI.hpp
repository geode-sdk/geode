#pragma once

#include "../loader/Mod.hpp"
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/ui/Popup.hpp>

class ModPopup;
class ModItem;
class ModLogoSprite;
class FLAlertLayer; // for macos :3

namespace geode {
    /**
     * Event posted whenever a popup is opened for a mod. Allows mods to modify
     * the Geode UI. See the [tutorial on Geode UI modification](https://docs.geode-sdk.org/tutorials/modify-geode)
     * for **very important notes on these events**!
     */
    class GEODE_DLL ModPopupUIEvent final : public SimpleEvent<ModPopupUIEvent, FLAlertLayer*, std::string_view, std::optional<Mod*>> {
    public:
        // listener params popup, modID, mod
        using SimpleEvent::SimpleEvent;  
    };

    /**
     * Event posted whenever a logo sprite is created for a mod. Allows mods to modify
     * the Geode UI. See the [tutorial on Geode UI modification](https://docs.geode-sdk.org/tutorials/modify-geode)
     * for **very important notes on these events**!
     */
    class GEODE_DLL ModItemUIEvent final : public SimpleEvent<ModItemUIEvent, cocos2d::CCNode*, std::string_view, std::optional<Mod*>> {
    public:
        // listener params item, modID, mod
        using SimpleEvent::SimpleEvent;  
    };

    /**
     * Event posted whenever a logo sprite is created for a mod. Allows mods to modify
     * the Geode UI. See the [tutorial on Geode UI modification](https://docs.geode-sdk.org/tutorials/modify-geode)
     * for **very important notes on these events**!
     */
     class GEODE_DLL ModLogoUIEvent final : public SimpleEvent<ModItemUIEvent, cocos2d::CCNode*, std::string_view, std::optional<Mod*>> {
    public:
        // listener params sprite, modID, mod
        using SimpleEvent::SimpleEvent;  
    };

    /**
     * Open the Geode mods list
     */
    GEODE_DLL void openModsList();
    /**
     * Open the info popup for a mod
     */
    GEODE_DLL void openInfoPopup(Mod* mod);
    /**
     * Open the info popup for a mod based on an ID. If the mod is installed,
     * its installed popup is opened. Otherwise will check if the servers
     * have this mod, or if not, show an error popup
     * @returns `std::nullopt` if the mod is installed, otherwise a Task
     * that completes to `true` if the mod was found and a popup was opened,
     * and `false` otherwise. If you wish to modify the created popup,
     * listen for the Geode UI events listed in `GeodeUI.hpp`
     */
    GEODE_DLL std::optional<arc::TaskHandle<bool>> openInfoPopup(std::string modID);
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
     * Open the settings popup for a mod (if it has any settings)
     */
    GEODE_DLL void openSettingsPopup(Mod* mod);
    /**
     * Open the settings popup for a mod (if it has any settings)
     * @param mod Mod the open the popup for
     * @param disableGeodeTheme If false, the popup follows the user's chosen
     * theme options. If true, the popup is always in the GD theme (not Geode's
     * dark purple colors)
     * @returns A pointer to the created Popup, or null if the mod has no
     * settings
     */
    GEODE_DLL Popup* openSettingsPopup(Mod* mod, bool disableGeodeTheme);
    /**
     * Create a default logo sprite
     */
    GEODE_DLL cocos2d::CCNode* createDefaultLogo();
    /**
     * Create a logo sprite for a mod
     */
    GEODE_DLL cocos2d::CCNode* createModLogo(Mod* mod);
    /**
     * Create a logo sprite for a mod from a .geode file
     */
    GEODE_DLL cocos2d::CCNode* createModLogo(std::filesystem::path const& geodePackage);
    /**
     * Create a logo sprite for a mod downloaded from the Geode servers. The
     * logo is initially a loading circle, with the actual sprite downloaded
     * asynchronously
     */
    GEODE_DLL cocos2d::CCNode* createServerModLogo(std::string id);
}
