#include <Geode/cocos/robtop/glfw/glfw3.h>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h>
#include <Geode/cocos/text_input_node/CCIMEDispatcher.h>
#include <Geode/modify/Modify.hpp>
#include <Geode/modify/CCEGLView.hpp>

using namespace geode::prelude;

class $modify(CCEGLView) {
    void onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (!isExtraKey(key)) {
            return CCEGLView::onGLFWKeyCallback(window, key, scancode, action, mods);
        }
        if (CCIMEDispatcher::sharedDispatcher()->hasDelegate()) {
            return CCEGLView::onGLFWKeyCallback(window, key, scancode, action, mods);
        }
        bool down = action == 1 || action == 2;
        bool repeat = action == 2;
        enumKeyCodes keyCode = this->extraKeyToKeyCode(key);
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
            // ;
            case GLFW_KEY_SEMICOLON:
            // '
            case GLFW_KEY_APOSTROPHE:
            // /
            case GLFW_KEY_SLASH:
            // =
            case GLFW_KEY_EQUAL:
            // [
            case GLFW_KEY_LEFT_BRACKET:
            // backslash
            case GLFW_KEY_BACKSLASH:
            // ]
            case GLFW_KEY_RIGHT_BRACKET:
            // ~
            case GLFW_KEY_GRAVE_ACCENT:
                return true;
            default:
                return false;
        }
    }

    enumKeyCodes extraKeyToKeyCode(int key) {
        switch (key) {
            // ;
            case GLFW_KEY_SEMICOLON:
                return enumKeyCodes::KEY_Semicolon;
            // '
            case GLFW_KEY_APOSTROPHE:
                return enumKeyCodes::KEY_Apostrophe;
            // /
            case GLFW_KEY_SLASH:
                return enumKeyCodes::KEY_Slash;
            // =
            case GLFW_KEY_EQUAL:
                return enumKeyCodes::KEY_Equal;
            // [
            case GLFW_KEY_LEFT_BRACKET:
                return enumKeyCodes::KEY_LeftBracket;
            // backslash
            case GLFW_KEY_BACKSLASH:
                return enumKeyCodes::KEY_Backslash;
            // ]
            case GLFW_KEY_RIGHT_BRACKET:
                return enumKeyCodes::KEY_RightBracket;
            // ~
            case GLFW_KEY_GRAVE_ACCENT:
                return enumKeyCodes::KEY_GraveAccent;
            default:
                return enumKeyCodes::KEY_Unknown;
        }
    }
};