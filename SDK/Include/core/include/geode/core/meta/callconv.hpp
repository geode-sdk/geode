#ifndef GEODE_CORE_META_CALLCONV_HPP
#define GEODE_CORE_META_CALLCONV_HPP

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

        template <
            size_t i,
            template <class> class Pred,
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

        template <
            size_t i,
            template <class> class Pred,
            class Else
        >
        static decltype(auto) value_if(const MyTuple& tuple, const Else e) {
            return ternary<
                    (MyTuple::size > i) &&
                    Pred<typename MyTuple::template type_at<i>>::value
                >::val(tuple.template at<i>(), e);
        }

        // Why is this here. Stop.
        template <template <class, class...> class Convention, auto Func> 
        struct ConventionFrom {
            using type = Convention<void>;
        };

        template <template <class, class...> class Convention, class Ret, class ...Args, Ret(* Func)(Args...)> 
        struct ConventionFrom<Convention, Func> {
            using type = Convention<Ret, Args...>;
        };
    };
}

#endif /* GEODE_CORE_META_CALLCONV_HPP */