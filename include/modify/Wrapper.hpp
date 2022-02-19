#pragma once
#include "Traits.hpp"
#ifdef GEODE_IS_WINDOWS
	#include <meta/meta.hpp>
#endif

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
			#ifdef GEODE_IS_WINDOWS
				constexpr static inline auto value = MyConv::template get_wrapper<&wrapper>();
			#else
				constexpr static inline auto value = &wrapper;
			#endif
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
		#ifdef GEODE_IS_WINDOWS
			constexpr static inline auto value = MyConv::template get_wrapper<&wrapper>();
		#else
			constexpr static inline auto value = &wrapper;
		#endif
		};
		
		#include <gen/Wrap.hpp>
	};
}
