#include "Traits.hpp"

// #define GEODE_WRAPPER_WITH_NAME(NAME)																								\
// template <class, class, class, class = void>																						\
// struct init {																														\
//     constexpr static inline auto value = nullptr;																					\
// };																																	\
// template <class Derived, class Base, class Ret, class ...Parameters>																\
// struct init<Derived, Base, Ret(Parameters...), std::enable_if_t<																	\
//     std::is_member_function_pointer_v<decltype(substitute<Ret, Base, Derived, Parameters...>(&Derived::init))>						\
// >> {																																\
//     using derived_type = decltype(substitute<Ret, Base, Derived, Parameters...>(&Derived::init));									\
//     static Ret wrapper(Derived* self, Parameters... ps) {																			\
//         return self->Derived::init(ps...);																							\
//     }																																\
//     constexpr static inline auto value = &wrapper;																					\
// };																																	\
// template <class Derived, class Base, class Ret, class ...Parameters>																\
// struct init<Derived, Base, Ret(Parameters...), std::enable_if_t<																	\
//     is_function_pointer_v<decltype(substitute<Ret, Base, Derived, Parameters...>(&Derived::init))>									\
// >> {																																\
//     using derived_type = decltype(substitute<Ret, Base, Derived, Parameters...>(&Derived::init));									\
//     static Ret wrapper(Parameters... ps) {																							\
//         return Derived::init(ps...);																								\
//     }																																\
//     constexpr static inline auto value = &wrapper;																					\
// };

namespace geode::modifier {

	struct wrap {
		
		#include <gen/Wrap.hpp>
	}
}
