#pragma once

#include "addresser.hpp"
#include <memory>
#include <optional>
#include "../modify/Traits.hpp"

namespace geode {
    class GEODE_DLL VMTHookManager {
    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
        VMTHookManager();
        ~VMTHookManager();

    #if defined(GEODE_IS_WINDOWS32) || defined(GEODE_IS_WINDOWS64) || defined(GEODE_IS_INTEL_MAC)
        template <auto UUID>
        static void emptyFunction() {}
    #elif defined(GEODE_IS_ARM_MAC) || defined(GEODE_IS_ANDROID64) || defined(GEODE_IS_IOS)
        template <auto UUID>
        static void __attribute__((naked)) emptyFunction() {
            __asm__ volatile(
                ".rept 8\n\t"    // AArch64 NOP is 4 bytes → 8 × 4 = 32
                "nop\n\t"
                ".endr\n\t"
                "ret\n\t"
            );
        }
    #elif defined(GEODE_IS_ANDROID32)
        template <auto UUID>
        static void __attribute__((naked)) emptyFunction() {
            __asm__ volatile(
            #if defined(__thumb__)
                // Thumb NOP is 16-bit → 16 × 2 = 32 bytes
                ".syntax unified\n\t"
                ".rept 16\n\t"
                "nop\n\t"
                ".endr\n\t"
                "bx lr\n\t"
            #else
                // ARM NOP is 32-bit → 8 × 4 = 32 bytes
                ".syntax unified\n\t"
                ".rept 8\n\t"
                "nop\n\t"
                ".endr\n\t"
                "bx lr\n\t"
            #endif
            );
        }
    #endif

        template <class Function>
        struct MeasureTableSize {};

        template <class Return, class Class, class... Params>
        struct MeasureTableSize<Return(Class::*)(Params...)> : Class {
        public:
            virtual void measureTableSize() {
                // This function is used to measure the size of the vtable
            }
        };

        template <class Return, class Class, class... Params>
        struct MeasureTableSize<Return(Class::*)(Params...) const> : Class {
        public:
            virtual void measureTableSize() {
                // This function is used to measure the size of the vtable
            }
        };

        Result<std::optional<std::shared_ptr<Hook>>> addHookInternal(
            void* instance, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset, size_t vtableSize,
            void* emptyFunc, void* newFunc, std::string const& typeName, std::string const& displayName,
            tulip::hook::HandlerMetadata const& handlerMetadata,
            tulip::hook::HookMetadata const& hookMetadata
        );

        Result<> forceDisableFunctionInternal(void* instance, std::string const& typeName, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset);
        Result<> forceEnableFunctionInternal(void* instance, std::string const& typeName, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset);

    public:
        static VMTHookManager& get();

        /**
         * Hooks a virtual function in a table that does not exist for the class.
         * This is useful for adding hooks to functions that are not part of the original class definition.
         * This should be called on every instance that needs to be hooked.
         *
         * @example
         * ```cpp
         *   auto mainHook = VMTHookManager::get().addHook<
         *       ResolveC<GJGarageLayerTest2>::func(&GJGarageLayerTest2::ccTouchBegan)
         *   >(this, "GJGarageLayer::ccTouchBegan");
         *   // This should also be called for delegate hooks, with ResolveC pointing to the Delegate
         *   // In Windows, this will be a no-op, but it will hook the other function in other platforms
         *   auto delegateHook = VMTHookManager::get().addHook<
         *       ResolveC<CCTouchDelegate>::func(&GJGarageLayerTest2::ccTouchBegan)
         *   >(this, "GJGarageLayer::ccTouchBegan");
         *
         *
         * bool ccTouchBegan(CCTouch* touch, CCEvent* event) override {
         *      // Yes, you need to call the original by seemingly recursing.
         *      // Otherwise you would just call the base class function, and
         *      // that wouldn't call other hooks.
         *      this->ccTouchBegan(touch, event);
         *      return true;
         *  }
         * ```
         *
         * @tparam Function The detour function.
         * @param instance The instance to hook.
         * @param displayName The display name for the hook.
         * @param convention The calling convention for the hook.
         * @param hookMetadata Additional metadata for the hook.
         *
         * @return A result containing the hook, if successful. Will return a nullopt if the hook has already been created.
         */
        template<auto Function, class Class>
        Result<std::optional<Hook*>> addHook(
            Class* instance, std::string const& displayName = "",
            tulip::hook::TulipConvention convention = tulip::hook::TulipConvention::Default,
            tulip::hook::HookMetadata const& hookMetadata = tulip::hook::HookMetadata{}
        ) {
            if constexpr (Function != nullptr) {
                auto const thunkOffset = addresser::getThunkOffset(Function);
                auto const vtableOffset = addresser::getVirtualOffset(Function);
                auto const newFunc = (void*)addresser::getVirtual(Function);
                auto const vtableSize = addresser::getVirtualOffset(
                    &MeasureTableSize<decltype(Function)>::measureTableSize
                );
                auto const handlerMetadata = tulip::hook::HandlerMetadata{
                    .m_convention = geode::hook::createConvention(convention),
                    .m_abstract = tulip::hook::AbstractFunction::from(
                        static_cast<modifier::AsStaticType<decltype(Function)>::type>(nullptr)
                    )
                };
                auto const emptyFunc = (void*)&emptyFunction<Function>;
                std::string typeName = typeid(Class).name();
                GEODE_UNWRAP_INTO(auto hook, this->addHookInternal(
                    instance, thunkOffset, vtableOffset, vtableSize, emptyFunc,
                    newFunc, typeName, displayName, handlerMetadata, hookMetadata
                ));
                if (hook.has_value()) {
                    GEODE_UNWRAP_INTO(auto hook, Mod::get()->claimHook(*hook));
                    return Ok(hook);
                }
            }
            return Ok(std::nullopt);
        }

        /**
         * Force enable a function by its address.
         * This will enable back the disabled function.
         */
        template<auto Function, class Class>
        Result<> forceEnableFunction(Class* instance) {
            auto const thunkOffset = addresser::getThunkOffset(Function);
            auto const vtableOffset = addresser::getVirtualOffset(Function);
            std::string typeName = typeid(Class).name();
            return this->forceEnableFunctionInternal(instance, typeName, thunkOffset, vtableOffset);
        }

        /**
         * Force disable a function by its address.
         * This will disable the function, and all of its hooks.
         * You need to call forceEnableFunction to enable back those hooks.
         */
        template<auto Function, class Class>
        Result<> forceDisableFunction(Class* instance) {
            auto const thunkOffset = addresser::getThunkOffset(Function);
            auto const vtableOffset = addresser::getVirtualOffset(Function);
            std::string typeName = typeid(Class).name();
            return this->forceDisableFunctionInternal(instance, typeName, thunkOffset, vtableOffset);
        }
    };
}