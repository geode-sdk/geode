#ifndef __CC_KEYBOARD_ACTION_MANAGER_H__
#define __CC_KEYBOARD_ACTION_MANAGER_H__

#include "../../cocoa/CCObject.h"

NS_CC_BEGIN

class CC_DLL CCKeyboardActionManager : public CCObject
{
    GEODE_FRIEND_MODIFY

public:
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCKeyboardActionManager, CCObject)

    virtual std::vector<int> actionsForKey(enumKeyCodes key, bool ctrl, bool alt, bool shift);
    virtual bool handleGlobalAction(int, bool);
};

NS_CC_END

#endif  // __CC_KEYBOARD_ACTION_MANAGER_H__
