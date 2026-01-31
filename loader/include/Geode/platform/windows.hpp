#pragma once

#pragma warning(default : 4067)
#pragma warning(disable : 4251) // dll-interface
#pragma warning(disable : 4244) // narrowing conversion

#include <Windows.h>
#include <cstring>
#include <type_traits>
#include <typeinfo>
#include <memory>
#include <intrin.h>  // for _ReadWriteBarrier
#include "_casts_shared.hpp"

namespace geode {
    struct PlatformInfo {
        HMODULE m_hmod;
    };

    namespace internal {
        inline void const volatile* volatile globalForceEscape;

        inline void useCharPointer(char const volatile* const ptr) {
            globalForceEscape = reinterpret_cast<void const volatile*>(ptr);
        }
    }

    template <class T>
    GEODE_INLINE inline void doNotOptimize(T const& value) {
        internal::useCharPointer(&reinterpret_cast<char const volatile&>(value));
        _ReadWriteBarrier();
    }

    template <class T>
    GEODE_INLINE inline void doNotOptimize(T& value) {
        internal::useCharPointer(&reinterpret_cast<char const volatile&>(value));
        _ReadWriteBarrier();
    }

    template <class T>
    GEODE_INLINE inline void doNotOptimize(T&& value) {
        internal::useCharPointer(&reinterpret_cast<char const volatile&>(value));
        _ReadWriteBarrier();
    }
}

namespace geode::base {
    GEODE_NOINLINE inline uintptr_t get() {
        static uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));
        return base;
    }

    GEODE_NOINLINE inline uintptr_t getCocos() {
        static uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandleW(L"libcocos2d.dll"));
        return base;
    }

    GEODE_NOINLINE inline uintptr_t getExtensions() {
        static uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandleW(L"libExtensions.dll"));
        return base;
    }
}


namespace geode::cast {
    template <class Type>
    struct ShrunkPointer {
        uint32_t m_ptrOffset;

        Type* into(uintptr_t base) {
            return reinterpret_cast<Type*>(base + m_ptrOffset);
        }
    };

    struct TypeDescriptorType {
        void* m_typeinfoTable;
        intptr_t m_spare;
        char m_typeDescriptorName[0x100];
    };

    struct ClassDescriptorType;

    struct BaseClassDescriptorType {
        ShrunkPointer<TypeDescriptorType> m_typeDescriptor;
        int32_t m_numContainedBases;
        int32_t m_memberDisplacement[3];
        int32_t m_attributes;
        ShrunkPointer<ClassDescriptorType> m_classDescriptor;
    };

    struct BaseClassArrayType {
        ShrunkPointer<BaseClassDescriptorType> m_descriptorEntries[0x100];
    };

    struct ClassDescriptorType {
        int32_t m_signature;
        int32_t m_attributes;
        int32_t m_numBaseClasses;
        ShrunkPointer<BaseClassArrayType> m_baseClassArray;
    };

    struct CompleteLocatorType {
        int32_t m_signature;
        int32_t m_offset;
        int32_t m_cdOffset;
        ShrunkPointer<TypeDescriptorType> m_typeDescriptor;
        ShrunkPointer<ClassDescriptorType> m_classDescriptor;
    #ifdef GEODE_IS_X64
        int32_t m_locatorOffset;
    #endif
    };

    struct MetaPointerType {
        CompleteLocatorType* m_completeLocator;
    };

    struct VftableType {
        void* m_vftable[0x100];
    };

    struct CompleteVftableType : MetaPointerType, VftableType {};

    template <class After, class Before>
    inline After typeinfo_cast(Before ptr) {
        ::geode::geode_internal::typeinfoCastChecks<After, Before>();

        if (!ptr) {
            return After();
        }

        auto vftable = *reinterpret_cast<VftableType const* const*>(ptr);

        auto metaPtr = static_cast<MetaPointerType const*>(static_cast<CompleteVftableType const*>(vftable));

        auto afterDesc =
            reinterpret_cast<TypeDescriptorType const*>(&typeid(std::remove_pointer_t<After>));

        auto afterIdent = static_cast<char const*>(afterDesc->m_typeDescriptorName);

    #ifdef GEODE_IS_X64
        auto locatorOffset = metaPtr->m_completeLocator->m_locatorOffset;
        auto base = reinterpret_cast<uintptr_t>(metaPtr->m_completeLocator) - locatorOffset;
    #else
        auto base = 0;
    #endif

        auto classDesc = metaPtr->m_completeLocator->m_classDescriptor.into(base);
        for (int32_t i = 0; i < classDesc->m_numBaseClasses; ++i) {
            auto entry = classDesc->m_baseClassArray.into(base)->m_descriptorEntries[i].into(base);
            auto optionIdent = static_cast<char const*>(
                entry->m_typeDescriptor.into(base)->m_typeDescriptorName
            );
            auto optionOffset = entry->m_memberDisplacement[0];

            if (std::strcmp(afterIdent, optionIdent) == 0) {
                auto afterPtr = (uintptr_t)ptr + optionOffset - metaPtr->m_completeLocator->m_offset;
                return reinterpret_cast<After>(afterPtr);
            }
        }

        return nullptr;
    }

    inline char const* getRuntimeTypeName(void const* ptr) {
        if (!ptr) {
            return "<null>";
        }

        auto vftable = *reinterpret_cast<VftableType const* const*>(ptr);

        auto metaPtr = static_cast<MetaPointerType const*>(static_cast<CompleteVftableType const*>(vftable));

    #ifdef GEODE_IS_X64
        auto locatorOffset = metaPtr->m_completeLocator->m_locatorOffset;
        auto base = reinterpret_cast<uintptr_t>(metaPtr->m_completeLocator) - locatorOffset;
    #else
        auto base = 0;
    #endif

        auto typeDesc = metaPtr->m_completeLocator->m_typeDescriptor.into(base);

        return typeDesc->m_typeDescriptorName;
    }

    inline char const* getRuntimeTypeName(std::type_info const& info) {
        auto typeDesc = reinterpret_cast<TypeDescriptorType const*>(&info);
        return typeDesc->m_typeDescriptorName;
    }
}
