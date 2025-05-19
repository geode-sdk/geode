#pragma once

#include <cstdint>

#if UINTPTR_MAX > 0xffffffff
# include "c++config-arm64.h"
#else
# include "c++config-armeabi-v7a.h"
#endif

#include "type_traits.h"

#define _GLIBCXX_NODISCARD __attribute__ ((__warn_unused_result__))
#define _GLIBCXX20_CONSTEXPR constexpr
#define _GLIBCXX17_CONSTEXPR constexpr
#define _GLIBCXX14_CONSTEXPR constexpr
#define _GLIBCXX_NOEXCEPT_IF(...) noexcept(__VA_ARGS__)

# define __glibcxx_requires_valid_range(_First,_Last)