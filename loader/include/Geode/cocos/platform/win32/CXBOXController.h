#ifndef __CXBOXCONTROLLER_WIN32_H__
#define __CXBOXCONTROLLER_WIN32_H__

#include "../../include/ccMacros.h"
#include "CCStdC.h"
#include "CCControllerState.h"
#include <Xinput.h>

class CC_DLL CXBOXController
{
    GEODE_FRIEND_MODIFY
public:
    //CCControllerHandler();
    //CCControllerState getState();
    //void updateConnected();

    XINPUT_STATE m_xinputState;
    int m_userIndex;
    bool m_controllerConnected;
    bool m_dpadUp;
    bool m_dpadDown;
    bool m_dpadLeft;
    bool m_dpadRight;
    bool m_start;
    bool m_back;
    bool m_lt;
    bool m_rt;
    bool m_lb;
    bool m_rb;
    bool m_buttonA;
    bool m_buttonB;
    bool m_buttonX;
    bool m_buttonY;
};

#endif
