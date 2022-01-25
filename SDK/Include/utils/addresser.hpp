#pragma once

/**
 * Adapted from https://gist.github.com/altalk23/29b97969e9f0624f783b673f6c1cd279
 */

#include <cstdlib>
#include <stddef.h>
#include <Macros.hpp>
#include <loader/Interface.hpp>
#include <loader/Mod.hpp>
#include <loader/Log.hpp>
#include <utils/general.hpp>

#if INT64_MAX == INTPTR_MAX
	#define GEODE_ADDRESSER_NEST1(macro, begin)       \
	macro(GEODE_CONCAT(begin, 0))                     \
	macro(GEODE_CONCAT(begin, 8))         
#else
	#define GEODE_ADDRESSER_NEST1(macro, begin)       \
	macro(GEODE_CONCAT(begin, 0))                     \
	macro(GEODE_CONCAT(begin, 4))                     \
	macro(GEODE_CONCAT(begin, 8))                     \
	macro(GEODE_CONCAT(begin, c))         
#endif

#define GEODE_ADDRESSER_NEST2(macro, begin)           \
GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, 0))  \
GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, 1))  \
GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, 2))  \
GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, 3))  \
GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, 4))  \
GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, 5))  \
GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, 6))  \
GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, 7))  \
GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, 8))  \
GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, 9))  \
GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, a))  \
GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, b))  \
GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, c))  \
GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, d))  \
GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, e))  \
GEODE_ADDRESSER_NEST1(macro, GEODE_CONCAT(begin, f))  

#define GEODE_ADDRESSER_NEST3(macro, begin)           \
GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, 0))  \
GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, 1))  \
GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, 2))  \
GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, 3))  \
GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, 4))  \
GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, 5))  \
GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, 6))  \
GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, 7))  \
GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, 8))  \
GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, 9))  \
GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, a))  \
GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, b))  \
GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, c))  \
GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, d))  \
GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, e))  \
GEODE_ADDRESSER_NEST2(macro, GEODE_CONCAT(begin, f))  

/**
 * static ptrdiff_t f000() {return 0x000;}
 * static ptrdiff_t f004() {return 0x004;}
 * static ptrdiff_t f008() {return 0x008;}
 * ...
 */
#define GEODE_ADDRESSER_METHOD_DEFINE(hex)           \
static ptrdiff_t GEODE_CONCAT(f, hex)() {return GEODE_CONCAT(0x, hex);}

/**
 * virtual ptrdiff_t v000() {}
 * virtual ptrdiff_t v004() {}
 * virtual ptrdiff_t v008() {}
 * ...
 */
#define GEODE_ADDRESSER_VMETHOD_DEFINE(hex)          \
virtual void GEODE_CONCAT(v, hex)() {}

/**
 * (intptr_t)Addresser::f000,
 * (intptr_t)Addresser::f004,
 * (intptr_t)Addresser::f008,
 * ...
 */
#define GEODE_ADDRESSER_TABLE_DEFINE(hex)            \
(intptr_t)GEODE_CONCAT(Addresser::f, hex),

/**
 * &Addresser::v000,
 * &Addresser::v004,
 * &Addresser::v008,
 * ...
 */
#define GEODE_ADDRESSER_VTABLE_DEFINE(hex)          \
&GEODE_CONCAT(Addresser::v, hex),

#define GEODE_ADDRESSER_METHOD_SET() GEODE_ADDRESSER_NEST3(GEODE_ADDRESSER_METHOD_DEFINE, )
#define GEODE_ADDRESSER_VMETHOD_SET() GEODE_ADDRESSER_NEST3(GEODE_ADDRESSER_VMETHOD_DEFINE, )
#define GEODE_ADDRESSER_TABLE_SET() GEODE_ADDRESSER_NEST3(GEODE_ADDRESSER_TABLE_DEFINE, )
#define GEODE_ADDRESSER_VTABLE_SET() GEODE_ADDRESSER_NEST3(GEODE_ADDRESSER_VTABLE_DEFINE, )

namespace geode::addresser {

	template<typename T>
	inline intptr_t getVirtual(T func);

	template<typename T>
	inline intptr_t getNonVirtual(T func);

	class Addresser final {
		static constexpr ptrdiff_t table_size = 0x1000 / sizeof(intptr_t);

		using tablemethodptr_t = ptrdiff_t(Addresser::*)();
		using methodptr_t = void(Addresser::*)();
		using table_t = intptr_t[table_size + 0x1]; 
		using tableptr_t = table_t*; 
		using vtable_t = methodptr_t[table_size + 0x1]; 
		using vtableptr_t = vtable_t*; 

		

		template<typename T>
		static ptrdiff_t indexOf(T ptr) { 
			auto func = reinterpret_cast<tablemethodptr_t&>(ptr);
			return (instance->*func)(); 
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
		template <typename R, typename T, typename ...Ps>
		static intptr_t addressOfVirtual(R(T::*func)(Ps...)) {
			static_assert(std::is_copy_constructible<T>::value, "must be copy constructable");

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

		template <typename R, typename T, typename ...Ps>
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

		GEODE_ADDRESSER_VMETHOD_SET()
		GEODE_ADDRESSER_METHOD_SET()
		static ptrdiff_t f() {return -1;} //because c++ cries when there is a trailing comma 
		virtual void v() {}

		inline static table_t table = {
			GEODE_ADDRESSER_TABLE_SET()
			(intptr_t)Addresser::f
		};
		inline static tableptr_t tableptr = &table;
		inline static Addresser* instance = reinterpret_cast<Addresser*>(&tableptr);

		inline static vtable_t vtable = {
			GEODE_ADDRESSER_VTABLE_SET()
			&Addresser::v
		};

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

/**
 * Cleanup
 */
#undef GEODE_ADDRESSER_NEST1
#undef GEODE_ADDRESSER_NEST2
#undef GEODE_ADDRESSER_NEST3
#undef GEODE_ADDRESSER_METHOD_DEFINE
#undef GEODE_ADDRESSER_TABLE_DEFINE
#undef GEODE_ADDRESSER_VMETHOD_DEFINE
#undef GEODE_ADDRESSER_VTABLE_DEFINE
#undef GEODE_ADDRESSER_METHOD_SET
#undef GEODE_ADDRESSER_TABLE_SET
#undef GEODE_ADDRESSER_VMETHOD_SET
#undef GEODE_ADDRESSER_VTABLE_SET
