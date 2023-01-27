#pragma once
#include "AsStaticFunction.hpp"
#include "Field.hpp"
#include "IDManager.hpp"

#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <iostream>
#include <tulip/TulipHook.hpp>

#define GEODE_APPLY_MODIFY_FOR_FUNCTION(AddressIndex_, Convention_, ClassName_, FunctionName_, ...) \
    do {                                                                                            \
        if constexpr (Unique::different<                                                            \
                          Resolve<__VA_ARGS__>::func(&Base::FunctionName_),                         \
                          Resolve<__VA_ARGS__>::func(&Derived::FunctionName_)>()) {                 \
            auto hook = Hook::create(                                                               \
                Mod::get(),                                                                         \
                reinterpret_cast<void*>(address<AddressIndex_>()),                                  \
                AsStaticFunction_##FunctionName_<                                                   \
                    Derived,                                                                        \
                    decltype(Resolve<__VA_ARGS__>::func(&Derived::FunctionName_))>::value,          \
                #ClassName_ "::" #FunctionName_,                                                    \
                tulip::hook::TulipConvention::Convention_                                           \
            );                                                                                      \
            this->m_hooks[#ClassName_ "::" #FunctionName_] = hook;                                  \
        }                                                                                           \
    } while (0);

#define GEODE_APPLY_MODIFY_FOR_CONSTRUCTOR(AddressIndex_, Convention_, ClassName_, ...)  \
    do {                                                                                 \
        if constexpr (HasConstructor<Derived>) {                                         \
            auto hook = Hook::create(                                                    \
                Mod::get(),                                                              \
                reinterpret_cast<void*>(address<AddressIndex_>()),                       \
                AsStaticFunction_##constructor<                                          \
                    Derived,                                                             \
                    decltype(Resolve<__VA_ARGS__>::func(&Derived::constructor))>::value, \
                #ClassName_ "::" #ClassName_,                                            \
                tulip::hook::TulipConvention::Convention_                                \
            );                                                                           \
            this->m_hooks[#ClassName_ "::" #ClassName_] = hook;                          \
        }                                                                                \
    } while (0);

#define GEODE_APPLY_MODIFY_FOR_DESTRUCTOR(AddressIndex_, Convention_, ClassName_)                               \
    do {                                                                                                        \
        if constexpr (HasDestructor<Derived>) {                                                                 \
            auto hook = Hook::create(                                                                           \
                Mod::get(),                                                                                     \
                reinterpret_cast<void*>(address<AddressIndex_>()),                                              \
                AsStaticFunction_##destructor<Derived, decltype(Resolve<>::func(&Derived::destructor))>::value, \
                #ClassName_ "::" #ClassName_,                                                                   \
                tulip::hook::TulipConvention::Convention_                                                       \
            );                                                                                                  \
            this->m_hooks[#ClassName_ "::" #ClassName_] = hook;                                                 \
        }                                                                                                       \
    } while (0);

namespace geode::modifier {

    template <class Derived, class Base>
    class ModifyDerive;

    template <class ModifyDerived>
    class ModifyBase {
    public:
        std::map<std::string, Hook*> m_hooks;

        Result<Hook*> getHook(std::string const& name) {
            if (m_hooks.find(name) == m_hooks.end()) {
                return Err("Hook not in this modify");
            }
            return Ok(m_hooks[name]);
        }

        Result<> setHookPriority(std::string const& name, int32_t priority) {
            auto res = this->getHook(name);
            if (!res) {
                return Err(res.unwrapErr());
            }
            res.unwrap()->setPriority(priority);
            return Ok();
        }

        // unordered_map<handles> idea
        ModifyBase() {
            // i really dont want to recompile codegen
            auto test = static_cast<ModifyDerived*>(this);
            test->ModifyDerived::apply();
            ModifyDerived::Derived::onModify(*this);
            for (auto& [uuid, hook] : m_hooks) {
                auto res = Mod::get()->addHook(hook);
                if (!res) {
                    log::error("Failed to add hook {}: {}", hook->getDisplayName(), res.error());
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
            static_assert(alwaysFalse<Derived>, "Custom Modify not implemented.");
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
        // abusing the internal stuff
        // basically we dont want modify to invoke base ctors and dtors
        // we already have utilities for these, which are ccdestructor
        // and the monostate constructor
        Modify() : Base(std::monostate(), sizeof(Base)) {}

        ~Modify() {
            cocos2d::CCDestructor::lock(this) = true;
        }

        Modify(Modify const&) = delete;
        Modify(Modify&&) = delete;
        Modify& operator=(Modify const&) = delete;
        Modify& operator=(Modify&&) = delete;

        modifier::FieldIntermediate<Derived, Base> m_fields;

        static void onModify(auto& self) {}
    };
}
