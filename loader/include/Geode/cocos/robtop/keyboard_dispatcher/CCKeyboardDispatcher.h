#ifndef __CCKEYBOARD_DISPATCHER_H__
#define __CCKEYBOARD_DISPATCHER_H__

#include "CCKeyboardDelegate.h"
#include "../../cocoa/CCArray.h"

RT_ADD(
	NS_CC_BEGIN

	class CC_DLL CCKeyboardDispatcher : public CCObject 
	{
	public:
		CCKeyboardDispatcher();
		virtual ~CCKeyboardDispatcher();

		void addDelegate(CCKeyboardDelegate* pDelegate);
		void removeDelegate(CCKeyboardDelegate* pDelegate);

		void forceAddDelegate(CCKeyboardDelegate* pDelegate);
		void forceRemoveDelegate(CCKeyboardDelegate* pDelegate);

		enumKeyCodes convertKeyCode(enumKeyCodes key);

		bool dispatchKeyboardMSG(enumKeyCodes key, bool);
		
		inline bool getAltKeyPressed() const {
			return m_bAltPressed;
		}
		inline bool getCommandKeyPressed() const {
			return m_bCommandPressed;
		}
		inline bool getControlKeyPressed() const {
			return m_bControlPressed;
		}
		inline bool getShiftKeyPressed() const {
			return m_bShiftPressed;
		}

		static GEODE_DLL CCKeyboardDispatcher* get();

		const char* keyToString(enumKeyCodes key);

		void updateModifierKeys(bool shft, bool ctrl, bool alt, bool cmd);

	protected:
		CCArray* m_pDelegates;		// 0x20
		bool m_bUnknown24;			// 0x24
		bool m_bUnknown25;			// 0x25
		ccCArray* m_pUnknown28;		// 0x28
		ccCArray* m_pUnknown2c;		// 0x2c
		bool m_bShiftPressed;		// 0x30
		bool m_bControlPressed;		// 0x31
		bool m_bAltPressed;			// 0x32
		bool m_bCommandPressed;		// 0x33
		//there's more here, check the initializer
	};

	NS_CC_END
)

#endif
