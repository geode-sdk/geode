#pragma once

#include <Geode/platform/platform.hpp>

#include "string.hpp"

#if defined(GEODE_IS_ANDROID)
#include "gnustl.hpp"
#else
// this is quite funny but msvcstl is just all aliases
#include "msvcstl.hpp"
#endif
