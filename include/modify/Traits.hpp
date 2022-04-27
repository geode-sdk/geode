#pragma once
#include <type_traits>
#include "../meta/common.hpp"

namespace geode::modifier {
	using geode::core::meta::always_false;
	/**
	 * The ~unevaluated~ function that gets the appropriate 
	 * version of a function type from its return, parameters, and classes.
	 * 
	 * nvm its no more unevaluated
	 */
	template <class Return, class Class, class ...Parameters>
	constexpr auto substitute(Return(Class::*function)(Parameters...)) /* -> Return(Class::*)(Parameters...) */ {
		return function;
	}
	
	template <class Return, class Class, class ...Parameters>
	constexpr auto substitute(Return(Class::*function)(Parameters...) const) /* -> Return(Class::*)(Parameters...) const */ {
		return function;
	}

	template <class Return, class Class, class ...Parameters>
	constexpr auto substitute(Return(*function)(Parameters...)) /* -> Return(*)(Parameters...) */ {
		return function;
	}

	/**
	 * An UUID system that generates an unique comparable
	 * value for every instance. Internally used for comparing member
	 * function pointers.
	 */
	template <auto a>
	struct function_uuid {
	private:
		constexpr static void function() {}
	public:
		constexpr static inline void(*value)() = &function_uuid::function;
	};
}
