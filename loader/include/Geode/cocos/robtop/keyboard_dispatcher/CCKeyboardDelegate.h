#ifndef __CCKEYBOARD_DELEGATE_H__
#define __CCKEYBOARD_DELEGATE_H__

#include "../../include/ccMacros.h"
#include "../../cocoa/CCObject.h"

RT_ADD(
	NS_CC_BEGIN

	/*no clue if this is accurate.
	* https://github.com/cocos2d/cocos2d-x/blob/cocos2d-x-3.13/cocos/base/CCEventKeyboard.h#L48
	* https://github.com/reneklacan/cocos2d-x/wiki/Keyboard-keycodes-enum
	* not sure which one is which
	*/
    typedef enum
    {
        // this one might not actually exist in gd itself
        KEY_Unknown = -0x01,
        KEY_None = 0x00,
        KEY_Backspace = 0x08,
        KEY_Tab = 0x09,
        KEY_Clear = 0x0C,
        KEY_Enter = 0x0D,
        KEY_Shift = 0x10,
        KEY_Control = 0x11,
        KEY_Alt = 0x12,
        KEY_Pause = 0x13,
        KEY_CapsLock = 0x14,
        KEY_Escape = 0x1B,
        KEY_Space = 0x20,
        KEY_PageUp = 0x21,
        KEY_PageDown = 0x22,
        KEY_End = 0x23,
        KEY_Home = 0x24,
        KEY_Left = 0x25,
        KEY_Up = 0x26,
        KEY_Right = 0x27,
        KEY_Down = 0x28,
        KEY_Select = 0x29,
        KEY_Print = 0x2A,
        KEY_Execute = 0x2B,
        KEY_PrintScreen = 0x2C,
        KEY_Insert = 0x2D,
        KEY_Delete = 0x2E,
        KEY_Help = 0x2F,
        KEY_Zero = 0x30,
        KEY_One = 0x31,
        KEY_Two = 0x32,
        KEY_Three = 0x33,
        KEY_Four = 0x34,
        KEY_Five = 0x35,
        KEY_Six = 0x36,
        KEY_Seven = 0x37,
        KEY_Eight = 0x38,
        KEY_Nine = 0x39,
        KEY_A = 0x41,
        KEY_B = 0x42,
        KEY_C = 0x43,
        KEY_D = 0x44,
        KEY_E = 0x45,
        KEY_F = 0x46,
        KEY_G = 0x47,
        KEY_H = 0x48,
        KEY_I = 0x49,
        KEY_J = 0x4A,
        KEY_K = 0x4B,
        KEY_L = 0x4C,
        KEY_M = 0x4D,
        KEY_N = 0x4E,
        KEY_O = 0x4F,
        KEY_P = 0x50,
        KEY_Q = 0x51,
        KEY_R = 0x52,
        KEY_S = 0x53,
        KEY_T = 0x54,
        KEY_U = 0x55,
        KEY_V = 0x56,
        KEY_W = 0x57,
        KEY_X = 0x58,
        KEY_Y = 0x59,
        KEY_Z = 0x5A,
        KEY_LeftWindowsKey = 0x5B,
        KEY_RightWindowsKey = 0x5C,
        KEY_ApplicationsKey = 0x5D,
        KEY_Sleep = 0x5F,
        KEY_NumPad0 = 0x60,
        KEY_NumPad1 = 0x61,
        KEY_NumPad2 = 0x62,
        KEY_NumPad3 = 0x63,
        KEY_NumPad4 = 0x64,
        KEY_NumPad5 = 0x65,
        KEY_NumPad6 = 0x66,
        KEY_NumPad7 = 0x67,
        KEY_NumPad8 = 0x68,
        KEY_NumPad9 = 0x69,
        KEY_Multiply = 0x6A,
        KEY_Add = 0x6B,
        KEY_Seperator = 0x6C,
        KEY_Subtract = 0x6D,
        KEY_Decimal = 0x6E,
        KEY_Divide = 0x6F,
        KEY_F1 = 0x70,
        KEY_F2 = 0x71,
        KEY_F3 = 0x72,
        KEY_F4 = 0x73,
        KEY_F5 = 0x74,
        KEY_F6 = 0x75,
        KEY_F7 = 0x76,
        KEY_F8 = 0x77,
        KEY_F9 = 0x78,
        KEY_F10 = 0x79,
        KEY_F11 = 0x7A,
        KEY_F12 = 0x7B,
        KEY_F13 = 0x7C,
        KEY_F14 = 0x7D,
        KEY_F15 = 0x7E,
        KEY_F16 = 0x7F,
        KEY_F17 = 0x80,
        KEY_F18 = 0x81,
        KEY_F19 = 0x82,
        KEY_F20 = 0x83,
        KEY_F21 = 0x84,
        KEY_F22 = 0x85,
        KEY_F23 = 0x86,
        KEY_F24 = 0x87,
        KEY_Numlock = 0x90,
        KEY_ScrollLock = 0x91,
        KEY_LeftShift = 0xA0,
        KEY_RightShift = 0xA1,
        KEY_LeftControl = 0xA2,
        KEY_RightContol = 0xA3,
        KEY_LeftMenu = 0xA4,
        KEY_RightMenu = 0xA5,
        KEY_BrowserBack = 0xA6,
        KEY_BrowserForward = 0xA7,
        KEY_BrowserRefresh = 0xA8,
        KEY_BrowserStop = 0xA9,
        KEY_BrowserSearch = 0xAA,
        KEY_BrowserFavorites = 0xAB,
        KEY_BrowserHome = 0xAC,
        KEY_VolumeMute = 0xAD,
        KEY_VolumeDown = 0xAE,
        KEY_VolumeUp = 0xAF,
        KEY_NextTrack = 0xB0,
        KEY_PreviousTrack = 0xB1,
        KEY_StopMedia = 0xB2,
        KEY_PlayPause = 0xB3,
        KEY_LaunchMail = 0xB4,
        KEY_SelectMedia = 0xB5,
        KEY_LaunchApp1 = 0xB6,
        KEY_LaunchApp2 = 0xB7,
        KEY_OEM1 = 0xBA,
        KEY_OEMPlus = 0xB8,
        KEY_OEMComma = 0xBC,
        KEY_OEMMinus = 0xBD,
        KEY_OEMPeriod = 0xBE,
        KEY_OEM2 = 0xBF,
        KEY_OEM3 = 0xC0,
        KEY_OEM4 = 0xDB,
        KEY_OEM5 = 0xDC,
        KEY_OEM6 = 0xDD,
        KEY_OEM7 = 0xDE,
        KEY_OEM8 = 0xDF,
        KEY_OEM102 = 0xE2,
        KEY_Process = 0xE5,
        KEY_Packet = 0xE7,
        KEY_Attn = 0xF6,
        KEY_CrSel = 0xF7,
        KEY_ExSel = 0xF8,
        KEY_EraseEOF = 0xF9,
        KEY_Play = 0xFA,
        KEY_Zoom = 0xFB,
        KEY_PA1 = 0xFD,
        KEY_OEMClear = 0xFE,
        KEY_ArrowUp = 0x11B,
        KEY_ArrowDown = 0x11C,
        KEY_ArrowLeft = 0x11D,
        KEY_ArrowRight = 0x11E,
        CONTROLLER_A = 0x3E9,
        CONTROLLER_B = 0x3EA,
        CONTROLLER_Y = 0x3EB,
        CONTROLLER_X = 0x3EC,
        CONTROLLER_Start = 0x3ED,
        CONTROLLER_Back = 0x3EE,
        CONTROLLER_RB = 0x3EF,
        CONTROLLER_LB = 0x3F0,
        CONTROLLER_RT = 0x3F1,
        CONTROLLER_LT = 0x3F2,
        CONTROLLER_Up = 0x3F3,
        CONTROLLER_Down = 0x3F4,
        CONTROLLER_Left = 0x3F5,
        CONTROLLER_Right = 0x3F6
    } enumKeyCodes;

	class CC_DLL CCKeyboardDelegate
	{
	public:
		virtual void keyDown(enumKeyCodes key) {}

		virtual void keyUp(enumKeyCodes key) {}

		//pretty certain there's no fields, based on initializer
	};

	class CC_DLL CCKeyboardHandler : public CCObject
	{
	public:
		GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCKeyboardHandler, CCObject)
		inline CCKeyboardHandler() = default;
		
		virtual ~CCKeyboardHandler();

		CCKeyboardHandler& operator=(const CCKeyboardHandler&);

		CCKeyboardDelegate* getDelegate();

		static CCKeyboardHandler* handlerWithDelegate(CCKeyboardDelegate* pDelegate);

		virtual bool initWithDelegate(CCKeyboardDelegate* pDelegate);

		void setDelegate(CCKeyboardDelegate* pDelegate);

	protected:
		CCKeyboardDelegate* m_pDelegate;
	};

	NS_CC_END
)

#endif
