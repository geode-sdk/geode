#include <Geode/DefaultInclude.hpp>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h>
#include <Geode/modify/CCEGLView.hpp>
#include <Geode/utils/Keyboard.hpp>

#include <mutex>
#include <queue>

using namespace geode::prelude;

struct RawInputEvent {
    std::chrono::steady_clock::time_point timestamp;

    union {
        struct {
            uint16_t vkey;
            uint16_t scanCode;
            uint16_t flags;
            bool isE0;
            bool isE1;
            bool isRepeat;
        } keyboard;

        struct {
            int32_t dx;
            int32_t dy;
            uint16_t buttonFlags;
            int16_t wheelDelta;
        } mouse;
    };

    enum class Type {
        KeyDown,
        KeyUp,
        MouseMove,
        MouseButtonDown,
        MouseButtonUp,
        MouseWheel
    } type;

    static RawInputEvent makeKeyboard(
        bool isDown, uint16_t vk, uint16_t scan, uint16_t flags, bool isRepeat
    ) {
        RawInputEvent evt;
        evt.type = isDown ? Type::KeyDown : Type::KeyUp;
        evt.timestamp = std::chrono::high_resolution_clock::now();
        evt.keyboard.vkey = vk;
        evt.keyboard.scanCode = scan;
        evt.keyboard.flags = flags;
        evt.keyboard.isE0 = (flags & RI_KEY_E0) != 0;
        evt.keyboard.isE1 = (flags & RI_KEY_E1) != 0;
        evt.keyboard.isRepeat = isRepeat;
        return evt;
    }

    static RawInputEvent makeMouse(int32_t dx, int32_t dy, uint16_t btnFlags, int16_t wheel) {
        RawInputEvent evt;
        if (dx != 0 || dy != 0) {
            evt.type = Type::MouseMove;
        } else if (wheel != 0) {
            evt.type = Type::MouseWheel;
        } else {
            constexpr uint16_t allBtnDownFlags =
                RI_MOUSE_BUTTON_1_DOWN | RI_MOUSE_BUTTON_2_DOWN |
                RI_MOUSE_BUTTON_3_DOWN | RI_MOUSE_BUTTON_4_DOWN |
                RI_MOUSE_BUTTON_5_DOWN;
            evt.type = (btnFlags & allBtnDownFlags)
                ? Type::MouseButtonDown
                : Type::MouseButtonUp;
        }
        evt.timestamp = std::chrono::steady_clock::now();
        evt.mouse.dx = dx;
        evt.mouse.dy = dy;
        evt.mouse.buttonFlags = btnFlags;
        evt.mouse.wheelDelta = wheel;
        return evt;
    }
};

class RawInputQueue {
private:
    std::queue<RawInputEvent> m_queue;
    mutable std::mutex m_mutex;

public:
    static RawInputQueue& get() noexcept {
        static RawInputQueue instance;
        return instance;
    }

    void push(RawInputEvent const& event) {
        std::lock_guard lock(m_mutex);
        m_queue.push(event);
    }

    bool pop(RawInputEvent& event) {
        std::lock_guard lock(m_mutex);
        if (m_queue.empty()) {
            return false;
        }
        event = m_queue.front();
        m_queue.pop();
        return true;
    }
};

class KeyStateTracker {
private:
    std::unordered_map<uint32_t, bool> m_keyStates;

    static uint32_t makeKey(uint16_t vkey, uint16_t scanCode, bool isE0) {
        return (static_cast<uint32_t>(vkey) << 16) | (static_cast<uint32_t>(scanCode) << 1) |
            (isE0 ? 1 : 0);
    }

public:
    static KeyStateTracker& get() {
        static KeyStateTracker instance;
        return instance;
    }

    bool updateState(uint16_t vkey, uint16_t scanCode, bool isE0, bool isDown) {
        uint32_t key = makeKey(vkey, scanCode, isE0);

        bool wasDown = false;
        auto it = m_keyStates.find(key);
        if (it != m_keyStates.end()) {
            wasDown = it->second;
        }

        m_keyStates[key] = isDown;
        return wasDown && isDown;
    }
};

static HWND g_rawInputHWND, g_mainWindowHWND;
static WNDPROC g_originalRawInputProc = nullptr;

static enumKeyCodes keyToKeyCode(uint16_t vkey, bool isE0) {
    switch (vkey) {
        case 'A': return enumKeyCodes::KEY_A;
        case 'B': return enumKeyCodes::KEY_B;
        case 'C': return enumKeyCodes::KEY_C;
        case 'D': return enumKeyCodes::KEY_D;
        case 'E': return enumKeyCodes::KEY_E;
        case 'F': return enumKeyCodes::KEY_F;
        case 'G': return enumKeyCodes::KEY_G;
        case 'H': return enumKeyCodes::KEY_H;
        case 'I': return enumKeyCodes::KEY_I;
        case 'J': return enumKeyCodes::KEY_J;
        case 'K': return enumKeyCodes::KEY_K;
        case 'L': return enumKeyCodes::KEY_L;
        case 'M': return enumKeyCodes::KEY_M;
        case 'N': return enumKeyCodes::KEY_N;
        case 'O': return enumKeyCodes::KEY_O;
        case 'P': return enumKeyCodes::KEY_P;
        case 'Q': return enumKeyCodes::KEY_Q;
        case 'R': return enumKeyCodes::KEY_R;
        case 'S': return enumKeyCodes::KEY_S;
        case 'T': return enumKeyCodes::KEY_T;
        case 'U': return enumKeyCodes::KEY_U;
        case 'V': return enumKeyCodes::KEY_V;
        case 'W': return enumKeyCodes::KEY_W;
        case 'X': return enumKeyCodes::KEY_X;
        case 'Y': return enumKeyCodes::KEY_Y;
        case 'Z': return enumKeyCodes::KEY_Z;

        case '0': return enumKeyCodes::KEY_Zero;
        case '1': return enumKeyCodes::KEY_One;
        case '2': return enumKeyCodes::KEY_Two;
        case '3': return enumKeyCodes::KEY_Three;
        case '4': return enumKeyCodes::KEY_Four;
        case '5': return enumKeyCodes::KEY_Five;
        case '6': return enumKeyCodes::KEY_Six;
        case '7': return enumKeyCodes::KEY_Seven;
        case '8': return enumKeyCodes::KEY_Eight;
        case '9': return enumKeyCodes::KEY_Nine;

        case VK_F1: return enumKeyCodes::KEY_F1;
        case VK_F2: return enumKeyCodes::KEY_F2;
        case VK_F3: return enumKeyCodes::KEY_F3;
        case VK_F4: return enumKeyCodes::KEY_F4;
        case VK_F5: return enumKeyCodes::KEY_F5;
        case VK_F6: return enumKeyCodes::KEY_F6;
        case VK_F7: return enumKeyCodes::KEY_F7;
        case VK_F8: return enumKeyCodes::KEY_F8;
        case VK_F9: return enumKeyCodes::KEY_F9;
        case VK_F10: return enumKeyCodes::KEY_F10;
        case VK_F11: return enumKeyCodes::KEY_F11;
        case VK_F12: return enumKeyCodes::KEY_F12;
        case VK_F13: return enumKeyCodes::KEY_F13;
        case VK_F14: return enumKeyCodes::KEY_F14;
        case VK_F15: return enumKeyCodes::KEY_F15;
        case VK_F16: return enumKeyCodes::KEY_F16;
        case VK_F17: return enumKeyCodes::KEY_F17;
        case VK_F18: return enumKeyCodes::KEY_F18;
        case VK_F19: return enumKeyCodes::KEY_F19;
        case VK_F20: return enumKeyCodes::KEY_F20;
        case VK_F21: return enumKeyCodes::KEY_F21;
        case VK_F22: return enumKeyCodes::KEY_F22;
        case VK_F23: return enumKeyCodes::KEY_F23;
        case VK_F24: return enumKeyCodes::KEY_F24;

        case VK_SPACE: return enumKeyCodes::KEY_Space;
        case VK_ESCAPE: return enumKeyCodes::KEY_Escape;
        case VK_TAB: return enumKeyCodes::KEY_Tab;
        case VK_BACK: return enumKeyCodes::KEY_Backspace;
        case VK_INSERT: return enumKeyCodes::KEY_Insert;
        case VK_DELETE: return enumKeyCodes::KEY_Delete;
        case VK_HOME: return enumKeyCodes::KEY_Home;
        case VK_END: return enumKeyCodes::KEY_End;
        case VK_PRIOR: return enumKeyCodes::KEY_PageUp;
        case VK_NEXT: return enumKeyCodes::KEY_PageDown;

        case VK_UP: return enumKeyCodes::KEY_Up;
        case VK_DOWN: return enumKeyCodes::KEY_Down;
        case VK_LEFT: return enumKeyCodes::KEY_Left;
        case VK_RIGHT: return enumKeyCodes::KEY_Right;

        case VK_CAPITAL: return enumKeyCodes::KEY_CapsLock;
        case VK_SCROLL: return enumKeyCodes::KEY_ScrollLock;
        case VK_NUMLOCK: return enumKeyCodes::KEY_Numlock;
        case VK_SNAPSHOT: return enumKeyCodes::KEY_PrintScreen;
        case VK_PAUSE: return enumKeyCodes::KEY_Pause;

        case VK_LSHIFT: return enumKeyCodes::KEY_LeftShift;
        case VK_RSHIFT: return enumKeyCodes::KEY_RightShift;
        case VK_LCONTROL: return enumKeyCodes::KEY_LeftControl;
        case VK_RCONTROL: return enumKeyCodes::KEY_RightContol;
        case VK_LMENU: return enumKeyCodes::KEY_LeftMenu;
        case VK_RMENU: return enumKeyCodes::KEY_RightMenu;

        case VK_RETURN: return isE0
            ? enumKeyCodes::KEY_NumEnter
            : enumKeyCodes::KEY_Enter;

        case VK_LWIN: return enumKeyCodes::KEY_LeftWindowsKey;
        case VK_RWIN: return enumKeyCodes::KEY_RightWindowsKey;
        case VK_APPS: return enumKeyCodes::KEY_ApplicationsKey;

        case VK_OEM_1: return enumKeyCodes::KEY_Semicolon;
        case VK_OEM_7: return enumKeyCodes::KEY_Apostrophe;
        case VK_OEM_2: return enumKeyCodes::KEY_Slash;
        case VK_OEM_PLUS: return enumKeyCodes::KEY_OEMEqual;
        case VK_OEM_4: return enumKeyCodes::KEY_LeftBracket;
        case VK_OEM_5: return enumKeyCodes::KEY_Backslash;
        case VK_OEM_6: return enumKeyCodes::KEY_RightBracket;
        case VK_OEM_3: return enumKeyCodes::KEY_GraveAccent;
        case VK_OEM_102: return enumKeyCodes::KEY_World1;

        case VK_NUMPAD0: return enumKeyCodes::KEY_NumPad0;
        case VK_NUMPAD1: return enumKeyCodes::KEY_NumPad1;
        case VK_NUMPAD2: return enumKeyCodes::KEY_NumPad2;
        case VK_NUMPAD3: return enumKeyCodes::KEY_NumPad3;
        case VK_NUMPAD4: return enumKeyCodes::KEY_NumPad4;
        case VK_NUMPAD5: return enumKeyCodes::KEY_NumPad5;
        case VK_NUMPAD6: return enumKeyCodes::KEY_NumPad6;
        case VK_NUMPAD7: return enumKeyCodes::KEY_NumPad7;
        case VK_NUMPAD8: return enumKeyCodes::KEY_NumPad8;
        case VK_NUMPAD9: return enumKeyCodes::KEY_NumPad9;
        case VK_DECIMAL: return enumKeyCodes::KEY_Decimal;
        case VK_DIVIDE: return enumKeyCodes::KEY_Divide;
        case VK_MULTIPLY: return enumKeyCodes::KEY_Multiply;
        case VK_SUBTRACT: return enumKeyCodes::KEY_Subtract;
        case VK_ADD: return enumKeyCodes::KEY_Add;

        default: return enumKeyCodes::KEY_Unknown;
    }
}

static uint16_t getActualVKey(uint16_t vkey, uint16_t scanCode, uint16_t flags) {
    bool isE0 = (flags & RI_KEY_E0) != 0;

    switch (vkey) {
        default: return vkey;
        case VK_CONTROL: return isE0 ? VK_RCONTROL : VK_LCONTROL;
        case VK_MENU: return isE0 ? VK_RMENU : VK_LMENU;
        case VK_SHIFT:
            return MapVirtualKeyEx(scanCode, MAPVK_VSC_TO_VK_EX, GetKeyboardLayout(0));
    }
}

LRESULT CALLBACK GeodeRawInputWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg != WM_INPUT) {
        return CallWindowProcW(g_originalRawInputProc, hwnd, msg, wParam, lParam);
    }

    if (GetForegroundWindow() != g_mainWindowHWND) {
        return 0;
    }

    UINT rawInputSize = 0;
    GetRawInputData(
        reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &rawInputSize, sizeof(RAWINPUTHEADER)
    );

    std::vector<BYTE> rawInputData(rawInputSize);
    GetRawInputData(
        reinterpret_cast<HRAWINPUT>(lParam),
        RID_INPUT,
        rawInputData.data(),
        &rawInputSize,
        sizeof(RAWINPUTHEADER)
    );

    RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawInputData.data());
    if (raw->header.dwType == RIM_TYPEKEYBOARD) {
        auto const& kb = raw->data.keyboard;
        bool isDown = !(kb.Flags & RI_KEY_BREAK);
        bool isE0 = (kb.Flags & RI_KEY_E0) != 0;

        uint16_t actualVKey = getActualVKey(kb.VKey, kb.MakeCode, kb.Flags);
        bool isRepeat = KeyStateTracker::get().updateState(
            actualVKey,
            kb.MakeCode,
            isE0,
            isDown
        );

        RawInputQueue::get().push(RawInputEvent::makeKeyboard(
            isDown,
            actualVKey,
            kb.MakeCode,
            kb.Flags,
            isRepeat
        ));
    } else if (raw->header.dwType == RIM_TYPEMOUSE) {
        auto const& mouse = raw->data.mouse;
        RawInputQueue::get().push(RawInputEvent::makeMouse(
            mouse.lLastX,
            mouse.lLastY,
            mouse.usButtonFlags,
            static_cast<int16_t>(mouse.usButtonData)
        ));
    }

    return 0;
}

class $modify(cocos2d::CCEGLView) {
    void pumpRawInput() {
        RawInputEvent evt;
        while (RawInputQueue::get().pop(evt)) {
            switch (evt.type) {
                case RawInputEvent::Type::KeyDown:
                case RawInputEvent::Type::KeyUp: {
                    using enum KeyboardInputEvent::Action;
                    bool isDown = evt.type == RawInputEvent::Type::KeyDown;

                    enumKeyCodes keyCode = keyToKeyCode(
                        evt.keyboard.vkey,
                        evt.keyboard.isE0
                    );

                    auto result = KeyboardInputEvent(
                        keyCode,
                        isDown ? (evt.keyboard.isRepeat ? Repeat : Press) : Release,
                        {0, evt.keyboard.vkey},
                        evt.timestamp
                    ).post();

                    if (result == ListenerResult::Propagate) {
                        auto* ime = CCIMEDispatcher::sharedDispatcher();
                        if (keyCode == enumKeyCodes::KEY_Backspace && isDown) {
                            ime->dispatchDeleteBackward();
                        } else if (keyCode == enumKeyCodes::KEY_Delete && isDown) {
                            ime->dispatchDeleteForward();
                        }

                        CCKeyboardDispatcher::get()->dispatchKeyboardMSG(
                            keyCode,
                            isDown,
                            evt.keyboard.isRepeat,
                            std::chrono::duration_cast<std::chrono::microseconds>(
                                evt.timestamp.time_since_epoch()
                            ).count() / 1'000'000.0
                        );

                        // text pasting
                        if (ime->hasDelegate() && keyCode == enumKeyCodes::KEY_V && isDown) {
                            if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
                                this->performSafeClipboardPaste();
                            }
                        }
                    }
                    break;
                }
                case RawInputEvent::Type::MouseButtonDown:
                case RawInputEvent::Type::MouseButtonUp: {
                    using enum MouseInputEvent::Action;
                    using enum MouseInputEvent::Button;

                    constexpr uint16_t downFlags =
                        RI_MOUSE_BUTTON_1_DOWN | RI_MOUSE_BUTTON_2_DOWN |
                        RI_MOUSE_BUTTON_3_DOWN | RI_MOUSE_BUTTON_4_DOWN |
                        RI_MOUSE_BUTTON_5_DOWN;

                    auto btn = evt.mouse.buttonFlags & (RI_MOUSE_BUTTON_1_DOWN | RI_MOUSE_BUTTON_1_UP) ? Left
                        : evt.mouse.buttonFlags & (RI_MOUSE_BUTTON_2_DOWN | RI_MOUSE_BUTTON_2_UP) ? Right
                        : evt.mouse.buttonFlags & (RI_MOUSE_BUTTON_3_DOWN | RI_MOUSE_BUTTON_3_UP) ? Middle
                        : evt.mouse.buttonFlags & (RI_MOUSE_BUTTON_4_DOWN | RI_MOUSE_BUTTON_4_UP) ? Button4
                        : Button5;

                    bool isDown = (evt.mouse.buttonFlags & downFlags) != 0;

                    auto result = MouseInputEvent(
                        btn,
                        isDown ? Press : Release,
                        evt.timestamp
                    ).post();

                    if (result == ListenerResult::Propagate && btn == Left) {
                        if (isDown) {
                            int id = 0;
                            m_bCaptured = true;
                            this->handleTouchesBegin(
                                1, &id,
                                &m_fMouseX,
                                &m_fMouseY,
                                std::chrono::duration_cast<std::chrono::microseconds>(
                                    evt.timestamp.time_since_epoch()
                                ).count() / 1'000'000.0
                            );
                        } else {
                            int id = 0;
                            m_bCaptured = false;
                            this->handleTouchesEnd(
                                1, &id,
                                &m_fMouseX,
                                &m_fMouseY,
                                std::chrono::duration_cast<std::chrono::microseconds>(
                                    evt.timestamp.time_since_epoch()
                                ).count() / 1'000'000.0
                            );
                        }
                    }
                    break;
                }
                case RawInputEvent::Type::MouseMove: {
                    // update mouse position
                    POINT p;
                    GetCursorPos(&p);
                    ScreenToClient(g_mainWindowHWND, &p);
                    m_fMouseX = static_cast<float>(p.x);
                    m_fMouseY = static_cast<float>(p.y);
                    int id = 0;
                    this->handleTouchesMove(
                        1, &id,
                        &m_fMouseX,
                        &m_fMouseY,
                        std::chrono::duration_cast<std::chrono::microseconds>(
                            evt.timestamp.time_since_epoch()
                        ).count() / 1'000'000.0
                    );
                    break;
                }
                case RawInputEvent::Type::MouseWheel:
                default:
                    break;
            }
        }
    }
};

$execute {
    queueInMainThread([] {
        g_rawInputHWND = FindWindowW(L"GD_RawInput", nullptr);
        g_mainWindowHWND = WindowFromDC(wglGetCurrentDC());
        g_originalRawInputProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(
            g_rawInputHWND, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(GeodeRawInputWndProc)
        ));
    });
}