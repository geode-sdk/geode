#import <Cocoa/Cocoa.h>
#include <objc/runtime.h>
#include <Carbon/Carbon.h>

#include <Geode/DefaultInclude.hpp>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h>
#include <Geode/cocos/text_input_node/CCIMEDispatcher.h>
#include <Geode/modify/Modify.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>

using namespace geode::prelude;

// https://github.com/SpaghettDev/BetterInputs/blob/44f249cd94f4cc19fca4de570dfab28f4efa3db8/src/platform/macos.mm#L121
// we use this instead of [event keyCode] because the returned value of keyCode for letters is keyboard locale-specific
int normalizedCodeFromEvent(NSEvent* event) {
    if ([[event characters] length] > 0) {
        return [[event characters] characterAtIndex:0];
    } else if ([[event charactersIgnoringModifiers] length] > 0) {
        return [[event charactersIgnoringModifiers] characterAtIndex:0];
    }

    return 0;
}

enumKeyCodes mouseButtonToKeyCode(NSEvent* event) {
    switch ([event buttonNumber]) {
        case 2:
            return enumKeyCodes::MOUSE_4;
        case 3:
            return enumKeyCodes::MOUSE_5;
        case 4:
            return enumKeyCodes::MOUSE_6;
        case 5:
            return enumKeyCodes::MOUSE_7;
        case 6:
            return enumKeyCodes::MOUSE_8;
        default:
            return enumKeyCodes::KEY_Unknown;
    }
}

bool isExtraMouseButton(NSEvent* event) {
    return [event buttonNumber] > 1;
}

enumKeyCodes extraKeyToKeyCode(NSEvent* event) {
    switch (normalizedCodeFromEvent(event)) {
        case ';':
            return enumKeyCodes::KEY_Semicolon;
        case '\'':
            return enumKeyCodes::KEY_Apostrophe;
        case '/':
            return enumKeyCodes::KEY_Slash;
        case '=':
            return enumKeyCodes::KEY_OEMEqual;
        case '[':
            return enumKeyCodes::KEY_LeftBracket;
        case '\\':
            return enumKeyCodes::KEY_Backslash;
        case ']':
            return enumKeyCodes::KEY_RightBracket;
        case '`':
            return enumKeyCodes::KEY_GraveAccent;
        default:
            break;
    }

    switch ([event keyCode]) {
        case kVK_ISO_Section:
            return enumKeyCodes::KEY_World2;
        default:
            break;
    }

    return enumKeyCodes::KEY_Unknown;
}

bool isExtraKey(NSEvent* event) {
    return extraKeyToKeyCode(event) != enumKeyCodes::KEY_Unknown;
}

enumKeyCodes numpadToKeyCode(NSEvent* event) {
    switch (normalizedCodeFromEvent(event)) {
        case '0':
            return enumKeyCodes::KEY_NumPad0;
        case '1':
            return enumKeyCodes::KEY_NumPad1;
        case '2':
            return enumKeyCodes::KEY_NumPad2;
        case '3':
            return enumKeyCodes::KEY_NumPad3;
        case '4':
            return enumKeyCodes::KEY_NumPad4;
        case '5':
            return enumKeyCodes::KEY_NumPad5;
        case '6':
            return enumKeyCodes::KEY_NumPad6;
        case '7':
            return enumKeyCodes::KEY_NumPad7;
        case '8':
            return enumKeyCodes::KEY_NumPad8;
        case '9':
            return enumKeyCodes::KEY_NumPad9;
        case '.':
            return enumKeyCodes::KEY_Decimal;
        case '/':
            return enumKeyCodes::KEY_Divide;
        case '*':
            return enumKeyCodes::KEY_Multiply;
        case '-':
            return enumKeyCodes::KEY_Subtract;
        case '+':
            return enumKeyCodes::KEY_Add;
        case 3:
            return enumKeyCodes::KEY_NumEnter;
        case '=':
            return enumKeyCodes::KEY_Equal;
        default:
            return enumKeyCodes::KEY_Unknown;
    }
}

bool isKeyNumpad(NSEvent* event) {
    return ([event modifierFlags] & NSEventModifierFlagNumericPad) != 0;
}


static void(*s_keyDownExecOrig)(void*, SEL, NSEvent*);
void keyDownExecHook(void* self, SEL sel, NSEvent* event) {
    bool extraKey = isExtraKey(event);
    bool numpad = isKeyNumpad(event);
    if (!extraKey && !numpad) {
        return s_keyDownExecOrig(self, sel, event);
    }
    if (CCIMEDispatcher::sharedDispatcher()->hasDelegate()) {
        return s_keyDownExecOrig(self, sel, event);
    }

    enumKeyCodes keyCode = enumKeyCodes::KEY_Unknown;
    if (extraKey) {
        keyCode = extraKeyToKeyCode(event);
    }
    if (numpad) {
        keyCode = numpadToKeyCode(event);
    }
    CCKeyboardDispatcher::get()->dispatchKeyboardMSG(keyCode, true, [event isARepeat]);
}

static void(*s_keyUpExecOrig)(void*, SEL, NSEvent*);
void keyUpExecHook(void* self, SEL sel, NSEvent* event) {
    bool extraKey = isExtraKey(event);
    bool numpad = isKeyNumpad(event);
    if (!extraKey && !numpad) {
        return s_keyUpExecOrig(self, sel, event);
    }
    if (CCIMEDispatcher::sharedDispatcher()->hasDelegate()) {
        return s_keyUpExecOrig(self, sel, event);
    }

    enumKeyCodes keyCode = enumKeyCodes::KEY_Unknown;
    if (extraKey) {
        keyCode = extraKeyToKeyCode(event);
    }
    if (numpad) {
        keyCode = numpadToKeyCode(event);
    }
    CCKeyboardDispatcher::get()->dispatchKeyboardMSG(keyCode, false, [event isARepeat]);
}

static void(*s_mouseDownExecOrig)(void*, SEL, NSEvent*);
void mouseDownExecHook(void* self, SEL sel, NSEvent* event) {
    if (!isExtraMouseButton(event)) {
        return s_mouseDownExecOrig(self, sel, event);
    }

    enumKeyCodes keyCode = mouseButtonToKeyCode(event);
    CCKeyboardDispatcher::get()->dispatchKeyboardMSG(keyCode, true, false);
}

static void(*s_mouseUpExecOrig)(void*, SEL, NSEvent*);
void mouseUpExecHook(void* self, SEL sel, NSEvent* event) {
    if (!isExtraMouseButton(event)) {
        return s_mouseUpExecOrig(self, sel, event);
    }

    enumKeyCodes keyCode = mouseButtonToKeyCode(event);
    CCKeyboardDispatcher::get()->dispatchKeyboardMSG(keyCode, false, false);
}


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


#define OBJC_SWIZZLE(klass, methodName) \
    auto methodName##Method = class_getInstanceMethod(klass, @selector(methodName:)); \
    s_##methodName##Orig = reinterpret_cast<decltype(s_##methodName##Orig)>(method_getImplementation(methodName##Method)); \
    method_setImplementation(methodName##Method, reinterpret_cast<IMP>(&methodName##Hook));

__attribute__((constructor)) void initialize_newKeyboardMSGKeysHooks() {
    auto eaglView = objc_getClass("EAGLView");

    OBJC_SWIZZLE(eaglView, keyDownExec);
    OBJC_SWIZZLE(eaglView, keyUpExec);

    OBJC_SWIZZLE(eaglView, mouseDownExec);
    OBJC_SWIZZLE(eaglView, mouseUpExec);
}
