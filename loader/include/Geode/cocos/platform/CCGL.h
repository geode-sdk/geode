
#ifndef __CC_GL_GENERIC_H__
#define __CC_GL_GENERIC_H__

#include "CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    #include "win32/CCGL.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "ios/CCGL.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "android/CCGL.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    #include "mac/CCGL.h"
#endif

#endif
