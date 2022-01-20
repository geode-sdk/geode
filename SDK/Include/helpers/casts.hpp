#pragma once

#include <inttypes.h>

namespace geode::cast {
    /** 
     * Alias for reinterpret_cast
     */
    template <typename T, typename R>
    static constexpr T as(R const v) {
        return reinterpret_cast<T>(v);
    }

    /**
     * Cast from anything to anything else,
     * provided they are the same size
     */
    template<typename R, typename T>
    static constexpr R union_cast(T v) {
        static_assert(sizeof(T) == sizeof(R), "union_cast: R and T don't match in size!");
        constexpr union {
            R r;
            T t;
        } x;
        x.t = v;
        return x.r;
    }
}
