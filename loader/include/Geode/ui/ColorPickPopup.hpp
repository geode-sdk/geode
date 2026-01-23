#pragma once

#include "Popup.hpp"
#include "TextInput.hpp"
#include "../loader/Event.hpp"
#include <Geode/binding/TextInputDelegate.hpp>
#include <Geode/utils/function.hpp>

namespace geode {
    class ColorPickPopup;

    class GEODE_DLL ColorPickPopup :
        public Popup,
        public cocos2d::extension::ColorPickerDelegate,
        public TextInputDelegate {
    protected:
        class Impl;
        std::unique_ptr<Impl> m_impl;

        static constexpr auto TAG_OPACITY_INPUT = 0;
        static constexpr auto TAG_R_INPUT = 1;
        static constexpr auto TAG_G_INPUT = 2;
        static constexpr auto TAG_B_INPUT = 3;
        static constexpr auto TAG_HEX_INPUT = 4;

        ColorPickPopup();
        ~ColorPickPopup();
        bool init(cocos2d::ccColor4B const& color, bool isRGBA);

        void onOpacitySlider(cocos2d::CCObject* sender);
        void onReset(cocos2d::CCObject* sender);
        void onClose(cocos2d::CCObject* sender) override;

        void textChanged(CCTextInputNode* input) override;
        void colorValueChanged(cocos2d::ccColor3B color) override;

        void updateState(cocos2d::CCNode* except = nullptr);

        static ColorPickPopup* create(cocos2d::ccColor4B const& color, bool isRGBA);

    public:
        static ColorPickPopup* create(bool isRGBA);
        static ColorPickPopup* create(cocos2d::ccColor3B const& color);
        static ColorPickPopup* create(cocos2d::ccColor4B const& color);

        void setColorTarget(cocos2d::CCSprite* spr);
        void setCallback(geode::Function<void(cocos2d::ccColor4B const&)> callback);
    };
}
