#pragma once

#include "CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    #include "third_party/win32/zlib/zlib.h"
#else
    #include <zlib.h>
    // i don't know where other platform get zlib
#endif
