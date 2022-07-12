#pragma once
#include "../Geode.hpp"
#include "Traits.hpp"
#include <vector>

namespace geode::modifier {
	class FieldContainer : public cocos2d::CCObject {
		std::vector<void*> m_containedFields;
		std::vector<std::function<void(void*)>> m_destructorFunctions;
	public:

		static FieldContainer* create() {
			auto ret = new (std::nothrow) FieldContainer;
			if (ret) {
				ret->autorelease();
				return ret;
			}
			return nullptr;
		}
		~FieldContainer() {
			for (auto i = 0u; i < m_containedFields.size(); i++)  {
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
	};
	
	template<class Base, class Intermediate, class Parent>
	class FieldIntermediate {
		// Padding used for guaranteeing any member of parents 
		// will be in between sizeof(Intermediate) and sizeof(Parent)
		std::max_align_t m_padding; 
	public:
		static void fieldConstructor(void* offsetField) {
			std::array<std::byte, sizeof(Parent)> parentContainer;

			auto parent = new (parentContainer.data()) Parent();

			parent->Intermediate::~Intermediate();

			std::memcpy(
				offsetField, 
				std::launder(&parentContainer[sizeof(Intermediate)]), 
				sizeof(Parent) - sizeof(Intermediate)
			);
		}
		static void fieldDestructor(void* offsetField) {
			std::array<std::byte, sizeof(Parent)> parentContainer;

			auto parent = new (parentContainer.data()) Intermediate();

			std::memcpy(
				std::launder(&parentContainer[sizeof(Intermediate)]), 
				offsetField, 
				sizeof(Parent) - sizeof(Intermediate)
			);

			static_cast<Parent*>(parent)->Parent::~Parent();
		}

		template <class=std::enable_if_t<true>>
		Parent* operator->() {
			// get the this pointer of the base
			auto node = reinterpret_cast<Parent*>(reinterpret_cast<std::byte*>(this) - offsetof(Parent, m_fields));
			auto container = reinterpret_cast<FieldContainer*>(node->getUserObject());
			if (!container) {
				container = FieldContainer::create();
				node->setUserObject(container);
			}
			static size_t index = Loader::get()->getFieldIndexForClass(typeid(Base).hash_code());
			// this pointer is offset 
			auto offsetField = container->getField(index);
			if (!offsetField) {
				offsetField = container->setField(index, sizeof(Parent) - sizeof(Intermediate), 
					&FieldIntermediate::fieldDestructor
				);

				FieldIntermediate::fieldConstructor(offsetField);
			}

			return reinterpret_cast<Parent*>(reinterpret_cast<std::byte*>(offsetField) - sizeof(Intermediate));
		}
	};

}
