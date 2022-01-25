#pragma once

/**
 * Adapted from https://gist.github.com/altalk23/29b97969e9f0624f783b673f6c1cd279
 */

#include <cstdlib>
#include <stddef.h>
#include <loader/Interface.hpp>
#include <loader/Mod.hpp>
#include <loader/Log.hpp>
#include <utils/general.hpp>

namespace geode::addresser {

	template<typename T>
	inline intptr_t getVirtual(T func);

	template<typename T>
	inline intptr_t getNonVirtual(T func);

	class Addresser final {
		using tablemethodptr_t = ptrdiff_t(Addresser::*)();

		static Addresser* instance();

		template<typename T>
		static ptrdiff_t indexOf(T ptr) { 
			auto func = reinterpret_cast<tablemethodptr_t&>(ptr);
			return (instance()->*func)(); 
		}

		template<typename T>
		static ptrdiff_t thunkOf(T ptr) {
			if (sizeof(T) == sizeof(ptrdiff_t)) return 0;
			auto thunk = *(reinterpret_cast<ptrdiff_t*>(&ptr)+1);
			if (thunk & 1) thunk >>= 1;
			return thunk;
		}

		template<typename T>
		static intptr_t pointerOf(T func) {
			return reinterpret_cast<intptr_t&>(func);
		}

		/**
		 * Specialized functionss
		 */
		template <typename R, typename T, typename ...Ps, typename = std::enable_if<std::is_copy_constructible_v<T> > >
		static intptr_t addressOfVirtual(R(T::*func)(Ps...)) {
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

			// [[this + thunk] + offset] is the f we want
			auto address = *(intptr_t*)(*(intptr_t*)(pointerOf(ins) + thunkOf(func)) + indexOf(func));

			// And we delete the new instance because we are good girls
			// and we don't leak memories
			operator delete(ins);

			return address;
		}

		template <typename R, typename T, typename ...Ps, , typename = std::enable_if<std::is_copy_constructible_v<T> >>
		static intptr_t addressOfVirtual(R(T::*func)(Ps...) const) {
			return addressOfVirtual(func);
		}

		template <typename R, typename T, typename ...Ps>
		static intptr_t addressOfNonVirtual(R(T::*func)(Ps...) const) {
			return addressOfNonVirtual(func);
		}

		template <typename R, typename T, typename ...Ps>
		static intptr_t addressOfNonVirtual(R(T::*func)(Ps...)) {
			return pointerOf(func);
		}

		template <typename R, typename ...Ps>
		static intptr_t addressOfNonVirtual(R(*func)(Ps...)) {
			return pointerOf(func);
		}

		template<typename T>
		friend intptr_t getVirtual(T func);

		template<typename T>
		friend intptr_t getNonVirtual(T func);
	};

	template<typename T>
	inline intptr_t getVirtual(T func) {
		Interface::get()->logInfo("Get virtual function address from " + utils::intToHex(Addresser::pointerOf(func)), Severity::Debug);
		return Addresser::addressOfVirtual(func);
	}

	template<typename T>
	inline intptr_t getNonVirtual(T func) {
		Interface::get()->logInfo("Get non virtual function address from " + utils::intToHex(Addresser::pointerOf(func)), Severity::Debug);
		return Addresser::addressOfNonVirtual(func);
	}
}
