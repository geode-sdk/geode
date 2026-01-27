#ifdef GEODE_IS_IOS
#include <Geode/DefaultInclude.hpp>

#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h>

#include <UIKit/UIKit.h>
#include <objc/runtime.h>

using namespace geode::prelude;

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

__attribute__((constructor)) void initialize_newKeyboardMSGKeysHooks() {
    @autoreleasepool {
        keyStates = [NSMutableDictionary dictionary];
    }

    auto rootViewController = objc_getClass("RootViewController");

    class_addMethod(rootViewController, @selector(pressesBegan:withEvent:), (IMP)pressesBegan, "v@:@@");
    class_addMethod(rootViewController, @selector(pressesEnded:withEvent:), (IMP)pressesEnded, "v@:@@");
}
#endif
