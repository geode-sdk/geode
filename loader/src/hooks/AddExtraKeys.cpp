#include <Geode/DefaultInclude.hpp>

#ifndef GEODE_IS_IOS

#include <Geode/cocos/include/cocos2d.h>
#include <Geode/modify/CCKeyboardDispatcher.hpp>

using namespace geode::prelude;

class $modify(CCKeyboardDispatcher) {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("CCKeyboardDispatcher new keys")

    const char* keyToString(enumKeyCodes key) {
        if (key == KEY_C) return "C";

        if (key < 0x405) {
            return CCKeyboardDispatcher::keyToString(key);
        }

        switch (key) {
            case CONTROLLER2_A:
                return "Controller2_A";
            case CONTROLLER2_B:
                return "Controller2_B";
            case CONTROLLER2_Y:
                return "Controller2_Y";
            case CONTROLLER2_X:
                return "Controller2_X";
            case CONTROLLER2_Start:
                return "Controller2_Start";
            case CONTROLLER2_Back:
                return "Controller2_Back";
            case CONTROLLER2_RB:
                return "Controller2_RB";
            case CONTROLLER2_LB:
                return "Controller2_LB";
            case CONTROLLER2_RT:
                return "Controller2_RT";
            case CONTROLLER2_LT:
                return "Controller2_LT";
            case CONTROLLER2_Up:
                return "Controller2_Up";
            case CONTROLLER2_Down:
                return "Controller2_Down";
            case CONTROLLER2_Left:
                return "Controller2_Left";
            case CONTROLLER2_Right:
                return "Controller2_Right";
            case CONTROLLER2_LTHUMBSTICK_UP:
                return "Controller2_LTHUMBSTICK_UP";
            case CONTROLLER2_LTHUMBSTICK_DOWN:
                return "Controller2_LTHUMBSTICK_DOWN";
            case CONTROLLER2_LTHUMBSTICK_LEFT:
                return "Controller2_LTHUMBSTICK_LEFT";
            case CONTROLLER2_LTHUMBSTICK_RIGHT:
                return "Controller2_LTHUMBSTICK_RIGHT";
            case CONTROLLER2_RTHUMBSTICK_UP:
                return "Controller2_RTHUMBSTICK_UP";
            case CONTROLLER2_RTHUMBSTICK_DOWN:
                return "Controller2_RTHUMBSTICK_DOWN";
            case CONTROLLER2_RTHUMBSTICK_LEFT:
                return "Controller2_RTHUMBSTICK_LEFT";
            case CONTROLLER2_RTHUMBSTICK_RIGHT:
                return "Controller2_RTHUMBSTICK_RIGHT";
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
            case CONTROLLER_L3:
                return "Controller_L3";
            case CONTROLLER2_L3:
                return "Controller2_L3";
            case CONTROLLER_R3:
                return "Controller_R3";
            case CONTROLLER2_R3:
                return "Controller2_R3";
            default:
                return CCKeyboardDispatcher::keyToString(KEY_Unknown);
        }
    }
};

#endif