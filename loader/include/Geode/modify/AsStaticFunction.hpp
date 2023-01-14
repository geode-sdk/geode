#pragma once
#include "../utils/addresser.hpp"
#include "Traits.hpp"

namespace geode::modifier {
/**
 * A helper struct that generates a static function that calls the given function.
 */
#define GEODE_AS_STATIC_FUNCTION(FunctionName_)                                                   \
    template <class Class2, auto Function>                                                        \
    struct AsStaticFunction_##FunctionName_ {                                                     \
        template <class FunctionType2>                                                            \
        struct Impl {};                                                                           \
        template <class Return, class... Params>                                                  \
        struct Impl<Return (*)(Params...)> {                                                      \
            static Return function(Params... params) { return Class2::FunctionName_(params...); } \
        };                                                                                        \
        template <class Return, class Class, class... Params>                                     \
        struct Impl<Return (Class::*)(Params...)> {                                               \
            static Return function(Class* self, Params... params) {                               \
                auto self2 = addresser::rthunkAdjust(Function, self);                             \
                return self2->Class2::FunctionName_(params...);                                   \
            }                                                                                     \
        };                                                                                        \
        template <class Return, class Class, class... Params>                                     \
        struct Impl<Return (Class::*)(Params...) const> {                                         \
            static Return function(Class const* self, Params... params) {                         \
                auto self2 = addresser::rthunkAdjust(Function, self);                             \
                return self2->Class2::FunctionName_(params...);                                   \
            }                                                                                     \
        };                                                                                        \
        static constexpr auto value = &Impl<decltype(Function)>::function;                        \
    };

    GEODE_AS_STATIC_FUNCTION(constructor)
    GEODE_AS_STATIC_FUNCTION(destructor)
}
