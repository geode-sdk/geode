#pragma once

#include "_casts_shared.hpp"

namespace geode::cast {

    struct DummyClass {
        virtual ~DummyClass() {}
    };

    struct DummySingleClass : DummyClass {};

    struct DummyClass2 {};

    struct DummyMultipleClass : DummySingleClass, DummyClass2 {};

    struct VtableType;

    struct ClassTypeinfoType {
        VtableType* m_typeinfoVtable;
        char const* m_typeinfoName;
    };

    struct SingleClassTypeinfoType : ClassTypeinfoType {
        ClassTypeinfoType* m_baseClassTypeinfo;
    };

    struct MultipleClassSingleEntryType {
        ClassTypeinfoType* m_baseClassTypeinfo;
        intptr_t m_metadata;

        uint8_t visibilityFlag() const {
            return m_metadata & 0xFF;
        }
        intptr_t offset() const {
            return m_metadata >> 8;
        }
    };

    struct MultipleClassTypeinfoType : ClassTypeinfoType {
        uint32_t m_flags;
        uint32_t m_numBaseClass;
        MultipleClassSingleEntryType m_baseClasses[0x100];
    };

    struct VtableTypeinfoType {
        intptr_t m_offset;
        ClassTypeinfoType* m_typeinfo;
    };

    struct VtableType {
        void* m_vtable[0x100];
    };

    struct CompleteVtableType : VtableTypeinfoType, VtableType {};

    inline void* traverseTypeinfoFor(
        void* ptr, ClassTypeinfoType const* typeinfo, char const* afterIdent
    ) {
        {
            auto optionIdent = typeinfo->m_typeinfoName;
            if (std::strcmp(optionIdent, afterIdent) == 0) {
                return ptr;
            }
        }
        auto typeinfoVtableName = static_cast<CompleteVtableType*>(typeinfo->m_typeinfoVtable)->m_typeinfo->m_typeinfoName;
        if (std::strcmp(typeinfoVtableName, "N10__cxxabiv120__si_class_type_infoE") == 0) {
            auto siTypeinfo = static_cast<SingleClassTypeinfoType const*>(typeinfo);
            return traverseTypeinfoFor(ptr, siTypeinfo->m_baseClassTypeinfo, afterIdent);
        }
        else if (std::strcmp(typeinfoVtableName, "N10__cxxabiv121__vmi_class_type_infoE") == 0) {
            auto vmiTypeinfo = static_cast<MultipleClassTypeinfoType const*>(typeinfo);
            for (int i = 0; i < vmiTypeinfo->m_numBaseClass; ++i) {
                auto& entry = vmiTypeinfo->m_baseClasses[i];
                auto optionPtr = reinterpret_cast<std::byte*>(ptr) + entry.offset();
                auto ret = traverseTypeinfoFor(optionPtr, entry.m_baseClassTypeinfo, afterIdent);
                if (ret != nullptr) return ret;
            }
        }

        return nullptr;
    }

    inline void* typeinfoCastInternal(void* ptr, ClassTypeinfoType const* beforeTypeinfo, ClassTypeinfoType const* afterTypeinfo, size_t hint) {
        // we're not using either because uhhh idk
        // hint is for diamond inheritance iirc which is never
        // used in gd, so should be pretty safe to ignore
        if (!ptr) {
            return nullptr;
        }

        (void)beforeTypeinfo;
        (void)hint;

        auto vftable = *reinterpret_cast<VtableType const* const*>(ptr);
        auto dataPointer = static_cast<VtableTypeinfoType const*>(static_cast<CompleteVtableType const*>(vftable));
        auto typeinfo = dataPointer->m_typeinfo;
        auto basePtr = static_cast<std::byte*>(ptr) + dataPointer->m_offset;

        auto afterIdent = afterTypeinfo->m_typeinfoName;

        return traverseTypeinfoFor(basePtr, typeinfo, afterIdent);
    }

    template <class After, class Before>
    inline After typeinfo_cast(Before ptr) {
        ::geode::geode_internal::typeinfoCastChecks<After, Before>();

        if (!ptr) {
            return static_cast<After>(nullptr);
        }

        auto beforeTypeinfo = reinterpret_cast<ClassTypeinfoType const*>(&typeid(std::remove_pointer_t<Before>));
        auto afterTypeinfo = reinterpret_cast<ClassTypeinfoType const*>(&typeid(std::remove_pointer_t<After>));
        return static_cast<After>(typeinfoCastInternal((void*)ptr, beforeTypeinfo, afterTypeinfo, 0));
    }

    inline char const* getRuntimeTypeName(void const* ptr) {
        if (!ptr) {
            return "<null>";
        }

        auto vftable = *reinterpret_cast<VtableType const* const*>(ptr);

        auto dataPointer = static_cast<VtableTypeinfoType const*>(static_cast<CompleteVtableType const*>(vftable));

        auto typeinfo = dataPointer->m_typeinfo;

        return typeinfo->m_typeinfoName;
    }

    inline char const* getRuntimeTypeName(std::type_info const& info) {
        return reinterpret_cast<ClassTypeinfoType const*>(&info)->m_typeinfoName;
    }
}