#pragma once

#include <mach-o/dyld.h>

#include <type_traits>
#include <typeinfo>
#include <cstring>

namespace geode {
	using dylib_t = void*;
    struct PlatformInfo {
    	dylib_t m_dylib;
    };
}


namespace geode::base {
	GEODE_NOINLINE inline uintptr_t get() {
		static uintptr_t base = _dyld_get_image_vmaddr_slide(0)+0x100000000;
		return base;
	}
}


extern "C" inline uintptr_t _geode_ios_base() {
	return geode::base::get();
}

namespace geode::cast {
	using uinthalf_t = uint32_t;
	using inthalf_t = int32_t;

	struct DummyClass {
	    virtual ~DummyClass() {}
	};
	struct DummySingleClass : DummyClass {};
	struct DummyClass2 {};
	struct DummyMultipleClass : DummySingleClass, DummyClass2 {};

	struct ClassTypeinfoType {
	    void** m_typeinfoVtable;
	    char const* m_typeinfoName;
	};

	struct SingleClassTypeinfoType : ClassTypeinfoType {
	    ClassTypeinfoType* m_baseClassTypeinfo;
	};

	#pragma pack(push, 1)
	struct MultipleClassSingleEntryType {
	    ClassTypeinfoType* m_baseClassTypeinfo;
	    uint8_t m_visibilityFlag;
	    inthalf_t m_offset;
	    uint8_t m_padding[sizeof(inthalf_t) - 1];
	};
	#pragma pack(pop)

	struct MultipleClassTypeinfoType : ClassTypeinfoType {
	    uint32_t m_flags;
	    uint32_t m_numBaseClass;
	    MultipleClassSingleEntryType m_baseClasses[0x100];
	};

	struct VtableTypeinfoType {
	    inthalf_t m_offset;
	    ClassTypeinfoType* m_typeinfo;
	};

	struct VtableType {
	    void* m_vtable[0x100];
	};

	struct CompleteVtableType : VtableTypeinfoType, VtableType {};

	inline void** typeinfoVtableOf(void* ptr) {
	    auto vftable = *reinterpret_cast<VtableType**>(ptr);
	    
	    auto typeinfoPtr = static_cast<VtableTypeinfoType*>(static_cast<CompleteVtableType*>(vftable));

	    return typeinfoPtr->m_typeinfo->m_typeinfoVtable;
	}

	inline void* traverseTypeinfoFor(void* ptr, ClassTypeinfoType* typeinfo, char const* afterIdent) {
	    DummySingleClass dummySingleClass;
	    DummyMultipleClass dummyMultipleClass;

	    {
	        auto optionIdent = typeinfo->m_typeinfoName;
	        if (std::strcmp(optionIdent, afterIdent) == 0) {
	            return ptr;
	        }
	    }
	    if (typeinfo->m_typeinfoVtable == typeinfoVtableOf(&dummySingleClass)) {
	        auto siTypeinfo = static_cast<SingleClassTypeinfoType*>(typeinfo);
	        return traverseTypeinfoFor(ptr, siTypeinfo->m_baseClassTypeinfo, afterIdent);
	    }
	    else if (typeinfo->m_typeinfoVtable == typeinfoVtableOf(&dummyMultipleClass)) {
	        auto vmiTypeinfo = static_cast<MultipleClassTypeinfoType*>(typeinfo);
	        for (int i = 0; i < vmiTypeinfo->m_numBaseClass; ++i) {
	            auto& entry = vmiTypeinfo->m_baseClasses[i];
	            auto optionPtr = reinterpret_cast<std::byte*>(ptr) + entry.m_offset;
	            auto ret = traverseTypeinfoFor(optionPtr, entry.m_baseClassTypeinfo, afterIdent);
	            if (ret != nullptr) return ret;
	        }
	    }

	    return nullptr;
	}

	template <class After, class Before>
	inline After typeinfo_cast(Before ptr) {
	    static_assert(
	        std::is_polymorphic_v<std::remove_pointer_t<Before>>,
	        "Input is not a polymorphic type"
	    );
	    auto basePtr = dynamic_cast<void*>(ptr);
	    auto vftable = *reinterpret_cast<VtableType**>(basePtr);
	    auto typeinfo = static_cast<VtableTypeinfoType*>(static_cast<CompleteVtableType*>(vftable))->m_typeinfo;

	    auto afterTypeinfo = reinterpret_cast<ClassTypeinfoType const*>(&typeid(std::remove_pointer_t<After>));
	    auto afterIdent = afterTypeinfo->m_typeinfoName;

	    return static_cast<After>(traverseTypeinfoFor(basePtr, typeinfo, afterIdent));
	} 
}
