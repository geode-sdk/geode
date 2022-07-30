#pragma once
#include "Traits.hpp"

namespace geode::modifier {
	struct compare {
		template <class, class, class, class = void>
		struct constructor {
			constexpr static inline bool value = false;
		};

		template <class Derived, class Base, class Ret, class ...Parameters>
		struct constructor<Derived, Base, Ret(Parameters...), std::void_t<
			decltype(substitute<Ret, Base, Derived, Parameters...>(&Derived::constructor))
		>> {
			constexpr static inline bool value = true;
		};

		template <class, class, class, class = void>
		struct destructor {
			constexpr static inline bool value = false;
		};

		template <class Derived, class Base, class Ret, class ...Parameters>
		struct destructor<Derived, Base, Ret(Parameters...), std::void_t<
			decltype(substitute<Ret, Base, Derived, Parameters...>(&Derived::destructor))
		>> {
			constexpr static inline bool value = true;
		};
		#include <codegenned/GeneratedCompare.hpp>
	};
}
