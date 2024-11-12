#pragma once

#include <cstdint>

#if UINTPTR_MAX > 0xffffffff
# include "c++config-arm64.h"
#else
# include "c++config-armeabi-v7a.h"
#endif

// i put this here cuz it's used all over the place
namespace geode::stl {
    template <typename T> using __not_ = std::integral_constant<bool, !T::value>;
}
