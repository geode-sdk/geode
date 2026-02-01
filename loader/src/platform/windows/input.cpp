#include <Geode/DefaultInclude.hpp>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h>
#include <Geode/modify/CCEGLView.hpp>
#include <Geode/utils/Keyboard.hpp>

#include <mutex>
#include <queue>

using namespace geode::prelude;

struct RawInputEvent {
    double timestamp = 0.0;

    union {
        struct {
            uint16_t vkey;
            uint16_t scanCode;
            uint16_t flags;
            KeyboardInputData::Modifiers mods;
            bool isE0;
            bool isE1;
            bool isRepeat;
        } keyboard;

        struct {
            uint16_t flags;
        } mouse;
    };

    enum class Type : uint8_t {
        KeyDown,
        KeyUp,
        MouseButton,
    } type;

    // this uses the same method done by RobTop
    static double getTimestamp() {
        static LARGE_INTEGER freq = []{
            LARGE_INTEGER f;
            QueryPerformanceFrequency(&f);
            return f;
        }();

        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);

        // he uses LowPart for some reason?
        return static_cast<double>(counter.LowPart) / static_cast<double>(freq.LowPart);
    }

    static RawInputEvent makeKeyboard(
        bool isDown, uint16_t vk, uint16_t scan, uint16_t flags, bool isRepeat, KeyboardInputData::Modifiers mods
    ) {
        RawInputEvent evt;
        evt.type = isDown ? Type::KeyDown : Type::KeyUp;
        evt.timestamp = getTimestamp();
        evt.keyboard.vkey = vk;
        evt.keyboard.scanCode = scan;
        evt.keyboard.flags = flags;
        evt.keyboard.mods = mods;
        evt.keyboard.isE0 = (flags & RI_KEY_E0) != 0;
        evt.keyboard.isE1 = (flags & RI_KEY_E1) != 0;
        evt.keyboard.isRepeat = isRepeat;
        return evt;
    }

    static RawInputEvent makeMouse(uint16_t btnFlags) {
        RawInputEvent evt;
        evt.type = Type::MouseButton;
        evt.timestamp = getTimestamp();
        evt.mouse.flags = btnFlags;
        return evt;
    }
};

class RawInputQueue {
private:
    std::deque<RawInputEvent> m_queue;
    mutable std::mutex m_mutex;

public:
    static RawInputQueue& get() noexcept {
        static RawInputQueue instance;
        return instance;
    }

    void push(RawInputEvent const& event) {
        std::lock_guard lock(m_mutex);
        m_queue.push_back(event);
    }

    bool pop(RawInputEvent& event) {
        std::lock_guard lock(m_mutex);
        if (m_queue.empty()) {
            return false;
        }
        event = m_queue.front();
        m_queue.pop_front();
        return true;
    }

    void clear() {
        std::lock_guard lock(m_mutex);
        m_queue.clear();
    }
};

class KeyStateTracker {
private:
    std::unordered_map<uint32_t, bool> m_keyStates;
    KeyboardInputData::Modifiers m_currentMods = KeyboardInputData::Mods_None;

    static uint32_t makeKey(uint16_t vkey, uint16_t scanCode, bool isE0) {
        return (static_cast<uint32_t>(vkey) << 16) | (static_cast<uint32_t>(scanCode) << 1) |
            (isE0 ? 1 : 0);
    }

public:
    static KeyStateTracker& get() {
        static KeyStateTracker instance;
        return instance;
    }

    KeyboardInputData::Modifiers getMods() const {
        return m_currentMods;
    }

    bool updateState(uint16_t vkey, uint16_t scanCode, bool isE0, bool isDown) {
        uint32_t key = makeKey(vkey, scanCode, isE0);

        bool wasDown = false;
        auto it = m_keyStates.find(key);
        if (it != m_keyStates.end()) {
            wasDown = it->second;
        }

        auto applyMods = [&](KeyboardInputData::Modifiers mod) {
            if (isDown) m_currentMods |= mod;
            else m_currentMods &= ~mod;
        };

        switch (vkey) {
            case VK_LSHIFT:
            case VK_RSHIFT:
            case VK_SHIFT:
                applyMods(KeyboardInputData::Mods_Shift);
                break;
            case VK_LCONTROL:
            case VK_RCONTROL:
            case VK_CONTROL:
                applyMods(KeyboardInputData::Mods_Control);
                break;
            case VK_LMENU:
            case VK_RMENU:
            case VK_MENU:
                applyMods(KeyboardInputData::Mods_Alt);
                break;
            case VK_LWIN:
            case VK_RWIN:
                applyMods(KeyboardInputData::Mods_Super);
                break;
            default: break;
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
        case VK_OEM_MINUS: return enumKeyCodes::KEY_OEMMinus;
        case VK_OEM_4: return enumKeyCodes::KEY_LeftBracket;
        case VK_OEM_5: return enumKeyCodes::KEY_Backslash;
        case VK_OEM_6: return enumKeyCodes::KEY_RightBracket;
        case VK_OEM_3: return enumKeyCodes::KEY_GraveAccent;
        case VK_OEM_102: return enumKeyCodes::KEY_World1;
        case VK_OEM_COMMA: return enumKeyCodes::KEY_OEMComma;
        case VK_OEM_PERIOD: return enumKeyCodes::KEY_OEMPeriod;

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

    alignas(RAWINPUT) std::array<BYTE, sizeof(RAWINPUT)> buffer;
    UINT rawInputSize = buffer.size();

    auto result = GetRawInputData(
        reinterpret_cast<HRAWINPUT>(lParam),
        RID_INPUT,
        buffer.data(),
        &rawInputSize,
        sizeof(RAWINPUTHEADER)
    );

    if (result == static_cast<UINT>(-1) || rawInputSize == 0) {
        return 0;
    }

    RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer.data());
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
            isRepeat,
            KeyStateTracker::get().getMods()
        ));
    } else if (raw->header.dwType == RIM_TYPEMOUSE) {
        auto const& mouse = raw->data.mouse;
        if (mouse.usButtonFlags == 0)  {
            return 0;
        }

        RawInputQueue::get().push(RawInputEvent::makeMouse(mouse.usButtonFlags));
    }

    return 0;
}

class $modify(cocos2d::CCEGLView) {
    void pumpRawInput() {
        if (GetForegroundWindow() != g_mainWindowHWND) {
            RawInputQueue::get().clear();
        }

        // update mouse position
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(g_mainWindowHWND, &p);
        float newX = static_cast<float>(p.x);
        float newY = static_cast<float>(p.y);
        bool moved = false;
        if (m_fMouseX != newX || m_fMouseY != newY) {
            m_fMouseX = newX;
            m_fMouseY = newY;
            moved = true;
        }

        // process raw input events
        RawInputEvent evt;
        while (RawInputQueue::get().pop(evt)) {
            switch (evt.type) {
                case RawInputEvent::Type::KeyDown:
                case RawInputEvent::Type::KeyUp: {
                    using enum KeyboardInputData::Action;
                    bool isDown = evt.type == RawInputEvent::Type::KeyDown;

                    enumKeyCodes keyCode = keyToKeyCode(
                        evt.keyboard.vkey,
                        evt.keyboard.isE0
                    );

                    KeyboardInputData data(
                        keyCode,
                        isDown ? (evt.keyboard.isRepeat ? Repeat : Press) : Release,
                        {evt.keyboard.vkey, evt.keyboard.scanCode},
                        evt.timestamp,
                        evt.keyboard.mods
                    );

                    auto result = KeyboardInputEvent(keyCode).send(data);

                    // copy values from event, if someone modifies it
                    isDown = data.action != Release;
                    keyCode = data.key;

                    if (result == ListenerResult::Propagate && keyCode != KEY_Unknown) {
                        auto* ime = CCIMEDispatcher::sharedDispatcher();
                        if (keyCode == enumKeyCodes::KEY_Backspace && isDown) {
                            ime->dispatchDeleteBackward();
                        } else if (keyCode == enumKeyCodes::KEY_Delete && isDown) {
                            ime->dispatchDeleteForward();
                        }

                        auto* keyboardDispatcher = CCKeyboardDispatcher::get();

                        keyboardDispatcher->updateModifierKeys(
                            data.modifiers & KeyboardInputData::Mods_Shift,
                            data.modifiers & KeyboardInputData::Mods_Control,
                            data.modifiers & KeyboardInputData::Mods_Alt,
                            data.modifiers & KeyboardInputData::Mods_Super
                        );

                        if (!ime->hasDelegate() || keyCode == KEY_Escape || keyCode == KEY_Enter) {
                            keyboardDispatcher->dispatchKeyboardMSG(
                                keyCode,
                                isDown,
                                data.action == Repeat,
                                data.timestamp
                            );
                        }

                        // text pasting
                        if (data.modifiers & KeyboardInputData::Mods_Control && keyCode == enumKeyCodes::KEY_V && isDown) {
                            if (ime->hasDelegate()) {
                                this->performSafeClipboardPaste();
                            }
                        }
                    }
                    break;
                }
                case RawInputEvent::Type::MouseButton: {
                    using enum MouseInputData::Action;
                    using enum MouseInputData::Button;

                    struct Btn {
                        USHORT down, up;
                        MouseInputData::Button btn;
                    };

                    constexpr Btn btns[] = {
                        {RI_MOUSE_BUTTON_1_DOWN, RI_MOUSE_BUTTON_1_UP, Left},
                        {RI_MOUSE_BUTTON_2_DOWN, RI_MOUSE_BUTTON_2_UP, Right},
                        {RI_MOUSE_BUTTON_3_DOWN, RI_MOUSE_BUTTON_3_UP, Middle},
                        {RI_MOUSE_BUTTON_4_DOWN, RI_MOUSE_BUTTON_4_UP, Button4},
                        {RI_MOUSE_BUTTON_5_DOWN, RI_MOUSE_BUTTON_5_UP, Button5},
                    };

                    // WinAPI can combine multiple button events into one
                    for (auto const& b : btns) {
                        bool isDown = (evt.mouse.flags & b.down) != 0;
                        bool isUp = (evt.mouse.flags & b.up) != 0;
                        if (isDown || isUp) {
                            MouseInputData data(
                                b.btn,
                                isDown ? Press : Release,
                                evt.timestamp
                            );

                            auto result = MouseInputEvent().send(data);
                            isDown = data.action == Press;

                            // handle cocos touches
                            if (data.button == Left && result == ListenerResult::Propagate) {
                                int id = 0;
                                if (isDown) {
                                    m_bCaptured = true;
                                    this->handleTouchesBegin(
                                        1, &id,
                                        &m_fMouseX,
                                        &m_fMouseY,
                                        data.timestamp
                                    );
                                } else {
                                    m_bCaptured = false;
                                    this->handleTouchesEnd(
                                        1, &id,
                                        &m_fMouseX,
                                        &m_fMouseY,
                                        data.timestamp
                                    );
                                }
                            }
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }

        if (moved) {
            if (MouseMoveEvent().send(p.x, p.y) == ListenerResult::Stop || !m_bCaptured) {
                return;
            }

            int id = 0;
            this->handleTouchesMove(
                1, &id,
                &m_fMouseX,
                &m_fMouseY,
                RawInputEvent::getTimestamp()
            );
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