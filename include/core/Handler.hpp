#pragma once

#include <iostream>
#include "../meta/meta.hpp"

namespace geode::core {
	
	namespace impl {
		/* the handler itself */
		template <auto& Det, class Ret, class ...Args>
		GEODE_DLL Ret handler(Args... args) {
			static thread_local int counter = 0;
			std::cout << "detour size: " << Det->size() << std::endl;
		    
		    if constexpr (std::is_same_v<Ret, void>) {
		    	if (counter == Det->size()) counter = 0;

		    	std::cout << "detour index: " << counter << std::endl;

				if (counter < Det->size()) {
					std::cout << "calling " << (void*)Det->at(counter) << std::endl;
					Det->at(counter++)(args...);
			    }

				if (--counter < 0) counter = Det->size() - 1;
		    }
		    else {
		    	Ret ret;
			    if (counter == Det->size()) counter = 0;

				if (counter < Det->size()) {
					ret = Det->at(counter++)(args...);
			    }

				if (--counter < 0) counter = Det->size() - 1;
			    return ret;
		    }
		    
		}

		template <template <class, class...> class Conv, auto& Func, class Ret, class ...Args>
		GEODE_DLL Ret trampoline(Args... args) {
			return meta::Function<Ret(Args...), Conv>(Func)(args...);
		}
	}

	template <template <class, class...> class Conv, auto& Det, class Ret, class ...Args>
	static constexpr inline auto handler = Conv<Ret, Args...>::template get_wrapper<&impl::handler<Det, Ret, Args...>>();

	template <template <class, class...> class Conv, auto& Det, class Ret, class ...Args>
	static constexpr inline auto trampoline = &impl::trampoline<Conv, Det, Ret, Args...>;
}
