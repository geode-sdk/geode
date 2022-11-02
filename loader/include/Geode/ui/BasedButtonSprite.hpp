#pragma once

#include <cocos2d.h>

namespace geode {
    enum class CircleBaseSize {
        Tiny = 0, // Equivalent to the tiny delete button
        Small = 1, // Equivalent to most circular buttons in the editor
        Small2 = 2, // Equivalent to the trash button in the editor
        Medium = 3, // Equivalent to most buttons
        Medium2 = 4, // Equivalent to the bottom buttons in MenuLayer
        Big = 5, // Equivalent to the New button
        Big2 = 6, // Equivalent to the Account button
        Large = 7, // Equivalent to the big Play Button
    };

    enum class CrossBaseSize {
        Small = 0,
        Huge = 1,
    };

    enum class CircleBaseColor {
        Green = 0,
        Pink = 1,
        Gray = 2,
        Blue = 3,
        Cyan = 4,
        Geode = 5,
    };

    enum class AccountBaseColor {
        Blue = 0,
        Gray = 1,
        Purple = 2,
    };

    enum class IconSelectBaseColor {
        Gray = 0,
        Selected = 1,
    };

    enum class EditorBaseColor {
        LightBlue = 0,
        Green = 1,
        Orange = 2,
        DarkGray = 3,
        Gray = 4,
        Pink = 5,
        Teal = 6,
        Aqua = 7,
        Cyan = 8,
    };

    enum class TabBaseColor {
        Unselected = 0,
        Selected = 1,
        UnselectedDark = 2,
    };

    enum class BaseType {
        // Circle buttons
        Circle = 0,
        // Like the buttons in the main menu
        Cross = 1,
        // Like the friend request / account buttons
        BevelledSquare = 2,
        // Like the icon select button
        IconSelect = 3,
        // Like the leaderboard buttons
        BevelledSquare2 = 4,
        // Like the buttons in the editor sidebar
        Editor = 5,
        // Like a list view tab
        Tab = 6,
        // Like the buttons in CreatorLayer
        Category = 7,
    };

    /**
     * Represents a GD button sprite where there's
     * an icon sprite on top another default sprite.
     * You know, it has a base. It's based.
     * lmao trademark lizbith
     */
    class GEODE_DLL BasedButtonSprite : public cocos2d::CCSprite {
    protected:
        int m_type;
        int m_size;
        int m_color;
        cocos2d::CCNode* m_onTop = nullptr;

        bool init(cocos2d::CCNode* ontop, int type, int size, int color);
        bool initWithSprite(char const* sprName, float sprScale, int type, int size, int color);
        bool initWithSpriteFrameName(
            char const* sprName, float sprScale, int type, int size, int color
        );

        cocos2d::CCPoint getTopOffset() const;

        virtual ~BasedButtonSprite();

    public:
        static BasedButtonSprite* create(cocos2d::CCNode* ontop, int type, int size, int color);

        cocos2d::CCNode* getTopNode() const;
    };

    class GEODE_DLL CircleButtonSprite : public BasedButtonSprite {
    public:
        static CircleButtonSprite* create(
            cocos2d::CCNode* top, CircleBaseColor color = CircleBaseColor::Green,
            CircleBaseSize size = CircleBaseSize::Medium
        );
        static CircleButtonSprite* createWithSprite(
            char const* sprName, float sprScale = 1.f,
            CircleBaseColor color = CircleBaseColor::Green,
            CircleBaseSize size = CircleBaseSize::Medium
        );
        static CircleButtonSprite* createWithSpriteFrameName(
            char const* sprName, float sprScale = 1.f,
            CircleBaseColor color = CircleBaseColor::Green,
            CircleBaseSize size = CircleBaseSize::Medium
        );
    };

    class GEODE_DLL EditorButtonSprite : public BasedButtonSprite {
    public:
        static EditorButtonSprite* create(cocos2d::CCNode* top, EditorBaseColor color);
        static EditorButtonSprite* createWithSprite(
            char const* sprName, float sprScale = 1.f,
            EditorBaseColor color = EditorBaseColor::Green
        );
        static EditorButtonSprite* createWithSpriteFrameName(
            char const* sprName, float sprScale = 1.f,
            EditorBaseColor color = EditorBaseColor::Green
        );
    };

    class GEODE_DLL TabButtonSprite : public BasedButtonSprite {
    public:
        static TabButtonSprite* create(char const* text, TabBaseColor color);
    };
}
