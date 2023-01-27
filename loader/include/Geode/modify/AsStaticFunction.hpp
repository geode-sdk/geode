#pragma once
#include "../utils/addresser.hpp"
#include "Traits.hpp"
#include "../loader/Log.hpp"

namespace geode::modifier {
/**
 * A helper struct that generates a static function that calls the given function.
 */
#define GEODE_AS_STATIC_FUNCTION(FunctionName_)                                                   \
    template <class Class2, class FunctionType>                                                   \
    struct AsStaticFunction_##FunctionName_ {                                                     \
        template <class FunctionType2>                                                            \
        struct Impl {};                                                                           \
        template <class Return, class... Params>                                                  \
        struct Impl<Return (*)(Params...)> {                                                      \
            static Return GEODE_CDECL_CALL function(Params... params) {                           \
                return Class2::FunctionName_(params...);                                          \
            }                                                                                     \
        };                                                                                        \
        template <class Return, class Class, class... Params>                                     \
        struct Impl<Return (Class::*)(Params...)> {                                               \
            static Return GEODE_CDECL_CALL function(Class* self, Params... params) {              \
                auto self2 = addresser::rthunkAdjust(                                             \
                    Resolve<Params...>::func(&Class2::FunctionName_), self                        \
                );                                                                                \
                return self2->Class2::FunctionName_(params...);                                   \
            }                                                                                     \
        };                                                                                        \
        template <class Return, class Class, class... Params>                                     \
        struct Impl<Return (Class::*)(Params...) const> {                                         \
            static Return GEODE_CDECL_CALL function(Class const* self, Params... params) {        \
                auto self2 = addresser::rthunkAdjust(                                             \
                    Resolve<Params...>::func(&Class2::FunctionName_), self                        \
                );                                                                                \
                return self2->Class2::FunctionName_(params...);                                   \
            }                                                                                     \
        };                                                                                        \
        static constexpr auto value = &Impl<FunctionType>::function;                              \
    };

    GEODE_AS_STATIC_FUNCTION(constructor)
    GEODE_AS_STATIC_FUNCTION(destructor)
}
