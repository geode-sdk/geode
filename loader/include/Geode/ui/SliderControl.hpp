#pragma once

#include <Geode/ui/NineSlice.hpp>
#include <Geode/utils/function.hpp>
#include <Geode/utils/ZStringView.hpp>
#include <cocos2d.h>

namespace geode {
    class GEODE_DLL SliderControl : public cocos2d::CCNodeRGBA, public cocos2d::CCTouchDelegate {
    public:
        using SliderCallback = geode::Function<void(SliderControl* sender, float value)>;

        /**
         * Create a custom Slider
         * 
         * @param thumb The sprite file name or frame name for the thumb when not selected
         * @param thumbSelected The sprite file name or frame name for the thumb when selected
         * @param groove The sprite file name or frame name for the grove
         * @param bar The sprite file name (cannot be a frame) for the progress bar
         * @param callback The callback for when the thumb is dragged
         * @param barOffset The size offset (will be inset for each side) for the bar to fit within the groove
         */
        static SliderControl* createCustom(ZStringView thumb, ZStringView thumbSelected, ZStringView groove, ZStringView bar, SliderCallback callback, cocos2d::CCSize const& barOffset = {2.f, 2.f});
        
        /**
         * Create a traditional Geometry Dash style Slider
         * 
         * @param callback The callback for when the thumb is dragged
         * @param alt Use the alternative slider design (white border and bar)
         */
        static SliderControl* create(SliderCallback callback, bool alt = false);

        /**
         * Set a callback for when the slider thumb is dragged
         */
        void setSlideCallback(SliderCallback callback);

        /**
         * Set a callback for when the slider thumb is clicked
         */
        void setClickCallback(SliderCallback callback);

        /**
         * Set a callback for when the slider thumb is released
         */
        void setReleaseCallback(SliderCallback callback);

        /**
         * Set the minimum value the slider can go to
         */
        void setMin(float min);
        float getMin();

        /**
         * Set the maximum value the slider can go to
         */
        void setMax(float max);
        float getMax();

        /**
         * Set if the slider can be controlled by the thumb
         */
        void setEnabled(bool enabled);
        bool isEnabled();

        /**
         * Set the value of the slider
         */
        void setValue(float value);
        float getValue();

        /**
         * Set the percent relative to the min and max values
         */
        void setPercent(float percent);
        float getPercent();

        /**
         * Set the snap steps the slider will snap to when dragging
         */
        void setSnapStep(float snapStep);
        float getSnapStep();

        /**
         * Link a text input. This will make the text input automatically update whenever
         * the slider is dragged. Typing in the text input will also update the slider.
         * Note that this will override any existing callback on the text input.
         */
        void linkTextInput(geode::TextInput* input, unsigned int precision);

        /**
         * Unlink a linked text input
         */
        void unlinkTextInput();

        /**
         * Sets the precision of the decimal shown in the linked text input. A value of 0 will make it int
         */
        void setTextInputPrecision(unsigned int precision);

        unsigned int getTextInputPrecision();
        geode::TextInput* getLinkedTextInput();

        /**
         * Link a label. This will make the label automatically update whenever
         * the slider is dragged.
         */
        void linkLabel(cocos2d::CCLabelBMFont* label, unsigned int precision);

        /**
         * Unlink a linked label
         */
        void unlinkLabel();

        /**
         * Sets the precision of the decimal shown in the linked label. A value of 0 will make it int
         */
        void setLabelPrecision(unsigned int precision);

        unsigned int getLabelPrecision();
        cocos2d::CCLabelBMFont* getLinkedLabel();

        /**
         * Set the slider as read only. This will hide the thumb and will disallow edits made
         * in linked text inputs
         */
        void setReadOnly(bool readOnly);
        bool isReadOnly();

        /**
         * Allow the slider min and max to be bypassed similar to the slider bypass hacks
         */
        void setSliderBypass(bool enabled);
        bool hasSliderBypass();

        cocos2d::CCNodeRGBA* getThumb();
        geode::NineSlice* getGroove();
        cocos2d::CCSprite* getBar();

        void setTouchPriority(int priority);
        int getTouchPriority();

        virtual void setContentSize(cocos2d::CCSize const& size) override;

        virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;

        virtual void onEnter() override;
        virtual void onExit() override;

        virtual void registerWithTouchDispatcher();

    protected:
        SliderControl();
        ~SliderControl();

        bool initCustom(ZStringView thumb, ZStringView thumbClicked, ZStringView groove, ZStringView bar, SliderCallback callback, cocos2d::CCSize const& barOffset);
        bool initStandard(SliderCallback callback, bool alt);

        bool setupThumb(ZStringView thumb, ZStringView thumbClicked);
        bool setupGroove(ZStringView groove, ZStringView bar);
        
        void updateSize();
        void updateFromTouch(cocos2d::CCTouch* touch);

        void updateLinkedTextInput();
        void updateLinkedLabel();

        float applySnap(float value);
    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
    };
}