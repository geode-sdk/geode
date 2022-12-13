#pragma once
#include "../meta/meta.hpp"
#include "Addresses.hpp"
#include "Field.hpp"
#include "IDManager.hpp"
#include "Types.hpp"
#include "Wrapper.hpp"

#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <iostream>
#include <tulip/TulipHook.hpp>

#define GEODE_APPLY_MODIFY_FOR_FUNCTION(addr_index, pure_index, convention, className, functionName)       \
    {                                                                                                      \
        using DerivedWrap = wrap::functionName<Derived, types::pure##pure_index>;                          \
        using BaseWrap = wrap::functionName<Base, types::pure##pure_index>;                                \
        if constexpr (DerivedWrap::uuid != nullptr && (void*)BaseWrap::uuid != (void*)DerivedWrap::uuid) { \
            auto hook = Hook::create<convention>(                                                          \
                Mod::get(),                                                                                \
                reinterpret_cast<void*>(addresses::address##addr_index()),                                 \
                DerivedWrap::value,                                                                        \
                #className "::" #functionName                                                              \
            );                                                                                             \
            BaseModify::m_hooks[FunctionUUID<DerivedWrap::value>::value] = hook;                           \
        }                                                                                                  \
    }

namespace geode::modifier {

    template <class Derived, class Base>
    class ModifyDerive;

    template <class ModifyDerived>
    class ModifyBase {
    public:
        std::map<void (*)(), Hook*> m_hooks;

        template <auto Function>
        Result<Hook*> getHook() {
            auto uuid = FunctionUUID<Function>::value;
            if (m_hooks.find(uuid) == m_hooks.end()) {
                return Err("Hook not in this modify");
            }
            return m_hooks[uuid];
        }

        // unordered_map<handles> idea
        ModifyBase() {
            this->apply();
            ModifyDerived::Derived::onModify(*this);
            for (auto& [uuid, hook] : m_hooks) {
                auto res = Mod::get()->addHook(hook);
                if (!res) {
                    log::error("Failed to add hook: {}", res.error());
                }
            }
        }

        virtual void apply() {}
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

        static void onModify(auto& self) {}
    };
}
