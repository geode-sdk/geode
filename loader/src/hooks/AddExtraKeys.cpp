#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_WINDOWS
// GLEW needs to be included before GL but GLFW3 doesn't do that so this is 
// just to make sure all of the GL-related headers are in order
#include <Geode/cocos/include/cocos2d.h>
#include <Geode/cocos/robtop/glfw/glfw3.h>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h>
#include <Geode/cocos/text_input_node/CCIMEDispatcher.h>
#include <Geode/modify/Modify.hpp>
#include <Geode/modify/CCEGLView.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>

using namespace geode::prelude;

class $modify(GeodeCCEGLView, CCEGLView) {
    void onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        bool extraKey = isExtraKey(key);
        bool numpad = isKeyNumpad(key);
        if (!extraKey && !numpad) {
            return CCEGLView::onGLFWKeyCallback(window, key, scancode, action, mods);
        }
        if (CCIMEDispatcher::sharedDispatcher()->hasDelegate()) {
            return CCEGLView::onGLFWKeyCallback(window, key, scancode, action, mods);
        }
        bool down = action == 1 || action == 2;
        bool repeat = action == 2;
        enumKeyCodes keyCode = enumKeyCodes::KEY_Unknown;
        if (extraKey) {
            keyCode = this->extraKeyToKeyCode(key);
        }
        if (numpad) {
            keyCode = this->numpadToKeyCode(key);
        }
        CCKeyboardDispatcher::get()->dispatchKeyboardMSG(keyCode, down, repeat);
    }

    void onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int mods) {
        if (!isExtraMouseButton(button)) {
            return CCEGLView::onGLFWMouseCallBack(window, button, action, mods);
        }
        bool down = action == 1;
        // mouse buttons never repeat
        bool repeat = false;
        enumKeyCodes keyCode = this->mouseButtonToKeyCode(button);
        CCKeyboardDispatcher::get()->dispatchKeyboardMSG(keyCode, down, repeat);
    }

    bool isExtraMouseButton(int code) {
        return code > GLFW_MOUSE_BUTTON_3;
    }

    enumKeyCodes mouseButtonToKeyCode(int button) {
        switch (button) {
            case GLFW_MOUSE_BUTTON_4:
                return enumKeyCodes::MOUSE_4;
            case GLFW_MOUSE_BUTTON_5:
                return enumKeyCodes::MOUSE_5;
            case GLFW_MOUSE_BUTTON_6:
                return enumKeyCodes::MOUSE_6;
            case GLFW_MOUSE_BUTTON_7:
                return enumKeyCodes::MOUSE_7;
            case GLFW_MOUSE_BUTTON_8:
                return enumKeyCodes::MOUSE_8; 
            default:
                return enumKeyCodes::KEY_Unknown;
        }
    }

    bool isExtraKey(int code) {
        switch (code) {
            case GLFW_KEY_WORLD_1:
            case GLFW_KEY_WORLD_2:
            case GLFW_KEY_SEMICOLON:
            case GLFW_KEY_APOSTROPHE:
            case GLFW_KEY_SLASH:
            case GLFW_KEY_EQUAL:
            case GLFW_KEY_LEFT_BRACKET:
            case GLFW_KEY_BACKSLASH:
            case GLFW_KEY_RIGHT_BRACKET:
            case GLFW_KEY_GRAVE_ACCENT:
                return true;
            default:
                return false;
        }
    }

    enumKeyCodes extraKeyToKeyCode(int key) {
        switch (key) {
            case GLFW_KEY_SEMICOLON:
                return enumKeyCodes::KEY_Semicolon;
            case GLFW_KEY_APOSTROPHE:
                return enumKeyCodes::KEY_Apostrophe;
            case GLFW_KEY_SLASH:
                return enumKeyCodes::KEY_Slash;
            case GLFW_KEY_EQUAL:
                return enumKeyCodes::KEY_OEMEqual;
            case GLFW_KEY_LEFT_BRACKET:
                return enumKeyCodes::KEY_LeftBracket;
            case GLFW_KEY_BACKSLASH:
                return enumKeyCodes::KEY_Backslash;
            case GLFW_KEY_RIGHT_BRACKET:
                return enumKeyCodes::KEY_RightBracket;
            case GLFW_KEY_GRAVE_ACCENT:
                return enumKeyCodes::KEY_GraveAccent;
            case GLFW_KEY_WORLD_1:
                return enumKeyCodes::KEY_World1;
            case GLFW_KEY_WORLD_2:
                return enumKeyCodes::KEY_World2;
            default:
                return enumKeyCodes::KEY_Unknown;
        }
    }

    bool isKeyNumpad(int code) {
        return code >= GLFW_KEY_KP_0 && code <= GLFW_KEY_KP_EQUAL;
    }

    enumKeyCodes numpadToKeyCode(int key) {
        switch (key) {
            case GLFW_KEY_KP_0:
                return enumKeyCodes::KEY_NumPad0;
            case GLFW_KEY_KP_1:
                return enumKeyCodes::KEY_NumPad1;
            case GLFW_KEY_KP_2:
                return enumKeyCodes::KEY_NumPad2;
            case GLFW_KEY_KP_3:
                return enumKeyCodes::KEY_NumPad3;
            case GLFW_KEY_KP_4:
                return enumKeyCodes::KEY_NumPad4;
            case GLFW_KEY_KP_5:
                return enumKeyCodes::KEY_NumPad5;
            case GLFW_KEY_KP_6:
                return enumKeyCodes::KEY_NumPad6;
            case GLFW_KEY_KP_7:
                return enumKeyCodes::KEY_NumPad7;
            case GLFW_KEY_KP_8:
                return enumKeyCodes::KEY_NumPad8;
            case GLFW_KEY_KP_9:
                return enumKeyCodes::KEY_NumPad9;
            case GLFW_KEY_KP_DECIMAL:
                return enumKeyCodes::KEY_Decimal;
            case GLFW_KEY_KP_DIVIDE:
                return enumKeyCodes::KEY_Divide;
            case GLFW_KEY_KP_MULTIPLY:
                return enumKeyCodes::KEY_Multiply;
            case GLFW_KEY_KP_SUBTRACT:
                return enumKeyCodes::KEY_Subtract;
            case GLFW_KEY_KP_ADD:
                return enumKeyCodes::KEY_Add;
            case GLFW_KEY_KP_ENTER:
                return enumKeyCodes::KEY_NumEnter;
            case GLFW_KEY_KP_EQUAL:
                return enumKeyCodes::KEY_Equal;
            default:
                return enumKeyCodes::KEY_Unknown;
        }
    }
};

class $modify(CCKeyboardDispatcher) {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("CCKeyboardDispatcher new keys")

    const char* keyToString(enumKeyCodes key) {
        if (key < 0x1000) {
            return CCKeyboardDispatcher::keyToString(key);
        }

        switch (key) {
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