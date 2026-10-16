#ifndef __CC_KEYBOARD_ACTION_MANAGER_H__
#define __CC_KEYBOARD_ACTION_MANAGER_H__

#include "../../cocoa/CCObject.h"

NS_CC_BEGIN

class CC_DLL CCKeyboardActionManager : public CCObject
{
public:
    virtual bool handleGlobalAction(int, bool);
};

NS_CC_END

#endif  // __CC_KEYBOARD_ACTION_MANAGER_H__
