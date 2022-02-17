#pragma once
#include "Traits.hpp"

namespace geode::modifier {
	struct wrap {
		template <class, class, class, class = void>
		struct constructor {
		private:
			static void wrapper(...) {}
		public:
			constexpr static inline auto value = &wrapper;
		};

		template <class Derived, class Base, class Ret, class ...Parameters>
		struct constructor<Derived, Base, Ret(Parameters...), std::enable_if_t<
			std::is_member_function_pointer_v<decltype(substitute<Ret, Base, Derived, Parameters...>(&Derived::constructor))>
		>> {
		private:
			static Ret wrapper(Derived* self, Parameters... ps) {
				return self->Derived::constructor(ps...);
			}
		public:
			constexpr static inline auto value = &wrapper;
		};

		template <class, class, class, class = void>
		struct destructor {
		private:
			static void wrapper(...) {}
		public:
			constexpr static inline auto value = &wrapper;
		};

		template <class Derived, class Base, class Ret, class ...Parameters>
		struct destructor<Derived, Base, Ret(Parameters...), std::enable_if_t<
			std::is_member_function_pointer_v<decltype(substitute<Ret, Base, Derived, Parameters...>(&Derived::destructor))>
		>> {
		private:
			static Ret wrapper(Derived* self, Parameters... ps) {
				return self->Derived::destructor(ps...);
			}
		public:
			constexpr static inline auto value = &wrapper;
		};
		
		#include <gen/Wrap.hpp>
	};
}
