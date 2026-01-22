#include <Geode/DefaultInclude.hpp>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h>
#include <Geode/utils/Keyboard.hpp>

using namespace geode::prelude;

static WNDPROC g_originalWndProc = nullptr;

static enumKeyCodes keyToKeyCode(WPARAM code, LPARAM lParam) {
    switch (code) {
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

        case VK_SHIFT: return MapVirtualKey((lParam >> 16) & 0xFF, MAPVK_VSC_TO_VK_EX) == VK_RSHIFT
            ? enumKeyCodes::KEY_RightShift
            : enumKeyCodes::KEY_LeftShift;
        case VK_CONTROL: return (lParam & 0x01000000)
            ? enumKeyCodes::KEY_RightContol
            : enumKeyCodes::KEY_LeftControl;
        case VK_MENU: return (lParam & 0x01000000)
            ? enumKeyCodes::KEY_RightMenu
            : enumKeyCodes::KEY_LeftMenu;
        case VK_RETURN: return lParam & 0x01000000
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

LRESULT CALLBACK GeodeWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    auto result = ListenerResult::Propagate;

    switch (msg) {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            using enum KeyboardInputEvent::Action;
            bool down = msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN;
            bool repeat = (lParam & 0x40000000) != 0;
            enumKeyCodes keyCode = keyToKeyCode(wParam, lParam);

            result = KeyboardInputEvent(
                keyCode,
                down ? (repeat ? Repeat : Press) : Release,
                { static_cast<uint64_t>(lParam), wParam },
                std::chrono::steady_clock::now()
            ).post();
            break;
        }
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP: {
            using enum MouseInputEvent::Action;
            using enum MouseInputEvent::Button;
            result = MouseInputEvent(
                msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP ? Left : Right,
                msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN ? Press : Release,
                std::chrono::steady_clock::now()
            ).post();
            break;
        }
        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP: {
            using enum MouseInputEvent::Action;
            using enum MouseInputEvent::Button;
            result = MouseInputEvent(
                GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? Button4 : Button5,
                msg == WM_XBUTTONDOWN ? Press : Release,
                std::chrono::steady_clock::now()
            ).post();
            break;
        }
        default: break;
    }

    if (result == ListenerResult::Stop) {
        return 0;
    }

    return CallWindowProcW(g_originalWndProc, hwnd, msg, wParam, lParam);
}

$execute {
    queueInMainThread([] {
        HWND hwnd = WindowFromDC(wglGetCurrentDC());
        g_originalWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(
            hwnd, GWLP_WNDPROC,
            reinterpret_cast<LONG_PTR>(GeodeWndProc)
        ));
    });
}