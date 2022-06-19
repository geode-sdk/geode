#pragma once

#pragma warning(default: 4067)
#pragma warning(disable: 4251) // dll-interface
#pragma warning(disable: 4244) // narrowing conversion

#include <Windows.h>
#include <meta/cdecl.hpp>
#include <meta/function.hpp>
#include <meta/optcall.hpp>
#include <meta/membercall.hpp>
#include <meta/thiscall.hpp>

#include <type_traits>
#include <typeinfo>
#include <cstring>

namespace geode {
    struct PlatformInfo {
        HMODULE m_hmod;
    };
}

namespace geode::base {
	GEODE_NOINLINE inline uintptr_t get() {
		static uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));
		return base;
	}
}


namespace geode::cast {
	struct TypeDescriptorType {
	    void* m_typeinfoTable;
	    int32_t m_spare;
	    char m_typeDescriptorName[0x100];
	};

	struct ClassDescriptorType;

	struct BaseClassDescriptorType {
	    TypeDescriptorType* m_typeDescriptor;
	    int32_t m_numContainedBases;
	    int32_t m_memberDisplacement[3];
	    int32_t m_attributes;
	    ClassDescriptorType* m_classDescriptor;
	};

	struct ClassDescriptorType {
	    int32_t m_signature;
	    int32_t m_attributes;
	    int32_t m_numBaseClasses;
	    BaseClassDescriptorType** m_baseClassArray;
	};

	struct CompleteLocatorType {
	    int32_t m_signature;
	    int32_t m_offset;
	    int32_t m_cdOffset;
	    TypeDescriptorType* m_typeDescriptor;
	    ClassDescriptorType* m_classDescriptor;
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
	    static_assert(
	        std::is_polymorphic_v<std::remove_pointer_t<Before>>,
	        "Input is not a polymorphic type"
	    );

	    auto basePtr = dynamic_cast<void*>(ptr);
	    auto vftable = *reinterpret_cast<VftableType**>(basePtr);

	    auto metaPtr = static_cast<MetaPointerType*>(static_cast<CompleteVftableType*>(vftable));

	    auto afterDesc = reinterpret_cast<TypeDescriptorType const*>(&typeid(std::remove_pointer_t<After>));

	    auto afterIdent = static_cast<char const*>(afterDesc->m_typeDescriptorName);

	    auto classDesc = metaPtr->m_completeLocator->m_classDescriptor;
	    for (int32_t i = 0; i < classDesc->m_numBaseClasses; ++i) {
	        auto optionIdent = static_cast<char const*>(classDesc->m_baseClassArray[i]->m_typeDescriptor->m_typeDescriptorName);
	        auto optionOffset = classDesc->m_baseClassArray[i]->m_memberDisplacement[0];

	        if (std::strcmp(afterIdent, optionIdent) == 0) {
	            auto afterPtr = reinterpret_cast<std::byte*>(basePtr) + optionOffset;
	            return reinterpret_cast<After>(afterPtr);
	        }
	    }

	    return nullptr;
	} 
}
