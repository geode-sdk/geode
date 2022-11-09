#pragma once
#include "../meta/meta.hpp"
#include "Addresses.hpp"
#include "Field.hpp"
#include "Types.hpp"
#include "Wrapper.hpp"

#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <iostream>

#define GEODE_APPLY_MODIFY_FOR_FUNCTION(                                                                                                                                                                                          \
    addr_index, pure_index, convention, className, functionName                                                                                                                                                                   \
)                                                                                                                                                                                                                                 \
    if constexpr (wrap::functionName<Derived, types::pure##pure_index>::uuid != nullptr && (void*)wrap::functionName<Base, types::pure##pure_index>::uuid != (void*)wrap::functionName<Derived, types::pure##pure_index>::uuid) { \
        (void)Mod::get()                                                                                                                                                                                                          \
            ->addHook<wrap::functionName<Derived, types::pure##pure_index>::value, convention>(                                                                                                                                   \
                #className "::" #functionName, (void*)addresses::address##addr_index()                                                                                                                                            \
            );                                                                                                                                                                                                                    \
    }

namespace geode::modifier {

    template <class Derived, class Base>
    class ModifyDerive;

    template <class Derived>
    class ModifyBase {
    public:
        // unordered_map<handles> idea
        ModifyBase() {
            Loader::get()->scheduleOnModLoad(getMod(), []() {
                Derived::apply();
            });
        }
        template <class, class>
        friend class ModifyDerive;
        // explicit Modify(Property property) idea
    };

    template <class Derived, class Base>
    class ModifyDerive {
    public:
        ModifyDerive() {
            static_assert(core::meta::always_false<Derived>, "Custom Modify not implemented.");
        }
    };
}

namespace geode {

    template <class Derived, class Base>
    class Modify : public Base {
    private:
        static inline modifier::ModifyDerive<Derived, Base> s_apply;
        // because for some reason we need it
        static inline auto s_applyRef = &Modify::s_apply;

    public:
        modifier::FieldIntermediate<Derived, Base> m_fields;
    };
}
