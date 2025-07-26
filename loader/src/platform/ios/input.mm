#include <Geode/platform/cplatform.h>
#ifdef GEODE_IS_IOS
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/ObjcHook.hpp>
#include <objc/message.h>
#import <UIKit/UIKit.h>

using namespace geode::prelude;

std::unordered_map<int, enumKeyCodes> keyMap = {
    {UIKeyboardHIDUsageKeyboardSpacebar, cocos2d::KEY_Space},
    {UIKeyboardHIDUsageKeyboardComma, cocos2d::KEY_OEMComma},
    {UIKeyboardHIDUsageKeyboardHyphen, cocos2d::KEY_OEMMinus},
    {UIKeyboardHIDUsageKeyboardPeriod, cocos2d::KEY_OEMPeriod},
    {UIKeyboardHIDUsageKeyboard0, cocos2d::KEY_Zero},
    {UIKeyboardHIDUsageKeyboard1, cocos2d::KEY_One},
    {UIKeyboardHIDUsageKeyboard2, cocos2d::KEY_Two},
    {UIKeyboardHIDUsageKeyboard3, cocos2d::KEY_Three},
    {UIKeyboardHIDUsageKeyboard4, cocos2d::KEY_Four},
    {UIKeyboardHIDUsageKeyboard5, cocos2d::KEY_Five},
    {UIKeyboardHIDUsageKeyboard6, cocos2d::KEY_Six},
    {UIKeyboardHIDUsageKeyboard7, cocos2d::KEY_Seven},
    {UIKeyboardHIDUsageKeyboard8, cocos2d::KEY_Eight},
    {UIKeyboardHIDUsageKeyboard9, cocos2d::KEY_Nine},
    {UIKeyboardHIDUsageKeyboardA, cocos2d::KEY_A},
    {UIKeyboardHIDUsageKeyboardB, cocos2d::KEY_B},
    {UIKeyboardHIDUsageKeyboardC, cocos2d::KEY_C},
    {UIKeyboardHIDUsageKeyboardD, cocos2d::KEY_D},
    {UIKeyboardHIDUsageKeyboardE, cocos2d::KEY_E},
    {UIKeyboardHIDUsageKeyboardF, cocos2d::KEY_F},
    {UIKeyboardHIDUsageKeyboardG, cocos2d::KEY_G},
    {UIKeyboardHIDUsageKeyboardH, cocos2d::KEY_H},
    {UIKeyboardHIDUsageKeyboardI, cocos2d::KEY_I},
    {UIKeyboardHIDUsageKeyboardJ, cocos2d::KEY_J},
    {UIKeyboardHIDUsageKeyboardK, cocos2d::KEY_K},
    {UIKeyboardHIDUsageKeyboardL, cocos2d::KEY_L},
    {UIKeyboardHIDUsageKeyboardM, cocos2d::KEY_M},
    {UIKeyboardHIDUsageKeyboardN, cocos2d::KEY_N},
    {UIKeyboardHIDUsageKeyboardO, cocos2d::KEY_O},
    {UIKeyboardHIDUsageKeyboardP, cocos2d::KEY_P},
    {UIKeyboardHIDUsageKeyboardQ, cocos2d::KEY_Q},
    {UIKeyboardHIDUsageKeyboardR, cocos2d::KEY_R},
    {UIKeyboardHIDUsageKeyboardS, cocos2d::KEY_S},
    {UIKeyboardHIDUsageKeyboardT, cocos2d::KEY_T},
    {UIKeyboardHIDUsageKeyboardU, cocos2d::KEY_U},
    {UIKeyboardHIDUsageKeyboardV, cocos2d::KEY_V},
    {UIKeyboardHIDUsageKeyboardW, cocos2d::KEY_W},
    {UIKeyboardHIDUsageKeyboardX, cocos2d::KEY_X},
    {UIKeyboardHIDUsageKeyboardY, cocos2d::KEY_Y},
    {UIKeyboardHIDUsageKeyboardZ, cocos2d::KEY_Z},
    {UIKeyboardHIDUsageKeyboardReturnOrEnter, cocos2d::KEY_Enter},
    {UIKeyboardHIDUsageKeyboardEscape, cocos2d::KEY_Escape},
    {UIKeyboardHIDUsageKeyboardDeleteOrBackspace, cocos2d::KEY_Backspace},
    {UIKeyboardHIDUsageKeyboardLeftArrow, cocos2d::KEY_Left},
    {UIKeyboardHIDUsageKeyboardRightArrow, cocos2d::KEY_Right},
    {UIKeyboardHIDUsageKeyboardUpArrow, cocos2d::KEY_Up},
    {UIKeyboardHIDUsageKeyboardDownArrow, cocos2d::KEY_Down},
    {UIKeyboardHIDUsageKeyboardF1, cocos2d::KEY_F1},
    {UIKeyboardHIDUsageKeyboardF2, cocos2d::KEY_F2},
    {UIKeyboardHIDUsageKeyboardF3, cocos2d::KEY_F3},
    {UIKeyboardHIDUsageKeyboardF4, cocos2d::KEY_F4},
    {UIKeyboardHIDUsageKeyboardF5, cocos2d::KEY_F5},
    {UIKeyboardHIDUsageKeyboardF6, cocos2d::KEY_F6},
    {UIKeyboardHIDUsageKeyboardF7, cocos2d::KEY_F7},
    {UIKeyboardHIDUsageKeyboardF8, cocos2d::KEY_F8},
    {UIKeyboardHIDUsageKeyboardF9, cocos2d::KEY_F9},
    {UIKeyboardHIDUsageKeyboardF10, cocos2d::KEY_F10},
    {UIKeyboardHIDUsageKeyboardF11, cocos2d::KEY_F11},
    {UIKeyboardHIDUsageKeyboardF12, cocos2d::KEY_F12},
    {UIKeyboardHIDUsageKeyboardLeftShift, cocos2d::KEY_LeftShift},
    {UIKeyboardHIDUsageKeyboardLeftControl, cocos2d::KEY_LeftControl},
    {UIKeyboardHIDUsageKeyboardLeftAlt, cocos2d::KEY_Alt},
    {UIKeyboardHIDUsageKeyboardRightShift, cocos2d::KEY_RightShift},
    {UIKeyboardHIDUsageKeyboardRightControl, cocos2d::KEY_RightContol},
    {UIKeyboardHIDUsageKeyboardRightAlt, cocos2d::KEY_Alt},
    {UIKeyboardHIDUsageKeyboardSemicolon, cocos2d::KEY_Semicolon},
    {UIKeyboardHIDUsageKeyboardQuote, cocos2d::KEY_Apostrophe},
    {UIKeyboardHIDUsageKeyboardSlash, cocos2d::KEY_Slash},
    {UIKeyboardHIDUsageKeyboardEqualSign, cocos2d::KEY_OEMEqual},
    {UIKeyboardHIDUsageKeyboardOpenBracket, cocos2d::KEY_LeftBracket},
    {UIKeyboardHIDUsageKeyboardBackslash, cocos2d::KEY_Backslash},
    {UIKeyboardHIDUsageKeyboardCloseBracket, cocos2d::KEY_RightBracket},
    {UIKeyboardHIDUsageKeyboardGraveAccentAndTilde, cocos2d::KEY_GraveAccent},
    {UIKeyboardHIDUsageKeypad0, cocos2d::KEY_NumPad0},
    {UIKeyboardHIDUsageKeypad1, cocos2d::KEY_NumPad1},
    {UIKeyboardHIDUsageKeypad2, cocos2d::KEY_NumPad2},
    {UIKeyboardHIDUsageKeypad3, cocos2d::KEY_NumPad3},
    {UIKeyboardHIDUsageKeypad4, cocos2d::KEY_NumPad4},
    {UIKeyboardHIDUsageKeypad5, cocos2d::KEY_NumPad5},
    {UIKeyboardHIDUsageKeypad6, cocos2d::KEY_NumPad6},
    {UIKeyboardHIDUsageKeypad7, cocos2d::KEY_NumPad7},
    {UIKeyboardHIDUsageKeypad8, cocos2d::KEY_NumPad8},
    {UIKeyboardHIDUsageKeypad9, cocos2d::KEY_NumPad9},
    {UIKeyboardHIDUsageKeypadPeriod, cocos2d::KEY_Decimal},
    {UIKeyboardHIDUsageKeypadSlash, cocos2d::KEY_Divide},
    {UIKeyboardHIDUsageKeypadAsterisk, cocos2d::KEY_Multiply},
    {UIKeyboardHIDUsageKeypadHyphen, cocos2d::KEY_Subtract},
    {UIKeyboardHIDUsageKeypadPlus, cocos2d::KEY_Add},
    {UIKeyboardHIDUsageKeypadEnter, cocos2d::KEY_NumEnter}
};

@interface KeyCatcherView : UIView {
    NSTimer* delayTimer;
    NSTimer* repeatTimer;
    UIKey* repeatKey;
}
@end

@implementation KeyCatcherView

- (BOOL)canBecomeFirstResponder {
    return YES;
}

- (void)startRepeat:(UIKey*)key {
    [self stopRepeat];
    repeatKey = key;
    delayTimer = [NSTimer scheduledTimerWithTimeInterval:1.0 repeats:NO block:^(NSTimer * _Nonnull timer) {
        delayTimer = nil;
        repeatTimer = [NSTimer scheduledTimerWithTimeInterval:0.05 repeats:YES block:^(NSTimer * _Nonnull timer) {
            CCKeyboardDispatcher* dispatcher = CCDirector::get()->getKeyboardDispatcher();
            auto it = keyMap.find(repeatKey.keyCode);
            if (it != keyMap.end()) {
                dispatcher->dispatchKeyboardMSG(it->second, true, true);
            }
        }];
    }];
}

- (void)stopRepeat {
    if (delayTimer) {
        [delayTimer invalidate];
        delayTimer = nil;
    }
    if (repeatTimer) {
        [repeatTimer invalidate];
        repeatTimer = nil;
    }
    repeatKey = nil;
}

- (void)pressesBegan:(NSSet<UIPress *> *)presses withEvent:(UIPressesEvent *)event {
    for (UIPress *press in presses) {
        UIKey *key = press.key;
        if (key) {
            NSString *chars = key.charactersIgnoringModifiers;
            NSUInteger mods = key.modifierFlags;
            CCKeyboardDispatcher* dispatcher = CCDirector::get()->getKeyboardDispatcher();
            dispatcher->updateModifierKeys(
                mods & UIKeyModifierShift, mods & UIKeyModifierControl, mods & UIKeyModifierAlternate, mods & UIKeyModifierCommand
            );
            if (auto it = keyMap.find(key.keyCode); it != keyMap.end()) {
                dispatcher->dispatchKeyboardMSG(it->second, true, false);
            } else {
                dispatcher->dispatchKeyboardMSG(enumKeyCodes::KEY_Unknown, true, false);
            }
            [self startRepeat:key];
        }
    }
    [super pressesBegan:presses withEvent:event];
}

- (void)pressesEnded:(NSSet<UIPress *> *)presses withEvent:(UIPressesEvent *)event {
    for (UIPress *press in presses) {
        UIKey *key = press.key;
        if (key) {
            NSString *chars = key.charactersIgnoringModifiers;
            NSUInteger mods = key.modifierFlags;
            CCKeyboardDispatcher* dispatcher = CCDirector::get()->getKeyboardDispatcher();
            dispatcher->updateModifierKeys(
                mods & UIKeyModifierShift, mods & UIKeyModifierControl, mods & UIKeyModifierAlternate, mods & UIKeyModifierCommand
            );
            if (auto it = keyMap.find(key.keyCode); it != keyMap.end()) {
                dispatcher->dispatchKeyboardMSG(it->second, false, false);
            } else {
                dispatcher->dispatchKeyboardMSG(enumKeyCodes::KEY_Unknown, false, false);
            }
            [self stopRepeat];
        }
    }
    [super pressesEnded:presses withEvent:event];
}

@end

void launchHook(id self, SEL sel, UIApplication* app, NSDictionary* opts) {
    reinterpret_cast<void(*)(id, SEL, UIApplication*, NSDictionary*)>(objc_msgSend)(self, sel, app, opts);

    UIWindow* window = nil;
    for (UIWindow* win in UIApplication.sharedApplication.windows) {
        if (win.isKeyWindow) {
            window = win;
            break;
        }
    }
    if (window) {
        CGRect frame = window.bounds;
        KeyCatcherView* keyView = [[KeyCatcherView alloc] initWithFrame:frame];
        keyView.backgroundColor = UIColor.clearColor;
        keyView.userInteractionEnabled = NO;
        [window addSubview:keyView];
        [keyView becomeFirstResponder];
    } else {
        geode::log::warn("No key window found, keyboard will not work.");
    }
}

$execute {
    if (auto hook = ObjcHook::create(
        "AppController",
        "application:didFinishLaunchingWithOptions:",
        &launchHook
    )) {
        (void) Mod::get()->claimHook(hook.unwrap());
    }
}

#endif
