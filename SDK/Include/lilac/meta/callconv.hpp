#ifndef LILAC_CORE_META_CALLCONV_HPP
#define LILAC_CORE_META_CALLCONV_HPP

namespace lilac::core::meta {
    /* CRTP class for creating calling conventions for Function and Hook.
    * Provides some utilities for less verbose filtering of parameters, and
    * some wrappers to require the custom calling convention to supply an
    * invoker and a way to get a wrapper for hooks.
    */
    template<template<class, class...> class Inherit, class Ret, class... Args>
    class CallConv {
    private:
        using MyInherit = Inherit<Ret, Args...>;

    protected:
        using MyTuple = Tuple<Args...>;

        template<
            size_t i,
            template<class> class Pred,
            class Else
        >
        using type_if = 
            typename ternary<
                (MyTuple::size > i) &&
                Pred<typename MyTuple::template type_at<i>>::value
                >::template type<
                        typename MyTuple::template type_at<i>,
                        Else
                    >;

        template<
            size_t i,
            template<class> class Pred,
            class Else
        >
        static decltype(auto) value_if(const MyTuple& tuple, const Else e) {
            return ternary<
                    (MyTuple::size > i) &&
                    Pred<typename MyTuple::template type_at<i>>::value
                >::val(tuple.template at<i>(), e);
        }

    /* TODO: REMOVE THIS!!
    public:
        template<size_t... indices>
        static Ret invoke(
            Ret(* address)(Args...), 
            const MyTuple&& tuple,
            const std::index_sequence<indices...>&&
        ) {
            return MyInherit::invoke<indices...>(address, tuple);
        }

        template<Ret(* detour)(Args...), size_t... indices>
        static decltype(auto) get_wrapper(
            const std::index_sequence<indices...>&&
        ) {
            return MyInherit::get_wrapper<detour, indices...>();
        }*/
    };
}

#endif /* LILAC_CORE_META_CALLCONV_HPP */