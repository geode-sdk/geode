#pragma once
#include "Traits.hpp"

#define GEODE_WRAPPER_FOR_IDENTIFIER(identifier)                                                                       \
/* Default - function Return Class::identifier(Parameters...) does not exist */                                        \
template <                                                                                                             \
	class Class,                                                                                                       \
	class FunctionType,                                                                                                \
	class = void                                                                                                       \
>                                                                                                                      \
struct identifier {                                                                                                    \
public:                                                                                                                \
	constexpr static inline auto value = nullptr;                                                                      \
	constexpr static inline auto uuid = nullptr;                                                                       \
};                                                                                                                     \
/* Specialization - function Return Class::identifier(Parameters...) is a member function */                           \
template <                                                                                                             \
	class Class,                                                                                                       \
	class Return,                                                                                                      \
	class ...Parameters                                                                                                \
>                                                                                                                      \
struct identifier<                                                                                                     \
	Class,                                                                                                             \
	Return(Parameters...), std::enable_if_t<                                                                           \
	    std::is_member_function_pointer_v<                                                                             \
            decltype(substitute<Return(Parameters...)>(&Class::identifier))                                            \
        >                                                                                                              \
	>                                                                                                                  \
> {                                                                                                                    \
private:                                                                                                               \
	static Return wrapperImpl(Class* self, Parameters... ps) {                                                         \
		return self->Class::identifier(ps...);                                                                         \
	}                                                                                                                  \
public:                                                                                                                \
	constexpr static inline auto value = &wrapperImpl;                                                                 \
	constexpr static inline auto uuid = function_uuid<                                                                 \
		substitute<Return(Parameters...)>(&Class::identifier)                                                          \
	>::value;                                                                                                          \
};                                                                                                                     \
/* Specialization - function Return Class::identifier(Parameters...) is a static function */                           \
template <                                                                                                             \
	class Class,                                                                                                       \
	class Return,                                                                                                      \
	class ...Parameters                                                                                                \
>                                                                                                                      \
struct identifier<                                                                                                     \
	Class,                                                                                                             \
	Return(Parameters...), std::enable_if_t<                                                                           \
        std::is_pointer_v<                                                                                             \
            decltype(substitute<Return(Parameters...)>(&Class::identifier))                                            \
        >                                                                                                              \
	>                                                                                                                  \
> {                                                                                                                    \
private:                                                                                                               \
	static Return wrapperImpl(Parameters... ps) {                                                                      \
		return Class::identifier(ps...);                                                                               \
	}                                                                                                                  \
public:                                                                                                                \
	constexpr static inline auto value = &wrapperImpl;                                                                 \
	constexpr static inline auto uuid = function_uuid<                                                                 \
		substitute<Return(Parameters...)>(&Class::identifier)                                                          \
	>::value;                                                                                                          \
};                                                                                                                     \

namespace geode::modifier {
	struct wrap {
		GEODE_WRAPPER_FOR_IDENTIFIER(constructor)
		GEODE_WRAPPER_FOR_IDENTIFIER(destructor)
		#include <codegen/GeneratedWrapper.hpp>
	};

	// template <template<class, class, class=void> class Identifier, class Base, class Derived, class ...Types>
	// struct PotentiallyWrongIdentifier {

	// 	template <typename D>
	//     static std::true_type existsImpl(...);

	//     template <typename D, typename = std::enable_if_t<
	//         (... && (
	//         	Identifier<D, Types>::uuid == nullptr ||
	//         	Identifier<Base, Types>::uuid == Identifier<D, Types>::uuid
	//         ))
	//     >>
	//     static std::false_type existsImpl(char);

	//     template <typename D, typename = std::enable_if_t<
	//         (... || (
	//         	Identifier<D, Types>::uuid != nullptr &&
	//         	Identifier<Base, Types>::uuid == Identifier<D, Types>::uuid
	//         ))
	//     >>
	//     static std::false_type existsImpl(int);

	//     static constexpr bool value = decltype(existsImpl<Derived>(0))::value;

	// };

}
