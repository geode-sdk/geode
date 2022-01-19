#ifndef LILAC_CORE_META_COMMON_HPP
#define LILAC_CORE_META_COMMON_HPP

#include <type_traits>

namespace lilac::core::meta {
    template<bool cond>
    class ternary {
    public:
        template<class T, class F>
        static constexpr decltype(auto) val(
            const T t,
            const F f
        ) { return f; }

        template<class T, class F>
        using type = F;
    };

    template<>
    class ternary<true> {
    public:
        template<class T, class F>
        static constexpr decltype(auto) val(
            const T t,
            const F f
        ) { return t; }

        template<class T, class F>
        using type = T;
    };

    template<class Class, class... Compare>
    static constexpr bool any_of = std::disjunction_v<std::is_same<Class, Compare>...>;

    template<class... Classes>
    static constexpr bool always_false = false;
}

#endif /* LILAC_CORE_META_COMMON_HPP */