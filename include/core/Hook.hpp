#pragma once

#include "Handler.hpp"
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

		void addHook(
			void* address, 
			void* detour, 
			VectorPointer* detourVectorAddress, 
			void* generatedHandler, 
			void** originalTrampolineAddress,
			void* generatedTrampoline
		);

		void removeHook(Handle const& handle);
	}

	namespace hook {
		template <auto Detour, template <class, class...> class Conv, class Ret, class ...Args>
		HookHandle add(Ret(*address)(Args...)) {
		    using FunctionType = decltype(Detour);
		    using MyConv = Conv<Ret, Args...>;

		    static std::vector<FunctionType>* detourVector;
		    static FunctionType originalTrampoline;

		    FunctionType generatedHandler = handler<Conv, detourVector, Ret, Args...>;
		    FunctionType generatedTrampoline = trampoline<Conv, originalTrampoline, Ret, Args...>;

		    impl::addHook(
		    	(void*)address, 
		    	(void*)Detour, 
		    	(impl::VectorPointer*)&detourVector, 
		    	(void*)generatedHandler, 
		    	(void**)&originalTrampoline, 
		    	(void*)generatedTrampoline
		    );

		    return {(void*)generatedHandler, (void*)address, (void*)Detour, (void*)generatedTrampoline};
		}

		void remove(HookHandle const& handle) {
			return impl::removeHook(handle);
		}
	}
}
