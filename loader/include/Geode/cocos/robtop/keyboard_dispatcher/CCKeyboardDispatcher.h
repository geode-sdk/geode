#ifndef __CCKEYBOARD_DISPATCHER_H__
#define __CCKEYBOARD_DISPATCHER_H__

#include "CCKeyboardDelegate.h"
#include "../../cocoa/CCArray.h"


NS_CC_BEGIN

// @note RobTop Addition
class CC_DLL CCKeyboardDispatcher : public CCObject 
{
public:
	GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCKeyboardDispatcher, CCObject)
	CCKeyboardDispatcher();
	virtual ~CCKeyboardDispatcher();

	void addDelegate(CCKeyboardDelegate* pDelegate);
	void removeDelegate(CCKeyboardDelegate* pDelegate);

	void forceAddDelegate(CCKeyboardDelegate* pDelegate);
	void forceRemoveDelegate(CCKeyboardDelegate* pDelegate);

	static enumKeyCodes convertKeyCode(enumKeyCodes key);

	bool dispatchKeyboardMSG(enumKeyCodes key, bool isKeyDown, bool isKeyRepeat);

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

	inline bool getBlockRepeat() const {
		return m_bBlockRepeat;
	}

	inline void setBlockRepeat(bool blockRepeat) {
		this->m_bBlockRepeat = blockRepeat;
	}

public:
	CCArray* m_pDelegates;	// 0x34
	bool m_bUnknown38;			// 0x38
	bool m_bUnknown39;      // 0x39
	bool m_bUnknown3a;			// 0x3a
	ccCArray* m_pUnknown3c; // 0x3c
	ccCArray* m_pUnknown40;	// 0x40
	bool m_bShiftPressed;		// 0x44
	bool m_bControlPressed;	// 0x45
	bool m_bAltPressed;			// 0x46
	bool m_bCommandPressed;	// 0x47
	bool m_bBlockRepeat;		// 0x48

	// ~~there's more here, check the initializer~~ no there's not??
};

NS_CC_END

#endif
