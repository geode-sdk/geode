#pragma once

#include "Popup.hpp"
#include "InputNode.hpp"
#include "Popup.hpp"

#include <Geode/binding/TextInputDelegate.hpp>

namespace geode {
    class ColorPickPopupDelegate {
    public:
        virtual void updateColor(cocos2d::ccColor4B const& color) {}
    };

    class ColorPickPopup :
        public Popup<cocos2d::ccColor4B const&, bool>,
        public cocos2d::extension::ColorPickerDelegate,
        public TextInputDelegate {
    protected:
        cocos2d::ccColor4B m_color;
        cocos2d::ccColor4B m_originalColor;
        cocos2d::extension::CCControlColourPicker* m_picker;
        Slider* m_opacitySlider = nullptr;
        InputNode* m_rInput;
        InputNode* m_gInput;
        InputNode* m_bInput;
        InputNode* m_hexInput;
        InputNode* m_opacityInput = nullptr;
        ColorPickPopupDelegate* m_delegate = nullptr;
        cocos2d::CCSprite* m_newColorSpr;
        CCMenuItemSpriteExtra* m_resetBtn;

        static constexpr auto TAG_OPACITY_INPUT = 0;
        static constexpr auto TAG_R_INPUT = 1;
        static constexpr auto TAG_G_INPUT = 2;
        static constexpr auto TAG_B_INPUT = 3;
        static constexpr auto TAG_HEX_INPUT = 4;

        bool setup(cocos2d::ccColor4B const& color, bool isRGBA) override;

        void onOpacitySlider(cocos2d::CCObject* sender);
        void onReset(cocos2d::CCObject* sender);

        void textChanged(CCTextInputNode* input) override;
        void colorValueChanged(cocos2d::ccColor3B color) override;

        void updateState(cocos2d::CCNode* except = nullptr);

        static ColorPickPopup* create(cocos2d::ccColor4B const& color, bool isRGBA);

    public:
        static ColorPickPopup* create(bool isRGBA);
        static ColorPickPopup* create(cocos2d::ccColor3B const& color);
        static ColorPickPopup* create(cocos2d::ccColor4B const& color);

        void setColorTarget(cocos2d::CCSprite* spr);
        void setDelegate(ColorPickPopupDelegate* delegate);
    };
}
