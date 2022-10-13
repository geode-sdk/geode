#pragma once

#include "BasedButtonSprite.hpp"
#include <Geode/binding/CCMenuItemToggler.hpp>

#pragma warning(disable : 4275)

namespace geode {
    class GEODE_DLL TabButton : public CCMenuItemToggler {
     public:
        static TabButton* create(
            TabBaseColor unselected,
            TabBaseColor selected,
            const char* text,
            cocos2d::CCObject* target,
            cocos2d::SEL_MenuHandler callback
        );
        
        static TabButton* create(
            const char* text,
            cocos2d::CCObject* target,
            cocos2d::SEL_MenuHandler callback
        );
    };
}
