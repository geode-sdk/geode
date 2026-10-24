#pragma once

#include <Geode/utils/ZStringView.hpp>
#include <Geode/utils/function.hpp>
#include <Geode/ui/Button.hpp>

namespace geode {
    /// A toggle button that supports on/off visuals and state callbacks.
    class GEODE_DLL Toggler : public Button {
    public:
        using ToggleCallback = geode::Function<void(Toggler* sender, bool isToggled)>;

        /// Creates a toggle with on/off nodes.
        ///
        /// @param offNode Node to display when off.
        /// @param onNode Node to display when on.
        /// @param toggleCallback Callback invoked when the toggle changes.
        ///
        /// @example
        /// auto offNode = CCSprite::create("toggle_off.png"_spr);
        /// auto onNode = CCSprite::create("toggle_on.png"_spr);
        ///
        /// auto toggler = Toggler::create(
        ///     offNode,
        ///     onNode,
        ///     [](Toggler* sender, bool isToggled) {
        ///         log::info("Toggle state: {}", isToggled);
        ///     }
        /// );
        static Toggler* create(
            cocos2d::CCNode* offNode,
            cocos2d::CCNode* onNode,
            ToggleCallback toggleCallback = nullptr
        );

        /// Creates a toggle with GD's standard checkbox sprites.
        ///
        /// @param toggleCallback Callback invoked when the toggle changes.
        ///
        /// @example
        /// auto toggler = Toggler::createWithStandardSprites(
        ///     [](Toggler* sender, bool isToggled) {
        ///         log::info("Toggle state: {}", isToggled);
        ///     }
        /// );
        static Toggler* createWithStandardSprites(
            ToggleCallback toggleCallback = nullptr
        );

        /// Creates a toggle with GD's standard checkbox sprites.
        ///
        /// @param scale Checkbox scale.
        /// @param toggleCallback Callback invoked when the toggle changes.
        static Toggler* createWithStandardSprites(
            float scale,
            ToggleCallback toggleCallback = nullptr
        );

        /// Creates a toggle using sprites from files.
        ///
        /// @param offFileName Sprite file to display when off.
        /// @param onFileName Sprite file to display when on.
        /// @param toggleCallback Callback invoked when the toggle changes.
        ///
        /// @example
        /// auto toggler = Toggler::createWithSprites(
        ///     "toggle_off.png"_spr,
        ///     "toggle_on.png"_spr,
        ///     [](Toggler* sender, bool isToggled) {
        ///         log::info("Toggle state: {}", isToggled);
        ///     }
        /// );
        static Toggler* createWithSprites(
            geode::ZStringView offFileName,
            geode::ZStringView onFileName,
            ToggleCallback toggleCallback = nullptr
        );

        /// Creates a toggle using sprites from files.
        ///
        /// @param offFileName Sprite file to display when off.
        /// @param onFileName Sprite file to display when on.
        /// @param scale Sprite scale.
        /// @param toggleCallback Callback invoked when the toggle changes.
        static Toggler* createWithSprites(
            geode::ZStringView offFileName,
            geode::ZStringView onFileName,
            float scale,
            ToggleCallback toggleCallback = nullptr
        );

        /// Creates a toggle using sprite frames.
        ///
        /// @param offFrameName Frame to display when off.
        /// @param onFrameName Frame to display when on.
        /// @param toggleCallback Callback invoked when the toggle changes.
        ///
        /// @example
        /// auto toggler = Toggler::createWithSpriteFrameNames(
        ///     "toggle_off.png"_spr,
        ///     "toggle_on.png"_spr,
        ///     [](Toggler* sender, bool isToggled) {
        ///         log::info("Toggle state: {}", isToggled);
        ///     }
        /// );
        static Toggler* createWithSpriteFrameNames(
            geode::ZStringView offFrameName,
            geode::ZStringView onFrameName,
            ToggleCallback toggleCallback = nullptr
        );

        /// Creates a toggle using sprite frames.
        ///
        /// @param offFrameName Frame to display when off.
        /// @param onFrameName Frame to display when on.
        /// @param scale Sprite scale.
        static Toggler* createWithSpriteFrameNames(
            geode::ZStringView offFrameName,
            geode::ZStringView onFrameName,
            float scale,
            ToggleCallback toggleCallback = nullptr
        );

        /// Check the toggle's state.
        bool isToggled() const;

        /// Set the toggle's state.
        ///
        /// @param toggled Whether the toggle should be on.
        /// @param triggerCallback Whether to invoke the callback if the state changes.
        void setToggled(bool toggled, bool triggerCallback = false);

        /// Toggle between the on and off states.
        ///
        /// @param triggerCallback Whether to invoke the callback after toggling.
        void toggle(bool triggerCallback = false);

        /// Get the node displayed when the toggle is off.
        cocos2d::CCNode* getOffNode() const;

        /// Change the node displayed when the toggle is off.
        ///
        /// @param node Node to display when off.
        void setOffNode(cocos2d::CCNode* node);

        /// Get the node displayed when the toggle is on.
        cocos2d::CCNode* getOnNode() const;

        /// Change the node displayed when the toggle is on.
        ///
        /// @param node Node to display when on.
        void setOnNode(cocos2d::CCNode* node);

        /// Change the callback.
        ///
        /// @param callback Callback to invoke when the toggle changes.
        void setToggleCallback(ToggleCallback callback);

        void activate() override;

    protected:
        Toggler();
        ~Toggler();

        bool init(
            cocos2d::CCNode* offNode,
            cocos2d::CCNode* onNode,
            ToggleCallback toggleCallback
        );

        void updateDisplay();

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
