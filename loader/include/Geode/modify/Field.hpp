#pragma once

#include "Traits.hpp"

#include <Geode/loader/Loader.hpp>
#include <vector>

namespace cocos2d {
    class CCNode;
}

namespace geode::modifier {
    class FieldContainer {
    private:
        std::vector<void*> m_containedFields;
        std::vector<std::function<void(void*)>> m_destructorFunctions;

    public:
        ~FieldContainer() {
            for (auto i = 0u; i < m_containedFields.size(); i++) {
                m_destructorFunctions[i](m_containedFields[i]);
                operator delete(m_containedFields[i]);
            }
        }

        void* getField(size_t index) {
            if (m_containedFields.size() <= index) {
                m_containedFields.resize(index + 1);
                m_destructorFunctions.resize(index + 1);
            }
            return m_containedFields.at(index);
        }

        void* setField(size_t index, size_t size, std::function<void(void*)> destructor) {
            m_containedFields.at(index) = operator new(size);
            m_destructorFunctions.at(index) = destructor;
            return m_containedFields.at(index);
        }

        static FieldContainer* from(cocos2d::CCNode* node) {
            return node->getFieldContainer();
        }
    };

    template <class Parent, class Base>
    class FieldIntermediate {
        // Padding used for guaranteeing any member of parents
        // will be in between sizeof(Intermediate) and sizeof(Parent)
        uintptr_t m_padding;

    public:
        static void fieldConstructor(void* offsetField) {
            std::array<std::byte, sizeof(Parent)> parentContainer;

            auto parent = new (parentContainer.data()) Parent();

            parent->Base::~Base();

            std::memcpy(
                offsetField, std::launder(&parentContainer[sizeof(Base)]),
                sizeof(Parent) - sizeof(Base)
            );
        }

        static void fieldDestructor(void* offsetField) {
            std::array<std::byte, sizeof(Parent)> parentContainer;

            auto parent = new (parentContainer.data()) Base();

            std::memcpy(
                std::launder(&parentContainer[sizeof(Base)]), offsetField,
                sizeof(Parent) - sizeof(Base)
            );

            static_cast<Parent*>(parent)->Parent::~Parent();
        }

        template <class = std::enable_if_t<true>>
        Parent* operator->() {
            // get the this pointer of the base
            auto node =
                reinterpret_cast<Parent*>(reinterpret_cast<std::byte*>(this) - sizeof(Base));
            static_assert(sizeof(Base) == offsetof(Parent, m_fields), "offsetof not correct");
            auto container = FieldContainer::from(node);
            static size_t index = Loader::get()->getFieldIndexForClass(typeid(Base).hash_code());
            // this pointer is offset
            auto offsetField = container->getField(index);
            if (!offsetField) {
                offsetField = container->setField(
                    index, sizeof(Parent) - sizeof(Base), &FieldIntermediate::fieldDestructor
                );

                FieldIntermediate::fieldConstructor(offsetField);
            }

            return reinterpret_cast<Parent*>(
                reinterpret_cast<std::byte*>(offsetField) - sizeof(Base)
            );
        }
    };

}
