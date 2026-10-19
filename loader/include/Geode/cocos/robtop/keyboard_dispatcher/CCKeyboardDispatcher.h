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

	bool dispatchKeyboardMSG(enumKeyCodes key, bool isKeyDown, bool isKeyRepeat, double);

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

	void flushPendingDelegates();
	bool getLastRepeat() const;
	bool getCaptureMode() const;

	void releaseAllKeys(double);

protected:
	void dispatchToTopDelegate(enumKeyCodes key, bool unk, std::vector<int> const& unk2, double unk3);

public:
	CCArray* m_pHandlers;	// 0x34
	bool m_bLocked;			// 0x38
	bool m_bToAdd;      // 0x39
	bool m_bToRemove;			// 0x3a
	ccCArray* m_pHandlersToAdd; // 0x3c
	ccCArray* m_pHandlersToRemove;	// 0x40
#ifdef GEODE_IS_WINDOWS
	// 2.209+: m_bShiftPressed moved from 0x58 to 0x98 (getShiftKeyPressed reads +0x98)
	char m_unknown2209[0x40];
#endif
	bool m_bShiftPressed;		// 0x44
	bool m_bControlPressed;	// 0x45
	bool m_bAltPressed;			// 0x46
	bool m_bCommandPressed;	// 0x47
	bool m_bBlockRepeat;		// 0x48
#ifdef GEODE_IS_WINDOWS
	// 2.209+: getLastRepeat reads +0x9d, getCaptureMode +0x9e, getActionManager +0xa0
	bool m_bLastRepeat;
	bool m_bCaptureMode;
	void* m_pActionManager;
#endif

	// ~~there's more here, check the initializer~~ no there's not??
};

NS_CC_END

#endif
