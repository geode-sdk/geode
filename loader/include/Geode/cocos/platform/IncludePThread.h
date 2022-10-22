#pragma once

#include "CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    #include "third_party/win32/pthread/pthread.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // nothing?
    #include <pthread.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // nothing?
    #include <pthread.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    // nothing?
    #include <pthread.h>
#endif

