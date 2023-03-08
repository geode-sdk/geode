#pragma once

/**
 * Adapted from https://gist.github.com/altalk23/29b97969e9f0624f783b673f6c1cd279
 */

#include "../utils/casts.hpp"
#include "casts.hpp"

#include <Geode/DefaultInclude.hpp>
#include <cocos-ext.h>
#include <concepts>
#include <cstdlib>
#include <stddef.h>
#include <type_traits>

namespace geode::addresser {

    template <class Function>
    intptr_t getVirtual(Function func);

    template <class Function>
    intptr_t getNonVirtual(Function func);

    template <class Function, class Class>
    Class thunkAdjust(Function func, Class self);

    template <class Function, class Class>
    Class rthunkAdjust(Function func, Class self);

    class GEODE_DLL Addresser final {
        template <char C>
        struct SingleInheritance {
            virtual ~SingleInheritance() {}
        };

        struct MultipleInheritance : SingleInheritance<'L'>, SingleInheritance<'F'> {
            virtual ~MultipleInheritance() {}
        };

        static MultipleInheritance* instance();

        template <typename R, typename T, typename... Ps>
        static ptrdiff_t indexOf(R (T::*func)(Ps...)) {
            using method_t = ptrdiff_t (T::*)();
            return (reinterpret_cast<T*>(instance())->*reinterpret_cast<method_t>(func))();
        }

        template <typename R, typename T, typename... Ps>
        static ptrdiff_t indexOf(R (T::*func)(Ps...) const) {
            return indexOf(reinterpret_cast<R (T::*)(Ps...)>(func));
        }

        template <typename T>
        static ptrdiff_t thunkOf(T ptr) {
            // msvc
            if (sizeof(T) == sizeof(ptrdiff_t)) return 0;

            // all
            auto thunk = *(reinterpret_cast<ptrdiff_t*>(&ptr) + 1);

            // arm
            if (thunk & 1) thunk >>= 1;
            return thunk;
        }

        template <class Class>
        static Class* cachedInstance() {
            static auto ret = new Class(ZeroConstructor);
            return ret;
        }

        /**
         * Specialized functionss
         */
        template <class Return, class Class, class... Parameters>
        static intptr_t addressOfVirtual(Return (Class::*func)(Parameters...)) {
            using geode::cast::reference_cast;

            auto ins = cachedInstance<Class>();
            // generateInstance will return nullptr on most abstract classes,
            // so dont bother getting the address
            if (ins == nullptr) {
                return 0;
            }
            auto index = indexOf(func);
            auto thunk = thunkOf(func);

            // [[this + thunk] + offset] is the function we want
            auto address = *reinterpret_cast<intptr_t*>(
                *reinterpret_cast<intptr_t*>(reinterpret_cast<intptr_t>(ins) + thunk) + index
            );

            address = followThunkFunction(address);

            return address;
        }

        template <class Return, class Class, class... Parameters>
        static intptr_t addressOfVirtual(Return (Class::*func)(Parameters...) const) {
            return addressOfVirtual(reinterpret_cast<Return (Class::*)(Parameters...)>(func));
        }

        template <class Return, class Class, class... Parameters>
        static intptr_t addressOfNonVirtual(Return (Class::*func)(Parameters...) const) {
            return addressOfNonVirtual(reinterpret_cast<Return (Class::*)(Parameters...)>(func));
        }

        static intptr_t followThunkFunction(intptr_t address);

        template <class Return, class Class, class... Parameters>
        static intptr_t addressOfNonVirtual(Return (Class::*func)(Parameters...)) {
            return followThunkFunction(geode::cast::reference_cast<intptr_t>(func));
        }

        template <class Return, class... Parameters>
        static intptr_t addressOfNonVirtual(Return (*func)(Parameters...)) {
            return followThunkFunction(geode::cast::reference_cast<intptr_t>(func));
        }

        template <class Function>
        friend intptr_t getVirtual(Function func);

        template <class Function>
        friend intptr_t getNonVirtual(Function func);

        template <class Function, class Class>
        friend Class thunkAdjust(Function func, Class self);

        template <class Function, class Class>
        friend Class rthunkAdjust(Function func, Class self);
    };

    /**
     * Gets the real address of a virtual function
     */
    template <class Function>
    inline intptr_t getVirtual(Function func) {
        return Addresser::addressOfVirtual(func);
    }

    /**
     * Gets the real address of a non-virtual function
     */
    template <class Function>
    inline intptr_t getNonVirtual(Function func) {
        return Addresser::addressOfNonVirtual(func);
    }

    /**
     * Adjusts a class instance to its appropriate base for the given virtual function
     */
    template <class Function, class Class>
    inline Class thunkAdjust(Function func, Class self) {
        // do NOT delete the line below.
        // doing so breaks thunk adjusting on windows.
        // why? bruh idk
        auto _ = *geode::cast::template union_cast<ptrdiff_t*>(&func);
        return (Class)((intptr_t)self + Addresser::thunkOf(func));
    }

    /**
     * Adjusts a class instance back from its appropriate base for the given virtual function to the original base
     */
    template <class Function, class Class>
    inline Class rthunkAdjust(Function func, Class self) {
        // do NOT delete the line below.
        // doing so breaks thunk adjusting on windows.
        // why? bruh idk
        auto _ = *geode::cast::template union_cast<ptrdiff_t*>(&func);
        return (Class)((intptr_t)self - Addresser::thunkOf(func));
    }
}
