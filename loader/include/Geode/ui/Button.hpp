#pragma once

#include <Geode/utils/ZStringView.hpp>
#include <cocos2d.h>

namespace geode {
    class GEODE_DLL Button : public cocos2d::CCNodeRGBA, public cocos2d::CCTouchDelegate {
    public:
        using ButtonCallback = geode::Function<void(Button* sender)>;

        enum class AnimationType {
            // No animations at all
            None,
            // Uses custom animations
            Custom,
            // Scales the button by a multiplier
            Scale,
            // Moves the button by an offset
            Move
        };

        /**
         * Create a Button with no nodes on it
         * You will need to add a child yourself and position accordingly.
         * @param activateCallback The callback for when the button is activated
         */
        static Button* create(ButtonCallback activateCallback = nullptr);

        /**
         * Create a Button with a nodes on it.
         * The node will be automatically positioned in the center.
         * @param node The node that will show on the button.
         * @param activateCallback The callback for when the button is activated
         */
        static Button* createWithNode(cocos2d::CCNode* node, ButtonCallback activateCallback = nullptr);

        /**
         * Create a Button with a sprite by file name
         * @param fileName The file name of the sprite (not in a spritesheet)
         * @param activateCallback The callback for when the button is activated
         */
        static Button* createWithSprite(geode::ZStringView fileName, ButtonCallback activateCallback = nullptr);

        /**
         * Create a Button with a sprite by frame name
         * @param fileName The frame name of the sprite (in a spritesheet)
         * @param activateCallback The callback for when the button is activated
         */
        static Button* createWithSpriteFrameName(geode::ZStringView frameName, ButtonCallback activateCallback = nullptr);

        /**
         * Create a Button with a label
         * @param text The text shown on the label
         * @param font The font of the label
         * @param activateCallback The callback for when the button is activated
         */
        static Button* createWithLabel(geode::ZStringView text, geode::ZStringView font, ButtonCallback activateCallback = nullptr);

        /**
         * Get the Display Node, which was created or passed in on button creation
         * Will be nullptr if you use the empty create method
         */
        cocos2d::CCNode* getDisplayNode();

        /**
         * Set an animation type, which dictates how the button moves or scales
         * or if it even has an animation at all
         */
        void setAnimationType(AnimationType type);

        /**
         * Set a click animation. This will override the AnimationType and set it
         * to Custom. Will run every time the button is pressed
         */
        void setClickAnimation(cocos2d::CCActionInterval* action);

        /**
         * Set a release animation. This will override the AnimationType and set it
         * to Custom. Will run every time the button is released
         */
        void setReleaseAnimation(cocos2d::CCActionInterval* action);

        /**
         * Set a callback for when the button is activated
         */
        void setActivateCallback(ButtonCallback callback);

        /**
         * Set a callback for when the button is selected
         */
        void setSelectCallback(ButtonCallback callback);

        /**
         * Set a callback for when the button is unselected
         */
        void setUnselectCallback(ButtonCallback callback);

        /**
         * Set the touch priority of the Button
         */
        void setTouchPriority(int priority);
        int getTouchPriority();

        /**
         * Set the touch multiplier, which increases the distance from the 
         * center that the button can be pressed (default is 1)
         */
        void setTouchMultiplier(float multipler);
        float getTouchMultiplier();

        /**
         * Set the scale multiplier, only used with AnimationType::Scale
         * Will scale the button by this amount when pressed
         */
        void setScaleMultiplier(float multiplier);
        float getScaleMultiplier();

        /**
         * Set the move offset, only used with AnimationType::Move
         * Will move the button by this amount when pressed
         */
        void setMoveOffset(cocos2d::CCPoint const& offset);
        cocos2d::CCPoint getMoveOffset();

        /**
         * Set how long the default animations will last when selected
         */
        void setSelectedDuration(float duration);
        float getSelectedDuration();

        /**
         * Set how long the default animations will last when unselected
         */
        void setUnselectedDuration(float duration);
        float getUnselectedDuration();

        /**
         * Set if the button can be clicked
         */
        virtual void setEnabled(bool enabled);
        virtual bool isEnabled();
        
        /**
         * Get the selected state of the button
         */
        virtual bool isSelected();

        virtual void onEnter() override;
        virtual void onExit() override;

        virtual void selected();
        virtual void unselected();
        virtual void activate();

        virtual void registerWithTouchDispatcher();

        virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;

    protected:
        Button();
        ~Button();

        void setDefaults();
        void resetDefaults();

        cocos2d::CCActionInterval* clickActionForType();
        cocos2d::CCActionInterval* releaseActionForType();

        bool init(ButtonCallback activateCallback);
        bool initWithNode(cocos2d::CCNode* node, ButtonCallback activateCallback);
        bool initWithSprite(geode::ZStringView fileName, ButtonCallback activateCallback);
        bool initWithSpriteFrameName(geode::ZStringView frameName, ButtonCallback activateCallback);
        bool initWithLabel(geode::ZStringView text, geode::ZStringView font, ButtonCallback activateCallback);

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
    };

    /**
     * A handler for buttons to share touches when they share a parent
     * Shouldn't be messed with usually unless you are manually registering
     */
    class GEODE_DLL SharedButtonHandler {
    public:
        static SharedButtonHandler* get();

        /**
         * Returns true if the touch is within the button's rect (multiplied by the touch multiplier)
         */
        static bool containsTouch(Button* button, cocos2d::CCTouch* touch);

        /**
         * Register a button to share touches with (must be called in onEnter)
         */
        void registerButton(Button* button);

        /**
         * Unregister a button (must be called in onExit)
         */
        void unregisterButton(Button* button);
        
        /**
         * Pass moves from ccTouchMoved to all buttons sharing a parent
         */
        void passMoveToButtons(Button* button, cocos2d::CCTouch* touch);

        /**
         * Pass click activate from one button to the active one
         */
        void passActivateToButtons();

        /**
         * Pass click cancelled from one button to the active one
         */
        void passCancelledToButtons();

        /**
         * Set an active button (should be called in ccTouchBegan)
         */
        void setActiveButton(Button* button);

    protected:
        SharedButtonHandler();
        ~SharedButtonHandler();

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
