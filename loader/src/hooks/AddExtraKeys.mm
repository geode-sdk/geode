#include <Geode/DefaultInclude.hpp>

#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h>

#ifdef GEODE_IS_MACOS
#include <Carbon/Carbon.h>
#import <Cocoa/Cocoa.h>

#import <Geode/cocos/platform/mac/EAGLView.h>
#include <Geode/cocos/text_input_node/CCIMEDispatcher.h>
#include <Geode/modify/Modify.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#else
#include <UIKit/UIKit.h>
#endif
#include <objc/runtime.h>

using namespace geode::prelude;

#ifdef GEODE_IS_MACOS

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
        case NSUpArrowFunctionKey:
            return enumKeyCodes::KEY_Up;
        case NSDownArrowFunctionKey:
            return enumKeyCodes::KEY_Down;
        case NSLeftArrowFunctionKey:
            return enumKeyCodes::KEY_Left;
        case NSRightArrowFunctionKey:
            return enumKeyCodes::KEY_Right;
        default:
            return enumKeyCodes::KEY_Unknown;
    }
}

bool isKeyNumpad(NSEvent* event) {
    return ([event modifierFlags] & NSEventModifierFlagNumericPad) != 0;
}


void keyDownExecHook(EAGLView* self, SEL sel, NSEvent* event) {
    bool extraKey = isExtraKey(event);
    bool numpad = isKeyNumpad(event);
    if (!extraKey && !numpad) {
        if ([[event characters] length] == 0) {
            // really dumb workaround for a really dumb crash
            // basically the game tries to read the 1st character on every keydown
            // but rob uses this instead of ignoring modifiers or whatever so sometimes there is no 1st character!!
            // hopefully fixed in 2.208 :(
            return;
        }

        GEODE_MACOS([self performSelector:sel withObject:event]);
        return;
    }
    if (CCIMEDispatcher::sharedDispatcher()->hasDelegate()) {
        if ([[event characters] length] == 0) {
            // we need this for every original call to keyDown btw
            return;
        }

        GEODE_MACOS([self performSelector:sel withObject:event]);
        return;
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

void keyUpExecHook(EAGLView* self, SEL sel, NSEvent* event) {
    bool extraKey = isExtraKey(event);
    bool numpad = isKeyNumpad(event);
    if (!extraKey && !numpad) {
        GEODE_MACOS([self performSelector:sel withObject:event]);
        return;
    }
    if (CCIMEDispatcher::sharedDispatcher()->hasDelegate()) {
        GEODE_MACOS([self performSelector:sel withObject:event]);
        return;
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

void mouseDownExecHook(EAGLView* self, SEL sel, NSEvent* event) {
    if (!isExtraMouseButton(event)) {
        GEODE_MACOS([self performSelector:sel withObject:event]);
        return;
    }

    enumKeyCodes keyCode = mouseButtonToKeyCode(event);
    CCKeyboardDispatcher::get()->dispatchKeyboardMSG(keyCode, true, false);
}

void mouseUpExecHook(EAGLView* self, SEL sel, NSEvent* event) {
    if (!isExtraMouseButton(event)) {
        GEODE_MACOS([self performSelector:sel withObject:event]);
        return;
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

#else

NSMutableDictionary<NSNumber*, NSNumber*>* keyStates;

enumKeyCodes keyCodeFromKeyCommand(UIKey* key) {
    switch (key.keyCode) {
        case UIKeyboardHIDUsageKeyboard0: return enumKeyCodes::KEY_Zero;
        case UIKeyboardHIDUsageKeyboard1: return enumKeyCodes::KEY_One;
        case UIKeyboardHIDUsageKeyboard2: return enumKeyCodes::KEY_Two;
        case UIKeyboardHIDUsageKeyboard3: return enumKeyCodes::KEY_Three;
        case UIKeyboardHIDUsageKeyboard4: return enumKeyCodes::KEY_Four;
        case UIKeyboardHIDUsageKeyboard5: return enumKeyCodes::KEY_Five;
        case UIKeyboardHIDUsageKeyboard6: return enumKeyCodes::KEY_Six;
        case UIKeyboardHIDUsageKeyboard7: return enumKeyCodes::KEY_Seven;
        case UIKeyboardHIDUsageKeyboard8: return enumKeyCodes::KEY_Eight;
        case UIKeyboardHIDUsageKeyboard9: return enumKeyCodes::KEY_Nine;
        case UIKeyboardHIDUsageKeyboardReturnOrEnter: return enumKeyCodes::KEY_Enter;
        case UIKeyboardHIDUsageKeyboardEscape: return enumKeyCodes::KEY_Escape;
        case UIKeyboardHIDUsageKeyboardDeleteOrBackspace: return enumKeyCodes::KEY_Backspace; // really..
        case UIKeyboardHIDUsageKeyboardTab: return enumKeyCodes::KEY_Tab;
        case UIKeyboardHIDUsageKeyboardSpacebar: return enumKeyCodes::KEY_Space;
        case UIKeyboardHIDUsageKeyboardHyphen: return enumKeyCodes::KEY_OEMMinus;
        case UIKeyboardHIDUsageKeyboardEqualSign: return enumKeyCodes::KEY_Equal;
        case UIKeyboardHIDUsageKeyboardOpenBracket: return enumKeyCodes::KEY_LeftBracket;
        case UIKeyboardHIDUsageKeyboardCloseBracket: return enumKeyCodes::KEY_RightBracket;
        case UIKeyboardHIDUsageKeyboardBackslash: return enumKeyCodes::KEY_Backslash;
        // case UIKeyboardHIDUsageKeyboardNonUSPound: return enumKeyCodes::KEY_;
        case UIKeyboardHIDUsageKeyboardSemicolon: return enumKeyCodes::KEY_Semicolon;
        // case UIKeyboardHIDUsageKeyboardQuote: return enumKeyCodes::KEY_;
        case UIKeyboardHIDUsageKeyboardGraveAccentAndTilde: return enumKeyCodes::KEY_GraveAccent;
        case UIKeyboardHIDUsageKeyboardComma: return enumKeyCodes::KEY_OEMComma;
        case UIKeyboardHIDUsageKeyboardPeriod: return enumKeyCodes::KEY_OEMPeriod;
        case UIKeyboardHIDUsageKeyboardSlash: return enumKeyCodes::KEY_Slash;
        case UIKeyboardHIDUsageKeyboardCapsLock: return enumKeyCodes::KEY_CapsLock;
        case UIKeyboardHIDUsageKeyboardLeftAlt: return enumKeyCodes::KEY_Alt;
        case UIKeyboardHIDUsageKeyboardLeftControl: return enumKeyCodes::KEY_LeftControl;
        case UIKeyboardHIDUsageKeyboardLeftShift: return enumKeyCodes::KEY_LeftShift;
        // case UIKeyboardHIDUsageKeyboardRightAlt: return enumKeyCodes::KEY_;
        case UIKeyboardHIDUsageKeyboardRightControl: return enumKeyCodes::KEY_RightContol;
        case UIKeyboardHIDUsageKeyboardRightShift: return enumKeyCodes::KEY_RightShift;
        case UIKeyboardHIDUsageKeyboardF1: return enumKeyCodes::KEY_F1;
        case UIKeyboardHIDUsageKeyboardF2: return enumKeyCodes::KEY_F2;
        case UIKeyboardHIDUsageKeyboardF3: return enumKeyCodes::KEY_F3;
        case UIKeyboardHIDUsageKeyboardF4: return enumKeyCodes::KEY_F4;
        case UIKeyboardHIDUsageKeyboardF5: return enumKeyCodes::KEY_F5;
        case UIKeyboardHIDUsageKeyboardF6: return enumKeyCodes::KEY_F6;
        case UIKeyboardHIDUsageKeyboardF7: return enumKeyCodes::KEY_F7;
        case UIKeyboardHIDUsageKeyboardF8: return enumKeyCodes::KEY_F8;
        case UIKeyboardHIDUsageKeyboardF9: return enumKeyCodes::KEY_F9;
        case UIKeyboardHIDUsageKeyboardF10: return enumKeyCodes::KEY_F10;
        case UIKeyboardHIDUsageKeyboardF11: return enumKeyCodes::KEY_F11;
        case UIKeyboardHIDUsageKeyboardF12: return enumKeyCodes::KEY_F12;
        case UIKeyboardHIDUsageKeyboardF13: return enumKeyCodes::KEY_F13;
        case UIKeyboardHIDUsageKeyboardF14: return enumKeyCodes::KEY_F14;
        case UIKeyboardHIDUsageKeyboardF15: return enumKeyCodes::KEY_F15;
        case UIKeyboardHIDUsageKeyboardF16: return enumKeyCodes::KEY_F16;
        case UIKeyboardHIDUsageKeyboardF17: return enumKeyCodes::KEY_F17;
        case UIKeyboardHIDUsageKeyboardF18: return enumKeyCodes::KEY_F18;
        case UIKeyboardHIDUsageKeyboardF19: return enumKeyCodes::KEY_F19;
        case UIKeyboardHIDUsageKeyboardF20: return enumKeyCodes::KEY_F20;
        case UIKeyboardHIDUsageKeyboardF21: return enumKeyCodes::KEY_F21;
        case UIKeyboardHIDUsageKeyboardF22: return enumKeyCodes::KEY_F22;
        case UIKeyboardHIDUsageKeyboardF23: return enumKeyCodes::KEY_F23;
        case UIKeyboardHIDUsageKeyboardF24: return enumKeyCodes::KEY_F24;
        case UIKeyboardHIDUsageKeyboardScrollLock: return enumKeyCodes::KEY_ScrollLock;
        case UIKeyboardHIDUsageKeyboardInsert: return enumKeyCodes::KEY_Insert;
        case UIKeyboardHIDUsageKeyboardHome: return enumKeyCodes::KEY_Home;
        case UIKeyboardHIDUsageKeyboardPageUp: return enumKeyCodes::KEY_PageUp;
        case UIKeyboardHIDUsageKeyboardDeleteForward: return enumKeyCodes::KEY_Delete;
        case UIKeyboardHIDUsageKeyboardEnd: return enumKeyCodes::KEY_End;
        case UIKeyboardHIDUsageKeyboardPageDown: return enumKeyCodes::KEY_PageDown;
        case UIKeyboardHIDUsageKeyboardRightArrow: return enumKeyCodes::KEY_ArrowRight;
        case UIKeyboardHIDUsageKeyboardLeftArrow: return enumKeyCodes::KEY_ArrowLeft;
        case UIKeyboardHIDUsageKeyboardDownArrow: return enumKeyCodes::KEY_ArrowDown;
        case UIKeyboardHIDUsageKeyboardUpArrow: return enumKeyCodes::KEY_ArrowUp;
        case UIKeyboardHIDUsageKeypadNumLock: return enumKeyCodes::KEY_Numlock;
        case UIKeyboardHIDUsageKeypadSlash: return enumKeyCodes::KEY_Slash;
        case UIKeyboardHIDUsageKeypadAsterisk: return enumKeyCodes::KEY_Multiply;
        case UIKeyboardHIDUsageKeypadHyphen: return enumKeyCodes::KEY_OEMMinus;
        case UIKeyboardHIDUsageKeypadPlus: return enumKeyCodes::KEY_OEMPlus;
        case UIKeyboardHIDUsageKeypadEnter: return enumKeyCodes::KEY_NumEnter;
        case UIKeyboardHIDUsageKeypad0: return enumKeyCodes::KEY_NumPad0;
        case UIKeyboardHIDUsageKeypad1: return enumKeyCodes::KEY_NumPad1;
        case UIKeyboardHIDUsageKeypad2: return enumKeyCodes::KEY_NumPad2;
        case UIKeyboardHIDUsageKeypad3: return enumKeyCodes::KEY_NumPad3;
        case UIKeyboardHIDUsageKeypad4: return enumKeyCodes::KEY_NumPad4;
        case UIKeyboardHIDUsageKeypad5: return enumKeyCodes::KEY_NumPad5;
        case UIKeyboardHIDUsageKeypad6: return enumKeyCodes::KEY_NumPad6;
        case UIKeyboardHIDUsageKeypad7: return enumKeyCodes::KEY_NumPad7;
        case UIKeyboardHIDUsageKeypad8: return enumKeyCodes::KEY_NumPad8;
        case UIKeyboardHIDUsageKeypad9: return enumKeyCodes::KEY_NumPad9;
        case UIKeyboardHIDUsageKeypadPeriod: return enumKeyCodes::KEY_OEMPeriod;
        // case UIKeyboardHIDUsageKeyboardNonUSBackslash: return enumKeyCodes::KEY_;
        case UIKeyboardHIDUsageKeyboardApplication: return enumKeyCodes::KEY_ApplicationsKey;
        // case UIKeyboardHIDUsageKeyboardPower: return enumKeyCodes::KEY_;
        case UIKeyboardHIDUsageKeypadEqualSign: return enumKeyCodes::KEY_OEMEqual;
        // case UIKeyboardHIDUsageKeyboardMenu: return enumKeyCodes::KEY_;
        case UIKeyboardHIDUsageKeyboardMute: return enumKeyCodes::KEY_VolumeMute;
        case UIKeyboardHIDUsageKeyboardVolumeUp: return enumKeyCodes::KEY_VolumeUp;
        case UIKeyboardHIDUsageKeyboardVolumeDown: return enumKeyCodes::KEY_VolumeDown;
        case UIKeyboardHIDUsageKeypadComma: return enumKeyCodes::KEY_OEMComma;

        default:
            return enumKeyCodes::KEY_Unknown;
    }
}

bool handleKeyEvent(UIKey* key, bool isKeyDown, bool isARepeat) {
    enumKeyCodes keyCode = keyCodeFromKeyCommand(key);
    if (keyCode == enumKeyCodes::KEY_Unknown) return false;

    keyStates[@(keyCode)] = @(isKeyDown);

    CCKeyboardDispatcher::get()->updateModifierKeys(
        [key modifierFlags] & UIKeyModifierShift,
        [key modifierFlags] & UIKeyModifierControl,
        [key modifierFlags] & UIKeyModifierAlternate,
        [key modifierFlags] & UIKeyModifierCommand
    );

    CCKeyboardDispatcher::get()->dispatchKeyboardMSG(keyCode, keyStates[@(keyCode)], isARepeat);

    return true;
}

void pressesBegan(id self, SEL _cmd, NSSet<UIPress*>* presses, UIPressesEvent* event) {
    for (UIPress* press in presses) {
        UIKey* key = press.key;
        UIKeyboardHIDUsage keyCode = key.keyCode;

        if (keyStates[@(keyCode)] == nil)
            keyStates[@(keyCode)] = @(NO);

        handleKeyEvent(key, true, [keyStates[@(keyCode)] boolValue]);
    }
}

void pressesEnded(id self, SEL _cmd, NSSet<UIPress*>* presses, UIPressesEvent* event) {
    for (UIPress* press in presses) {
        UIKey* key = press.key;
        UIKeyboardHIDUsage keyCode = key.keyCode;

        handleKeyEvent(key, false, false);
    }
}

#endif


#define HOOK_OBJC_METHOD(klass, methodName) \
    auto methodName##Addr = class_getInstanceMethod(klass, @selector(methodName:)); \
    static_cast<void>(Mod::get()->hook(reinterpret_cast<void*>(method_getImplementation(methodName##Addr)), &methodName##Hook, #klass " " #methodName));

__attribute__((constructor)) void initialize_newKeyboardMSGKeysHooks() {
#if defined(GEODE_IS_MACOS)
    auto eaglView = objc_getClass("EAGLView");

    HOOK_OBJC_METHOD(eaglView, keyDownExec);
    HOOK_OBJC_METHOD(eaglView, keyUpExec);

    HOOK_OBJC_METHOD(eaglView, mouseDownExec);
    HOOK_OBJC_METHOD(eaglView, mouseUpExec);
#else
    @autoreleasepool {
        keyStates = [NSMutableDictionary dictionary];
    }

    auto rootViewController = objc_getClass("RootViewController");

    class_addMethod(rootViewController, @selector(pressesBegan:withEvent:), (IMP)pressesBegan, "v@:@@");
    class_addMethod(rootViewController, @selector(pressesEnded:withEvent:), (IMP)pressesEnded, "v@:@@");
#endif
}
