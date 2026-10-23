#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/ui/General.hpp>
#include <Geode/ui/IconButtonSprite.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/ZStringView.hpp>

namespace geode {
    enum class GeodePopupStyle {
        Default,
        Alt,
        Alt2,
    };

    bool isGeodeTheme(bool forceDisableTheme = false);

    class GeodePopup : public Popup {
    protected:
        bool m_forceDisableTheme = false;

        bool init(
            float width, float height, GeodePopupStyle style = GeodePopupStyle::Default,
            bool forceDisableTheme = false
        );
    };

    class GeodeSquareSprite final : public cocos2d::CCSprite {
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

    cocos2d::CCNode* createLoadingCircle(float sideLength, ZStringView id = "loading-spinner");

    enum class GeodeButtonSprite {
        Default,
        Install,
        Delete,
        Enable,
        Gray,
    };
    ZStringView getGeodeButtonSpriteName(GeodeButtonSprite spr, bool forceDisableTheme = false);
    IconButtonSprite* createGeodeButton(
        cocos2d::CCNode* icon, ZStringView text, GeodeButtonSprite bg = GeodeButtonSprite::Default,
        bool forceDisableTheme = false
    );
    IconButtonSprite* createGeodeButton(
        cocos2d::CCNode* icon, ZStringView text, bool gold,
        GeodeButtonSprite bg = GeodeButtonSprite::Default, bool forceDisableTheme = false
    );
    ButtonSprite* createGeodeButton(
        ZStringView text, int width, bool absolute = false, bool gold = false,
        GeodeButtonSprite bg = GeodeButtonSprite::Default, bool forceDisableTheme = false
    );
    ButtonSprite* createGeodeButton(
        ZStringView text, bool gold = false, GeodeButtonSprite bg = GeodeButtonSprite::Default,
        bool forceDisableTheme = false
    );

    CircleButtonSprite* createGeodeCircleButton(
        cocos2d::CCSprite* top, float scale = 1.f, CircleBaseSize size = CircleBaseSize::Medium,
        bool altColor = false, bool forceDisableTheme = false
    );

    ListBorders* createGeodeListBorders(cocos2d::CCSize const& size, bool forceDisableTheme = false);

    class GeodeTabSprite final : public cocos2d::CCNode {
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

#include <server/Server.hpp>

ButtonSprite* createGeodeTagLabel(server::ServerTag const& tag);
std::pair<cocos2d::ccColor3B, cocos2d::ccColor3B> geodeTagColors(server::ServerTag const& tag);

ButtonSprite* createTagLabel(
    ZStringView text, std::pair<cocos2d::ccColor3B, cocos2d::ccColor3B> const& color
);
IconButtonSprite* createTagLabelWithIcon(
    cocos2d::CCNode* icon, ZStringView text,
    std::pair<cocos2d::ccColor3B, cocos2d::ccColor3B> const& color
);