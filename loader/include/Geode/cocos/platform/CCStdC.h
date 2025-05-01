
#ifndef __CC_STD_C_GENERIC_H__
#define __CC_STD_C_GENERIC_H__

#include "CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    #include "win32/CCStdC.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "ios/CCStdC.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "android/CCStdC.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    #include "mac/CCStdC.h"
#endif

#endif
