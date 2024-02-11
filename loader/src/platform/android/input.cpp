#include <unordered_map>

#include <android/keycodes.h>
#include <jni.h>

namespace {
    std::unordered_map<int, cocos2d::enumKeyCodes> g_keymap = {
        {AKEYCODE_ESCAPE, cocos2d::KEY_Escape},
        {AKEYCODE_SPACE, cocos2d::KEY_Space},
        {AKEYCODE_DPAD_LEFT, cocos2d::KEY_Left},
        {AKEYCODE_DPAD_RIGHT, cocos2d::KEY_Right},
        {AKEYCODE_DPAD_UP, cocos2d::KEY_Up},
        {AKEYCODE_DPAD_DOWN, cocos2d::KEY_Down},
        {AKEYCODE_0, cocos2d::KEY_Zero},
        {AKEYCODE_1, cocos2d::KEY_One},
        {AKEYCODE_2, cocos2d::KEY_Two},
        {AKEYCODE_3, cocos2d::KEY_Three},
        {AKEYCODE_4, cocos2d::KEY_Four},
        {AKEYCODE_5, cocos2d::KEY_Five},
        {AKEYCODE_6, cocos2d::KEY_Six},
        {AKEYCODE_7, cocos2d::KEY_Seven},
        {AKEYCODE_8, cocos2d::KEY_Eight},
        {AKEYCODE_9, cocos2d::KEY_Nine},
        {AKEYCODE_Q, cocos2d::KEY_Q},
        {AKEYCODE_W, cocos2d::KEY_W},
        {AKEYCODE_E, cocos2d::KEY_E},
        {AKEYCODE_R, cocos2d::KEY_R},
        {AKEYCODE_T, cocos2d::KEY_T},
        {AKEYCODE_Y, cocos2d::KEY_Y},
        {AKEYCODE_A, cocos2d::KEY_A},
        {AKEYCODE_S, cocos2d::KEY_S},
        {AKEYCODE_D, cocos2d::KEY_D},
        {AKEYCODE_F, cocos2d::KEY_F},
        {AKEYCODE_G, cocos2d::KEY_G},
        {AKEYCODE_H, cocos2d::KEY_H},
        {AKEYCODE_J, cocos2d::KEY_J},
        {AKEYCODE_K, cocos2d::KEY_K},
        {AKEYCODE_L, cocos2d::KEY_L},
        {AKEYCODE_Z, cocos2d::KEY_Z},
        {AKEYCODE_X, cocos2d::KEY_X},
        {AKEYCODE_C, cocos2d::KEY_C},
        {AKEYCODE_V, cocos2d::KEY_V},
        {AKEYCODE_B, cocos2d::KEY_B},
        {AKEYCODE_N, cocos2d::KEY_N},
        {AKEYCODE_M, cocos2d::KEY_M},
        {AKEYCODE_F1, cocos2d::KEY_F1},
        {AKEYCODE_F2, cocos2d::KEY_F2},
        {AKEYCODE_F3, cocos2d::KEY_F3},
        {AKEYCODE_F4, cocos2d::KEY_F4},
        {AKEYCODE_F5, cocos2d::KEY_F5},
        {AKEYCODE_F6, cocos2d::KEY_F6},
        {AKEYCODE_F7, cocos2d::KEY_F7},
        {AKEYCODE_F8, cocos2d::KEY_F8},
        {AKEYCODE_F9, cocos2d::KEY_F9},
        {AKEYCODE_F10, cocos2d::KEY_F10},
        {AKEYCODE_F11, cocos2d::KEY_F11},
        {AKEYCODE_F12, cocos2d::KEY_F12},
        {AKEYCODE_ENTER, cocos2d::KEY_Enter},
        {AKEYCODE_FORWARD_DEL, cocos2d::KEY_Delete},
        {AKEYCODE_DEL, cocos2d::KEY_Backspace},
        {AKEYCODE_CTRL_LEFT, cocos2d::KEY_LeftControl},
        {AKEYCODE_CTRL_RIGHT, cocos2d::KEY_RightContol}, // sic
        {AKEYCODE_SHIFT_LEFT, cocos2d::KEY_LeftShift},
        {AKEYCODE_SHIFT_RIGHT, cocos2d::KEY_RightShift},
        {AKEYCODE_ALT_LEFT, cocos2d::KEY_Alt},
        {AKEYCODE_ALT_RIGHT, cocos2d::KEY_Alt},
        {AKEYCODE_BUTTON_A, cocos2d::CONTROLLER_A},
        {AKEYCODE_BUTTON_B, cocos2d::CONTROLLER_B},
        {AKEYCODE_BUTTON_X, cocos2d::CONTROLLER_X},
        {AKEYCODE_BUTTON_Y, cocos2d::CONTROLLER_Y},
        {AKEYCODE_BUTTON_START, cocos2d::CONTROLLER_Start},
        {AKEYCODE_BUTTON_SELECT, cocos2d::CONTROLLER_Back},
        {AKEYCODE_BUTTON_R1, cocos2d::CONTROLLER_RB},
        {AKEYCODE_BUTTON_L1, cocos2d::CONTROLLER_LB},
        {AKEYCODE_BUTTON_R2, cocos2d::CONTROLLER_RT},
        {AKEYCODE_BUTTON_L2, cocos2d::CONTROLLER_LT},
    };

    cocos2d::enumKeyCodes translateAndroidKeyCodeToWindows(int keyCode) {
        if (auto it = g_keymap.find(keyCode); it != g_keymap.end()) {
            return it->second;
        }
        else {
            return cocos2d::KEY_None;
        }
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_geode_launcher_utils_GeodeUtils_nativeKeyDown(
	JNIEnv* env, jobject, jint keycode, jint modifiers, jboolean isRepeat
) {
    if (keycode != AKEYCODE_BACK && keycode != AKEYCODE_MENU) {
        auto keyboard_dispatcher = cocos2d::CCDirector::sharedDirector()->getKeyboardDispatcher();
        auto translated_code = translateAndroidKeyCodeToWindows(keycode);

        auto isShiftPressed = modifiers & 0x1;
        auto isCtrlPressed = modifiers & 0x1000;
        auto isAltPressed = modifiers & 0x2;

        keyboard_dispatcher->updateModifierKeys(isShiftPressed, isCtrlPressed, isAltPressed, false);

        keyboard_dispatcher->dispatchKeyboardMSG(translated_code, true, isRepeat);
    } else {
        auto keypad_dispatcher = cocos2d::CCDirector::sharedDirector()->getKeypadDispatcher();
        if (keycode == AKEYCODE_BACK) {
            keypad_dispatcher->dispatchKeypadMSG(cocos2d::kTypeBackClicked);
        } else {
            keypad_dispatcher->dispatchKeypadMSG(cocos2d::kTypeMenuClicked);
        }
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_nativeKeyUp(
    JNIEnv* env, jobject, jint keycode, jint modifiers
) {
    // back/menu keys
    if (keycode != AKEYCODE_BACK && keycode != AKEYCODE_MENU) {
        auto keyboard_dispatcher = cocos2d::CCDirector::sharedDirector()->getKeyboardDispatcher();
        auto translated_code = translateAndroidKeyCodeToWindows(keycode);

        auto isShiftPressed = modifiers & 0x1;
        auto isCtrlPressed = modifiers & 0x1000;
        auto isAltPressed = modifiers & 0x2;

        keyboard_dispatcher->updateModifierKeys(
            isShiftPressed,
            isCtrlPressed,
            isAltPressed,
            false
        );

        keyboard_dispatcher->dispatchKeyboardMSG(translated_code, false, false);
    }
}

constexpr auto g_scrollFactor = -15.0f;

extern "C" JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_nativeActionScroll(
    JNIEnv* env, jobject, jfloat scrollX, jfloat scrollY
) {
    cocos2d::CCDirector::sharedDirector()->getMouseDispatcher()->dispatchScrollMSG(
        scrollX * g_scrollFactor, scrollY * g_scrollFactor
    );
}

extern "C" JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_resizeSurface(
    JNIEnv* env, jobject, jint width, jint height
) {
    auto fWidth = static_cast<float>(width);
    auto fHeight = static_cast<float>(height);

    cocos2d::CCEGLView::sharedOpenGLView()->setFrameSize(fWidth, fHeight);
    cocos2d::CCDirector::sharedDirector()->updateScreenScale({fWidth, fHeight});
    cocos2d::CCDirector::sharedDirector()->setViewport();
    cocos2d::CCDirector::sharedDirector()->setProjection(cocos2d::kCCDirectorProjection2D);
}
