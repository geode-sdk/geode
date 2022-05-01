#ifndef GEODE_CORE_META_COMMON_HPP
#define GEODE_CORE_META_COMMON_HPP

#include <type_traits>

namespace geode::core::meta {
    template <class Type, class... Compare>
    static constexpr bool any_of = (std::is_same_v<Type, Compare> || ...);

    template <class... Classes>
    static constexpr bool always_false = false;
}

#endif /* GEODE_CORE_META_COMMON_HPP */