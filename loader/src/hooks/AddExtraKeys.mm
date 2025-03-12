#include <Geode/DefaultInclude.hpp>

#import <Cocoa/Cocoa.h>
#include <objc/runtime.h>
#include <Carbon/Carbon.h>

#ifdef GEODE_IS_IOS
#include <AppKit/AppKit.h>
#endif

#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h>
#import <Geode/cocos/platform/mac/EAGLView.h>
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

#ifdef GEODE_IS_MACOS
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
#elif defined(GEODE_IS_IOS)
    @autoreleasepool
    {
        [NSEvent addGlobalMonitorForEventsMatchingMask:NSEventTypeKeyDown handler: ^(NSEvent* event) {
            keyDownExecHook(nullptr, 0, event);
        }];
        [NSEvent addGlobalMonitorForEventsMatchingMask:NSEventTypeKeyUp handler: ^(NSEvent* event) {
            keyUpExecHook(nullptr, 0, event);
        }];

        [NSEvent
            addGlobalMonitorForEventsMatchingMask:NSEventTypeLeftMouseDown | NSEventTypeRightMouseDown | NSEventTypeOtherMouseDown
                handler: ^(NSEvent* event) {
                    mouseDownExecHook(nullptr, 0, event);
                }
        ];
        [NSEvent
            addGlobalMonitorForEventsMatchingMask:NSEventTypeLeftMouseUp | NSEventTypeRightMouseUp | NSEventTypeOtherMouseUp
                handler: ^(NSEvent* event) {
                    mouseUpExecHook(nullptr, 0, event);
                }
        ];
    }
#endif
}
