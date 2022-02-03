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

        template <bool, size_t i>
        struct type_at_wrap_impl {
            using result = void;
        };

        template <size_t i>
        struct type_at_wrap_impl<true, i> {
            using result = typename MyTuple::template type_at<i>;
        };

        template <size_t i>
        using type_at_wrap = typename type_at_wrap_impl<i < MyTuple::size, i>::result;

        template <size_t i>
        static constexpr decltype(auto) at_wrap(const MyTuple& tuple) {
            if constexpr (i < MyTuple::size) {
                return tuple.template at<i>();
            }
            else {
                return 0;
            }
        }

        template <
            size_t i,
            template <class> class Pred,
            class Else
        >
        using type_if = 
            typename ternary<
                    (MyTuple::size > i) &&
                    Pred<type_at_wrap<i>>::value
                >::template type<
                        type_at_wrap<i>,
                        Else
                    >;

        template <
            size_t i,
            template <class> class Pred,
            class Else
        >
        static constexpr decltype(auto) value_if(const MyTuple& tuple, const Else e) {
            return ternary<
                    (MyTuple::size > i) &&
                    Pred<type_at_wrap<i>>::value
                >::val(at_wrap<i>(tuple), e);
        }
    };
}

#endif /* GEODE_CORE_META_CALLCONV_HPP */