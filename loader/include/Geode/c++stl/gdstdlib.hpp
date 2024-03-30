#pragma once

#include <Geode/platform/platform.hpp>

#include "string.hpp"

#if defined(GEODE_IS_ANDROID)
#include "gnustl.hpp"
#elif defined(GEODE_IS_WINDOWS)
#include "msvcstl.hpp"
#else
#include "aliastl.hpp"
#endif
