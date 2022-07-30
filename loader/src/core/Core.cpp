#include "Core.hpp"
#include <map>

#ifndef GEODE_IS_WINDOWS

#if defined(GEODE_IS_MACOS)
    #include "MacOS.hpp"
#elif defined(GEODE_IS_IOS)
    // #include "iOS.hpp"
#endif

namespace geode::core::impl {
	namespace {
		auto& originalBytes() {
			static std::map<void*, std::vector<std::byte>*> ret;
			return ret;
		}

		auto& unhandledTrampolines() {
			static std::map<void*, void*> ret;
			return ret;
		}

		auto& unhandledHandlers() {
			static std::map<void*, void*> ret;
			return ret;
		}
	}

	void* generateRawTrampoline(void* address) {
		static constexpr size_t MAX_TRAMPOLINE_SIZE = 0x40;
		auto trampoline = TargetPlatform::allocateVM(MAX_TRAMPOLINE_SIZE);
		unhandledTrampolines()[address] = trampoline;

		// we need to add a trap instruction to the trampoline
		// so that it will go to handleContext
		const size_t trapSize = TargetPlatform::getTrapSize();
		TargetPlatform::writeMemory(trampoline, (void*)TargetPlatform::getTrap().data(), trapSize);
		return trampoline;
	}

	void addJump(void* at, void* to) {
		if (unhandledTrampolines().find(at) != unhandledTrampolines().end()) {
			// we havent set up the trampoline yet, so this means
			// we need to preserve the original bytes to use when
			// populating the trampoline. we need to save the handler
			// to set a jump to it when we finish the trampoline too.

			const size_t jumpSize = TargetPlatform::getJumpSize(at, to);

			originalBytes().insert( {at, new std::vector<std::byte>(jumpSize, std::byte(0x00))} );
			TargetPlatform::writeMemory((void*)originalBytes()[at]->data(), at, jumpSize);

			unhandledHandlers()[at] = to;
		}
		TargetPlatform::writeMemory(at, (void*)TargetPlatform::getJump(at, to).data(), TargetPlatform::getJumpSize(at, to));
	}

	void handleContext(void* context, void* current) {
		static thread_local void* original = nullptr;
		static thread_local bool originalFlag = false;

		for (auto& [k, v] : unhandledTrampolines()) {
			if (v == current) {
				// we are inside a trampoline, which means we need 
				// to jump to the original function and add a trap
				// instruction to the original function to mark the
				// beginning of the instruction measuring
				
				const size_t jumpSize = TargetPlatform::getJumpSize(current, k);
				TargetPlatform::writeMemory(current, (void*)TargetPlatform::getJump(current, k).data(), jumpSize);

				const size_t trapSize = TargetPlatform::getTrapSize();
				TargetPlatform::writeMemory(k, (void*)TargetPlatform::getTrap().data(), trapSize);

				originalFlag = true;

				return;
			}
		}
		if (originalFlag) { 
			// we are at the beginning of the original function,
			// which means we need to put back the original 
			// instructions and enable single stepping to
			// come back to here every instruction
			//
			// we can get rid of the original bytes vector since
			// we already recovered it

			original = current;
			originalFlag = false;

			auto origBytes = originalBytes()[original];
			TargetPlatform::writeMemory(original, (void*)origBytes->data(), origBytes->size()); 
			delete origBytes;
			originalBytes().erase(original);

			TargetPlatform::enableSingleStep(context);
			return;
		}
		else {
			// we are at an instruction somewhere after the 
			// beginning of the original function, which
			// means we need to check if we can fit a jump
			// instruction 
			auto trampoline = unhandledTrampolines()[original];

			const size_t jumpSize = TargetPlatform::getJumpSize(trampoline, unhandledHandlers()[original]);
			const size_t difference = (size_t)current - (size_t)original;

			if (difference >= jumpSize) {
				// we have enough space to fit a jump from the 
				// beginning of the function to the handler
				// now we can copy the instructions we have
				// measured to the trampoline, add a jump back
				// at the end of it. and also put the jump from
				// the original function to the handler.
				// now that we are done, we can disable single 
				// stepping

				TargetPlatform::writeMemory(trampoline, original, difference);
				TargetPlatform::writeMemory(
					(void*)((size_t)trampoline + difference), 
					(void*)TargetPlatform::getJump(
						(void*)((size_t)trampoline + difference), 
						(void*)((size_t)original + difference)
					).data(), difference);

				TargetPlatform::writeMemory(
					original, 
					(void*)TargetPlatform::getJump(original, unhandledHandlers()[original]).data(), 
					TargetPlatform::getJumpSize(original, unhandledHandlers()[original])); 

				TargetPlatform::disableSingleStep(context);

				unhandledTrampolines().erase(original);
				unhandledHandlers().erase(original);
			}
			return;
		}
	}
}

bool geode::core::hook::initialize() {
	return geode::core::impl::TargetPlatform::initialize();
}

#else

#include <dobby.h>

namespace geode::core::impl {
	namespace {
		auto& trampolines() {
			static std::map<void*, void*> ret;
			return ret;
		}
	}

	void* generateRawTrampoline(void* address) {
        return trampolines()[address];
	}

	void addJump(void* at, void* to) {
        DobbyDestroy(at);
        DobbyHook(at, to, &trampolines()[at]);
	}
}

bool geode::core::hook::initialize() {
	return true;
}

#endif