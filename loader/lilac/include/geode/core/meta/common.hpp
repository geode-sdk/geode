#ifndef LILAC_CORE_META_COMMON_HPP
#define LILAC_CORE_META_COMMON_HPP

#include <type_traits>

namespace lilac::meta {
    template <class Type, class... Compare>
    static constexpr bool any_of = (std::is_same_v<Type, Compare> || ...);

    template <class... Classes>
    static constexpr bool always_false = false;
}

#endif /* LILAC_CORE_META_COMMON_HPP */