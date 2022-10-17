#pragma once

#include "CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    #include "third_party/win32/curl/curl.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // nothing?
    #include "third_party/ios/curl/curl.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "third_party/android/prebuilt/libcurl/include/curl/curl.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    // nothing?
    #include <curl/curl.h>
#endif

