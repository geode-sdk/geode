#pragma once
#include <Geode/platform/cplatform.h>

#ifdef GEODE_IS_WINDOWS
#include "win32/CCEGLView.h"
#elif defined(GEODE_IS_MACOS)
#include "mac/CCEGLView.h"
#elif defined(GEODE_IS_ANDROID)
#include "android/CCEGLView.h"
#elif defined(GEODE_IS_IOS)
#include "ios/CCEGLView.h"
#endif