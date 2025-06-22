#pragma once

#include <type_traits>

namespace geode {
    template <class T>
    GEODE_INLINE inline void doNotOptimize(T const& value) {
        asm volatile("" : : "r,m"(value) : "memory");
    }

    template <class T>
    GEODE_INLINE inline void doNotOptimize(T& value) {
        asm volatile("" : "+r,m"(value) : : "memory");
    }

    template <class T>
    GEODE_INLINE inline void doNotOptimize(T&& value) {
        asm volatile("" : "+r,m"(value) : : "memory");
    }
}