#pragma once
#include <Geode/platform/cplatform.h>

#ifdef GEODE_IS_WINDOWS
#include "win32/CCApplication.h"
#elif defined(GEODE_IS_MACOS)
#include "mac/CCApplication.h"
#elif defined(GEODE_IS_ANDROID)
#include "android/CCApplication.h"
#elif defined(GEODE_IS_IOS)
#include "ios/CCApplication.h"
#endif