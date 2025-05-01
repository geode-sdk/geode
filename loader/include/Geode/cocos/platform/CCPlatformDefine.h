
#ifndef __CC_PLATFORM_DEFINE_GENERIC_H__
#define __CC_PLATFORM_DEFINE_GENERIC_H__

#include "CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    #include "win32/CCPlatformDefine.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "ios/CCPlatformDefine.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "android/CCPlatformDefine.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    #include "mac/CCPlatformDefine.h"
#endif

#endif
