#pragma once
#include <type_traits>
#include <meta/common.hpp>

namespace geode::modifier {
	using geode::core::meta::always_false;
	/**
	 * The unevaluated function that gets the appropriate 
	 * version of a function type from its return, parameters, and classes.
	 */
	template <class Ret, class Base, class Derived, class ...Parameters>
	auto substitute(Ret(Base::*)(Parameters...)) -> Ret(Base::*)(Parameters...) {
		static_assert(always_false<Ret>, "This function is for unevaluated context");
	}

	template <class Ret, class Base, class Derived, class ...Parameters>
	auto substitute(Ret(Derived::*)(Parameters...)) -> Ret(Derived::*)(Parameters...) {
		static_assert(always_false<Ret>, "This function is for unevaluated context");
	}

	template <class Ret, class Base, class Derived, class ...Parameters>
	auto substitute(Ret(Base::*)(Parameters...) const) -> Ret(Base::*)(Parameters...) const {
		static_assert(always_false<Ret>, "This function is for unevaluated context");
	}

	template <class Ret, class Base, class Derived, class ...Parameters>
	auto substitute(Ret(Derived::*)(Parameters...) const) -> Ret(Derived::*)(Parameters...) const {
		static_assert(always_false<Ret>, "This function is for unevaluated context");
	}

	template <class Ret, class Base, class Derived, class ...Parameters>
	auto substitute(Ret(*)(Parameters...)) -> Ret(*)(Parameters...) {
		static_assert(always_false<Ret>, "This function is for unevaluated context");
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

	template<class Type>
	struct is_function_pointer {
	    constexpr static inline bool value =
	        std::is_pointer<Type>::value ?
	        std::is_function<typename std::remove_pointer<Type>::type>::value :
	        false;
	};

	template<class Type>
	constexpr inline bool is_function_pointer_v = is_function_pointer<Type>::value;
}
