#pragma once
#include "Comparer.hpp"
#include "Wrapper.hpp"
#include "Types.hpp"
#include "Addresses.hpp"
#include <iostream>

namespace geode::modifier {

	template <class Derived, class Base>
	class Modify;

	template <class Derived>
	class ModifyBase {
		ModifyBase() {
			Derived::apply();
		}
		template <class, class>
		friend class Modify;
		// explicit Modify(Property property) idea
	};

	template <class Derived, class Base>
	class Modify {
		void apply() {}
	};

	#include <gen/Modify.hpp>
}
