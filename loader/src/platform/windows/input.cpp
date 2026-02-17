#include <Geode/DefaultInclude.hpp>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h>
#include <Geode/modify/CCApplication.hpp>
#include <Geode/modify/CCEGLView.hpp>
#include <Geode/utils/Keyboard.hpp>

#include <mutex>
#include <queue>

using namespace geode::prelude;

struct RawInputEvent {
    double timestamp = 0.0;
    KeyboardModifiers mods;

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
            uint16_t flags;
        } mouse;
    };

    enum class Type : uint8_t {
        KeyDown,
        KeyUp,
        MouseButton,
    } type;

    static RawInputEvent makeKeyboard(
        bool isDown, uint16_t vk, uint16_t scan, uint16_t flags, bool isRepeat, KeyboardModifiers mods
    ) {
        RawInputEvent evt;
        evt.type = isDown ? Type::KeyDown : Type::KeyUp;
        evt.timestamp = getInputTimestamp();
        evt.mods = mods;
        evt.keyboard.vkey = vk;
        evt.keyboard.scanCode = scan;
        evt.keyboard.flags = flags;
        evt.keyboard.isE0 = (flags & RI_KEY_E0) != 0;
        evt.keyboard.isE1 = (flags & RI_KEY_E1) != 0;
        evt.keyboard.isRepeat = isRepeat;
        return evt;
    }

    static RawInputEvent makeMouse(uint16_t btnFlags, KeyboardModifiers mods) {
        RawInputEvent evt;
        evt.type = Type::MouseButton;
        evt.timestamp = getInputTimestamp();
        evt.mods = mods;
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
    KeyboardModifiers m_currentMods = KeyboardModifiers::None;

    static uint32_t makeKey(uint16_t vkey, uint16_t scanCode, bool isE0) {
        return (static_cast<uint32_t>(vkey) << 16) | (static_cast<uint32_t>(scanCode) << 1) |
            (isE0 ? 1 : 0);
    }

public:
    static KeyStateTracker& get() {
        static KeyStateTracker instance;
        return instance;
    }

    KeyboardModifiers getMods() const {
        return m_currentMods;
    }

    bool updateState(uint16_t vkey, uint16_t scanCode, bool isE0, bool isDown) {
        uint32_t key = makeKey(vkey, scanCode, isE0);

        bool wasDown = false;
        auto it = m_keyStates.find(key);
        if (it != m_keyStates.end()) {
            wasDown = it->second;
        }

        auto applyMods = [&](KeyboardModifiers mod) {
            if (isDown) m_currentMods |= mod;
            else m_currentMods &= ~mod;
        };

        switch (vkey) {
            case VK_LSHIFT:
            case VK_RSHIFT:
            case VK_SHIFT:
                applyMods(KeyboardModifiers::Shift);
                break;
            case VK_LCONTROL:
            case VK_RCONTROL:
            case VK_CONTROL:
                applyMods(KeyboardModifiers::Control);
                break;
            case VK_LMENU:
            case VK_RMENU:
            case VK_MENU:
                applyMods(KeyboardModifiers::Alt);
                break;
            case VK_LWIN:
            case VK_RWIN:
                applyMods(KeyboardModifiers::Super);
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

// Some devices map F13-F24 to extended scan codes (e.g. Logitech mouse/keyboard macros)
static uint16_t scanCodeToExtendedFKey(uint16_t scanCode) {
    switch (scanCode) {
        case 0x64: case 0xB7: return VK_F13;
        case 0x65: case 0xB8: return VK_F14;
        case 0x66: case 0xB9: return VK_F15;
        case 0x67: case 0xBA: return VK_F16;
        case 0x68: case 0xBB: return VK_F17;
        case 0x69: case 0xBC: return VK_F18;
        case 0x6A: case 0xBD: return VK_F19;
        case 0x6B: case 0xBE: return VK_F20;
        case 0x6C: case 0xBF: return VK_F21;
        case 0x6D: case 0xC0: return VK_F22;
        case 0x6E: case 0xC1: return VK_F23;
        case 0x6F: case 0xC2: return VK_F24;
        default: return 0;
    }
}

static uint16_t getActualVKey(uint16_t vkey, uint16_t scanCode, uint16_t flags) {
    bool isE0 = (flags & RI_KEY_E0) != 0;

    // Remap vkey if it's 0 or 255 (unknown)
    if (vkey == 0 || vkey == 255) {
        UINT mappedScanCode = scanCode;
        if (isE0) {
            mappedScanCode |= 0xE000;
        }
        vkey = static_cast<uint16_t>(MapVirtualKeyEx(mappedScanCode, MAPVK_VSC_TO_VK_EX, GetKeyboardLayout(0)));
        if (vkey == 0) {
            vkey = scanCodeToExtendedFKey(scanCode);
        }
    }

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

        RawInputQueue::get().push(RawInputEvent::makeMouse(
            mouse.usButtonFlags,
            KeyStateTracker::get().getMods()
        ));
    }

    return 0;
}

class DummyEGLView : public CCEGLView {
public:
    void onGLFWMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        CCEGLView::onGLFWMouseScrollCallback(window, xoffset, yoffset);
    }

    void onGLFWCharCallback(GLFWwindow* window, unsigned int c) {
        CCEGLView::onGLFWCharCallback(window, c);
    }
};

static void GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (ScrollWheelEvent().send(xoffset, yoffset) == ListenerResult::Stop) {
        return;
    }
    static_cast<DummyEGLView*>(CCEGLView::get())->onGLFWMouseScrollCallback(window, xoffset, yoffset);
}

static void GLFWCharCallback(GLFWwindow* window, unsigned int c) {
    static_cast<DummyEGLView*>(CCEGLView::get())->onGLFWCharCallback(window, c);
}

struct GeodeRawInput : Modify<GeodeRawInput, CCEGLView> {
    void pumpRawInput() {
        bool isForeground = GetForegroundWindow() == g_mainWindowHWND;
        if (!isForeground) {
            RawInputQueue::get().clear();
        }

        // update mouse position
        POINT p;
        float newX = m_fMouseX;
        float newY = m_fMouseY;

        if (!m_bCursorLocked && GetCursorPos(&p) && ScreenToClient(g_mainWindowHWND, &p)) {
            newX = static_cast<float>(p.x);
            newY = static_cast<float>(p.y);

            if (m_fFrameZoomFactor != 0.0f) {
                newX /= m_fFrameZoomFactor;
                newY /= m_fFrameZoomFactor;
            }

            if (m_bInRetinaMonitor && m_nRetinaFactor == 1) {
                newX *= 2.f;
                newY *= 2.f;
            }
        }

        bool moved = false;
        if (m_fMouseX != newX || m_fMouseY != newY) {
            m_fMouseX = newX;
            m_fMouseY = newY;
            moved = true;
        }

        // process raw input events
        RawInputEvent evt;
        while (isForeground && RawInputQueue::get().pop(evt)) {
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
                        evt.mods
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
                            data.modifiers & KeyboardModifiers::Shift,
                            data.modifiers & KeyboardModifiers::Control,
                            data.modifiers & KeyboardModifiers::Alt,
                            data.modifiers & KeyboardModifiers::Super
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
                        if (data.modifiers & KeyboardModifiers::Control && keyCode == enumKeyCodes::KEY_V && isDown) {
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
                                evt.timestamp,
                                evt.mods
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
                getInputTimestamp()
            );
        }
    }

    static void attemptHookRawInput() {
        g_rawInputHWND = FindWindowW(L"GD_RawInput", nullptr);
        if (!g_rawInputHWND) {
            // in case it takes more than 1 frame, try getting the window again next frame
            queueInMainThread([]{ attemptHookRawInput(); });
            return;
        }

        g_originalRawInputProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(
            g_rawInputHWND, GWLP_WNDPROC,
            reinterpret_cast<LONG_PTR>(GeodeRawInputWndProc)
        ));
    }

    void setupWindow(CCRect rect) {
        CCEGLView::setupWindow(rect);

        g_mainWindowHWND = *reinterpret_cast<HWND*>(reinterpret_cast<uintptr_t>(m_pMainWindow) + 0x370);
        *reinterpret_cast<GLFWscrollfun*>(reinterpret_cast<uintptr_t>(m_pMainWindow) + 0x340) = &GLFWScrollCallback;
        *reinterpret_cast<GLFWcharfun*>(reinterpret_cast<uintptr_t>(m_pMainWindow) + 0x350) = &GLFWCharCallback;

        // window is created on a different thread, so it needs time to initialize
        queueInMainThread([]{ attemptHookRawInput(); });
    }
};

struct GeodeControllerInput : Modify<GeodeControllerInput, CCApplication> {
    void updateControllerKeys(CXBOXController* controller, int userIndex) {
        if (!controller) return;

        auto timestamp = getInputTimestamp();
        if (XInputGetState(controller->m_userIndex, &controller->m_xinputState) != ERROR_SUCCESS) {
            return;
        }

        XINPUT_STATE state = controller->m_xinputState;
        auto* dispatcher = CCKeyboardDispatcher::get();

        int player2Offset = userIndex > 1 ? 1 : 0;

        auto dispatchAction = [&](enumKeyCodes keyCode, bool isDown) {
            KeyboardInputData data(
                static_cast<enumKeyCodes>(static_cast<int>(keyCode) + player2Offset),
                isDown ? KeyboardInputData::Action::Press : KeyboardInputData::Action::Release,
                {},
                timestamp,
                KeyboardModifiers::None
            );

            if (KeyboardInputEvent(data.key).send(data) == ListenerResult::Stop) {
                return;
            }

            dispatcher->dispatchKeyboardMSG(
                data.key,
                data.action != KeyboardInputData::Action::Release,
                data.action == KeyboardInputData::Action::Repeat,
                data.timestamp
            );
        };

        auto handleButton = [&](uint16_t btnMask, enumKeyCodes keyCode, bool& stateVar) {
            auto isDown = (state.Gamepad.wButtons & btnMask) != 0;
            if (isDown != stateVar) {
                stateVar = isDown;
                dispatchAction(keyCode, isDown);
            }
        };

        auto handleTrigger = [&](uint8_t triggerValue, enumKeyCodes keyCode, bool& stateVar) {
            bool isDown = triggerValue > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
            if (isDown != stateVar) {
                stateVar = isDown;
                dispatchAction(keyCode, isDown);
            }
        };

        auto isADown = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
        if (isADown != controller->m_buttonA) {
            controller->m_buttonA = isADown;

            KeyboardInputData data(
                static_cast<enumKeyCodes>(static_cast<int>(CONTROLLER_A) + player2Offset),
                isADown ? KeyboardInputData::Action::Press : KeyboardInputData::Action::Release,
                {},
                timestamp,
                KeyboardModifiers::None
            );

            if (KeyboardInputEvent(data.key).send(data) == ListenerResult::Propagate) {
                // A button simulates a mouse click
                auto eglView = CCEGLView::get();
                if (m_bMouseControl && !eglView->m_bShouldHideCursor && !eglView->m_bCursorLocked) {
                    INPUT input{};
                    if (isADown) {
                        eglView->capture();
                        input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                    } else {
                        eglView->releaseCapture();
                        input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
                    }
                    SendInput(1, &input, sizeof(input));
                }

                dispatcher->dispatchKeyboardMSG(
                    data.key,
                    data.action != KeyboardInputData::Action::Release,
                    data.action == KeyboardInputData::Action::Repeat,
                    data.timestamp
                );
            }
        }

        handleButton(XINPUT_GAMEPAD_B, CONTROLLER_B, controller->m_buttonB);
        handleButton(XINPUT_GAMEPAD_X, CONTROLLER_X, controller->m_buttonX);
        handleButton(XINPUT_GAMEPAD_Y, CONTROLLER_Y, controller->m_buttonY);
        handleButton(XINPUT_GAMEPAD_LEFT_SHOULDER, CONTROLLER_LB, controller->m_lb);
        handleButton(XINPUT_GAMEPAD_RIGHT_SHOULDER, CONTROLLER_RB, controller->m_rb);

        handleTrigger(controller->m_xinputState.Gamepad.bLeftTrigger, CONTROLLER_LT, controller->m_lt);
        handleTrigger(controller->m_xinputState.Gamepad.bRightTrigger, CONTROLLER_RT, controller->m_rt);

        handleButton(XINPUT_GAMEPAD_START, CONTROLLER_Start, controller->m_start);

        // back button triggers CCKeypadDispatcher message
        bool isBackDown = (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
        if (isBackDown != controller->m_back) {
            controller->m_back = isBackDown;
            KeyboardInputData data(
                static_cast<enumKeyCodes>(static_cast<int>(CONTROLLER_Back) + player2Offset),
                isBackDown ? KeyboardInputData::Action::Press : KeyboardInputData::Action::Release,
                {},
                timestamp,
                KeyboardModifiers::None
            );

            if (KeyboardInputEvent(data.key).send(data) == ListenerResult::Propagate) {
                if (data.action != KeyboardInputData::Action::Release) {
                    CCDirector::get()->getKeypadDispatcher()->dispatchKeypadMSG(kTypeBackClicked);
                }
            }
        }

        handleButton(XINPUT_GAMEPAD_DPAD_UP, CONTROLLER_Up, controller->m_dpadUp);
        handleButton(XINPUT_GAMEPAD_DPAD_DOWN, CONTROLLER_Down, controller->m_dpadDown);
        handleButton(XINPUT_GAMEPAD_DPAD_LEFT, CONTROLLER_Left, controller->m_dpadLeft);
        handleButton(XINPUT_GAMEPAD_DPAD_RIGHT, CONTROLLER_Right, controller->m_dpadRight);

        auto processAxis = [](SHORT value) {
            constexpr float DEADZONE = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE; // robtop uses the same deadzone for both sticks
            float fValue = value;
            if (fabs(fValue) <= DEADZONE) return 0.f;
            float adjusted = fValue > 0 ? fValue - DEADZONE : fValue + DEADZONE;
            return adjusted / (std::numeric_limits<int16_t>::max() - DEADZONE);
        };

        float leftX = processAxis(state.Gamepad.sThumbLX);
        float leftY = -processAxis(state.Gamepad.sThumbLY);
        m_obLeftThumb = CCPoint{ leftX, leftY } * 10.f;

        auto handleStickMove = [&](bool condition, enumKeyCodes keyCode, bool& stateVar) {
            if (condition != stateVar) {
                stateVar = condition;
                dispatchAction(keyCode, condition);
            }
        };

        handleStickMove(leftX <= -0.2f, CONTROLLER_LTHUMBSTICK_LEFT, controller->m_leftThumbLeft);
        handleStickMove(leftX >= 0.2f, CONTROLLER_LTHUMBSTICK_RIGHT, controller->m_leftThumbRight);
        handleStickMove(leftY <= -0.2f, CONTROLLER_LTHUMBSTICK_UP, controller->m_leftThumbUp);
        handleStickMove(leftY >= 0.2f, CONTROLLER_LTHUMBSTICK_DOWN, controller->m_leftThumbDown);

        float rightX = processAxis(state.Gamepad.sThumbRX);
        float rightY = -processAxis(state.Gamepad.sThumbRY);
        m_obRightThumb = CCPoint{ rightX, rightY } * 10.f;

        handleStickMove(rightX <= -0.2f, CONTROLLER_RTHUMBSTICK_LEFT, controller->m_rightThumbLeft);
        handleStickMove(rightX >= 0.2f, CONTROLLER_RTHUMBSTICK_RIGHT, controller->m_rightThumbRight);
        handleStickMove(rightY <= -0.2f, CONTROLLER_RTHUMBSTICK_UP, controller->m_rightThumbUp);
        handleStickMove(rightY >= 0.2f, CONTROLLER_RTHUMBSTICK_DOWN, controller->m_rightThumbDown);
    }
};