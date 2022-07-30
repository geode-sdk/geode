#include <unordered_map>
#include "Core.hpp"

#include <Geode/hook-core/Hook.hpp>

namespace geode::core::impl {
	namespace {
		inline auto& generatedTrampolines() {
			static std::unordered_map<void*, void*> ret;
			return ret;
		}
		inline auto& currentHandlers() {
			static std::unordered_map<void*, void*> ret;
			return ret;
		}
		inline auto& mappedTrampolines() {
			static std::unordered_map<void*, VectorPointer> ret;
			return ret;
		}
		inline auto& mappedHandlers() {
			static std::unordered_map<void*, VectorPointer> ret;
			return ret;
		}
		inline auto& mappedDetours() {
			static std::unordered_map<void*, VectorPointer> ret;
			return ret;
		}
	}
	void addHook(
		void* address, 
		void* detour, 
		VectorPointer* detourVectorAddress, 
		void* generatedHandler, 
		void** originalTrampolineAddress,
		void* generatedTrampoline
		) {

#ifdef GEODE_IS_WINDOWS
		if (mappedHandlers().find(address) == mappedHandlers().end()) {
	        // std::cout << "allocate handler vector for " << address << std::endl;
	        mappedHandlers().insert({address, new std::vector<void*>});
	        currentHandlers()[address] = generatedHandler;
	        addJump(address, generatedHandler);
	    }
	    mappedHandlers()[address]->push_back(generatedHandler);
#endif
		if (mappedTrampolines().find(address) == mappedTrampolines().end()) {
	        // std::cout << "allocate trampoline vector for " << address << std::endl;
	        mappedTrampolines().insert({address, new std::vector<void*>});
	        if (generatedTrampolines().find(address) == generatedTrampolines().end()) {
	        	generatedTrampolines()[address] = generateRawTrampoline(address);
	        }
	    }
	    auto puretramp = generatedTrampolines()[address];
	    mappedTrampolines()[address]->push_back(generatedTrampoline); 
	    *originalTrampolineAddress = puretramp;

#ifndef GEODE_IS_WINDOWS
	    if (mappedHandlers().find(address) == mappedHandlers().end()) {
	        // std::cout << "allocate handler vector for " << address << std::endl;
	        mappedHandlers().insert({address, new std::vector<void*>});
	        currentHandlers()[address] = generatedHandler;
	        addJump(address, generatedHandler);
	    }
	    mappedHandlers()[address]->push_back(generatedHandler);
#endif

	    if (mappedDetours().find(address) == mappedDetours().end()) {
	        // std::cout << "allocate detour vector for " << address << std::endl;
	        mappedDetours().insert({address, new std::vector<void*>});
	    }
	    auto detours = mappedDetours().at(address);
	    *detourVectorAddress = detours;

	    if (detours->size() == 0) {
	        // std::cout << "add trampoline to the detour vector (it will not get deallocated now)" << std::endl;
	        detours->push_back(mappedTrampolines()[address]->front());
	    }
	    detours->insert(detours->end()-1, detour);
	}

	void removeHook(HookHandle const& handle) {
	    auto [handler, address, detour, trampoline] = handle;

	    auto detours = mappedDetours().at(address);
	    detours->erase(std::remove(detours->begin(), detours->end(), detour), detours->end());

	    auto handlers = mappedHandlers().at(address);
	    handlers->erase(std::remove(handlers->begin(), handlers->end(), handler), handlers->end());

	    auto trampolines = mappedTrampolines().at(address);
	    trampolines->erase(std::remove(trampolines->begin(), trampolines->end(), trampoline), trampolines->end());

	    if (handlers->size() == 0 /* trampolines size should be 0 now */ ) {
	        // std::cout << "deallocate handler vector for " << address << std::endl;
	        delete handlers;
	        mappedHandlers().erase(address);

	        // std::cout << "deallocate trampoline vector for " << address << std::endl;
	        delete trampolines;
	        mappedTrampolines().erase(address);

	        // TODO: bandaid
	        // addJump(address, generatedTrampolines()[address]);
	        // afterHookHander = (decltype(afterHookHander))generatedTrampoline[address]; // switch the trampoline (inline)
	    }
	    else if (currentHandlers()[address] == handler) {
	        // std::cout << "using a different trampoline and handler for " << address << std::endl;
	        detours->back() = trampolines->front();

	        currentHandlers()[address] = handlers->front();
	        addJump(address, handlers->front());
	        // afterHookHander = (decltype(afterHookHander))(handlers->front()); // switch the handler (inline)
	    }
	}

}
