#pragma once

#include <Geode/platform/platform.hpp>

#include "string.hpp"

#if defined(GEODE_IS_ANDROID)
#include "gnustl.hpp"
#else
#include "aliastl.hpp"
#endif
