#pragma once

#include <Geode/platform/platform.hpp>
#include <inttypes.h>
#include <iostream>
#include <string>
#include <type_traits>

namespace geode::cast {
    /**
     * Cast from anything to anything else,
     * provided they are the same size
     */
    template <class T, class F>
    static constexpr T union_cast(F const v) {
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
    template <class T, class F>
    static constexpr T reference_cast(F v) {
        return reinterpret_cast<T&>(v);
    }


    template <class Type>
    struct ToVoidPtr {
        using type = void*;
    };

    template <class Type>
    struct ToVoidPtr<Type const*> {
        using type = void const*;
    };

    /**
     * Cast based on RTTI. Casts an adjusted this pointer
     * to it's non offset form.
     */
    template <class T, class F>
    static constexpr T base_cast(F const obj) {
        return static_cast<T>(dynamic_cast<typename ToVoidPtr<T>::type>(obj));
    }

    /**
     * Cast based on RTTI. This is used to check
     * if an object is exactly the class needed. Returns
     * nullptr on failure.
     */
    template <class T, class F>
    static T exact_cast(F const obj) {
        using PureType = std::remove_cvref_t<std::remove_pointer_t<T>>;
        if (std::strcmp(getRuntimeTypeName(obj), getRuntimeTypeName(typeid(PureType))) == 0) {
            return (T)obj;
        }
        return nullptr;
    }

}
