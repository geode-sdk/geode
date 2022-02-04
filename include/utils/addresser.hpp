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

	class GEODE_DLL Addresser final {
		using tablemethodptr_t = ptrdiff_t(Addresser::*)();

		static Addresser* instance();

		template<typename T>
		static ptrdiff_t indexOf(T ptr) { 
			auto func = reinterpret_cast<tablemethodptr_t&>(ptr);
			return (instance()->*func)(); 
		}

	public:
		template<typename T>
		static ptrdiff_t thunkOf(T ptr) {
			if (sizeof(T) == sizeof(ptrdiff_t)) return 0;
			auto thunk = *(reinterpret_cast<ptrdiff_t*>(&ptr)+1);
			if (thunk & 1) thunk >>= 1;
			return thunk;
		}
	private:

		template<typename T>
		static intptr_t pointerOf(T func) {
			return reinterpret_cast<intptr_t&>(func);
		}

		/**
		 * Specialized functionss
		 */
		template <typename R, typename T, typename ...Ps>
		static intptr_t addressOfVirtual(R(T::*func)(Ps...), 
			typename std::enable_if_t<std::is_copy_constructible_v<T> && !std::is_abstract_v<T>>* = 0) {
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

			auto thunk = thunkOf(func);
			std::cout << "thunkOf(func) = " << thunk << "\n";

			// [[this + thunk] + offset] is the f we want
			auto address = *(intptr_t*)(*(intptr_t*)(pointerOf(ins) + thunk) + indexOf(func));

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
		auto addr = Addresser::addressOfVirtual(func);
		Interface::get()->logInfo("The address is: " + utils::intToHex(addr), Severity::Debug);
		return addr;
	}

	template<typename T>
	inline intptr_t getNonVirtual(T func) {
		Interface::get()->logInfo("Get non virtual function address from " + utils::intToHex(Addresser::pointerOf(func)), Severity::Debug);
		auto addr = Addresser::addressOfNonVirtual(func);
		Interface::get()->logInfo("The address is: " + utils::intToHex(addr), Severity::Debug);
		return addr;
	}

	template<typename T, typename F>
	inline F thunkAdjust(T func, F self) {
		return (F)((intptr_t)self + Addresser::thunkOf(func));
	}
}
