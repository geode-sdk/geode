#include <Geode/DefaultInclude.hpp>

#ifndef GEODE_IS_IOS

#include <Geode/cocos/include/cocos2d.h>
#include <Geode/modify/CCKeyboardDispatcher.hpp>

using namespace geode::prelude;

class $modify(CCKeyboardDispatcher) {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("CCKeyboardDispatcher new keys")

    const char* keyToString(enumKeyCodes key) {
        if (key < 0x405) {
            return CCKeyboardDispatcher::keyToString(key);
        }

        switch (key) {
            case CONTROLLER_LTHUMBSTICK_UP:
                return "Controller_LTHUMBSTICK_UP";
            case CONTROLLER_LTHUMBSTICK_DOWN:
                return "Controller_LTHUMBSTICK_DOWN";
            case CONTROLLER_LTHUMBSTICK_LEFT:
                return "Controller_LTHUMBSTICK_LEFT";
            case CONTROLLER_LTHUMBSTICK_RIGHT:
                return "Controller_LTHUMBSTICK_RIGHT";
            case CONTROLLER_RTHUMBSTICK_UP:
                return "Controller_RTHUMBSTICK_UP";
            case CONTROLLER_RTHUMBSTICK_DOWN:
                return "Controller_RTHUMBSTICK_DOWN";
            case CONTROLLER_RTHUMBSTICK_LEFT:
                return "Controller_RTHUMBSTICK_LEFT";
            case CONTROLLER_RTHUMBSTICK_RIGHT:
                return "Controller_RTHUMBSTICK_RIGHT";
            case KEY_GraveAccent:
                return "`";
            case KEY_OEMEqual:
                return "=";
            case KEY_LeftBracket:
                return "[";
            case KEY_RightBracket:
                return "]";
            case KEY_Backslash:
                return "\\";
            case KEY_Semicolon:
                return ";";
            case KEY_Apostrophe:
                return "'";
            case KEY_Slash:
                return "/";
            case KEY_Equal:
                return "=";
            case KEY_NumEnter:
                return "NumEnter";
            case KEY_World1:
                return "INTL-1";
            case KEY_World2:
                return "INTL-2";
            case MOUSE_4:
                return "Mouse 4";
            case MOUSE_5:
                return "Mouse 5";
            case MOUSE_6:
                return "Mouse 6";
            case MOUSE_7:
                return "Mouse 7";
            case MOUSE_8:
                return "Mouse 8";
            default:
                return CCKeyboardDispatcher::keyToString(KEY_Unknown);
        }
    }
};

#endif