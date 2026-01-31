#pragma once

#include <type_traits>

namespace geode {
    template <class Derived, class Base>
    class Modify;
}

namespace geode::geode_internal {
    template <class D, class B>
    inline void dummyModifyFn(::geode::Modify<D, B> const&);

    template <class T>
    concept IsModifyClass = requires(std::remove_pointer_t<T> const& a) { ::geode::geode_internal::dummyModifyFn(a); };

    template <class After, class Before>
    inline constexpr void typeinfoCastChecks() {
        static_assert(
            std::is_polymorphic_v<std::remove_pointer_t<Before>> && std::is_polymorphic_v<std::remove_pointer_t<After>>,
            "Input is not a polymorphic type"
        );

        static_assert(!geode_internal::IsModifyClass<After>,
            "typeinfo_cast will not work with a Modify class. use static_cast<MyModifyClass*>(typeinfo_cast<Class*>(...)) instead");
    }

    template<class From, class To>
    struct CopyConst {
        using type = std::conditional_t<
            std::is_const_v<std::remove_pointer_t<From>>,
            std::add_const_t<std::remove_pointer_t<To>>,
            std::remove_pointer_t<To>
        >*;
    };
}