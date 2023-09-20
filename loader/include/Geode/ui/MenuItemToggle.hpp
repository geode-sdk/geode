#pragma once

#include <cocos2d.h>
#include "MenuItemSprite.hpp"

#pragma warning(disable : 4275)

namespace geode {
    class GEODE_DLL MenuItemToggle : public cocos2d::CCMenuItem {
    protected:
        MenuItemSprite* m_offButton;
        MenuItemSprite* m_onButton;
        bool m_toggled;

        MenuItemToggle();
        ~MenuItemToggle() override;

    public:
        static MenuItemToggle* create(
            cocos2d::CCNode* onSprite, cocos2d::CCNode* offSprite,
            cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback
        );

        bool init(
            cocos2d::CCNode* onSprite, cocos2d::CCNode* offSprite,
            cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback
        );

        void setOffButton(MenuItemSprite* button);
        void setOnButton(MenuItemSprite* button);
        MenuItemSprite* getOffButton() const;
        MenuItemSprite* getOnButton() const;

        bool isToggled() const;
        void toggle(bool enabled);

        void setScale(float scale) override;
        void activate() override;
        void selected() override;
        void unselected() override;
        void setEnabled(bool enabled) override;
    };
}
