#pragma once

#include <cocos2d.h>

#pragma warning(disable: 4275)

namespace geode {
    // all of these are explicitly numbered so if we add a new size / color in 
    // the future we can slot it in reasonably and just set the number to be at 
    // the end so backwards compatability stays
    // (for example TinyAlt = 8 to CircleBaseSize)

    enum class CircleBaseSize {
        Tiny = 0,      // Equivalent to the tiny delete button
        Small = 1,     // Equivalent to most circular buttons in the editor
        SmallAlt = 2,  // Equivalent to the trash button in the editor
        Medium = 3,    // Equivalent to most buttons
        MediumAlt = 4, // Equivalent to the bottom buttons in MenuLayer
        Big = 5,       // Equivalent to the New button
        BigAlt = 6,    // Equivalent to the Account button
        Large = 7,     // Equivalent to the big Play Button
    };
    GEODE_DLL const char* baseEnumToString(CircleBaseSize);

    enum class CircleBaseColor {
        Green = 0,
        Pink = 1,
        Gray = 2,
        Blue = 3,
        Cyan = 4,
        Geode = 5,
    };
    GEODE_DLL const char* baseEnumToString(CircleBaseColor);

    enum class CrossBaseSize {
        Small = 0,  // the size of the icon kit / create button
        Huge = 1,   // the size of the play button
    };
    GEODE_DLL const char* baseEnumToString(CrossBaseSize);

    enum class CrossBaseColor {
        Green = 0,
    };
    GEODE_DLL const char* baseEnumToString(CrossBaseColor);

    enum class AccountBaseSize {
        Normal = 0,
    };
    GEODE_DLL const char* baseEnumToString(AccountBaseSize);

    enum class AccountBaseColor {
        Blue = 0,
        Gray = 1,
        Purple = 2,
    };
    GEODE_DLL const char* baseEnumToString(AccountBaseColor);

    enum class IconSelectBaseSize {
        Normal = 0,
    };
    GEODE_DLL const char* baseEnumToString(IconSelectBaseSize);

    enum class IconSelectBaseColor {
        Unselected = 0,
        Selected = 1,
    };
    GEODE_DLL const char* baseEnumToString(IconSelectBaseColor);

    enum class EditorBaseSize {
        Normal = 0,
    };
    GEODE_DLL const char* baseEnumToString(EditorBaseSize);

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
    GEODE_DLL const char* baseEnumToString(EditorBaseColor);

    enum class TabBaseSize {
        Normal = 0,
    };
    GEODE_DLL const char* baseEnumToString(TabBaseSize);

    enum class TabBaseColor {
        Unselected = 0,
        Selected = 1,
        UnselectedDark = 2,
    };
    GEODE_DLL const char* baseEnumToString(TabBaseColor);

    enum class LeaderboardBaseSize {
        Normal = 0,
    };
    GEODE_DLL const char* baseEnumToString(LeaderboardBaseSize);

    enum class LeaderboardBaseColor {
        Blue = 0,
    };
    GEODE_DLL const char* baseEnumToString(LeaderboardBaseColor);

    enum class CategoryBaseSize {
        Big = 0,
    };
    GEODE_DLL const char* baseEnumToString(CategoryBaseSize);

    enum class CategoryBaseColor {
        Green = 0,
    };
    GEODE_DLL const char* baseEnumToString(CategoryBaseColor);

    enum class BaseType {
        // Circle buttons
        Circle = 0,
        // Like the buttons in the main menu
        Cross = 1,
        // Like the friend request / account buttons
        Account = 2,
        // Like the icon select button
        IconSelect = 3,
        // Like the leaderboard buttons
        Leaderboard = 4,
        // Like the buttons in the editor sidebar
        Editor = 5,
        // Like a list view tab
        Tab = 6,
        // Like the buttons in CreatorLayer
        Category = 7,
    };
    GEODE_DLL const char* baseEnumToString(BaseType);

    /**
     * Represents a GD button sprite where there's
     * an icon sprite on top another default sprite.
     * You know, it has a base. It's based.
     * lmao trademark lizbith
     */
    class GEODE_DLL BasedButtonSprite : public cocos2d::CCSprite {
    protected:
        BaseType m_type;
        int m_size;
        int m_color;
        cocos2d::CCNode* m_onTop = nullptr;

        bool init(cocos2d::CCNode* ontop, BaseType type, int size, int color);
        bool initWithSprite(
            const char* sprName, float sprScale, BaseType type, int size, int color
        );
        bool initWithSpriteFrameName(
            const char* sprName, float sprScale, BaseType type, int size, int color
        );

        cocos2d::CCPoint getTopOffset() const;
        virtual cocos2d::CCSize getMaxTopSize() const;

        virtual ~BasedButtonSprite();

    public:
        /**
         * Create a BasedButtonSprite. In general, it is recommended to use 
         * the specialized subclasses, such as CircleButtonSprite or 
         * TabButtonSprite
         * @param ontop The top node of the sprite
         * @param type The type of the base sprite
         * @param size The size of the base sprite. Cast this from the base 
         * type's respective BaseSize enum
         * @param color The color of the base sprite. Cast this from the base 
         * type's respective BaseColor enum
         * @returns Created BasedButtonSprite, or nullptr if parameters were 
         * invalid 
         */
        static BasedButtonSprite* create(
            cocos2d::CCNode* ontop, BaseType type, int size, int color
        );

        cocos2d::CCNode* getTopNode() const;
    };

    /**
     * Creates a button sprite with a circular base, similar to the buttons at 
     * the bottom of the main menu
     */
    class GEODE_DLL CircleButtonSprite : public BasedButtonSprite {
    public:
        static CircleButtonSprite* create(
            cocos2d::CCNode* top, CircleBaseColor color = CircleBaseColor::Green,
            CircleBaseSize size = CircleBaseSize::Medium
        );
        static CircleButtonSprite* createWithSprite(
            const char* sprName, float sprScale = 1.f,
            CircleBaseColor color = CircleBaseColor::Green,
            CircleBaseSize size = CircleBaseSize::Medium
        );
        static CircleButtonSprite* createWithSpriteFrameName(
            const char* sprName, float sprScale = 1.f,
            CircleBaseColor color = CircleBaseColor::Green,
            CircleBaseSize size = CircleBaseSize::Medium
        );
    };

    /**
     * Creates a button sprite with a cross base, like the buttons in the main 
     * menu
     */
    class GEODE_DLL CrossButtonSprite : public BasedButtonSprite {
    public:
        static CrossButtonSprite* create(
            cocos2d::CCNode* top,
            CrossBaseColor color = CrossBaseColor::Green,
            CrossBaseSize size = CrossBaseSize::Small
        );
        static CrossButtonSprite* createWithSprite(
            const char* sprName, float sprScale = 1.f,
            CrossBaseColor color = CrossBaseColor::Green,
            CrossBaseSize size = CrossBaseSize::Small
        );
        static CrossButtonSprite* createWithSpriteFrameName(
            const char* sprName, float sprScale = 1.f,
            CrossBaseColor color = CrossBaseColor::Green,
            CrossBaseSize size = CrossBaseSize::Small
        );
    };

    /**
     * Creates a button sprite with a cross base, like the buttons in the main 
     * menu
     */
    class GEODE_DLL AccountButtonSprite : public BasedButtonSprite {
    public:
        static AccountButtonSprite* create(
            cocos2d::CCNode* top,
            AccountBaseColor color = AccountBaseColor::Blue,
            AccountBaseSize size = AccountBaseSize::Normal
        );
        static AccountButtonSprite* createWithSprite(
            const char* sprName, float sprScale = 1.f,
            AccountBaseColor color = AccountBaseColor::Blue,
            AccountBaseSize size = AccountBaseSize::Normal
        );
        static AccountButtonSprite* createWithSpriteFrameName(
            const char* sprName, float sprScale = 1.f,
            AccountBaseColor color = AccountBaseColor::Blue,
            AccountBaseSize size = AccountBaseSize::Normal
        );
    };

    /**
     * Creates a button sprite with a cross base, like the buttons in the main 
     * menu
     */
    class GEODE_DLL IconSelectButtonSprite : public BasedButtonSprite {
    public:
        static IconSelectButtonSprite* create(
            cocos2d::CCNode* top,
            IconSelectBaseColor color = IconSelectBaseColor::Unselected,
            IconSelectBaseSize size = IconSelectBaseSize::Normal
        );
        static IconSelectButtonSprite* createWithSprite(
            const char* sprName, float sprScale = 1.f,
            IconSelectBaseColor color = IconSelectBaseColor::Unselected,
            IconSelectBaseSize size = IconSelectBaseSize::Normal
        );
        static IconSelectButtonSprite* createWithSpriteFrameName(
            const char* sprName, float sprScale = 1.f,
            IconSelectBaseColor color = IconSelectBaseColor::Unselected,
            IconSelectBaseSize size = IconSelectBaseSize::Normal
        );
    };

    /**
     * Creates a button sprite with a cross base, like the buttons in the main 
     * menu
     */
    class GEODE_DLL LeaderboardButtonSprite : public BasedButtonSprite {
    public:
        static LeaderboardButtonSprite* create(
            cocos2d::CCNode* top,
            LeaderboardBaseColor color = LeaderboardBaseColor::Blue,
            LeaderboardBaseSize size = LeaderboardBaseSize::Normal
        );
        static LeaderboardButtonSprite* createWithSprite(
            const char* sprName, float sprScale = 1.f,
            LeaderboardBaseColor color = LeaderboardBaseColor::Blue,
            LeaderboardBaseSize size = LeaderboardBaseSize::Normal
        );
        static LeaderboardButtonSprite* createWithSpriteFrameName(
            const char* sprName, float sprScale = 1.f,
            LeaderboardBaseColor color = LeaderboardBaseColor::Blue,
            LeaderboardBaseSize size = LeaderboardBaseSize::Normal
        );
    };

    /**
     * Creates a button sprite with the same base as the right-side action 
     * buttons in the editor
     */
    class GEODE_DLL EditorButtonSprite : public BasedButtonSprite {
    public:
        static EditorButtonSprite* create(
            cocos2d::CCNode* top, EditorBaseColor color,
            EditorBaseSize size = EditorBaseSize::Normal
        );
        static EditorButtonSprite* createWithSprite(
            const char* sprName, float sprScale = 1.f,
            EditorBaseColor color = EditorBaseColor::Green,
            EditorBaseSize size = EditorBaseSize::Normal
        );
        static EditorButtonSprite* createWithSpriteFrameName(
            const char* sprName, float sprScale = 1.f,
            EditorBaseColor color = EditorBaseColor::Green,
            EditorBaseSize size = EditorBaseSize::Normal
        );
        cocos2d::CCSize getMaxTopSize() const override;
    };

    /**
     * Creates a button sprite for list view tabs
     */
    class GEODE_DLL TabButtonSprite : public BasedButtonSprite {
    public:
        static TabButtonSprite* create(
            const char* text,
            TabBaseColor color,
            TabBaseSize size = TabBaseSize::Normal
        );
    };

    /**
     * Creates a button sprite for category buttons, i.e. the big buttons in 
     * CreatorLayer
     */
    class GEODE_DLL CategoryButtonSprite : public BasedButtonSprite {
    public:
        static CategoryButtonSprite* create(
            cocos2d::CCNode* top,
            CategoryBaseColor color = CategoryBaseColor::Green,
            CategoryBaseSize size = CategoryBaseSize::Big
        );
        static CategoryButtonSprite* createWithSprite(
            const char* sprName, float sprScale = 1.f,
            CategoryBaseColor color = CategoryBaseColor::Green,
            CategoryBaseSize size = CategoryBaseSize::Big
        );
        static CategoryButtonSprite* createWithSpriteFrameName(
            const char* sprName, float sprScale = 1.f,
            CategoryBaseColor color = CategoryBaseColor::Green,
            CategoryBaseSize size = CategoryBaseSize::Big
        );
    };
}
