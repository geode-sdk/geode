#pragma once
#include <vector>
#include <Geode/platform/platform.hpp>

namespace geode::core::impl {
	template <typename T>
	class Platform {
	public:
		static const std::vector<std::byte> getTrap() {
            return T::trap;
        }

        static const size_t getTrapSize() {
            return T::trap.size();
        }

        static const std::vector<std::byte> getJump(void* from, void* to) {
			// static_assert(&Platform<T>::jump != T::jump, "implement jump");
            return T::jump(from, to);
        }

        static const size_t getJumpSize(void* from, void* to) {
        	// static_assert(&Platform<T>::jump != T::jump, "implement trap");
            return T::jump(from, to).size();
        }

        static void* allocateVM(size_t size) {
        	// static_assert(&Platform<T>::allocateVM != T::allocateVM, "implement allocateVM");
            return T::allocateVM(size);
        }

	    static bool writeMemory(void* to, void* from, size_t size) {
	    	static_assert(&Platform<T>::writeMemory != &T::writeMemory, "implement writeMemory");
	        return T::writeMemory(to, from, size);
	    }

	    // static bool readMemory(const void* addr, const void* to, const size_t size) {
	    // 	static_assert(&Platform<T>::read_memory != &T::read_memory, "implement readMemory");
	    //     return T::readMemory(addr, to, size);
	    // }

	    static bool initialize() {
	    	static_assert(&Platform<T>::initialize != &T::initialize, "implement initialize");
	        return T::initialize();
	    }

	    static bool enableSingleStep(void* context) {
	    	static_assert(&Platform<T>::enableSingleStep != &T::enableSingleStep, "implement enableSingleStep");
	        return T::enableSingleStep(context);
	    }

	    static bool disableSingleStep(void* context) {
	    	static_assert(&Platform<T>::disableSingleStep != &T::disableSingleStep, "implement disableSingleStep");
	        return T::disableSingleStep(context);
	    }
	};
}
