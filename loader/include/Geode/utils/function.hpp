#pragma once

#include <tuple>

namespace geode::utils::function {
    namespace detail {
        template <class F>
        struct ImplExtract;

        template <class R, class... A>
        struct ImplExtract<R(A...)> {
            using Type = R(A...);
            using Return = R;
            using Args = std::tuple<A...>;
            static constexpr std::size_t ARG_COUNT = std::tuple_size_v<Args>;
        };
        template <class R, class... A>
        struct ImplExtract<R(*)(A...)> {
            using Type = R(A...);
            using Return = R;
            using Args = std::tuple<A...>;
            static constexpr std::size_t ARG_COUNT = std::tuple_size_v<Args>;
        };
        template <class R, class C, class... A>
        struct ImplExtract<R(C::*)(A...)> {
            using Type = R(A...);
            using Class = C;
            using Return = R;
            using Args = std::tuple<A...>;
            static constexpr std::size_t ARG_COUNT = std::tuple_size_v<Args>;
        };
        template <class R, class C, class... A>
        struct ImplExtract<R(C::*)(A...) const> {
            using Type = R(A...);
            using Class = C;
            using Return = R;
            using Args = std::tuple<A...>;
            static constexpr std::size_t ARG_COUNT = std::tuple_size_v<Args>;
        };
        template <class F>
            requires requires { &F::operator(); }
        struct ImplExtract<F> : public ImplExtract<decltype(&F::operator())> {};

        template <class F>
        using Extract = ImplExtract<std::remove_cvref_t<F>>;
    }

    template <class F>
    using FunctionInfo = detail::Extract<F>;

    template <class F>
    using Return = typename detail::Extract<F>::Return;

    template <class F>
    using Args = typename detail::Extract<F>::Args;

    template <std::size_t Ix, class F>
    using Arg = std::tuple_element_t<Ix, typename detail::Extract<F>::Args>;
}
