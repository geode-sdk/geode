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
            decltype(substitute<Return, Class, Parameters...>(&Class::identifier))                                     \
        >                                                                                                              \
	>                                                                                                                  \
> {                                                                                                                    \
private:                                                                                                               \
	static Return wrapperImpl(Class* self, Parameters... ps) {                                                         \
		return self->Class::identifier(ps...);                                                                         \
	}                                                                                                                  \
public:                                                                                                                \
	constexpr static inline auto value = &wrapperImpl;                                                                 \
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
            decltype(substitute<Return, Class, Parameters...>(&Class::identifier))                                     \
        >                                                                                                              \
	>                                                                                                                  \
> {                                                                                                                    \
private:                                                                                                               \
	static Return wrapperImpl(Parameters... ps) {                                                                      \
		return Class::identifier(ps...);                                                                               \
	}                                                                                                                  \
public:                                                                                                                \
	constexpr static inline auto value = &wrapperImpl;                                                                 \
};                                                                                                                     \

namespace geode::modifier {
	struct wrap {
		GEODE_WRAPPER_FOR_IDENTIFIER(constructor)
		GEODE_WRAPPER_FOR_IDENTIFIER(destructor)
		#include <gen/Wrapper.hpp>
	};
}
