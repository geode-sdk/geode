#include "Geode/loader/Log.hpp"
#include <Geode/DefaultInclude.hpp>
#include <Geode/cocos/CCDirector.h>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h>
#include <Geode/cocos/robtop/mouse_dispatcher/CCMouseDispatcher.h>
#include <Geode/cocos/text_input_node/CCIMEDispatcher.h>
#include <Geode/utils/Keyboard.hpp>
#include <objc/message.h>
#include <objc/runtime.h>
#include <UIKit/UIKit.h>
#include <GameController/GameController.h>

using namespace geode::prelude;

static enumKeyCodes keyToKeyCode(int code) {
    switch (code) {
        case UIKeyboardHIDUsageKeyboardA: return enumKeyCodes::KEY_A;
        case UIKeyboardHIDUsageKeyboardB: return enumKeyCodes::KEY_B;
        case UIKeyboardHIDUsageKeyboardC: return enumKeyCodes::KEY_C;
        case UIKeyboardHIDUsageKeyboardD: return enumKeyCodes::KEY_D;
        case UIKeyboardHIDUsageKeyboardE: return enumKeyCodes::KEY_E;
        case UIKeyboardHIDUsageKeyboardF: return enumKeyCodes::KEY_F;
        case UIKeyboardHIDUsageKeyboardG: return enumKeyCodes::KEY_G;
        case UIKeyboardHIDUsageKeyboardH: return enumKeyCodes::KEY_H;
        case UIKeyboardHIDUsageKeyboardI: return enumKeyCodes::KEY_I;
        case UIKeyboardHIDUsageKeyboardJ: return enumKeyCodes::KEY_J;
        case UIKeyboardHIDUsageKeyboardK: return enumKeyCodes::KEY_K;
        case UIKeyboardHIDUsageKeyboardL: return enumKeyCodes::KEY_L;
        case UIKeyboardHIDUsageKeyboardM: return enumKeyCodes::KEY_M;
        case UIKeyboardHIDUsageKeyboardN: return enumKeyCodes::KEY_N;
        case UIKeyboardHIDUsageKeyboardO: return enumKeyCodes::KEY_O;
        case UIKeyboardHIDUsageKeyboardP: return enumKeyCodes::KEY_P;
        case UIKeyboardHIDUsageKeyboardQ: return enumKeyCodes::KEY_Q;
        case UIKeyboardHIDUsageKeyboardR: return enumKeyCodes::KEY_R;
        case UIKeyboardHIDUsageKeyboardS: return enumKeyCodes::KEY_S;
        case UIKeyboardHIDUsageKeyboardT: return enumKeyCodes::KEY_T;
        case UIKeyboardHIDUsageKeyboardU: return enumKeyCodes::KEY_U;
        case UIKeyboardHIDUsageKeyboardV: return enumKeyCodes::KEY_V;
        case UIKeyboardHIDUsageKeyboardW: return enumKeyCodes::KEY_W;
        case UIKeyboardHIDUsageKeyboardX: return enumKeyCodes::KEY_X;
        case UIKeyboardHIDUsageKeyboardY: return enumKeyCodes::KEY_Y;
        case UIKeyboardHIDUsageKeyboardZ: return enumKeyCodes::KEY_Z;

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

        case UIKeyboardHIDUsageKeyboardEscape: return enumKeyCodes::KEY_Escape;
        case UIKeyboardHIDUsageKeyboardTab: return enumKeyCodes::KEY_Tab;
        case UIKeyboardHIDUsageKeyboardSpacebar: return enumKeyCodes::KEY_Space;
        case UIKeyboardHIDUsageKeyboardDeleteOrBackspace: return enumKeyCodes::KEY_Backspace;
        case UIKeyboardHIDUsageKeyboardInsert: return enumKeyCodes::KEY_Insert;
        case UIKeyboardHIDUsageKeyboardDeleteForward: return enumKeyCodes::KEY_Delete;
        case UIKeyboardHIDUsageKeyboardHome: return enumKeyCodes::KEY_Home;
        case UIKeyboardHIDUsageKeyboardEnd: return enumKeyCodes::KEY_End;
        case UIKeyboardHIDUsageKeyboardPageUp: return enumKeyCodes::KEY_PageUp;
        case UIKeyboardHIDUsageKeyboardPageDown: return enumKeyCodes::KEY_PageDown;

        case UIKeyboardHIDUsageKeyboardUpArrow: return enumKeyCodes::KEY_Up;
        case UIKeyboardHIDUsageKeyboardDownArrow: return enumKeyCodes::KEY_Down;
        case UIKeyboardHIDUsageKeyboardLeftArrow: return enumKeyCodes::KEY_Left;
        case UIKeyboardHIDUsageKeyboardRightArrow: return enumKeyCodes::KEY_Right;

        case UIKeyboardHIDUsageKeyboardCapsLock: return enumKeyCodes::KEY_CapsLock;
        case UIKeyboardHIDUsageKeyboardScrollLock: return enumKeyCodes::KEY_ScrollLock;
        case UIKeyboardHIDUsageKeypadNumLock: return enumKeyCodes::KEY_Numlock;
        case UIKeyboardHIDUsageKeyboardPrintScreen: return enumKeyCodes::KEY_PrintScreen;
        case UIKeyboardHIDUsageKeyboardPause: return enumKeyCodes::KEY_Pause;

        case UIKeyboardHIDUsageKeyboardLeftShift: return enumKeyCodes::KEY_LeftShift;
        case UIKeyboardHIDUsageKeyboardRightShift: return enumKeyCodes::KEY_RightShift;
        case UIKeyboardHIDUsageKeyboardLeftControl: return enumKeyCodes::KEY_LeftControl;
        case UIKeyboardHIDUsageKeyboardRightControl: return enumKeyCodes::KEY_RightControl;
        case UIKeyboardHIDUsageKeyboardLeftAlt: return enumKeyCodes::KEY_LeftMenu;
        case UIKeyboardHIDUsageKeyboardRightAlt: return enumKeyCodes::KEY_RightMenu;
        case UIKeyboardHIDUsageKeyboardReturnOrEnter: return enumKeyCodes::KEY_Enter;
        case UIKeyboardHIDUsageKeyboardReturn: return enumKeyCodes::KEY_Enter;

        case UIKeyboardHIDUsageKeyboardLeftGUI: return enumKeyCodes::KEY_LeftWindowsKey;
        case UIKeyboardHIDUsageKeyboardRightGUI: return enumKeyCodes::KEY_RightWindowsKey;
        case UIKeyboardHIDUsageKeyboardMenu: return enumKeyCodes::KEY_ApplicationsKey;

        case UIKeyboardHIDUsageKeyboardHyphen: return enumKeyCodes::KEY_OEMMinus;
        case UIKeyboardHIDUsageKeyboardComma: return enumKeyCodes::KEY_OEMComma;
        case UIKeyboardHIDUsageKeyboardPeriod: return enumKeyCodes::KEY_OEMPeriod;
        case UIKeyboardHIDUsageKeyboardSemicolon: return enumKeyCodes::KEY_Semicolon;
        case UIKeyboardHIDUsageKeyboardQuote: return enumKeyCodes::KEY_Apostrophe;
        case UIKeyboardHIDUsageKeyboardSlash: return enumKeyCodes::KEY_Slash;
        case UIKeyboardHIDUsageKeyboardEqualSign: return enumKeyCodes::KEY_Equal;
        case UIKeyboardHIDUsageKeyboardOpenBracket: return enumKeyCodes::KEY_LeftBracket;
        case UIKeyboardHIDUsageKeyboardBackslash: return enumKeyCodes::KEY_Backslash;
        case UIKeyboardHIDUsageKeyboardCloseBracket: return enumKeyCodes::KEY_RightBracket;
        case UIKeyboardHIDUsageKeyboardGraveAccentAndTilde: return enumKeyCodes::KEY_GraveAccent;
        case UIKeyboardHIDUsageKeyboardClear: return enumKeyCodes::KEY_Clear;
        case UIKeyboardHIDUsageKeyboardNonUSBackslash: return enumKeyCodes::KEY_World1;
        case UIKeyboardHIDUsageKeyboardNonUSPound: return enumKeyCodes::KEY_World2;

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
        case UIKeyboardHIDUsageKeypadPeriod: return enumKeyCodes::KEY_Decimal;
        case UIKeyboardHIDUsageKeypadSlash: return enumKeyCodes::KEY_Divide;
        case UIKeyboardHIDUsageKeypadAsterisk: return enumKeyCodes::KEY_Multiply;
        case UIKeyboardHIDUsageKeypadHyphen: return enumKeyCodes::KEY_Subtract;
        case UIKeyboardHIDUsageKeypadPlus: return enumKeyCodes::KEY_Add;
        case UIKeyboardHIDUsageKeypadEnter: return enumKeyCodes::KEY_NumEnter;
        case UIKeyboardHIDUsageKeypadEqualSign: return enumKeyCodes::KEY_OEMEqual;

        default: return enumKeyCodes::KEY_Unknown;
    }
}

static enumKeyCodes buttonToKeyCode(GCExtendedGamepad* pad, GCControllerButtonInput* btn) {
    if (btn == pad.buttonA) return enumKeyCodes::CONTROLLER_A;
    if (btn == pad.buttonB) return enumKeyCodes::CONTROLLER_B;
    if (btn == pad.buttonX) return enumKeyCodes::CONTROLLER_X;
    if (btn == pad.buttonY) return enumKeyCodes::CONTROLLER_Y;

    if (btn == pad.leftShoulder) return enumKeyCodes::CONTROLLER_LB;
    if (btn == pad.rightShoulder) return enumKeyCodes::CONTROLLER_RB;

    if (btn == pad.leftTrigger) return enumKeyCodes::CONTROLLER_LT;
    if (btn == pad.rightTrigger) return enumKeyCodes::CONTROLLER_RT;

    if (btn == pad.dpad.up) return enumKeyCodes::CONTROLLER_Up;
    if (btn == pad.dpad.down) return enumKeyCodes::CONTROLLER_Down;
    if (btn == pad.dpad.left) return enumKeyCodes::CONTROLLER_Left;
    if (btn == pad.dpad.right) return enumKeyCodes::CONTROLLER_Right;

    if (btn == pad.buttonOptions) return enumKeyCodes::CONTROLLER_Back;
    if (btn == pad.buttonMenu) return enumKeyCodes::CONTROLLER_Start;

    return enumKeyCodes::KEY_Unknown;
}

@implementation UIWindow (KeyInterceptor)

- (void)handleKeyDown:(UIEvent*) event withPress:(UIPress*) press withTime:(double) timestamp withRepeat:(bool) isRepeat {
    UIKey* key = [press key];

    unichar nativeCode = [key keyCode];
    enumKeyCodes keyCode = keyToKeyCode(nativeCode);
    CCIMEDispatcher* imeDispatcher = CCIMEDispatcher::sharedDispatcher();
    CCKeyboardDispatcher* keyboardDispatcher = CCKeyboardDispatcher::get();
    bool isDown = true;
    UIKeyModifierFlags flags = [event modifierFlags];

    KeyboardInputData::Modifiers modifiers = KeyboardInputData::Mods_None;
    if ((flags & UIKeyModifierShift) != 0) {
        modifiers |= KeyboardInputData::Mods_Shift;
    }
    if ((flags & UIKeyModifierControl) != 0) {
        modifiers |= KeyboardInputData::Mods_Control;
    }
    if ((flags & UIKeyModifierAlternate) != 0) {
        modifiers |= KeyboardInputData::Mods_Alt;
    }
    if ((flags & UIKeyModifierCommand) != 0) {
        modifiers |= KeyboardInputData::Mods_Super;
    }

    KeyboardInputData data(
        keyCode, isRepeat ? KeyboardInputData::Action::Repeat : KeyboardInputData::Action::Press,
        { nativeCode },
        timestamp,
        modifiers
    );

    if (KeyboardInputEvent(keyCode).send(data) != ListenerResult::Propagate) return;

    isDown = data.action != KeyboardInputData::Action::Release;
    isRepeat = data.action == KeyboardInputData::Action::Repeat;
    keyCode = data.key;
    modifiers = data.modifiers;

    if (keyCode != KEY_Unknown && (!imeDispatcher->hasDelegate() || keyCode == KEY_Escape || keyCode == KEY_Enter)) {
        keyboardDispatcher->dispatchKeyboardMSG(keyCode, isDown, isRepeat, timestamp);
    }

    keyboardDispatcher->updateModifierKeys(
        modifiers & KeyboardInputData::Mods_Shift,
        modifiers & KeyboardInputData::Mods_Control,
        modifiers & KeyboardInputData::Mods_Alt,
        modifiers & KeyboardInputData::Mods_Super
    );
}

- (void)handleKeyUp:(UIEvent*) event withPress:(UIPress*) press {
    UIKey* key = [press key];

    unichar nativeCode = [key keyCode];
    enumKeyCodes keyCode = keyToKeyCode(nativeCode);
    CCIMEDispatcher* imeDispatcher = CCIMEDispatcher::sharedDispatcher();
    CCKeyboardDispatcher* keyboardDispatcher = CCKeyboardDispatcher::get();
    double timestamp = [event timestamp];
    bool isRepeat = false;
    bool isDown = false;
    UIKeyModifierFlags flags = [event modifierFlags];

    KeyboardInputData::Modifiers modifiers = KeyboardInputData::Mods_None;
    if ((flags & UIKeyModifierShift) != 0) {
        modifiers |= KeyboardInputData::Mods_Shift;
    }
    if ((flags & UIKeyModifierControl) != 0) {
        modifiers |= KeyboardInputData::Mods_Control;
    }
    if ((flags & UIKeyModifierAlternate) != 0) {
        modifiers |= KeyboardInputData::Mods_Alt;
    }
    if ((flags & UIKeyModifierCommand) != 0) {
        modifiers |= KeyboardInputData::Mods_Super;
    }

    KeyboardInputData data(
        keyCode, KeyboardInputData::Action::Release,
        { nativeCode },
        timestamp,
        modifiers
    );

    if (KeyboardInputEvent(keyCode).send(data) != ListenerResult::Propagate) return;

    isDown = data.action != KeyboardInputData::Action::Release;
    isRepeat = data.action == KeyboardInputData::Action::Repeat;
    keyCode = data.key;
    modifiers = data.modifiers;
    if (keyCode != KEY_Unknown && (!imeDispatcher->hasDelegate() || keyCode == KEY_Escape || keyCode == KEY_Enter)) {
        keyboardDispatcher->dispatchKeyboardMSG(keyCode, isDown, isRepeat, timestamp);
    }

    keyboardDispatcher->updateModifierKeys(
        modifiers & KeyboardInputData::Mods_Shift,
        modifiers & KeyboardInputData::Mods_Control,
        modifiers & KeyboardInputData::Mods_Alt,
        modifiers & KeyboardInputData::Mods_Super
    );
}

static NSTimer* delayTimer = nil;
static NSTimer* repeatTimer = nil;
static UIEvent* repeatEvent = nil;
static UIPress* repeatPress = nil;

// iOS doesn't have built in repeat, so we handle it ourselves
- (void)startRepeat:(UIEvent*)event withPress:(UIPress*)press {
    [self stopRepeat];
    repeatEvent = event;
    repeatPress = press;
    delayTimer = [NSTimer scheduledTimerWithTimeInterval:.5 repeats:NO block:^(NSTimer * _Nonnull t) {
        delayTimer = nil;
        repeatTimer = [NSTimer scheduledTimerWithTimeInterval:0.05 repeats:YES block:^(NSTimer * _Nonnull t2) {
            [self handleKeyDown:repeatEvent withPress:repeatPress withTime:[[NSProcessInfo processInfo] systemUptime] withRepeat:true];
        }];
    }];
}

- (void)stopRepeat {
    if (delayTimer) { [delayTimer invalidate]; delayTimer = nil; }
    if (repeatTimer) { [repeatTimer invalidate]; repeatTimer = nil; }
    repeatPress = nil;
    repeatEvent = nil;
}

typedef CGVector(*ScrollDeltaFunc)(id, SEL);
constexpr auto g_scrollFactor = 13.0f;

- (void)g_sendEvent:(UIEvent*)event {
    if ([event type] == UIEventTypePresses || [event type] == 4) { // it's sending as 4 for me, even though it should be sending as 3 (which is UIEventTypePresses)
        UIPressesEvent* pe = (UIPressesEvent*)event;
        for (UIPress* press in [pe allPresses]) {
            switch([press phase]) {
                case UIPressPhaseBegan: {
                    [self handleKeyDown:event withPress:press withTime:[event timestamp] withRepeat:false];
                    [self startRepeat:event withPress:press];
                    break;
                }
                case UIPressPhaseEnded: {
                    [self handleKeyUp:event withPress:press];
                    [self stopRepeat];
                    break;
                }
                default: break;
            }
        }
    } else if ([event type] == UIEventTypeScroll) {
        // UIScrollEvent is a private class, we love apple's private APIs
        Class scrollClass = NSClassFromString(@"UIScrollEvent");
        if (scrollClass && [event isKindOfClass:scrollClass]) {
            SEL selector = NSSelectorFromString(@"nonAcceleratedDelta");
            if (![event respondsToSelector:selector]) {
                return;
            }
            IMP imp = [event methodForSelector:selector];
            ScrollDeltaFunc getDelta = (ScrollDeltaFunc)imp;
            CGVector delta = getDelta(event, selector);

            if (ScrollWheelEvent().send(delta.dx, delta.dy) == ListenerResult::Stop) {
                return;
            }

            CCDirector::get()->getMouseDispatcher()->dispatchScrollMSG(
                delta.dx * g_scrollFactor, delta.dy * g_scrollFactor
            );
        }
    }
    [self g_sendEvent:event];
}

inline void handleControllerButton(GCExtendedGamepad* gamepad, GCControllerButtonInput* btn) {
    CCIMEDispatcher* imeDispatcher = CCIMEDispatcher::sharedDispatcher();
    CCKeyboardDispatcher* keyboardDispatcher = CCKeyboardDispatcher::get();
    enumKeyCodes key = buttonToKeyCode(gamepad, btn);
    KeyboardInputData data(
        key,
        [btn isPressed] ? KeyboardInputData::Action::Press : KeyboardInputData::Action::Release,
        {0}, // Controller buttons don't have native codes.
        (double) [gamepad lastEventTimestamp],
        KeyboardInputData::Modifiers::Mods_None
    );
    if (KeyboardInputEvent(key).send(data) != ListenerResult::Propagate) return;
    if (data.key != KEY_Unknown && (!imeDispatcher->hasDelegate() || data.key == KEY_Escape || data.key == KEY_Enter)) {
        keyboardDispatcher->dispatchKeyboardMSG(data.key, data.action != KeyboardInputData::Action::Release, data.action == KeyboardInputData::Action::Repeat, data.timestamp);
    }
}

$execute {
    Method m1 = class_getInstanceMethod([UIWindow class], @selector(sendEvent:));
    Method m2 = class_getInstanceMethod([UIWindow class], @selector(g_sendEvent:));
    method_exchangeImplementations(m1, m2);

    [[NSNotificationCenter defaultCenter] addObserverForName:GCControllerDidConnectNotification object:nil queue:nil usingBlock:^(NSNotification* note) {
        GCController* controller = [note object];
        if (![controller extendedGamepad]) return;
        GCExtendedGamepad* pad = [controller extendedGamepad];
        pad.valueChangedHandler = ^(GCExtendedGamepad* gamepad, GCControllerElement* element) {
        if ([element isKindOfClass:[GCControllerButtonInput class]]) {
            handleControllerButton(gamepad, (GCControllerButtonInput*) element);
        } else if ([element isKindOfClass:[GCControllerDirectionPad class]]) {
            if (element == [pad dpad])  {
                static BOOL prevUp, prevDown, prevLeft, prevRight;
                bool upPressed = [[[pad dpad] up] isPressed];
                bool downPressed = [[[pad dpad] down] isPressed];
                bool leftPressed = [[[pad dpad] left] isPressed];
                bool rightPressed = [[[pad dpad] right] isPressed];
                if (upPressed != prevUp) {
                    auto btn = [[pad dpad] up];
                    handleControllerButton(gamepad, btn);
                    prevUp = upPressed;
                }
                if (downPressed != prevDown) {
                    auto btn = [[pad dpad] down];
                    handleControllerButton(gamepad, btn);
                    prevDown = downPressed;
                }
                if (leftPressed != prevLeft) {
                    auto btn = [[pad dpad] left];
                    handleControllerButton(gamepad, btn);
                    prevLeft = leftPressed;
                }
                if (rightPressed != prevRight) {
                    auto btn = [[pad dpad] right];
                    handleControllerButton(gamepad, btn);
                    prevRight = rightPressed;
                }
            }
        }
    };

    }];
}

@end