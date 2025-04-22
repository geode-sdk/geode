#pragma once

#include <Geode/loader/Mod.hpp>
#include <ccTypes.h>
#include "../DefaultInclude.hpp"
#include "../loader/Event.hpp"

namespace geode {
    struct GEODE_DLL ColorProvidedEvent final : public Event {
        std::string id;
        cocos2d::ccColor4B color;

        ColorProvidedEvent(std::string const& id, cocos2d::ccColor4B const& color);
    };

    class GEODE_DLL ColorProvidedFilter final : public EventFilter<ColorProvidedEvent> {
    public:
        using Callback = void(ColorProvidedEvent*);

    protected:
        std::string m_id;

    public:
        ListenerResult handle(std::function<Callback> fn, ColorProvidedEvent* event);

        ColorProvidedFilter(std::string const& id);
    };

    /**
     * GD has a lot of hardcoded colors. In addition, mods may very well also 
     * use hardcoded colors in their UIs, for example for CCLayerColors. This 
     * makes it hard for texture packs to reliably change colors, since they'd 
     * have to manually deal with every mod.
     * 
     * To help with this, Geode provides the `ColorProvider` class, which is 
     * just an index of colors with associated IDs. Mods should use this to 
     * define their hardcoded colors for their UIs, and texture packs can then 
     * change the color dynamically.
     * 
     * Mods are not expected to handle in-game color changes, since in nearly 
     * all cases the texture pack will be applied far from the mod's UI and as 
     * such just getting the defined color on layer enter will be more than 
     * enough. However, if the mod does add some static UI that won't be 
     * naturally refreshed after a texture pack is applied, it should listen 
     * for `ColorProvidedEvent`s to react accordingly.
     */
    class GEODE_DLL ColorProvider final {
    private:
        class Impl;

        Impl* m_impl;

        ColorProvider();

    public:
        // @note See class description
        static ColorProvider* get();

        /**
         * Define a new color with an associated ID. The ID should be prefixed 
         * with the mod ID. If the color has already been defined, nothing 
         * happens
         * @param id The ID of the color; should be prefixed with mod ID
         * @param color The color
         * @returns The current value of the color with the ID (same as the 
         * `color` function)
         */
        cocos2d::ccColor4B define(std::string const& id, cocos2d::ccColor4B const& color);
        /**
         * Define a new color with an associated ID. The ID should be prefixed 
         * with the mod ID. If the color has already been defined, nothing 
         * happens
         * @param id The ID of the color; should be prefixed with mod ID
         * @param color The color. Alpha component is assumed to be 255
         * @returns The current value of the color with the ID (same as the 
         * `color` function, although with the value truncated to cc3b)
         */
        cocos2d::ccColor3B define(std::string const& id, cocos2d::ccColor3B const& color);
        /**
         * Override the current value of a color with an associated ID
         * @param id The ID of the color
         * @param color The color to override with
         * @returns The new value of the color, or ccWHITE if the ID doesn't 
         * exist
         */
        cocos2d::ccColor4B override(std::string const& id, cocos2d::ccColor4B const& color);
        /**
         * Override the current value of a color with an associated ID
         * @param id The ID of the color
         * @param color The color to override with. Alpha component is assumed 
         * to be 255
         * @returns The new value of the color, or ccWHITE if the ID doesn't 
         * exist (truncated to cc3b)
         */
        cocos2d::ccColor3B override(std::string const& id, cocos2d::ccColor3B const& color);
        /**
         * Reset the current value of a color to its original definition
         * @param id The ID of the color
         * @returns The original value of the color, or ccWHITE if the ID 
         * doesn't exist
         */
        cocos2d::ccColor4B reset(std::string const& id);
        /**
         * Get the current value of a color
         * @param id The ID of the color
         * @returns The value of the color, or ccWHITE if the ID doesn't exist
         */
        cocos2d::ccColor4B color(std::string const& id) const;
        /**
         * Get the current value of a color as a ccColor3B
         * @param id The ID of the color
         * @returns The value of the color, or ccWHITE if the ID doesn't exist
         */
        cocos2d::ccColor3B color3b(std::string const& id) const;
    };
}

GEODE_HIDDEN inline cocos2d::ccColor4B operator""_cc4b_gd(const char* str, size_t) {
    return geode::ColorProvider::get()->color(str);
}
GEODE_HIDDEN inline cocos2d::ccColor3B operator""_cc3b_gd(const char* str, size_t) {
    return geode::ColorProvider::get()->color3b(str);
}
GEODE_HIDDEN inline cocos2d::ccColor4B operator""_cc4b(const char* str, size_t) {
    return geode::ColorProvider::get()->color(std::string(geode::Mod::get()->expandSpriteName(str)));
}
GEODE_HIDDEN inline cocos2d::ccColor3B operator""_cc3b(const char* str, size_t) {
    return geode::ColorProvider::get()->color3b(std::string(geode::Mod::get()->expandSpriteName(str)));
}
