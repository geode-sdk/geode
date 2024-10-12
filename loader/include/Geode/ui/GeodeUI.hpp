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
    class GEODE_DLL ModPopupUIEvent final : public Event {
    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

        friend class ::ModPopup;

        ModPopupUIEvent(std::unique_ptr<Impl>&& impl);

    public:
        virtual ~ModPopupUIEvent();

        /**
         * Get the popup itself
         */
        FLAlertLayer* getPopup() const;
        /**
         * Get the ID of the mod this popup is for
         */
        std::string getModID() const;
        /**
         * If this popup is of an installed mod, get it
         */
        std::optional<Mod*> getMod() const;
    };

    /**
     * Event posted whenever a logo sprite is created for a mod. Allows mods to modify 
     * the Geode UI. See the [tutorial on Geode UI modification](https://docs.geode-sdk.org/tutorials/modify-geode) 
     * for **very important notes on these events**!
     */
    class GEODE_DLL ModItemUIEvent final : public Event {
    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

        friend class ::ModItem;

        ModItemUIEvent(std::unique_ptr<Impl>&& impl);

    public:
        virtual ~ModItemUIEvent();

        /**
         * Get the item itself
         */
        cocos2d::CCNode* getItem() const;
        /**
         * Get the ID of the mod this logo is for
         */
        std::string getModID() const;
        /**
         * If this logo is of an installed mod, get it
         */
        std::optional<Mod*> getMod() const;
    };

    /**
     * Event posted whenever a logo sprite is created for a mod. Allows mods to modify 
     * the Geode UI. See the [tutorial on Geode UI modification](https://docs.geode-sdk.org/tutorials/modify-geode) 
     * for **very important notes on these events**!
     */
    class GEODE_DLL ModLogoUIEvent final : public Event {
    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

        friend class ::ModLogoSprite;

        ModLogoUIEvent(std::unique_ptr<Impl>&& impl);

    public:
        virtual ~ModLogoUIEvent();

        /**
         * Get the sprite itself
         */
        cocos2d::CCNode* getSprite() const;
        /**
         * Get the ID of the mod this logo is for
         */
        std::string getModID() const;
        /**
         * If this logo is of an installed mod, get it
         */
        std::optional<Mod*> getMod() const;
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
     * @returns A Task that completes to `true` if the mod was found and a 
     * popup was opened, and `false` otherwise. If you wish to modify the 
     * created popup, listen for the Geode UI events listed in `GeodeUI.hpp`
     */
    GEODE_DLL Task<bool> openInfoPopup(std::string const& modID);
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
     * Open the settings popup for a mod (if it has any settings)
     * @param mod Mod the open the popup for
     * @param disableGeodeTheme If false, the popup follows the user's chosen 
     * theme options. If true, the popup is always in the GD theme (not Geode's 
     * dark purple colors)
     * @returns A pointer to the created Popup, or null if the mod has no 
     * settings
     */
    GEODE_DLL Popup<Mod*>* openSettingsPopup(Mod* mod, bool disableGeodeTheme);
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
