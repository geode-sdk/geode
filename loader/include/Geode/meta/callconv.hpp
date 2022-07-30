#ifndef GEODE_CORE_META_CALLCONV_HPP
#define GEODE_CORE_META_CALLCONV_HPP

#include "tuple.hpp"

#include <array>
#include <utility>

namespace geode::core::meta {
    /* CRTP class for creating calling conventions for Function and Hook.
     * Provides some utilities for less verbose filtering of parameters, and
     * some wrappers to require the custom calling convention to supply an
     * invoker and a way to get a wrapper for hooks.
     */
    template <class Ret, class... Args>
    class CallConv {
    protected:
        using MyTuple = Tuple<Args...>;

        template <auto>
        class arr_to_seq_impl;

        template <class Type, size_t length, const std::array<Type, length>* arr>
        class arr_to_seq_impl<arr> {
        private:
            template <class>
            class getter;

            template <size_t... indices>
            class getter<std::index_sequence<indices...>> {
            public:
                using result = std::index_sequence<arr->at(indices)...>;
            };

        public:
            using result = typename getter<std::make_index_sequence<length>>::result;
        };

        template <auto& arr>
        using arr_to_seq = typename arr_to_seq_impl<&arr>::result;
    };
}

#endif /* GEODE_CORE_META_CALLCONV_HPP */
