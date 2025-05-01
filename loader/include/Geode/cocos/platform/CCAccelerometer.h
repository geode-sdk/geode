
#ifndef __CC_ACCELEROMETER_GENERIC_H__
#define __CC_ACCELEROMETER_GENERIC_H__

#include "CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    #include "win32/CCAccelerometer.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "ios/CCAccelerometer.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "android/CCAccelerometer.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    #include "mac/CCAccelerometer.h"
#endif

#endif
