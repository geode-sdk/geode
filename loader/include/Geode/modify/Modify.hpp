#pragma once
#include "AsStaticFunction.hpp"
#include "Field.hpp"
#include <Geode/Enums.hpp>
#include "IDManager.hpp"

#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <iostream>
#include <tulip/TulipHook.hpp>

#define GEODE_APPLY_MODIFY_FOR_FUNCTION(AddressInline_, Convention_, ClassName_, FunctionName_, ...)          \
    do {                                                                                                      \
        static auto constexpr different = Unique::different<                                                  \
            Resolve<__VA_ARGS__>::func(&Base::FunctionName_),                                                 \
            Resolve<__VA_ARGS__>::func(&Derived::FunctionName_)                                               \
        >();                                                                                                  \
        using BaseFuncType = decltype(Resolve<__VA_ARGS__>::func(&Base::FunctionName_));                      \
        using DerivedFuncType = decltype(Resolve<__VA_ARGS__>::func(&Derived::FunctionName_));                \
        if constexpr (different) {                                                                            \
            static auto address = AddressInline_;                                                             \
            static_assert(                                                                                    \
                !different || !std::is_same_v<typename ReturnType<BaseFuncType>::type, TodoReturn>,           \
                "Function" #ClassName_ "::" #FunctionName_ " has a TodoReturn type, "                         \
                "please fix it by editing the bindings."                                                      \
            );                                                                                                \
            if (address == 0) {                                                                               \
                log::error(                                                                                   \
                    "Address of {} returned nullptr, can't hook", #ClassName_ "::" #FunctionName_             \
                );                                                                                            \
                break;                                                                                        \
            }                                                                                                 \
            auto hook = Hook::create(                                                                         \
                reinterpret_cast<void*>(address),                                                             \
                AsStaticFunction_##FunctionName_<                                                             \
                    Derived,                                                                                  \
                    DerivedFuncType>::value,                                                                  \
                #ClassName_ "::" #FunctionName_,                                                              \
                tulip::hook::TulipConvention::Convention_                                                     \
            );                                                                                                \
            this->m_hooks[#ClassName_ "::" #FunctionName_] = hook;                                            \
        }                                                                                                     \
    } while (0);

#define GEODE_APPLY_MODIFY_FOR_FUNCTION_ERROR(ClassName_, FunctionName_, ...)                                 \
    do {                                                                                                      \
        static_assert(!FunctionExists_##FunctionName_<Derived __VA_ARGS__>,                                   \
            "Function " #ClassName_ "::" #FunctionName_ " does not have an available address in the"          \
            " bindings, please add it to the bindings to hook it."                                            \
        );                                                                                                    \
    } while (0);

#define GEODE_APPLY_MODIFY_FOR_FUNCTION_ERROR_DEFINED(ClassName_, FunctionName_, ...)                         \
    do {                                                                                                      \
        static auto constexpr different = Unique::different<                                                  \
            Resolve<__VA_ARGS__>::func(&Base::FunctionName_),                                                 \
            Resolve<__VA_ARGS__>::func(&Derived::FunctionName_)                                               \
        >();                                                                                                  \
        static_assert(!different,                                                                             \
            "Function " #ClassName_ "::" #FunctionName_ " does not have an available address in the"          \
            " bindings, please add it to the bindings to hook it."                                            \
        );                                                                                                    \
    } while (0);

#define GEODE_APPLY_MODIFY_FOR_FUNCTION_ERROR_INLINE(ClassName_, FunctionName_, ...)                          \
    do {                                                                                                      \
        static auto constexpr different = Unique::different<                                                  \
            Resolve<__VA_ARGS__>::func(&Base::FunctionName_),                                                 \
            Resolve<__VA_ARGS__>::func(&Derived::FunctionName_)                                               \
        >();                                                                                                  \
        static_assert(!different,                                                                             \
            "Function " #ClassName_ "::" #FunctionName_ " cannot be hooked due to an inline definition"       \
            " existing for the function."                                                                     \
        );                                                                                                    \
    } while (0);

#define GEODE_APPLY_MODIFY_FOR_CONSTRUCTOR(AddressInline_, Convention_, ClassName_, ...)  \
    do {                                                                                  \
        if constexpr (HasConstructor<Derived>) {                                          \
            static auto address = AddressInline_;                                         \
            auto hook = Hook::create(                                                     \
                reinterpret_cast<void*>(address),                                         \
                AsStaticFunction_##constructor<                                           \
                    Derived,                                                              \
                    decltype(Resolve<__VA_ARGS__>::func(&Derived::constructor))>::value,  \
                #ClassName_ "::" #ClassName_,                                             \
                tulip::hook::TulipConvention::Convention_                                 \
            );                                                                            \
            this->m_hooks[#ClassName_ "::" #ClassName_] = hook;                           \
        }                                                                                 \
    } while (0);

#define GEODE_APPLY_MODIFY_FOR_DESTRUCTOR(AddressInline_, Convention_, ClassName_)                               \
    do {                                                                                                         \
        if constexpr (HasDestructor<Derived>) {                                                                  \
            static auto address = AddressInline_;                                                                \
            auto hook = Hook::create(                                                                            \
                reinterpret_cast<void*>(address),                                                                \
                AsStaticFunction_##destructor<Derived, decltype(Resolve<>::func(&Derived::destructor))>::value,  \
                #ClassName_ "::" #ClassName_,                                                                    \
                tulip::hook::TulipConvention::Convention_                                                        \
            );                                                                                                   \
            this->m_hooks[#ClassName_ "::" #ClassName_] = hook;                                                  \
        }                                                                                                        \
    } while (0);

namespace geode {
    class Priority {
    public:
        static inline constexpr int32_t First = -3000;
        static inline constexpr int32_t VeryEarly = -2000;
        static inline constexpr int32_t Early = -1000;
        static inline constexpr int32_t Normal = 0;
        static inline constexpr int32_t Late = 1000;
        static inline constexpr int32_t VeryLate = 2000;
        static inline constexpr int32_t Last = 3000;

        static inline constexpr int32_t FirstPre = First;
        static inline constexpr int32_t VeryEarlyPre = VeryEarly;
        static inline constexpr int32_t EarlyPre = Early;
        static inline constexpr int32_t NormalPre = Normal;
        static inline constexpr int32_t LatePre = Late;
        static inline constexpr int32_t VeryLatePre = VeryLate;
        static inline constexpr int32_t LastPre = Last;

        static inline constexpr int32_t FirstPost = Last;
        static inline constexpr int32_t VeryEarlyPost = VeryLate;
        static inline constexpr int32_t EarlyPost = Late;
        static inline constexpr int32_t NormalPost = Normal;
        static inline constexpr int32_t LatePost = Early;
        static inline constexpr int32_t VeryLatePost = VeryEarly;
        static inline constexpr int32_t LastPost = First;
    };
}

namespace geode::modifier {
    template <class Derived, class Base>
    class ModifyDerive;

    template <class ModifyDerived>
    class ModifyBase {
    public:
        std::map<std::string, std::shared_ptr<Hook>> m_hooks;

        /// @brief Get a hook by name
        /// @param name The name of the hook to get
        /// @returns Ok if the hook was found, Err if the hook was not found
        Result<Hook*> getHook(std::string const& name) {
            if (m_hooks.find(name) == m_hooks.end()) {
                return Err("Hook not in this modify");
            }
            return Ok(m_hooks[name].get());
        }

        /// @brief Set the priority of a hook
        /// @param name The name of the hook to set the priority of
        /// @param priority The priority to set the hook to
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found
        Result<> setHookPriority(std::string const& name, int32_t priority = Priority::Normal) {
            auto res = this->getHook(name);
            if (!res) {
                return Err(std::move(res).unwrapErr());
            }
            res.unwrap()->setPriority(priority);
            return Ok();
        }

        /// @brief Set the priority of a hook
        /// @param name The name of the hook to set the priority of
        /// @param priority The priority to set the hook to
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found
        Result<> setHookPriorityPre(std::string const& name, int32_t priority = Priority::Normal) {
            return this->setHookPriority(name, priority);
        }

        /// @brief Set the priority of a hook
        /// @param name The name of the hook to set the priority of
        /// @param priority The priority to set the hook to
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found
        Result<> setHookPriorityPost(std::string const& name, int32_t priority = Priority::Normal) {
            return this->setHookPriority(name, -priority);
        }

        /// @brief Set the priority of a hook to be after another hook in different mods
        /// @param name The name of the hook to set the priority of
        /// @param after The mod ids of the mods to set the priority after
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found
        template<class... C> requires (std::is_convertible_v<C, std::string> && ...)
        Result<> setHookPriorityAfter(std::string const& name, C&&... after) {
            auto res = this->getHook(name);
            if (!res) {
                return Err(std::move(res).unwrapErr());
            }
            auto hook = std::move(res).unwrap();
            ([&](){
                auto mod = Loader::get()->getInstalledMod(after);
                if (!mod) return;
                auto hooks = mod->getHooks();
                auto func = [=](){
                    for (auto modHook : hooks) {
                        if (modHook->getAddress() != hook->getAddress()) continue;
                        auto priority = hook->getPriority();
                        if (priority < mod->getPriority()) {
                            hook->setPriority(mod->getPriority() + 1);
                        }
                    }
                };
                if (Loader::get()->isModLoaded(mod)) {
                    func();
                }
                else {
                    new EventListener(func, ModStateFilter(mod, ModEventType::Loaded));
                }
            } (), ...);
            return Ok();
        }

        /// @brief Set the priority of a hook to be before another hook in different mods
        /// @param name The name of the hook to set the priority of
        /// @param before The mod ids of the mods to set the priority before
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found
        template<class... C> requires (std::is_convertible_v<C, std::string> && ...)
        Result<> setHookPriorityBefore(std::string const& name, C&&... before) {
            auto res = this->getHook(name);
            if (!res) {
                return Err(std::move(res).unwrapErr());
            }
            auto hook = std::move(res).unwrap();
            ([&](){
                auto mod = Loader::get()->getInstalledMod(before);
                if (!mod) return;
                auto hooks = mod->getHooks();
                auto func = [=](){
                    for (auto modHook : hooks) {
                        if (modHook->getAddress() != hook->getAddress()) continue;
                        auto priority = hook->getPriority();
                        if (priority > mod->getPriority()) {
                            hook->setPriority(mod->getPriority() - 1);
                        }
                    }
                };
                if (Loader::get()->isModLoaded(mod)) {
                    func();
                }
                else {
                    new EventListener(func, ModStateFilter(mod, ModEventType::Loaded));
                }
            } (), ...);
            return Ok();
        }

        /// @brief Set the priority of a hook to be after another hook in different mods
        /// @param name The name of the hook to set the priority of
        /// @param after The mod ids of the mods to set the priority after
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found
        template<class... C> requires (std::is_convertible_v<C, std::string> && ...)
        Result<> setHookPriorityAfterPre(std::string const& name, C&&... after) {
            return this->setHookPriorityAfter(name, Priority::NormalPre, std::forward<C>(after)...);
        }

        /// @brief Set the priority of a hook to be before another hook in different mods
        /// @param name The name of the hook to set the priority of
        /// @param before The mod ids of the mods to set the priority before
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found
        template<class... C> requires (std::is_convertible_v<C, std::string> && ...)
        Result<> setHookPriorityBeforePre(std::string const& name, C&&... before) {
            return this->setHookPriorityBefore(name, Priority::NormalPre, std::forward<C>(before)...);
        }

        /// @brief Set the priority of a hook to be after another hook in different mods
        /// @param name The name of the hook to set the priority of
        /// @param after The mod ids of the mods to set the priority after
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found
        template<class... C> requires (std::is_convertible_v<C, std::string> && ...)
        Result<> setHookPriorityAfterPost(std::string const& name, C&&... after) {
            return this->setHookPriorityBefore(name, Priority::NormalPost, std::forward<C>(after)...);
        }

        /// @brief Set the priority of a hook to be before another hook in different mods
        /// @param name The name of the hook to set the priority of
        /// @param before The mod ids of the mods to set the priority before
        /// @returns Ok if the hook was found and the priority was set, Err if the hook was not found
        template<class... C> requires (std::is_convertible_v<C, std::string> && ...)
        Result<> setHookPriorityBeforePost(std::string const& name, C&&... before) {
            return this->setHookPriorityAfter(name, Priority::NormalPost, std::forward<C>(before)...);
        }

        // unordered_map<handles> idea
        ModifyBase() {
            struct EboCheck : ModifyDerived::Base {
                std::aligned_storage_t<
                    std::alignment_of_v<typename ModifyDerived::Base>, 
                    std::alignment_of_v<typename ModifyDerived::Base>
                > m_padding;
            };
            static constexpr auto baseSize = sizeof(typename ModifyDerived::Base);
            static constexpr auto derivedSize = sizeof(typename ModifyDerived::Derived);
            static constexpr auto alignment = std::alignment_of_v<typename ModifyDerived::Base>;
            static constexpr bool hasEbo = sizeof(EboCheck) == sizeof(typename ModifyDerived::Base);
            static constexpr bool hasImproperCustomFields = hasEbo ? derivedSize != baseSize : derivedSize != baseSize + alignment;
            static_assert(!hasImproperCustomFields,
                "\n--- Error in modify class:\n"
                "  Do not add members to a modify class, use `struct Fields` instead.\n"
                "  See https://docs.geode-sdk.org/tutorials/fields for more info."
                "\n---"
            );

            // i really dont want to recompile codegen
            auto test = static_cast<ModifyDerived*>(this);
            test->ModifyDerived::apply();
            ModifyDerived::Derived::onModify(*this);
            std::vector<std::string> added;
            for (auto& [uuid, hook] : m_hooks) {
                auto res = Mod::get()->claimHook(hook);
                if (!res) {
                    log::error("Failed to claim hook {}: {}", hook->getDisplayName(), res.unwrapErr());
                }
                else {
                    added.push_back(uuid);
                }
            }
            for (auto& uuid : added) {
                m_hooks.erase(uuid);
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
            static_assert(
                alwaysFalse<Derived>,
                "Modified class not recognized, please include <Geode/modify/ClassName.hpp> to be "
                "able to use it."
            );
        }
    };
}

namespace geode {

// The intellisense compiler is quite dumb, and will very often error on modify classes
// with an error of "incomplete type is not allowed", despite not being an issue in actual compilation.
// So as a workaround use the compiler defined "__INTELLISENSE__" macro, which gets set to 1 on the intellisense pass.
// See https://learn.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-170#microsoft-specific-predefined-macros
#if __INTELLISENSE__ != 1 && !defined(__CLION_IDE__)

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
        // and the cutoff constructor
        Modify() : Base(CutoffConstructor, sizeof(Base)) {}

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

#else

    template <class Derived, class Base>
    class Modify : public Base {
    public:
        modifier::FieldIntermediate<Derived, Base> m_fields;
    };

#endif
}

/**
 * Main class implementation, it has the structure
 *
 * class hook0Dummy;
 * template<typename>
 * struct hook0 {};
 * namespace {
 *     struct hook0Parent {};
 * }
 * template<>
 * struct GEODE_HIDDEN hook0<hook0Parent> : Modify<hook0<hook0Parent>, MenuLayer> {
 *     // code stuff idk
 * };
 *
 * I tried to make the macro as verbose as it can be but
 * I am bad at this stuff
 */

#if __INTELLISENSE__ != 1 && !defined(__CLION_IDE__)

#define GEODE_MODIFY_DECLARE_ANONYMOUS(base, derived) \
    derived##Dummy;                                   \
    template <class>                                  \
    struct derived {};                                \
    namespace {                                       \
        struct derived##Parent {};                    \
    }                                                 \
    template <>                                       \
    struct GEODE_HIDDEN derived<derived##Parent> : geode::Modify<derived<derived##Parent>, base>

#define GEODE_MODIFY_DECLARE(base, derived) \
    derived##Dummy;                         \
    struct GEODE_HIDDEN derived : geode::Modify<derived, base>

#else

// Simplify the modify macro for intellisense, to hopefully help perfomance a bit

#define GEODE_MODIFY_DECLARE(base, derived) \
    derived##Dummy; \
    struct derived : geode::Modify<derived, base>

#define GEODE_MODIFY_DECLARE_ANONYMOUS(base, derived) GEODE_MODIFY_DECLARE(base, derived)

#endif

#define GEODE_MODIFY_REDIRECT4(base, derived) GEODE_MODIFY_DECLARE(base, derived)
#define GEODE_MODIFY_REDIRECT3(base, derived) GEODE_MODIFY_DECLARE_ANONYMOUS(base, derived)
#define GEODE_MODIFY_REDIRECT2(base) GEODE_MODIFY_REDIRECT3(base, GEODE_CONCAT(hook, __LINE__))
#define GEODE_MODIFY_REDIRECT1(base) GEODE_MODIFY_REDIRECT2(base)

/**
 * Interfaces for the class implementation
 *
 * class $modify(MenuLayer) {};
 * class $modify(MyMenuLayerInterface, MenuLayer) {};
 */

#define GEODE_CRTP1(base) GEODE_MODIFY_REDIRECT1(base)
#define GEODE_CRTP2(derived, base) GEODE_MODIFY_REDIRECT4(base, derived)
#define $modify(...) \
    GEODE_INVOKE(GEODE_CONCAT(GEODE_CRTP, GEODE_NUMBER_OF_ARGS(__VA_ARGS__)), __VA_ARGS__)

/** 
 * This function is meant to hook / override a GD function in a Modified class. 
 * **This is merely an annotation for clarity** - while there may be linters that 
 * check for it, it is not required
 */
#define $override
