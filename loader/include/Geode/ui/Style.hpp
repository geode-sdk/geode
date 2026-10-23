#pragma once

#include "General.hpp"
#include "IconButtonSprite.hpp"
#include "Popup.hpp"

#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/utils/ZStringView.hpp>

namespace geode {
    enum class GeodePopupStyle {
        Default,
        Alt,
        Alt2,
    };

    GEODE_DLL bool isGeodeTheme(bool forceDisableTheme = false);

    class GEODE_DLL GeodePopup : public Popup {
    protected:
        bool m_forceDisableTheme = false;

        bool init(
            float width, float height, GeodePopupStyle style = GeodePopupStyle::Default,
            bool forceDisableTheme = false
        );
    };

    class GEODE_DLL GeodeSquareSprite final : public cocos2d::CCSprite {
        class Impl;
        std::unique_ptr<Impl> m_impl;

    protected:
        GeodeSquareSprite();
        ~GeodeSquareSprite();

        bool init(CCSprite* top, bool* state, bool forceDisableTheme = false);

        void update(float dt) override;
        void updateImage();

    public:
        static GeodeSquareSprite* create(
            ZStringView top, bool* state = nullptr, bool forceDisableTheme = false
        );
        static GeodeSquareSprite* createWithSpriteFrameName(
            ZStringView top, bool* state = nullptr, bool forceDisableTheme = false
        );

        cocos2d::CCSprite* getTopSprite() const;
        void setState(bool state);
    };

    GEODE_DLL cocos2d::CCNode* createLoadingCircle(
        float sideLength, ZStringView id = "loading-spinner"
    );

    enum class GeodeButtonSprite {
        Default,
        Install,
        Delete,
        Enable,
        Gray,
    };

    GEODE_DLL ZStringView
    getGeodeButtonSpriteName(GeodeButtonSprite spr, bool forceDisableTheme = false);

    GEODE_DLL IconButtonSprite* createGeodeButton(
        cocos2d::CCNode* icon, ZStringView text, GeodeButtonSprite bg = GeodeButtonSprite::Default,
        bool forceDisableTheme = false
    );
    GEODE_DLL IconButtonSprite* createGeodeButton(
        cocos2d::CCNode* icon, ZStringView text, bool gold,
        GeodeButtonSprite bg = GeodeButtonSprite::Default, bool forceDisableTheme = false
    );
    GEODE_DLL ButtonSprite* createGeodeButton(
        ZStringView text, int width, bool absolute = false, bool gold = false,
        GeodeButtonSprite bg = GeodeButtonSprite::Default, bool forceDisableTheme = false
    );
    GEODE_DLL ButtonSprite* createGeodeButton(
        ZStringView text, bool gold = false, GeodeButtonSprite bg = GeodeButtonSprite::Default,
        bool forceDisableTheme = false
    );

    GEODE_DLL CircleButtonSprite* createGeodeCircleButton(
        cocos2d::CCSprite* top, float scale = 1.f, CircleBaseSize size = CircleBaseSize::Medium,
        bool altColor = false, bool forceDisableTheme = false
    );

    GEODE_DLL ListBorders* createGeodeListBorders(
        cocos2d::CCSize const& size, bool forceDisableTheme = false
    );

    class GEODE_DLL GeodeTabSprite final : public cocos2d::CCNode {
        class Impl;
        std::unique_ptr<Impl> m_impl;

    protected:
        GeodeTabSprite();
        ~GeodeTabSprite();

        bool init(ZStringView iconFrame, std::string text, float width, bool altColor);

    public:
        static GeodeTabSprite* create(
            ZStringView iconFrame, std::string text, float width, bool altColor = false
        );

        void select(bool selected);
        void disable(bool disabled);
    };
};