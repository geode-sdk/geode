#ifndef __CC_CONTROLLER_STATE_WIN32_H__
#define __CC_CONTROLLER_STATE_WIN32_H__

#include "../../include/ccMacros.h"
#include "CCStdC.h"

NS_CC_BEGIN

class CC_DLL CCControllerState
{
    GEODE_FRIEND_MODIFY
public:
    DWORD m_packetNumber;
    WORD m_buttons;
    unsigned char m_leftTrigger;
    unsigned char m_rightTrigger;
    short m_thumbLX;
    short m_thumbLY;
    short m_thumbRX;
    short m_thumbRY;
};

NS_CC_END

#endif