#pragma once
#include "Wrapper.hpp"
#include "Types.hpp"
#include "Addresses.hpp"
#include "../meta/meta.hpp"
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <iostream>
#include "IDManager.hpp"

#define GEODE_APPLY_MODIFY_FOR_FUNCTION(addr_index, pure_index, convention, className, functionName)                                        \
if constexpr (                                                                                                                              \
		wrap::functionName<Derived, types::pure##pure_index>::uuid != nullptr &&                                                            \
		(void*)wrap::functionName<Base, types::pure##pure_index>::uuid != (void*)wrap::functionName<Derived, types::pure##pure_index>::uuid \
	) {                                                                                                                                     \
	(void)Mod::get()->addHook<wrap::functionName<Derived, types::pure##pure_index>::value, convention>(                                           \
		#className "::" #functionName,                                                                                                      \
		(void*)addresses::address##addr_index()                                                                                             \
	);                                                                                                                                      \
}                                                                                                                                           \

template<class T>
concept HasStringIDProvider = requires {
	T::provideStringIDs();
};

namespace geode::modifier {
	template <class Derived, class Base>
	class Modify;

	template <class Derived>
	class ModifyBase {
	public:
		// unordered_map<handles> idea
		ModifyBase() {
			Loader::get()->scheduleOnModLoad(getMod(), [](){
				Derived::apply();
			});
		}
		template <class, class>
		friend class Modify;
		// explicit Modify(Property property) idea
	};

	template <class Derived, class Base>
	class Modify {
	public:
		Modify() {
			static_assert(core::meta::always_false<Derived>, "Custom Modify not implemented.");
		}
	};
}
