#include <Geode/utils/AndroidEvent.hpp>
#include <Geode/utils/Keyboard.hpp>
#include <Geode/Prelude.hpp>
#include <cocos2d.h>
#include <android/keycodes.h>
#include <jni.h>
#include <unordered_map>

using namespace geode::prelude;

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

    cocos2d::enumKeyCodes translateAndroidKeyCodeToWindows(int keyCode, bool isController) {
        if (isController) {
            switch (keyCode) {
                case AKEYCODE_DPAD_DOWN:
                    return cocos2d::CONTROLLER_Down;
                case AKEYCODE_DPAD_UP:
                    return cocos2d::CONTROLLER_Up;
                case AKEYCODE_DPAD_LEFT:
                    return cocos2d::CONTROLLER_Left;
                case AKEYCODE_DPAD_RIGHT:
                    return cocos2d::CONTROLLER_Right;
                default:
                    break;
            }
        }

        if (auto it = g_keymap.find(keyCode); it != g_keymap.end()) {
            return it->second;
        }
        else {
            return cocos2d::KEY_None;
        }
    }

    void onKeyDown(bool isController, jint keycode, jint modifiers, bool isRepeat, double timestamp) {
        if (keycode != AKEYCODE_BACK && keycode != AKEYCODE_MENU) {
            auto keyboard_dispatcher = cocos2d::CCDirector::sharedDirector()->getKeyboardDispatcher();
            auto translated_code = translateAndroidKeyCodeToWindows(keycode, isController);

            auto isShiftPressed = modifiers & 0x1;
            auto isCtrlPressed = modifiers & 0x1000;
            auto isAltPressed = modifiers & 0x2;

            KeyboardInputData data(
                translated_code,
                isRepeat ? KeyboardInputData::Action::Repeat : KeyboardInputData::Action::Press,
                { static_cast<uint64_t>(keycode), 0 },
                timestamp,
                (isShiftPressed ? KeyboardInputData::Mods_Shift : KeyboardInputData::Mods_None) |
                (isCtrlPressed ? KeyboardInputData::Mods_Control : KeyboardInputData::Mods_None) |
                (isAltPressed ? KeyboardInputData::Mods_Alt : KeyboardInputData::Mods_None)
            );

            if (KeyboardInputEvent(translated_code).send(data) != ListenerResult::Propagate) {
                return;
            }

            keyboard_dispatcher->updateModifierKeys(
                data.modifiers & KeyboardInputData::Mods_Shift,
                data.modifiers & KeyboardInputData::Mods_Control,
                data.modifiers & KeyboardInputData::Mods_Alt,
                false
            );

            keyboard_dispatcher->dispatchKeyboardMSG(
                data.key,
                data.action != KeyboardInputData::Action::Release,
                data.action == KeyboardInputData::Action::Repeat,
                data.timestamp
            );
        } else {
            auto keypad_dispatcher = cocos2d::CCDirector::sharedDirector()->getKeypadDispatcher();
            if (keycode == AKEYCODE_BACK) {
                keypad_dispatcher->dispatchKeypadMSG(cocos2d::kTypeBackClicked);
            } else {
                keypad_dispatcher->dispatchKeypadMSG(cocos2d::kTypeMenuClicked);
            }
        }
    }

    void onKeyUp(bool isController, jint keycode, jint modifiers, double timestamp) {
        // back/menu keys
        if (keycode != AKEYCODE_BACK && keycode != AKEYCODE_MENU) {
            auto keyboard_dispatcher = cocos2d::CCDirector::sharedDirector()->getKeyboardDispatcher();
            auto translated_code = translateAndroidKeyCodeToWindows(keycode, isController);

            auto isShiftPressed = modifiers & 0x1;
            auto isCtrlPressed = modifiers & 0x1000;
            auto isAltPressed = modifiers & 0x2;

            KeyboardInputData data(
                translated_code,
                KeyboardInputData::Action::Release,
                { static_cast<uint64_t>(keycode), 0 },
                timestamp,
                (isShiftPressed ? KeyboardInputData::Mods_Shift : KeyboardInputData::Mods_None) |
                (isCtrlPressed ? KeyboardInputData::Mods_Control : KeyboardInputData::Mods_None) |
                (isAltPressed ? KeyboardInputData::Mods_Alt : KeyboardInputData::Mods_None)
            );

            if (KeyboardInputEvent(translated_code).send(data) != ListenerResult::Propagate) {
                return;
            }

            keyboard_dispatcher->updateModifierKeys(
                data.modifiers & KeyboardInputData::Mods_Shift,
                data.modifiers & KeyboardInputData::Mods_Control,
                data.modifiers & KeyboardInputData::Mods_Alt,
                false
            );

            keyboard_dispatcher->dispatchKeyboardMSG(
                data.key,
                data.action != KeyboardInputData::Action::Release,
                data.action == KeyboardInputData::Action::Repeat,
                data.timestamp
            );
        }
    }

    std::vector<float> extractFloatArray(JNIEnv* env, jfloatArray array) {
        if (!array) {
            return {};
        }

        auto size = env->GetArrayLength(array);

        std::vector<float> res(size);

        env->GetFloatArrayRegion(array, 0, size, res.data());

        return res;
    }

    std::vector<int> extractIntArray(JNIEnv* env, jintArray array) {
        if (!array) {
            return {};
        }

        auto size = env->GetArrayLength(array);

        std::vector<int> res(size);

        env->GetIntArrayRegion(array, 0, size, res.data());

        return res;
    }
}


extern "C" JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_internalKeyEvent(
    JNIEnv* env, jobject, jlong timestamp, jint deviceId, jint eventSource, jint keyCode, jint modifiers, jboolean isDown, jint repeatCount
) {
    if (AndroidRichInputEvent().send(timestamp, deviceId, eventSource, AndroidKeyInput(keyCode, modifiers, isDown, repeatCount)) != ListenerResult::Propagate) {
        return;
    }

    auto isController = eventSource == 0x00000401 || eventSource == 0x01000010;
    double timeInSeconds = static_cast<double>(timestamp) / 1'000'000.0;

    if (isDown) {
        onKeyDown(isController, keyCode, modifiers, repeatCount > 0, timeInSeconds);
    } else {
        onKeyUp(isController, keyCode, modifiers, timeInSeconds);
    }
}


constexpr auto g_scrollFactor = -13.0f;

extern "C" JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_internalScrollEvent(
    JNIEnv* env, jobject, jlong timestamp, jint deviceId, jint eventSource, jfloat scrollX, jfloat scrollY
) {
    if (AndroidRichInputEvent().send(timestamp, deviceId, eventSource, AndroidScrollInput(scrollX, scrollY)) != ListenerResult::Propagate) {
        return;
    }

    if (ScrollWheelEvent().send(scrollX, scrollY) == ListenerResult::Stop) {
        return;
    }

    cocos2d::CCDirector::sharedDirector()->getMouseDispatcher()->dispatchScrollMSG(
        scrollY * g_scrollFactor, scrollX * g_scrollFactor
    );
}

extern "C" JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_internalTouchEvent(
    JNIEnv* env, jobject, jlong timestamp, jint deviceId, jint eventSource, jint eventType, jintArray ids, jfloatArray xs, jfloatArray ys
) {
    auto idArr = extractIntArray(env, ids);
    auto xArr = extractFloatArray(env, xs);
    auto yArr = extractFloatArray(env, ys);

    std::vector<AndroidTouchInput::Data> touches(idArr.size());

    // sometimes i wish we had c++23
    for (int i = 0; i < idArr.size(); i++) {
        touches.emplace_back(idArr[i], xArr[i], yArr[i]);
    }

    auto type = static_cast<AndroidTouchInput::Type>(eventType);

    if (AndroidRichInputEvent().send(timestamp, deviceId, eventSource, AndroidTouchInput(std::move(touches), type)) != ListenerResult::Propagate) {
        return;
    }

    double timeInSeconds = static_cast<double>(timestamp) / 1'000'000.0;
    auto glView = cocos2d::CCDirector::sharedDirector()->getOpenGLView();
    switch (type) {
        case AndroidTouchInput::Type::Began:
            // idArr.size() should == 1, but we're going to be passing this array anyways so it doesn't matter
            glView->handleTouchesBegin(idArr.size(), idArr.data(), xArr.data(), yArr.data(), timeInSeconds);
            break;
        case AndroidTouchInput::Type::Moved:
            glView->handleTouchesMove(idArr.size(), idArr.data(), xArr.data(), yArr.data(), timeInSeconds);
            break;
        case AndroidTouchInput::Type::Ended:
            glView->handleTouchesEnd(idArr.size(), idArr.data(), xArr.data(), yArr.data(), timeInSeconds);
            break;
        case AndroidTouchInput::Type::Cancelled:
            glView->handleTouchesCancel(idArr.size(), idArr.data(), xArr.data(), yArr.data(), timeInSeconds);
            break;
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_internalJoystickEvent(
    JNIEnv* env, jobject, jlong timestamp, jint deviceId, jint eventSource, jfloatArray leftX, jfloatArray leftY, jfloatArray rightX, jfloatArray rightY, jfloatArray hatX, jfloatArray hatY, jfloatArray leftTrigger, jfloatArray rightTrigger
) {
    auto leftXArr = extractFloatArray(env, leftX);
    auto leftYArr = extractFloatArray(env, leftY);

    auto rightXArr = extractFloatArray(env, rightX);
    auto rightYArr = extractFloatArray(env, rightY);

    auto hatXArr = extractFloatArray(env, hatX);
    auto hatYArr = extractFloatArray(env, hatY);

    auto leftTriggerArr = extractFloatArray(env, leftTrigger);
    auto rightTriggerArr = extractFloatArray(env, rightTrigger);

    std::vector<AndroidJoystickInput::Data> inputs(leftXArr.size());

    for (int i = 0; i < leftXArr.size(); i++) {
        inputs.emplace_back(
            leftXArr[i], leftYArr[i],
            rightXArr[i], rightYArr[i],
            hatXArr[i], hatYArr[i],
            leftTriggerArr[i], rightTriggerArr[i]
        );
    }

    AndroidRichInputEvent().send(timestamp, deviceId, eventSource, AndroidJoystickInput(std::move(inputs)));
}

AndroidScrollInput::AndroidScrollInput(float scrollX, float scrollY) : m_scrollX(scrollX), m_scrollY(scrollY) {}

float AndroidScrollInput::scrollX() const {
    return m_scrollX;
}

float AndroidScrollInput::scrollY() const {
    return m_scrollY;
}

AndroidKeyInput::AndroidKeyInput(int keycode, int modifiers, bool isDown, int repeatCount)
    : m_keycode(keycode), m_modifiers(modifiers), m_repeatCount(repeatCount), m_isDown(isDown) {}

int AndroidKeyInput::keycode() const {
    return m_keycode;
}

bool AndroidKeyInput::isDown() const {
    return m_isDown;
}

int AndroidKeyInput::repeatCount() const {
    return m_repeatCount;
}

int AndroidKeyInput::modifiers() const {
    return m_modifiers;
}

AndroidTouchInput::AndroidTouchInput(std::vector<Data> touches, Type type) : m_touches(std::move(touches)), m_type(type) {}

std::vector<AndroidTouchInput::Data> const& AndroidTouchInput::touches() const {
    return m_touches;
}

AndroidTouchInput::Type AndroidTouchInput::type() const {
    return m_type;
}

AndroidJoystickInput::AndroidJoystickInput(std::vector<Data> packets) : m_packets(std::move(packets)) {}

std::vector<AndroidJoystickInput::Data> const& AndroidJoystickInput::packets() const {
    return m_packets;
}

extern "C"
JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_inputDeviceAdded(JNIEnv*, jobject, jint deviceId, jint eventSource) {
    geode::AndroidInputDeviceEvent().send(deviceId, geode::AndroidInputDeviceStatus::Added);
}

extern "C"
JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_inputDeviceChanged(JNIEnv*, jobject, jint deviceId, jint eventSource) {
    geode::AndroidInputDeviceEvent().send(deviceId, geode::AndroidInputDeviceStatus::Changed);
}

extern "C"
JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_inputDeviceRemoved(JNIEnv*, jobject, jint deviceId, jint eventSource) {
    geode::AndroidInputDeviceEvent().send(deviceId, geode::AndroidInputDeviceStatus::Removed);
}
