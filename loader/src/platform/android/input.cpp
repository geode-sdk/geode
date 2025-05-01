#include <unordered_map>

#include <android/keycodes.h>
#include <jni.h>
#include <cocos2d.h>

namespace {
    std::unordered_map<int, cocos2d::enumKeyCodes> g_keymap = {
        {AKEYCODE_SPACE, cocos2d::KEY_Space},
        {AKEYCODE_COMMA, cocos2d::KEY_OEMComma},
        {AKEYCODE_PLUS, cocos2d::KEY_OEMPlus},
        {AKEYCODE_MINUS, cocos2d::KEY_OEMMinus},
        {AKEYCODE_PERIOD, cocos2d::KEY_OEMPeriod},
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
        {AKEYCODE_A, cocos2d::KEY_A},
        {AKEYCODE_B, cocos2d::KEY_B},
        {AKEYCODE_C, cocos2d::KEY_C},
        {AKEYCODE_D, cocos2d::KEY_D},
        {AKEYCODE_E, cocos2d::KEY_E},
        {AKEYCODE_F, cocos2d::KEY_F},
        {AKEYCODE_G, cocos2d::KEY_G},
        {AKEYCODE_H, cocos2d::KEY_H},
        {AKEYCODE_I, cocos2d::KEY_I},
        {AKEYCODE_J, cocos2d::KEY_J},
        {AKEYCODE_K, cocos2d::KEY_K},
        {AKEYCODE_L, cocos2d::KEY_L},
        {AKEYCODE_M, cocos2d::KEY_M},
        {AKEYCODE_N, cocos2d::KEY_N},
        {AKEYCODE_O, cocos2d::KEY_O},
        {AKEYCODE_P, cocos2d::KEY_P},
        {AKEYCODE_Q, cocos2d::KEY_Q},
        {AKEYCODE_R, cocos2d::KEY_R},
        {AKEYCODE_S, cocos2d::KEY_S},
        {AKEYCODE_T, cocos2d::KEY_T},
        {AKEYCODE_U, cocos2d::KEY_U},
        {AKEYCODE_V, cocos2d::KEY_V},
        {AKEYCODE_W, cocos2d::KEY_W},
        {AKEYCODE_X, cocos2d::KEY_X},
        {AKEYCODE_Z, cocos2d::KEY_Z},
        {AKEYCODE_Y, cocos2d::KEY_Y},

        {AKEYCODE_ESCAPE, cocos2d::KEY_Escape},
        {AKEYCODE_ENTER, cocos2d::KEY_Enter},
        {AKEYCODE_DEL, cocos2d::KEY_Backspace},
        {AKEYCODE_FORWARD_DEL, cocos2d::KEY_Delete},
        {AKEYCODE_DPAD_LEFT, cocos2d::KEY_Left},
        {AKEYCODE_DPAD_RIGHT, cocos2d::KEY_Right},
        {AKEYCODE_DPAD_UP, cocos2d::KEY_Up},
        {AKEYCODE_DPAD_DOWN, cocos2d::KEY_Down},
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
        {AKEYCODE_SHIFT_LEFT, cocos2d::KEY_LeftShift},
        {AKEYCODE_CTRL_LEFT, cocos2d::KEY_LeftControl},
        {AKEYCODE_ALT_LEFT, cocos2d::KEY_Alt},
        {AKEYCODE_SHIFT_RIGHT, cocos2d::KEY_RightShift},
        {AKEYCODE_CTRL_RIGHT, cocos2d::KEY_RightContol}, // sic
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

        // Geode Additions
        {AKEYCODE_SEMICOLON, cocos2d::KEY_Semicolon},
        {AKEYCODE_APOSTROPHE, cocos2d::KEY_Apostrophe},
        {AKEYCODE_SLASH, cocos2d::KEY_Slash},
        {AKEYCODE_EQUALS, cocos2d::KEY_OEMEqual},
        {AKEYCODE_LEFT_BRACKET, cocos2d::KEY_LeftBracket},
        {AKEYCODE_BACKSLASH, cocos2d::KEY_Backslash},
        {AKEYCODE_RIGHT_BRACKET, cocos2d::KEY_RightBracket},
        {AKEYCODE_GRAVE, cocos2d::KEY_GraveAccent},
        {AKEYCODE_NUMPAD_0, cocos2d::KEY_NumPad0},
        {AKEYCODE_NUMPAD_1, cocos2d::KEY_NumPad1},
        {AKEYCODE_NUMPAD_2, cocos2d::KEY_NumPad2},
        {AKEYCODE_NUMPAD_3, cocos2d::KEY_NumPad3},
        {AKEYCODE_NUMPAD_4, cocos2d::KEY_NumPad4},
        {AKEYCODE_NUMPAD_5, cocos2d::KEY_NumPad5},
        {AKEYCODE_NUMPAD_6, cocos2d::KEY_NumPad6},
        {AKEYCODE_NUMPAD_7, cocos2d::KEY_NumPad7},
        {AKEYCODE_NUMPAD_8, cocos2d::KEY_NumPad8},
        {AKEYCODE_NUMPAD_9, cocos2d::KEY_NumPad9},
        {AKEYCODE_NUMPAD_DOT, cocos2d::KEY_Decimal},
        {AKEYCODE_NUMPAD_DIVIDE, cocos2d::KEY_Divide},
        {AKEYCODE_NUMPAD_MULTIPLY, cocos2d::KEY_Multiply},
        {AKEYCODE_NUMPAD_SUBTRACT, cocos2d::KEY_Subtract},
        {AKEYCODE_NUMPAD_ADD, cocos2d::KEY_Add},
        {AKEYCODE_NUMPAD_ENTER, cocos2d::KEY_NumEnter},
        {AKEYCODE_NUMPAD_EQUALS, cocos2d::KEY_Equal}
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

constexpr auto g_scrollFactor = -13.0f;

extern "C" JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_nativeActionScroll(
    JNIEnv* env, jobject, jfloat scrollX, jfloat scrollY
) {
    cocos2d::CCDirector::sharedDirector()->getMouseDispatcher()->dispatchScrollMSG(
        scrollY * g_scrollFactor, scrollX * g_scrollFactor
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
