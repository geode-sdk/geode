#pragma once

#include <inttypes.h>

namespace geode::cast {
    /** 
     * Alias for reinterpret_cast
     */
    template <typename T, typename F>
    static constexpr T as(F const v) {
        return reinterpret_cast<T>(v);
    }

    /**
     * Cast from anything to anything else,
     * provided they are the same size
     */
    template<typename T, typename F>
    static constexpr T union_cast(F v) {
        static_assert(sizeof(F) == sizeof(T), "union_cast: R and T don't match in size!");
        union {
            T t;
            F f;
        } x;
        x.f = v;
        return x.t;
    }

    /** 
     * Reference casting, it's pretty much black 
     * magic so idk how to explain it
     */
    template<typename T, typename F>
	static constexpr T reference_cast(F v) {
		return reinterpret_cast<T&>(v);
	}

	/** 
     * Cast an adjusted this pointer to it's base pointer
     */
	template <typename T, typename F>
	static constexpr T base_cast(F obj) {
	    return static_cast<T>(dynamic_cast<void*>(obj));
	}
}
