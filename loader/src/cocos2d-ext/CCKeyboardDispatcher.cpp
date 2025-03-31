#include <cocos2d.h>

using namespace cocos2d;

#ifdef GEODE_IS_IOS

CCKeyboardDispatcher::CCKeyboardDispatcher()
    : m_bUnknown38(false),
    m_bUnknown39(false),
    m_bUnknown3a(false),
    m_bShiftPressed(false),
    m_bControlPressed(false),
    m_bAltPressed(false),
    m_bCommandPressed(false),
    m_bBlockRepeat(false),
    m_pDelegates(CCArray::create()),
    m_pUnknown3c(ccCArrayNew(8)),
    m_pUnknown40(ccCArrayNew(8))
{
    m_pDelegates->retain();
}

CCKeyboardDispatcher::~CCKeyboardDispatcher()
{
    CC_SAFE_RELEASE(m_pDelegates);
    if (m_pUnknown3c)
    {
        ccCArrayFree(m_pUnknown3c);
    }
    if (m_pUnknown40)
    {
        ccCArrayFree(m_pUnknown40);
    }
}

void CCKeyboardDispatcher::addDelegate(CCKeyboardDelegate* pDelegate)
{
    if (!pDelegate) return;

    if (m_bUnknown38)
    {
        ccCArrayAppendValue(m_pUnknown3c, pDelegate);
        m_bUnknown39 = true;
    }
    else
    {
        forceAddDelegate(pDelegate);
    }
}

void CCKeyboardDispatcher::removeDelegate(CCKeyboardDelegate* pDelegate)
{
    if (!pDelegate) return;

    if (m_bUnknown38)
    {
        ccCArrayAppendValue(m_pUnknown40, pDelegate);
        m_bUnknown3a = true;
    }
    else
    {
        forceRemoveDelegate(pDelegate);
    }
}

void CCKeyboardDispatcher::forceAddDelegate(CCKeyboardDelegate* pDelegate)
{
    if (auto handler = CCKeyboardHandler::handlerWithDelegate(pDelegate))
    {
        m_pDelegates->addObject(handler);
    }
}

void CCKeyboardDispatcher::forceRemoveDelegate(CCKeyboardDelegate* pDelegate)
{
    CCObject* handler = nullptr;
    CCARRAY_FOREACH(m_pDelegates, handler)
    {
        if (pDelegate && pDelegate == static_cast<CCKeyboardHandler*>(handler)->getDelegate())
        {
            m_pDelegates->removeObject(handler, true);
        }
    }
}

enumKeyCodes CCKeyboardDispatcher::convertKeyCode(enumKeyCodes key)
{
    switch (key)
    {
        case enumKeyCodes::KEY_ArrowUp:
            return enumKeyCodes::KEY_Up;
        case enumKeyCodes::KEY_ArrowDown:
            return enumKeyCodes::KEY_Down;
        case enumKeyCodes::KEY_ArrowLeft:
            return enumKeyCodes::KEY_Left;
        case enumKeyCodes::KEY_ArrowRight:
            return enumKeyCodes::KEY_Right;
        default:
            return key;
    }
}

bool CCKeyboardDispatcher::dispatchKeyboardMSG(enumKeyCodes key, bool isKeyDown, bool isKeyRepeat)
{
    if (isKeyRepeat && m_bBlockRepeat)
    {
        return false;
    }

    enumKeyCodes convertedKey = convertKeyCode(key);

    switch (key)
    {
        case KEY_Shift:
        case KEY_Control:
        case KEY_Alt:
        case CONTROLLER_Back:
            return false;

        default:
            break;
    }

    m_bUnknown38 = true;
    CCObject* handler;
    CCARRAY_FOREACH(m_pDelegates, handler)
    {
        auto keyboardHandler = static_cast<CCKeyboardHandler*>(handler);
        if (!handler)
        {
            break;
        }

        auto delegate = keyboardHandler->getDelegate();
        if (isKeyDown)
        {
            delegate->keyDown(key);
        }
        else
        {
            delegate->keyUp(key);
        }
    }
    m_bUnknown38 = false;
    
    if (m_bUnknown3a)
    {
        m_bUnknown3a = false;

        void* delegate;
        CCARRAYDATA_FOREACH(m_pUnknown40, delegate)
        {
            forceRemoveDelegate(static_cast<CCKeyboardDelegate*>(delegate));
        }
        ccCArrayRemoveAllValues(m_pUnknown40);
    }

    if (!m_bUnknown39)
        return true;

    m_bUnknown39 = false;

    void* delegate;
    CCARRAYDATA_FOREACH(m_pUnknown3c, delegate)
    {
        forceAddDelegate(static_cast<CCKeyboardDelegate*>(delegate));
    }
    ccCArrayRemoveAllValues(m_pUnknown3c);

    return true;
}

const char* CCKeyboardDispatcher::keyToString(enumKeyCodes key)
{
    switch (key)
    {
        case KEY_Backspace: return "Backspace";
        case KEY_Tab: return "Tab";
        case KEY_Clear: return "Clear";
        case KEY_Enter: return "Enter";
        case KEY_Shift: return "Shift";
        case KEY_Control: return "Control";
        case KEY_Alt: return "Alt";
        case KEY_Pause: return "Pause";
        case KEY_CapsLock: return "CapsLock";
        case KEY_Escape: return "Escape";
        case KEY_Space: return "Space";
        case KEY_PageUp: return "PageUp";
        case KEY_PageDown: return "PageDown";
        case KEY_End: return "End";
        case KEY_Home: return "Home";
        case KEY_Left: return "Left";
        case KEY_Up: return "Up";
        case KEY_Right: return "Right";
        case KEY_Down: return "Down";
        case KEY_Select: return "Select";
        case KEY_Print: return "Print";
        case KEY_Execute: return "Execute";
        case KEY_PrintScreen: return "PrintScreen";
        case KEY_Insert: return "Insert";
        case KEY_Delete: return "Delete";
        case KEY_Help: return "Help";
        case KEY_Zero: return "Zero";
        case KEY_One: return "One";
        case KEY_Two: return "Two";
        case KEY_Three: return "Three";
        case KEY_Four: return "Four";
        case KEY_Five: return "Five";
        case KEY_Six: return "Six";
        case KEY_Seven: return "Seven";
        case KEY_Eight: return "Eight";
        case KEY_Nine: return "Nine";
        case KEY_A: return "A";
        case KEY_B: return "B";
        case KEY_C: return "C";
        case KEY_D: return "D";
        case KEY_E: return "E";
        case KEY_F: return "F";
        case KEY_G: return "G";
        case KEY_H: return "H";
        case KEY_I: return "I";
        case KEY_J: return "J";
        case KEY_K: return "K";
        case KEY_L: return "L";
        case KEY_M: return "M";
        case KEY_N: return "N";
        case KEY_O: return "O";
        case KEY_P: return "P";
        case KEY_Q: return "Q";
        case KEY_R: return "R";
        case KEY_S: return "S";
        case KEY_T: return "T";
        case KEY_U: return "U";
        case KEY_V: return "V";
        case KEY_W: return "W";
        case KEY_X: return "X";
        case KEY_Y: return "Y";
        case KEY_Z: return "Z";
        case KEY_LeftWindowsKey: return "LeftWindowsKey";
        case KEY_RightWindowsKey: return "RightWindowsKey";
        case KEY_ApplicationsKey: return "ApplicationsKey";
        case KEY_Sleep: return "Sleep";
        case KEY_NumPad0: return "NumPad0";
        case KEY_NumPad1: return "NumPad1";
        case KEY_NumPad2: return "NumPad2";
        case KEY_NumPad3: return "NumPad3";
        case KEY_NumPad4: return "NumPad4";
        case KEY_NumPad5: return "NumPad5";
        case KEY_NumPad6: return "NumPad6";
        case KEY_NumPad7: return "NumPad7";
        case KEY_NumPad8: return "NumPad8";
        case KEY_NumPad9: return "NumPad9";
        case KEY_Multiply: return "Multiply";
        case KEY_Add: return "Add";
        case KEY_Seperator: return "Seperator";
        case KEY_Subtract: return "Subtract";
        case KEY_Decimal: return "Decimal";
        case KEY_Divide: return "Divide";
        case KEY_F1: return "F1";
        case KEY_F2: return "F2";
        case KEY_F3: return "F3";
        case KEY_F4: return "F4";
        case KEY_F5: return "F5";
        case KEY_F6: return "F6";
        case KEY_F7: return "F7";
        case KEY_F8: return "F8";
        case KEY_F9: return "F9";
        case KEY_F10: return "F10";
        case KEY_F11: return "F11";
        case KEY_F12: return "F12";
        case KEY_F13: return "F13";
        case KEY_F14: return "F14";
        case KEY_F15: return "F15";
        case KEY_F16: return "F16";
        case KEY_F17: return "F17";
        case KEY_F18: return "F18";
        case KEY_F19: return "F19";
        case KEY_F20: return "F20";
        case KEY_F21: return "F21";
        case KEY_F22: return "F22";
        case KEY_F23: return "F23";
        case KEY_F24: return "F24";
        case KEY_Numlock: return "Numlock";
        case KEY_ScrollLock: return "ScrollLock";
        case KEY_LeftShift: return "LeftShift";
        case KEY_RightShift: return "RightShift";
        case KEY_LeftControl: return "LeftControl";
        case KEY_RightContol: return "RightContol";
        case KEY_LeftMenu: return "LeftMenu";
        case KEY_RightMenu: return "RightMenu";
        case KEY_BrowserBack: return "BrowserBack";
        case KEY_BrowserForward: return "BrowserForward";
        case KEY_BrowserRefresh: return "BrowserRefresh";
        case KEY_BrowserStop: return "BrowserStop";
        case KEY_BrowserSearch: return "BrowserSearch";
        case KEY_BrowserFavorites: return "BrowserFavorites";
        case KEY_BrowserHome: return "BrowserHome";
        case KEY_VolumeMute: return "VolumeMute";
        case KEY_VolumeDown: return "VolumeDown";
        case KEY_VolumeUp: return "VolumeUp";
        case KEY_NextTrack: return "NextTrack";
        case KEY_PreviousTrack: return "PreviousTrack";
        case KEY_StopMedia: return "StopMedia";
        case KEY_PlayPause: return "PlayPause";
        case KEY_LaunchMail: return "LaunchMail";
        case KEY_SelectMedia: return "SelectMedia";
        case KEY_LaunchApp1: return "LaunchApp1";
        case KEY_LaunchApp2: return "LaunchApp2";
        case KEY_OEM1: return "OEM1";
        case KEY_OEMPlus: return "OEMPlus";
        case KEY_OEMComma: return "OEMComma";
        case KEY_OEMMinus: return "OEMMinus";
        case KEY_OEMPeriod: return "OEMPeriod";
        case KEY_OEM2: return "OEM2";
        case KEY_OEM3: return "OEM3";
        case KEY_OEM4: return "OEM4";
        case KEY_OEM5: return "OEM5";
        case KEY_OEM6: return "OEM6";
        case KEY_OEM7: return "OEM7";
        case KEY_OEM8: return "OEM8";
        case KEY_OEM102: return "OEM102";
        case KEY_Process: return "Process";
        case KEY_Packet: return "Packet";
        case KEY_Attn: return "Attn";
        case KEY_CrSel: return "CrSel";
        case KEY_ExSel: return "ExSel";
        case KEY_EraseEOF: return "EraseEOF";
        case KEY_Play: return "Play";
        case KEY_Zoom: return "Zoom";
        case KEY_PA1: return "PA1";
        case KEY_OEMClear: return "OEMClear";
        case KEY_ArrowUp: return "ArrowUp";
        case KEY_ArrowDown: return "ArrowDown";
        case KEY_ArrowLeft: return "ArrowLeft";
        case KEY_ArrowRight: return "ArrowRight";
        case CONTROLLER_A: return "Controller_A";
        case CONTROLLER_B: return "Controller_B";
        case CONTROLLER_Y: return "Controller_Y";
        case CONTROLLER_X: return "Controller_X";
        case CONTROLLER_Start: return "Controller_Start";
        case CONTROLLER_Back: return "Controller_Back";
        case CONTROLLER_RB: return "Controller_RB";
        case CONTROLLER_LB: return "Controller_LB";
        case CONTROLLER_RT: return "Controller_RT";
        case CONTROLLER_LT: return "Controller_LT";
        case CONTROLLER_Up: return "Controller_Up";
        case CONTROLLER_Down: return "Controller_Down";
        case CONTROLLER_Left: return "Controller_Left";
        case CONTROLLER_Right: return "Controller_Right";

        // Geode Additions

        case CONTROLLER_LTHUMBSTICK_UP: return "Controller_LTHUMBSTICK_UP";
        case CONTROLLER_LTHUMBSTICK_DOWN: return "Controller_LTHUMBSTICK_DOWN";
        case CONTROLLER_LTHUMBSTICK_LEFT: return "Controller_LTHUMBSTICK_LEFT";
        case CONTROLLER_LTHUMBSTICK_RIGHT: return "Controller_LTHUMBSTICK_RIGHT";
        case CONTROLLER_RTHUMBSTICK_UP: return "Controller_RTHUMBSTICK_UP";
        case CONTROLLER_RTHUMBSTICK_DOWN: return "Controller_RTHUMBSTICK_DOWN";
        case CONTROLLER_RTHUMBSTICK_LEFT: return "Controller_RTHUMBSTICK_LEFT";
        case CONTROLLER_RTHUMBSTICK_RIGHT: return "Controller_RTHUMBSTICK_RIGHT";
        case KEY_GraveAccent: return "`";
        case KEY_OEMEqual: return "=";
        case KEY_LeftBracket: return "[";
        case KEY_RightBracket: return "]";
        case KEY_Backslash: return "\\";
        case KEY_Semicolon: return ";";
        case KEY_Apostrophe: return "'";
        case KEY_Slash: return "/";
        case KEY_NumEnter: return "=";
        case KEY_World1: return "INTL-1";
        case KEY_World2: return "INTL-2";
        case MOUSE_4: return "Mouse 4";
        case MOUSE_5: return "Mouse 5";
        case MOUSE_6: return "Mouse 6";
        case MOUSE_7: return "Mouse 7";
        case MOUSE_8: return "Mouse 8";

        case KEY_None:
        case KEY_Unknown:
        default:
            return nullptr;
    }
}

void CCKeyboardDispatcher::updateModifierKeys(bool shft, bool ctrl, bool alt, bool cmd)
{
    m_bShiftPressed = shft;
    m_bAltPressed = alt;
    m_bControlPressed = ctrl || cmd;
    m_bCommandPressed = cmd;
}

#endif
