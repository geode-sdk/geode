#pragma once

#include "../utils/MiniFunction.hpp"
#include "Traits.hpp"

#include <Geode/loader/Loader.hpp>
#include <cocos2d.h>
#include <vector>

namespace cocos2d {
    class CCNode;
}

namespace geode {
    template <class, class>
    class Modify;
}

namespace geode::modifier {
    class FieldContainer {
    private:
        std::vector<void*> m_containedFields;
        std::vector<utils::MiniFunction<void(void*)>> m_destructorFunctions;

    public:
        ~FieldContainer() {
            for (auto i = 0u; i < m_containedFields.size(); i++) {
                if (m_destructorFunctions[i] && m_containedFields[i]) {
                    m_destructorFunctions[i](m_containedFields[i]);
                    operator delete(m_containedFields[i]);
                }
            }
        }

        void* getField(size_t index) {
            while (m_containedFields.size() <= index) {
                m_containedFields.push_back(nullptr);
                m_destructorFunctions.push_back(nullptr);
            }
            return m_containedFields.at(index);
        }

        void* setField(size_t index, size_t size, utils::MiniFunction<void(void*)> destructor) {
            m_containedFields.at(index) = operator new(size);
            m_destructorFunctions.at(index) = destructor;
            return m_containedFields.at(index);
        }

        static FieldContainer* from(cocos2d::CCNode* node, char const* forClass) {
            return node->getFieldContainer(forClass);
        }
    };

    GEODE_DLL size_t getFieldIndexForClass(char const* name);

    template <class Parent, class Base>
    class FieldIntermediate {
        using Intermediate = Modify<Parent, Base>;
        // Padding used for guaranteeing any member of parents
        // will be in between sizeof(Intermediate) and sizeof(Parent)
        std::aligned_storage_t<std::alignment_of_v<Base>, std::alignment_of_v<Base>> m_padding;

    public:
        // the constructor that constructs the fields.
        // we construct the Parent first,
        static void fieldConstructor(void* offsetField) {
            (void) new (offsetField) typename Parent::Fields();
        }

        static void fieldDestructor(void* offsetField) {
            static_cast<typename Parent::Fields*>(offsetField)->~Fields();
        }

        auto self() {
            // get the this pointer of the base
            // field intermediate is the first member of Modify
            // meaning we can get the base from ourself
            auto node = reinterpret_cast<Parent*>(reinterpret_cast<std::byte*>(this) - sizeof(Base));
            // static_assert(sizeof(Base) + sizeof() == sizeof(Intermediate), "offsetof not correct");

            // generating the container if it doesn't exist
            auto container = FieldContainer::from(node, typeid(Base).name());

            // the index is global across all mods, so the
            // function is defined in the loader source
            static size_t index = getFieldIndexForClass(typeid(Base).name());

            // the fields are actually offset from their original
            // offset, this is done to save on allocation and space
            auto offsetField = container->getField(index);
            if (!offsetField) {
                offsetField = container->setField(
                    index, sizeof(typename Parent::Fields), &FieldIntermediate::fieldDestructor
                );

                FieldIntermediate::fieldConstructor(offsetField);
            }

            return reinterpret_cast<typename Parent::Fields*>(offsetField);
        }

        auto operator->() {
            return this->self();
        }
    };

}
