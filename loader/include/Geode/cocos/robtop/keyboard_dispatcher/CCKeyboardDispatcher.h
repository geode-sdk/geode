#ifndef __CCKEYBOARD_DISPATCHER_H__
#define __CCKEYBOARD_DISPATCHER_H__

#include "CCKeyboardDelegate.h"
#include "../../cocoa/CCArray.h"
#include <unordered_map>


NS_CC_BEGIN

class CCKeyboardActionManager;

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
	inline bool getLastRepeat() const {
		return m_bLastRepeat;
	}
	inline bool getCaptureMode() const {
		return m_bCaptureMode;
	}

	void releaseAllKeys(double);

protected:
	void dispatchToTopDelegate(enumKeyCodes key, bool down, std::vector<int> const& actions, double timestamp);

public:
	CCArray* m_pHandlers;	// 0x38
	bool m_bLocked;			// 0x40
	bool m_bToAdd;      // 0x41
	bool m_bToRemove;			// 0x42
	ccCArray* m_pHandlersToAdd; // 0x48
	ccCArray* m_pHandlersToRemove;	// 0x50
	std::unordered_map<enumKeyCodes, std::vector<int>> m_pressedKeyActions; // 0x58
	bool m_bShiftPressed;		// 0x98
	bool m_bControlPressed;	// 0x99
	bool m_bAltPressed;			// 0x9a
	bool m_bCommandPressed;	// 0x9b
	bool m_bBlockRepeat;		// 0x9c
	bool m_bLastRepeat;		// 0x9d
	bool m_bCaptureMode;		// 0x9e
	CCKeyboardActionManager* m_pActionManager;	// 0xa0
};

NS_CC_END

#endif
