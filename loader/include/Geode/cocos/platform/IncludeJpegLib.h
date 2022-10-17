#pragma once

#include "CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    #include "third_party/win32/libjpeg/jpeglib.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // nothing?
    #include <jpeglib.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "third_party/android/prebuilt/libjpeg/include/jpeglib.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    // nothing?
    #include <jpeglib.h>
#endif

