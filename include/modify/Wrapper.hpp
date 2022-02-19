#pragma once
#include "Traits.hpp"
#ifdef GEODE_IS_WINDOWS
	#include <meta/meta.hpp>
#endif

namespace geode::modifier {

	template <class Ret, class... Args>
    class DefaultConv {
    public:
    	template <Ret(*detour)(Args...)>
        static constexpr decltype(auto) get_wrapper() {
            return detour;
        }
    };

	struct wrap {
		template <template <class, class...> class, class, class, class, class = void>
		struct constructor {
		private:
			static void wrapper(...) {}
		public:
			constexpr static inline auto value = &wrapper;
		};

		template <template <class, class...> class Conv, class Derived, class Base, class Ret, class ...Parameters>
		struct constructor<Conv, Derived, Base, Ret(Parameters...), std::enable_if_t<
			std::is_member_function_pointer_v<decltype(substitute<Ret, Base, Derived, Parameters...>(&Derived::constructor))>
		>> {
		private:
			static Ret wrapper(Derived* self, Parameters... ps) {
				return self->Derived::constructor(ps...);
			}
		public:
			using MyConv = Conv<Ret, Derived*, Parameters...>;
			constexpr static inline auto value = MyConv::template get_wrapper<&wrapper>();
		};

		template <template <class, class...> class, class, class, class, class = void>
		struct destructor {
		private:
			static void wrapper(...) {}
		public:
			constexpr static inline auto value = &wrapper;
		};

		template <template <class, class...> class Conv, class Derived, class Base, class Ret, class ...Parameters>
		struct destructor<Conv, Derived, Base, Ret(Parameters...), std::enable_if_t<
			std::is_member_function_pointer_v<decltype(substitute<Ret, Base, Derived, Parameters...>(&Derived::destructor))>
		>> {
		private:
			static Ret wrapper(Derived* self, Parameters... ps) {
				return self->Derived::destructor(ps...);
			}
		public:
			using MyConv = Conv<Ret, Derived*, Parameters...>;
			constexpr static inline auto value = MyConv::template get_wrapper<&wrapper>();
		};
		
		#include <gen/Wrap.hpp>
	};
}
