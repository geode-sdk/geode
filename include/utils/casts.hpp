#pragma once

#include <inttypes.h>
#include <string>
#include <type_traits>
#include <iostream>

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
     * Reference casting. Does a pointer-to-pointer
     * cast but uses reference syntactic sugar to
     * look cleaner.
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
	    return reinterpret_cast<T>(dynamic_cast<void*>(obj));
	}


    /** 
     * Cast based on RTTI. This is a replacement for 
     * dynamic_cast, since it doesn't work for gd.
     */
    template <typename T, typename F>
    static T typeid_cast(F obj) {
        if (std::string(typeid(*obj).name()) == typeid(std::remove_pointer_t<T>).name())
            return reinterpret_cast<T>(obj);
        else
            return nullptr;
    }
}
