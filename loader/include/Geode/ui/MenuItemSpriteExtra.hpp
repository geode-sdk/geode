#pragma once

#include <cocos2d.h>
#include "../utils/MiniFunction.hpp"

#pragma warning(disable : 4275)

namespace geode {
    class GEODE_DLL MenuItemSpriteExtra : public cocos2d::CCMenuItemSprite {
    protected:
        bool m_scaleSprites;

        MenuItemSpriteExtra();
        ~MenuItemSpriteExtra() override;

    public:
        static MenuItemSpriteExtra* create(
            cocos2d::CCNode* normalSprite, cocos2d::CCNode* disabledSprite,
            cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback
        );

        static MenuItemSpriteExtra* create(
            cocos2d::CCNode* normalSprite,
            cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback
        );

        bool init(
            cocos2d::CCNode* normalSprite, cocos2d::CCNode* disabledSprite,
            cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback
        );

        void runActionOnSprite(cocos2d::CCNode* sprite, cocos2d::CCAction* generator);
        void runActionOnSprites(utils::MiniFunction<cocos2d::CCAction*()> generator);

        utils::MiniFunction<cocos2d::CCAction*()> m_selectedAction;
        utils::MiniFunction<cocos2d::CCAction*()> m_unselectedAction;
        utils::MiniFunction<cocos2d::CCAction*()> m_activateAction;

        void activate() override;
        void selected() override;
        void unselected() override;

        void setScaleSprites(bool scale);

        void setImage(cocos2d::CCNode* image);
        void setNormalImage(cocos2d::CCNode* image) override;
        void setDisabledImage(cocos2d::CCNode* image) override;
        void setSelectedImage(cocos2d::CCNode* image) override;
    };
}
