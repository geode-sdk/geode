#pragma once
#include <type_traits>
#include <Geode/meta/common.hpp>

namespace geode::modifier {
	template <class FunctionType>
	struct MemberFunc {
		template <class Class>
		using with = FunctionType Class::*;
	};

	template <class FunctionType>
	struct ConstMemberFunc {
	    template <class Class>
	    using with = FunctionType Class::*;
	};

	// why
	template <class Return, class ...Parameters>
	struct ConstMemberFunc<Return(Parameters...)> {
	    using FunctionType = Return(Parameters...) const;
	    template <class Class>
	    using with = FunctionType Class::*;
	};

	template <class FunctionType>
	struct StaticFunc {
		using type = FunctionType*;
	};

	using geode::core::meta::always_false;
	/**
	 * The ~unevaluated~ function that gets the appropriate 
	 * version of a function type from its return, parameters, and classes.
	 * 
	 * nvm its no more unevaluated
	 */
	template <class FunctionType, class Class>
	constexpr auto substitute(typename MemberFunc<FunctionType>::template with<Class> function) {
		return function;
	}

	template <class FunctionType, class Class>
	constexpr auto substitute(typename ConstMemberFunc<FunctionType>::template with<Class> function) {
		return function;
	}
	
	template <class FunctionType>
	constexpr auto substitute(typename StaticFunc<FunctionType>::type function) {
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
