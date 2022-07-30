#pragma once

#include "Handler.hpp"
#include "../utils/Result.hpp"
#include <vector>

namespace geode::core {
	
	// The handle returned from adding a hook. It is used to remove a hook.
	struct HookHandle {
	    void* handler;
	    void* address;
	    void* detour;
	    void* trampoline;
	};

	namespace impl {
		// a type alias for convenience and for clear type differentiation inside the source file
		using VectorPointer = std::vector<void*>*;

		GEODE_DLL void addHook(
			void* address, 
			void* detour, 
			VectorPointer* detourVectorAddress, 
			void* generatedHandler, 
			void** originalTrampolineAddress,
			void* generatedTrampoline
		);

		GEODE_DLL void removeHook(HookHandle const& handle);
	}

	namespace hook {
		template <auto Detour, template <class, class...> class Conv, class Ret, class ...Args>
		Result<HookHandle> add(Ret(*address)(Args...)) {

		    static std::vector<decltype(Detour)>* detourVector;
		    static decltype(Detour) originalTrampoline;

		    void* generatedHandler = (void*)handler<Conv, detourVector, Ret, Args...>;
		    void* generatedTrampoline = (void*)trampoline<Conv, originalTrampoline, Ret, Args...>;

		    impl::addHook(
		    	(void*)address, 
		    	(void*)Detour, 
		    	(impl::VectorPointer*)&detourVector, 
		    	(void*)generatedHandler, 
		    	(void**)&originalTrampoline, 
		    	(void*)generatedTrampoline
		    );

		    return Ok<HookHandle>({
		    	(void*)generatedHandler, 
		    	(void*)address, 
		    	(void*)Detour, 
		    	(void*)generatedTrampoline
		   	});
		}

		inline Result<> remove(HookHandle const& handle) {
			impl::removeHook(handle);
			return Ok<>();
		}
	}
}