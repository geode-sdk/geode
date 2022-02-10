#include <type_traits>

namespace geode::modifier {
	/**
	 * The unevaluated function that gets the appropriate 
	 * version of a function type from its return, parameters, and classes.
	 */
	template <class Ret, class Base, class Derived, class ...Parameters>
	auto substitute(Ret(Base::*)(Parameters...)) -> Ret(Base::*)(Parameters...) {
		static_assert(!std::is_same_v<Ret, Ret>, "This function is for unevaluated context");
	}

	template <class Ret, class Base, class Derived, class ...Parameters>
	auto substitute(Ret(Derived::*)(Parameters...)) -> Ret(Derived::*)(Parameters...) {
		static_assert(!std::is_same_v<Ret, Ret>, "This function is for unevaluated context");
	}

	template <class Ret, class Base, class Derived, class ...Parameters>
	auto substitute(Ret(Base::*)(Parameters...) const) -> Ret(Base::*)(Parameters...) const {
		static_assert(!std::is_same_v<Ret, Ret>, "This function is for unevaluated context");
	}

	template <class Ret, class Base, class Derived, class ...Parameters>
	auto substitute(Ret(Derived::*)(Parameters...) const) -> Ret(Derived::*)(Parameters...) const {
		static_assert(!std::is_same_v<Ret, Ret>, "This function is for unevaluated context");
	}

	template <class Ret, class Base, class Derived, class ...Parameters>
	auto substitute(Ret(*)(Parameters...)) -> Ret(*)(Parameters...) {
		static_assert(!std::is_same_v<Ret, Ret>, "This function is for unevaluated context");
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
		constexpr static inline void(*value)() = &function_id::function;
	};

	/**
	 * Macro for creating a comparer a specified function name,
	 * since it is impossible to supply this value with templates.
	 */
	#define GEODE_CREATE_COMPARER_FOR_NAME(name)                                                        \
	template <class, class, class, class = void>                                                        \
	struct name##Exists {                                                                               \
		constexpr static inline bool value = false;                                                     \
	};                                                                                                  \
	template <class Derived, class Base, class Ret, class ...Parameters>                                \
	struct name##Exists<Derived, Base, Ret(Parameters...), std::void_t<                                 \
		decltype(substitute<Ret, Base, Derived, Parameters...>(&Base::init)),                           \
		decltype(substitute<Ret, Base, Derived, Parameters...>(&Derived::init))                         \
	>> {                                                                                                \
	private:                                                                                            \
		using base_type = decltype(substitute<Ret, Base, Derived, Parameters...>(&Base::init));         \
		using derived_type = decltype(substitute<Ret, Base, Derived, Parameters...>(&Derived::init));   \
	public:                                                                                             \
		constexpr static inline bool value =                                                            \
			function_uuid<(derived_type)&Derived::init>::value !=                                       \
			function_uuid<(base_type)&Base::init>::value;                                               \
	};
}
