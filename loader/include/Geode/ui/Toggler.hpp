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

        /// Make a blank toggle.
        ///
        /// @param toggleCallback Called when the toggle changes.
        ///
        /// @example
        /// auto toggler = Toggler::create([](Toggler* sender, bool isToggled) {
        ///     log::info("Toggled: {}", isToggled);
        /// });
        static Toggler* create(TogglerCallback toggleCallback = nullptr);

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
        /// auto toggler = Toggler::createWithNodes(
        ///     offNode,
        ///     onNode,
        ///     [](Toggler* sender, bool isToggled) {
        ///         log::info("Toggled: {}", isToggled);
        ///     }
        /// );
        static Toggler* createWithNodes(
            cocos2d::CCNode* offNode,
            cocos2d::CCNode* onNode,
            TogglerCallback toggleCallback = nullptr
        );

        /// Make a toggle with GD's checkbox sprites.
        ///
        /// @param toggleCallback Called when the toggle changes.
        /// @param scale Checkbox scale.
        ///
        /// @example
        /// auto toggler = Toggler::createWithStandardSprites(
        ///     [](Toggler* sender, bool isToggled) {
        ///         log::info("Checkbox: {}", isToggled);
        ///     },
        ///     0.8f
        /// );
        static Toggler* createWithStandardSprites(
            TogglerCallback toggleCallback = nullptr,
            float scale = 1.0f
        );

        /// Make a toggle using sprites from files.
        ///
        /// @param offFileName Sprite shown when off.
        /// @param onFileName Sprite shown when on.
        /// @param toggleCallback Called when the toggle changes.
        /// @param scale Sprite scale.
        ///
        /// @example
        /// auto toggler = Toggler::createWithSprites(
        ///     "off.png"_spr,
        ///     "on.png"_spr,
        ///     [](Toggler* sender, bool isToggled) {
        ///         log::info("Custom toggle: {}", isToggled);
        ///     },
        ///     1.25f
        /// );
        static Toggler* createWithSprites(
            geode::ZStringView offFileName,
            geode::ZStringView onFileName,
            TogglerCallback toggleCallback = nullptr,
            float scale = 1.0f
        );

        /// Make a toggle using sprites from a sprite sheet.
        ///
        /// @param offFrameName Frame shown when off.
        /// @param onFrameName Frame shown when on.
        /// @param toggleCallback Called when the toggle changes.
        /// @param scale Sprite scale.
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
            TogglerCallback toggleCallback = nullptr,
            float scale = 1.0f
        );

        /// Check if the toggle is on.
        ///
        /// @example
        /// if (toggler->isToggled()) {
        ///     log::info("Feature is enabled");
        /// }
        bool isToggled() const;

        /// Set whether the toggle is on.
        ///
        /// @param toggled Whether the toggle should be on.
        /// @param triggerCallback Whether to call the toggle callback.
        ///
        /// @example
        /// toggler->setToggled(true);
        ///
        /// // Change the state without calling the callback.
        /// toggler->setToggled(false, false);
        void setToggled(bool toggled, bool triggerCallback = false);

        /// Toggle between the on and off states.
        ///
        /// @param triggerCallback Whether to call the toggle callback.
        ///
        /// @example
        /// toggler->toggle();
        ///
        /// // Toggle without calling the callback.
        /// toggler->toggle(false);
        void toggle(bool triggerCallback = true);

        /// Get the node shown when the toggle is off.
        ///
        /// @example
        /// auto offNode = toggler->getOffNode();
        /// offNode->setOpacity(200);
        cocos2d::CCNode* getOffNode() const;

        /// Change the node shown when the toggle is off.
        ///
        /// @param node New off node.
        ///
        /// @example
        /// auto offNode = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
        /// toggler->setOffNode(offNode);
        void setOffNode(cocos2d::CCNode* node);

        /// Get the node shown when the toggle is on.
        ///
        /// @example
        /// auto onNode = toggler->getOnNode();
        /// onNode->setScale(0.8f);
        cocos2d::CCNode* getOnNode() const;

        /// Change the node shown when the toggle is on.
        ///
        /// @param node New on node.
        ///
        /// @example
        /// auto onNode = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
        /// toggler->setOnNode(onNode);
        void setOnNode(cocos2d::CCNode* node);

        /// Change the callback.
        ///
        /// @param callback New callback.
        ///
        /// @example
        /// toggler->setToggleCallback([](Toggler* sender, bool isToggled) {
        ///     if (isToggled) {
        ///         log::info("Enabled");
        ///     } else {
        ///         log::info("Disabled");
        ///     }
        /// });
        void setToggleCallback(TogglerCallback callback);

        virtual void activate() override;

    protected:
        Toggler();
        virtual ~Toggler();

        bool init(TogglerCallback toggleCallback);

        bool initWithNodes(
            cocos2d::CCNode* offNode,
            cocos2d::CCNode* onNode,
            TogglerCallback toggleCallback
        );

        bool initWithSprites(
            geode::ZStringView offFileName,
            geode::ZStringView onFileName,
            TogglerCallback toggleCallback,
            float scale
        );

        bool initWithSpriteFrameNames(
            geode::ZStringView offFrameName,
            geode::ZStringView onFrameName,
            TogglerCallback toggleCallback,
            float scale
        );

        void updateVisuals();

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
    };
}