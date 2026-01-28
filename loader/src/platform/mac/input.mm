#include <Carbon/Carbon.h>
#import <Cocoa/Cocoa.h>

#import <Geode/cocos/platform/mac/EAGLView.h>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/utils/Keyboard.hpp>
#include <Geode/utils/ObjcHook.hpp>
#include <objc/message.h>

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

enumKeyCodes extraKeyToKeyCode(NSEvent* event) {
    switch (normalizedCodeFromEvent(event)) {
        case ';':
            return KEY_Semicolon;
        case '\'':
            return KEY_Apostrophe;
        case '/':
            return KEY_Slash;
        case '=':
            return KEY_OEMEqual;
        case '[':
            return KEY_LeftBracket;
        case '\\':
            return KEY_Backslash;
        case ']':
            return KEY_RightBracket;
        case '`':
            return KEY_GraveAccent;
        default:
            break;
    }

    switch ([event keyCode]) {
        case kVK_ISO_Section:
            return KEY_World2;
        default:
            break;
    }

    return KEY_Unknown;
}
enumKeyCodes numpadToKeyCode(NSEvent* event) {
    switch (normalizedCodeFromEvent(event)) {
        case '0':
            return KEY_NumPad0;
        case '1':
            return KEY_NumPad1;
        case '2':
            return KEY_NumPad2;
        case '3':
            return KEY_NumPad3;
        case '4':
            return KEY_NumPad4;
        case '5':
            return KEY_NumPad5;
        case '6':
            return KEY_NumPad6;
        case '7':
            return KEY_NumPad7;
        case '8':
            return KEY_NumPad8;
        case '9':
            return KEY_NumPad9;
        case '.':
            return KEY_Decimal;
        case '/':
            return KEY_Divide;
        case '*':
            return KEY_Multiply;
        case '-':
            return KEY_Subtract;
        case '+':
            return KEY_Add;
        case 3:
            return KEY_NumEnter;
        case '=':
            return KEY_Equal;
        case NSUpArrowFunctionKey:
            return KEY_Up;
        case NSDownArrowFunctionKey:
            return KEY_Down;
        case NSLeftArrowFunctionKey:
            return KEY_Left;
        case NSRightArrowFunctionKey:
            return KEY_Right;
        default:
            return KEY_Unknown;
    }
}

enumKeyCodes convertKeyCode(NSEvent* event) {
    switch ([event keyCode]) {
        case kVK_ANSI_A: return KEY_A;
        case kVK_ANSI_S: return KEY_S;
        case kVK_ANSI_D: return KEY_D;
        case kVK_ANSI_F: return KEY_F;
        case kVK_ANSI_H: return KEY_H;
        case kVK_ANSI_G: return KEY_G;
        case kVK_ANSI_Z: return KEY_Z;
        case kVK_ANSI_X: return KEY_X;
        case kVK_ANSI_C: return KEY_C;
        case kVK_ANSI_V: return KEY_V;
        case kVK_ISO_Section: return KEY_World2;
        case kVK_ANSI_B: return KEY_B;
        case kVK_ANSI_Q: return KEY_Q;
        case kVK_ANSI_W: return KEY_W;
        case kVK_ANSI_E: return KEY_E;
        case kVK_ANSI_R: return KEY_R;
        case kVK_ANSI_Y: return KEY_Y;
        case kVK_ANSI_T: return KEY_T;
        case kVK_ANSI_1: return KEY_One;
        case kVK_ANSI_2: return KEY_Two;
        case kVK_ANSI_3: return KEY_Three;
        case kVK_ANSI_4: return KEY_Four;
        case kVK_ANSI_6: return KEY_Six;
        case kVK_ANSI_5: return KEY_Five;
        case kVK_ANSI_Equal: return KEY_Equal;
        case kVK_ANSI_9: return KEY_Nine;
        case kVK_ANSI_7: return KEY_Seven;
        case kVK_ANSI_Minus: return KEY_Subtract;
        case kVK_ANSI_8: return KEY_Eight;
        case kVK_ANSI_0: return KEY_Zero;
        case kVK_ANSI_RightBracket: return KEY_RightBracket;
        case kVK_ANSI_O: return KEY_O;
        case kVK_ANSI_U: return KEY_U;
        case kVK_ANSI_LeftBracket: return KEY_LeftBracket;
        case kVK_ANSI_I: return KEY_I;
        case kVK_ANSI_P: return KEY_P;
        case kVK_Return: return KEY_Enter;
        case kVK_ANSI_L: return KEY_L;
        case kVK_ANSI_J: return KEY_J;
        case kVK_ANSI_Quote: return KEY_Apostrophe;
        case kVK_ANSI_K: return KEY_K;
        case kVK_ANSI_Semicolon: return KEY_Semicolon;
        case kVK_ANSI_Backslash: return KEY_Backslash;
        case kVK_ANSI_Comma: return KEY_OEMComma;
        case kVK_ANSI_Slash: return KEY_Slash;
        case kVK_ANSI_N: return KEY_N;
        case kVK_ANSI_M: return KEY_M;
        case kVK_ANSI_Period: return KEY_OEMPeriod;
        case kVK_Tab: return KEY_Tab;
        case kVK_Space: return KEY_Space;
        case kVK_ANSI_Grave: return KEY_GraveAccent;
        case kVK_Delete: return KEY_Backspace;
        case kVK_Escape: return KEY_Escape;
        case kVK_Shift: return KEY_LeftShift;
        case kVK_CapsLock: return KEY_CapsLock;
        case kVK_Option: return KEY_Alt;
        case kVK_Control: return KEY_LeftControl;
        case kVK_RightShift: return KEY_RightShift;
        case kVK_RightOption: return KEY_RightMenu;
        case kVK_RightControl: return KEY_RightControl;
        case kVK_F17: return KEY_F17;
        case kVK_ANSI_KeypadDecimal: return KEY_Decimal;
        case kVK_ANSI_KeypadMultiply: return KEY_Multiply;
        case kVK_ANSI_KeypadPlus: return KEY_OEMPlus;
        case kVK_ANSI_KeypadClear: return KEY_Clear;
        case kVK_VolumeUp: return KEY_VolumeUp;
        case kVK_VolumeDown: return KEY_VolumeDown;
        case kVK_Mute: return KEY_VolumeMute;
        case kVK_ANSI_KeypadDivide: return KEY_Divide;
        case kVK_ANSI_KeypadEnter: return KEY_Enter;
        case kVK_ANSI_KeypadMinus: return KEY_OEMMinus;
        case kVK_F18: return KEY_F18;
        case kVK_F19: return KEY_F19;
        case kVK_ANSI_KeypadEquals: return KEY_Equal;
        case kVK_ANSI_Keypad0: return KEY_NumPad0;
        case kVK_ANSI_Keypad1: return KEY_NumPad1;
        case kVK_ANSI_Keypad2: return KEY_NumPad2;
        case kVK_ANSI_Keypad3: return KEY_NumPad3;
        case kVK_ANSI_Keypad4: return KEY_NumPad4;
        case kVK_ANSI_Keypad5: return KEY_NumPad5;
        case kVK_ANSI_Keypad6: return KEY_NumPad6;
        case kVK_ANSI_Keypad7: return KEY_NumPad7;
        case kVK_F20: return KEY_F20;
        case kVK_ANSI_Keypad8: return KEY_NumPad8;
        case kVK_ANSI_Keypad9: return KEY_NumPad9;
        case kVK_F5: return KEY_F5;
        case kVK_F6: return KEY_F6;
        case kVK_F7: return KEY_F7;
        case kVK_F3: return KEY_F3;
        case kVK_F8: return KEY_F8;
        case kVK_F9: return KEY_F9;
        case kVK_F11: return KEY_F11;
        case kVK_F13: return KEY_F13;
        case kVK_F16: return KEY_F16;
        case kVK_F14: return KEY_F14;
        case kVK_F10: return KEY_F10;
        case kVK_F12: return KEY_F12;
        case kVK_F15: return KEY_F15;
        case kVK_Help: return KEY_Help;
        case kVK_Home: return KEY_Home;
        case kVK_PageUp: return KEY_PageUp;
        case kVK_F4: return KEY_F4;
        case kVK_End: return KEY_End;
        case kVK_F2: return KEY_F2;
        case kVK_PageDown: return KEY_PageDown;
        case kVK_F1: return KEY_F1;
        case kVK_LeftArrow: return KEY_Left;
        case kVK_RightArrow: return KEY_Right;
        case kVK_DownArrow: return KEY_Down;
        case kVK_UpArrow: return KEY_Up;
        default: return KEY_Unknown;
    }
}

enumKeyCodes getKeyCode(NSEvent* event) {
    enumKeyCodes keyCode = extraKeyToKeyCode(event);
    if (keyCode != KEY_Unknown) {
        return keyCode;
    } else if (([event modifierFlags] & NSEventModifierFlagNumericPad) != 0) {
        return numpadToKeyCode(event);
    } else {
        return convertKeyCode(event);
    }
}

void keyDownExecHook(EAGLView* self, SEL sel, NSEvent* event) {
    unichar nativeCode = [event keyCode];
    enumKeyCodes keyCode = getKeyCode(event);
    CCIMEDispatcher* imeDispatcher = CCIMEDispatcher::sharedDispatcher();
    CCKeyboardDispatcher* keyboardDispatcher = CCKeyboardDispatcher::get();
    double timestamp = [event timestamp];
    bool isRepeat = [event isARepeat];
    bool isDown = true;
    NSEventModifierFlags flags = [event modifierFlags];

    KeyboardInputEvent::Modifiers modifiers = KeyboardInputEvent::Mods_None;
    if ((flags & NSEventModifierFlagShift) != 0) {
        modifiers |= KeyboardInputEvent::Mods_Shift;
    }
    if ((flags & NSEventModifierFlagControl) != 0) {
        modifiers |= KeyboardInputEvent::Mods_Control;
    }
    if ((flags & NSEventModifierFlagOption) != 0) {
        modifiers |= KeyboardInputEvent::Mods_Alt;
    }
    if ((flags & NSEventModifierFlagCommand) != 0) {
        modifiers |= KeyboardInputEvent::Mods_Super;
    }

    KeyboardInputEvent inputEvent(
        keyCode, isRepeat ? KeyboardInputEvent::Action::Press : KeyboardInputEvent::Action::Repeat, { nativeCode }, timestamp, modifiers
    );

    if (inputEvent.post() != ListenerResult::Propagate) return;

    isDown = inputEvent.action != KeyboardInputEvent::Action::Release;
    isRepeat = inputEvent.action == KeyboardInputEvent::Action::Repeat;
    keyCode = inputEvent.key;
    modifiers = inputEvent.modifiers;

    if (keyCode != KEY_Unknown && (!imeDispatcher->hasDelegate() || keyCode == KEY_Escape || keyCode == KEY_Enter)) {
        keyboardDispatcher->dispatchKeyboardMSG(keyCode, isDown, isRepeat, timestamp);
    }

    keyboardDispatcher->updateModifierKeys(
        (modifiers & KeyboardInputEvent::Mods_Shift) != KeyboardInputEvent::Mods_None,
        (modifiers & KeyboardInputEvent::Mods_Control) != KeyboardInputEvent::Mods_None,
        (modifiers & KeyboardInputEvent::Mods_Alt) != KeyboardInputEvent::Mods_None,
        (modifiers & KeyboardInputEvent::Mods_Super) != KeyboardInputEvent::Mods_None
    );

    NSString* characters = [event charactersIgnoringModifiers];
    if ([characters length] != 0) {
        unichar character = [characters characterAtIndex:0];
        if (character == NSDeleteFunctionKey || character == 0x7f) {
            imeDispatcher->dispatchDeleteBackward();
        }
        else if (keyCode == KEY_V && (modifiers & KeyboardInputEvent::Mods_Super) != KeyboardInputEvent::Mods_None) {
            NSString* str = [[NSPasteboard generalPasteboard] stringForType:NSPasteboardTypeString];
            if (![str isEqualToString:NSPasteboardTypeString]) {
                for (size_t i = 0; i < [str length]; i++) {
                    char ch = [str characterAtIndex:i];
                    imeDispatcher->dispatchInsertText(&ch, 1, keyCode);
                }
            }
        }
        else {
            char ch = character;
            imeDispatcher->dispatchInsertText(&ch, 1, keyCode);
        }
    }
}

void keyUpExecHook(EAGLView* self, SEL sel, NSEvent* event) {
    unichar nativeCode = [event keyCode];
    enumKeyCodes keyCode = getKeyCode(event);
    CCIMEDispatcher* imeDispatcher = CCIMEDispatcher::sharedDispatcher();
    CCKeyboardDispatcher* keyboardDispatcher = CCKeyboardDispatcher::get();
    double timestamp = [event timestamp];
    bool isRepeat = false;
    bool isDown = false;
    NSEventModifierFlags flags = [event modifierFlags];

    KeyboardInputEvent::Modifiers modifiers;
    if ((flags & NSEventModifierFlagShift) != 0) {
        modifiers |= KeyboardInputEvent::Mods_Shift;
    }
    if ((flags & NSEventModifierFlagControl) != 0) {
        modifiers |= KeyboardInputEvent::Mods_Control;
    }
    if ((flags & NSEventModifierFlagOption) != 0) {
        modifiers |= KeyboardInputEvent::Mods_Alt;
    }
    if ((flags & NSEventModifierFlagCommand) != 0) {
        modifiers |= KeyboardInputEvent::Mods_Super;
    }

    KeyboardInputEvent inputEvent(
        keyCode, KeyboardInputEvent::Action::Release, { nativeCode }, timestamp, modifiers
    );

    if (inputEvent.post() != ListenerResult::Propagate) return;

    isDown = inputEvent.action != KeyboardInputEvent::Action::Release;
    isRepeat = inputEvent.action == KeyboardInputEvent::Action::Repeat;
    keyCode = inputEvent.key;
    modifiers = inputEvent.modifiers;

    if (keyCode != KEY_Unknown && (!imeDispatcher->hasDelegate() || keyCode == KEY_Escape || keyCode == KEY_Enter)) {
        keyboardDispatcher->dispatchKeyboardMSG(keyCode, isDown, isRepeat, timestamp);
    }

    keyboardDispatcher->updateModifierKeys(
        (modifiers & KeyboardInputEvent::Mods_Shift) != KeyboardInputEvent::Mods_None,
        (modifiers & KeyboardInputEvent::Mods_Control) != KeyboardInputEvent::Mods_None,
        (modifiers & KeyboardInputEvent::Mods_Alt) != KeyboardInputEvent::Mods_None,
        (modifiers & KeyboardInputEvent::Mods_Super) != KeyboardInputEvent::Mods_None
    );
}

void mouseDownExecHook(EAGLView* self, SEL sel, NSEvent* event) {
    double timestamp = [event timestamp];

    MouseInputEvent inputEvent(
        MouseInputEvent::Button::Left, MouseInputEvent::Action::Press, timestamp
    );

    if (inputEvent.post() != ListenerResult::Propagate) return;

    NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    int eventNumber = [event eventNumber];
    float frameZoomFactor = [self frameZoomFactor];
    float backingScaleFactor = [self getBackingFactor];
    float mouseX = (point.x / frameZoomFactor) / backingScaleFactor;
    float mouseY = (([self getHeight] - point.y) / frameZoomFactor) / backingScaleFactor;
    CCEGLView* eglView = CCEGLView::get();
    if (inputEvent.action == MouseInputEvent::Action::Press) {
        eglView->handleTouchesBegin(1, &eventNumber, &mouseX, &mouseY, timestamp);
    }
    else {
        eglView->handleTouchesEnd(1, &eventNumber, &mouseX, &mouseY, timestamp);
    }
}

void mouseMovedExecHook(EAGLView* self, SEL sel, NSEvent* event) {
    NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];

    if (MouseMoveEvent(point.x, point.y).post() != ListenerResult::Propagate) return;

    [self performSelector:sel withObject:event];
}

void mouseDraggedExecHook(EAGLView* self, SEL sel, NSEvent* event) {
    NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    if (MouseMoveEvent(point.x, point.y).post() != ListenerResult::Propagate) return;

    [self performSelector:sel withObject:event];
}

void mouseUpExecHook(EAGLView* self, SEL sel, NSEvent* event) {
    double timestamp = [event timestamp];

    MouseInputEvent inputEvent(
        MouseInputEvent::Button::Left, MouseInputEvent::Action::Release, timestamp
    );

    if (inputEvent.post() != ListenerResult::Propagate) return;

    NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    int eventNumber = [event eventNumber];
    float frameZoomFactor = [self frameZoomFactor];
    float backingScaleFactor = [self getBackingFactor];
    float mouseX = (point.x / frameZoomFactor) / backingScaleFactor;
    float mouseY = (([self getHeight] - point.y) / frameZoomFactor) / backingScaleFactor;
    CCEGLView* eglView = CCEGLView::get();
    if (inputEvent.action == MouseInputEvent::Action::Press) {
        eglView->handleTouchesBegin(1, &eventNumber, &mouseX, &mouseY, timestamp);
    }
    else {
        eglView->handleTouchesEnd(1, &eventNumber, &mouseX, &mouseY, timestamp);
    }
}

void rightMouseDownHook(EAGLView* self, SEL sel, NSEvent* event) {
    if (MouseInputEvent(
        MouseInputEvent::Button::Right, MouseInputEvent::Action::Press, [event timestamp]
    ).post() != ListenerResult::Propagate) return;

    [self performSelector:sel withObject:event];
}

void rightMouseDraggedHook(EAGLView* self, SEL sel, NSEvent* event) {
    NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];

    if (MouseMoveEvent(point.x, point.y).post() != ListenerResult::Propagate) return;

    [self performSelector:sel withObject:event];
}

void rightMouseUpHook(EAGLView* self, SEL sel, NSEvent* event) {
    if (MouseInputEvent(
        MouseInputEvent::Button::Right, MouseInputEvent::Action::Release, [event timestamp]
    ).post() != ListenerResult::Propagate) return;

    [self performSelector:sel withObject:event];
}

void otherMouseDownHook(EAGLView* self, SEL sel, NSEvent* event) {
    if (MouseInputEvent(
        static_cast<MouseInputEvent::Button>([event buttonNumber]), MouseInputEvent::Action::Press, [event timestamp]
    ).post() != ListenerResult::Propagate) return;

    [self performSelector:sel withObject:event];
}

void otherMouseDraggedHook(EAGLView* self, SEL sel, NSEvent* event) {
    NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];

    if (MouseMoveEvent(point.x, point.y).post() != ListenerResult::Propagate) return;

    [self performSelector:sel withObject:event];
}

void otherMouseUpHook(EAGLView* self, SEL sel, NSEvent* event) {
    if (MouseInputEvent(
        static_cast<MouseInputEvent::Button>([event buttonNumber]), MouseInputEvent::Action::Release, [event timestamp]
    ).post() != ListenerResult::Propagate) return;

    [self performSelector:sel withObject:event];
}

template <class Func>
void hookObjcMethod(const char* className, const char* methodName, Func newImp) {
    if (Result<std::shared_ptr<Hook>> res = ObjcHook::create(className, methodName, newImp)) {
        std::shared_ptr<Hook> hook = res.unwrap();
        hook->setPriority(Priority::Replace);
        (void)Mod::get()->claimHook(hook);
    }
}

$execute {
    hookObjcMethod("EAGLView", "keyDownExec:", &keyDownExecHook);
    hookObjcMethod("EAGLView", "keyUpExec:", &keyUpExecHook);
    hookObjcMethod("EAGLView", "mouseDownExec:", &mouseDownExecHook);
    hookObjcMethod("EAGLView", "mouseMovedExec:", &mouseMovedExecHook);
    hookObjcMethod("EAGLView", "mouseDraggedExec:", &mouseDraggedExecHook);
    hookObjcMethod("EAGLView", "mouseUpExec:", &mouseUpExecHook);
    hookObjcMethod("EAGLView", "rightMouseDown:", &rightMouseDownHook);
    hookObjcMethod("EAGLView", "rightMouseDragged:", &rightMouseDraggedHook);
    hookObjcMethod("EAGLView", "rightMouseUp:", &rightMouseUpHook);
    hookObjcMethod("EAGLView", "otherMouseDown:", &otherMouseDownHook);
    hookObjcMethod("EAGLView", "otherMouseDragged:", &otherMouseDraggedHook);
    hookObjcMethod("EAGLView", "otherMouseUp:", &otherMouseUpHook);
}
