#pragma once

#include <Geode/utils/ZStringView.hpp>
#include <Geode/utils/function.hpp>
#include <Geode/ui/Button.hpp>
#include <Geode/cocos/base_nodes/CCNode.h>

namespace geode {
    /// A customizable toggle button for interactive on/off controls.
    ///
    /// Supports custom visuals, state management, and callbacks.
    class GEODE_DLL Toggler : public Button {
    public:
        using TogglerCallback = geode::Function<void(Toggler* sender, bool isToggled)>;

        /// Make a toggle with custom on/off nodes.
        ///
        /// @param offNode Node shown when off.
        /// @param onNode Node shown when on.
        /// @param toggleCallback Called when the toggle changes.
        ///
        /// @example
        /// auto offNode = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
        /// auto onNode = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
        ///
        /// auto toggler = Toggler::create(
        ///     offNode,
        ///     onNode,
        ///     [](Toggler* sender, bool isToggled) {
        ///         log::info("Toggled: {}", isToggled);
        ///     }
        /// );
        static Toggler* create(
            cocos2d::CCNode* offNode,
            cocos2d::CCNode* onNode,
            TogglerCallback toggleCallback = nullptr
        );

        /// Make a toggle with GD's standard checkbox sprites.
        ///
        /// @param toggleCallback Called when the toggle changes.
        ///
        /// @example
        /// auto toggler = Toggler::createWithStandardSprites(
        ///     [](Toggler* sender, bool isToggled) {
        ///         log::info("Checkbox: {}", isToggled);
        ///     }
        /// );
        static Toggler* createWithStandardSprites(
            TogglerCallback toggleCallback = nullptr
        );

        /// Make a toggle with GD's standard checkbox sprites.
        ///
        /// @param scale Checkbox scale.
        /// @param toggleCallback Called when the toggle changes.
        static Toggler* createWithStandardSprites(
            float scale,
            TogglerCallback toggleCallback = nullptr
        );

        /// Make a toggle using sprites from files.
        ///
        /// @param offFileName Sprite shown when off.
        /// @param onFileName Sprite shown when on.
        /// @param toggleCallback Called when the toggle changes.
        ///
        /// @example
        /// auto toggler = Toggler::createWithSprites(
        ///     "off.png"_spr,
        ///     "on.png"_spr,
        ///     [](Toggler* sender, bool isToggled) {
        ///         log::info("Custom toggle: {}", isToggled);
        ///     }
        /// );
        static Toggler* createWithSprites(
            geode::ZStringView offFileName,
            geode::ZStringView onFileName,
            TogglerCallback toggleCallback = nullptr
        );

        /// Make a toggle using sprites from files.
        ///
        /// @param offFileName Sprite shown when off.
        /// @param onFileName Sprite shown when on.
        /// @param scale Sprite scale.
        /// @param toggleCallback Called when the toggle changes.
        static Toggler* createWithSprites(
            geode::ZStringView offFileName,
            geode::ZStringView onFileName,
            float scale,
            TogglerCallback toggleCallback = nullptr
        );

        /// Make a toggle using sprites from a sprite sheet.
        ///
        /// @param offFrameName Frame shown when off.
        /// @param onFrameName Frame shown when on.
        /// @param toggleCallback Called when the toggle changes.
        ///
        /// @example
        /// auto toggler = Toggler::createWithSpriteFrameNames(
        ///     "GJ_checkOff_001.png"_spr,
        ///     "GJ_checkOn_001.png"_spr,
        ///     [](Toggler* sender, bool isToggled) {
        ///         log::info("Checkbox state changed: {}", isToggled);
        ///     }
        /// );
        static Toggler* createWithSpriteFrameNames(
            geode::ZStringView offFrameName,
            geode::ZStringView onFrameName,
            TogglerCallback toggleCallback = nullptr
        );

        /// Make a toggle using sprites from a sprite sheet.
        ///
        /// @param offFrameName Frame shown when off.
        /// @param onFrameName Frame shown when on.
        /// @param scale Sprite scale.
        /// @param toggleCallback Called when the toggle changes.
        static Toggler* createWithSpriteFrameNames(
            geode::ZStringView offFrameName,
            geode::ZStringView onFrameName,
            float scale,
            TogglerCallback toggleCallback = nullptr
        );

        /// Check if the toggle is on.
        bool isToggled() const;

        /// Set whether the toggle is on.
        ///
        /// @param toggled Whether the toggle should be on.
        /// @param triggerCallback Whether to trigger the callback if the state changes.
        void setToggled(bool toggled, bool triggerCallback = false);

        /// Toggle between the on and off states.
        ///
        /// @param triggerCallback Whether to trigger the callback.
        void toggle(bool triggerCallback = true);

        /// Get the node shown when the toggle is off.
        cocos2d::CCNode* getOffNode() const;

        /// Change the node shown when the toggle is off.
        ///
        /// @param node Node to show when off.
        void setOffNode(cocos2d::CCNode* node);

        /// Get the node shown when the toggle is on.
        cocos2d::CCNode* getOnNode() const;

        /// Change the node shown when the toggle is on.
        ///
        /// @param node Node to show when on.
        void setOnNode(cocos2d::CCNode* node);

        /// Change the callback.
        ///
        /// @param callback Callback to trigger when the toggle changes.
        void setToggleCallback(TogglerCallback callback);

        void activate() override;

    protected:
        Toggler();
        ~Toggler();

        bool init(
            cocos2d::CCNode* offNode,
            cocos2d::CCNode* onNode,
            TogglerCallback toggleCallback
        );

        void updateDisplay();

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
