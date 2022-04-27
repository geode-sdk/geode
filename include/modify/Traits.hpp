#pragma once
#include <type_traits>
#include "../meta/common.hpp"

namespace geode::modifier {
	using geode::core::meta::always_false;
	/**
	 * The unevaluated function that gets the appropriate 
	 * version of a function type from its return, parameters, and classes.
	 */
	template <class Return, class Class, class ...Parameters>
	auto substitute(Return(Class::*)(Parameters...)) -> Return(Class::*)(Parameters...) {
		static_assert(always_false<Return>, "This function is for unevaluated context");
	}
	
	template <class Return, class Class, class ...Parameters>
	auto substitute(Return(Class::*)(Parameters...) const) -> Return(Class::*)(Parameters...) const {
		static_assert(always_false<Return>, "This function is for unevaluated context");
	}

	template <class Return, class Class, class ...Parameters>
	auto substitute(Return(*)(Parameters...)) -> Return(*)(Parameters...) {
		static_assert(always_false<Return>, "This function is for unevaluated context");
	}
}
