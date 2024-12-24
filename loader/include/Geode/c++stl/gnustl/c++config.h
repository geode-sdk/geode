#pragma once

#include <cstdint>

#if UINTPTR_MAX > 0xffffffff
# include "c++config-arm64.h"
#else
# include "c++config-armeabi-v7a.h"
#endif

#define _GLIBCXX_NODISCARD __attribute__ ((__warn_unused_result__))
#define _GLIBCXX20_CONSTEXPR constexpr
#define _GLIBCXX17_CONSTEXPR constexpr
#define _GLIBCXX_NOEXCEPT_IF(...) noexcept(__VA_ARGS__)

// i put this here cuz it's used all over the place
namespace geode::stl {
    template <typename T> using __not_ = std::integral_constant<bool, !T::value>;
    template <typename... T> using __or_ = std::disjunction<T...>;
    template <typename... T> using __and_ = std::conjunction<T...>;
}
