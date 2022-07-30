#pragma once

/**
 * Adapted from https://gist.github.com/altalk23/29b97969e9f0624f783b673f6c1cd279
 */

#include <cstdlib>
#include <stddef.h>
#include <Geode/DefaultInclude.hpp>
#include <type_traits>
#include "../loader/Interface.hpp"
#include "../loader/Mod.hpp"
#include "../loader/Log.hpp"
#include "general.hpp"
#include "casts.hpp"

namespace geode::addresser {

	template<typename T>
	inline intptr_t getVirtual(T func);

	template<typename T>
	inline intptr_t getNonVirtual(T func);

	template<typename T, typename F>
	inline F thunkAdjust(T func, F self);

	class GEODE_DLL Addresser final {
		template <char C>
		struct SingleInheritance {
			virtual ~SingleInheritance() {}
		};
		struct MultipleInheritance : 
			SingleInheritance<'L'>, 
			SingleInheritance<'F'> {
			virtual ~MultipleInheritance() {}
		};

		static MultipleInheritance* instance();

		template <typename R, typename T, typename ...Ps>
		static ptrdiff_t indexOf(R(T::*func)(Ps...)) { 
			using method_t = ptrdiff_t(T::*)();
			return (reinterpret_cast<T*>(instance())->*reinterpret_cast<method_t>(func))(); 
		}

		template <typename R, typename T, typename ...Ps>
		static ptrdiff_t indexOf(R(T::*func)(Ps...) const) { 
			return indexOf(reinterpret_cast<R(T::*)(Ps...)>(func));
		}

		template<typename T>
		static ptrdiff_t thunkOf(T ptr) {
			// msvc
			if (sizeof(T) == sizeof(ptrdiff_t)) return 0;

			// all
			auto thunk = *(reinterpret_cast<ptrdiff_t*>(&ptr)+1);

			// arm
			if (thunk & 1) thunk >>= 1;
			return thunk;
		}

		/**
		 * Specialized functionss
		 */
		template <typename R, typename T, typename ...Ps>
		static intptr_t addressOfVirtual(R(T::*func)(Ps...), 
			typename std::enable_if_t<!std::is_abstract_v<T>>* = 0) {
			using geode::cast::reference_cast;

			// Create a random memory block with the size of T
			// Assign a pointer to that block and cast it to type T*
			uint8_t dum[sizeof(T)] {};
			auto ptr = reinterpret_cast<T*>(dum);
			// Now you have a object of T that actually isn't an object of T and is just a random memory
			// But C++ doesn't know that of course
			// So now you can copy an object that wasn't there in the first place
			// ((oh also get the offsets of the virtual tables))
			auto ins = new T(*ptr);
			// this is how the first human was made


			auto index = indexOf(func);
			auto thunk = thunkOf(func);

			Interface::get()->logInfo("[[" + utils::intToHex((void*)ins) + " + " + utils::intToHex(thunk) + "] + " + utils::intToHex(index) + "]", Severity::Debug);

			// [[this + thunk] + offset] is the f we want
			auto address = *(intptr_t*)(*(intptr_t*)(reference_cast<intptr_t>(ins) + thunk) + index);

			#ifdef GEODE_IS_WINDOWS
				// check if first instruction is a jmp, i.e. if the func is a thunk
				if (*reinterpret_cast<uint16_t*>(address) == 0x25ff) {
					// read where the jmp points to and jump there
					address = *reinterpret_cast<uintptr_t*>(address + 2);
					// that then contains the actual address of the func
					address = *reinterpret_cast<uintptr_t*>(address);
				}
			#endif

			// And we delete the new instance because we are good girls
			// and we don't leak memories
			operator delete(ins);

			return address;
		}

		template <typename R, typename T, typename ...Ps>
		static intptr_t addressOfVirtual(R(T::*func)(Ps...) const,
			typename std::enable_if_t<std::is_copy_constructible_v<T> && !std::is_abstract_v<T>>* = 0) {
			return addressOfVirtual(reinterpret_cast<R(T::*)(Ps...)>(func));
		}

		template <typename R, typename T, typename ...Ps>
		static intptr_t addressOfVirtual(R(T::*func)(Ps...), typename std::enable_if_t<std::is_abstract_v<T>>* = 0) {
			return 0;
		}

		template <typename R, typename T, typename ...Ps>
		static intptr_t addressOfVirtual(R(T::*func)(Ps...) const, typename std::enable_if_t<std::is_abstract_v<T>>* = 0) {
			return 0;
		}

		template <typename R, typename T, typename ...Ps>
		static intptr_t addressOfNonVirtual(R(T::*func)(Ps...) const) {
			return addressOfNonVirtual(reinterpret_cast<R(T::*)(Ps...)>(func));
		}

		template <typename R, typename T, typename ...Ps>
		static intptr_t addressOfNonVirtual(R(T::*func)(Ps...)) {
			return geode::cast::reference_cast<intptr_t>(func);
		}

		template <typename R, typename ...Ps>
		static intptr_t addressOfNonVirtual(R(*func)(Ps...)) {
			return geode::cast::reference_cast<intptr_t>(func);
		}

		template<typename T>
		friend intptr_t getVirtual(T func);

		template<typename T>
		friend intptr_t getNonVirtual(T func);

		template<typename T, typename F>
		friend F thunkAdjust(T func, F self);
	};

	#ifdef GEODE_ADDRESSER_TEST
		template<typename T>
		inline intptr_t getVirtual(T func) {
			return Addresser::addressOfVirtual(func);
		}

		template<typename T>
		inline intptr_t getNonVirtual(T func) {
			return Addresser::addressOfNonVirtual(func);
		}
	#else
		template<typename T>
		inline intptr_t getVirtual(T func) {
			Interface::get()->logInfo("Get virtual function address from " + utils::intToHex(geode::cast::reference_cast<intptr_t>(func)), Severity::Debug);
			auto addr = Addresser::addressOfVirtual(func);
			Interface::get()->logInfo("The address is: " + utils::intToHex(addr), Severity::Debug);
			return addr;
		}

		template<typename T>
		inline intptr_t getNonVirtual(T func) {
			Interface::get()->logInfo("Get non-virtual function address from " + utils::intToHex(geode::cast::reference_cast<intptr_t>(func)), Severity::Debug);
			auto addr = Addresser::addressOfNonVirtual(func);
			Interface::get()->logInfo("The address is: " + utils::intToHex(addr), Severity::Debug);
			return addr;
		}

		template<typename T, typename F>
		inline F thunkAdjust(T func, F self) {
			return (F)((intptr_t)self + Addresser::thunkOf(func));
		}
	#endif
}
