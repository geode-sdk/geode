
#pragma once

#include "../meta/meta.hpp"

namespace geode::core {
	
	namespace impl {
		/* the handler itself */
		template <auto& Det, class Ret, class ...Args>
		Ret handler(Args... args) {
			static thread_local int counter = 0;
		    
		    if constexpr (std::is_same_v<Ret, void>) {
		    	if (counter == (int)Det->size()) counter = 0;

				Det->at(counter++)(args...);

				if (--counter < 0) counter = Det->size() - 1;
		    }
		    else {
			    if (counter == (int)Det->size()) counter = 0;

				Ret ret = Det->at(counter++)(args...);

				if (--counter < 0) counter = Det->size() - 1;
			    return ret;
		    }
		    
		}

		template <template <class, class...> class Conv, auto& Func, class Ret, class ...Args>
		Ret trampoline(Args... args) {
			return meta::Function<Ret(Args...), Conv>(Func)(args...);
		}
	}

	template <template <class, class...> class Conv, auto& Det, class Ret, class ...Args>
	static constexpr inline auto handler = Conv<Ret, Args...>::template get_wrapper<&impl::handler<Det, Ret, Args...>>();

	template <template <class, class...> class Conv, auto& Det, class Ret, class ...Args>
	static constexpr inline auto trampoline = &impl::trampoline<Conv, Det, Ret, Args...>;
}