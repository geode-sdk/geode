// 
// Copyright camila314 & alk1m123 2022. 
//
#pragma once 

/**
 * Adapted from https://gist.github.com/altalk23/29b97969e9f0624f783b673f6c1cd279
 */

#include <cstdlib>
#include <stddef.h>

#include <PlatformBase.hpp>
#include <MacroBase.hpp>
#include <Base.hpp>

#if INT64_MAX == INTPTR_MAX
	#define NEST1(macro, begin)     \
	macro(CONCAT(begin, 0))         \
	macro(CONCAT(begin, 8))         
#else
	#define NEST1(macro, begin)     \
	macro(CONCAT(begin, 0))         \
	macro(CONCAT(begin, 4))         \
	macro(CONCAT(begin, 8))         \
	macro(CONCAT(begin, c))         
#endif

#define NEST2(macro, begin)     \
NEST1(macro, CONCAT(begin, 0))  \
NEST1(macro, CONCAT(begin, 1))  \
NEST1(macro, CONCAT(begin, 2))  \
NEST1(macro, CONCAT(begin, 3))  \
NEST1(macro, CONCAT(begin, 4))  \
NEST1(macro, CONCAT(begin, 5))  \
NEST1(macro, CONCAT(begin, 6))  \
NEST1(macro, CONCAT(begin, 7))  \
NEST1(macro, CONCAT(begin, 8))  \
NEST1(macro, CONCAT(begin, 9))  \
NEST1(macro, CONCAT(begin, a))  \
NEST1(macro, CONCAT(begin, b))  \
NEST1(macro, CONCAT(begin, c))  \
NEST1(macro, CONCAT(begin, d))  \
NEST1(macro, CONCAT(begin, e))  \
NEST1(macro, CONCAT(begin, f))  \

#define NEST3(macro, begin)     \
NEST2(macro, CONCAT(begin, 0))  \
NEST2(macro, CONCAT(begin, 1))  \
NEST2(macro, CONCAT(begin, 2))  \
NEST2(macro, CONCAT(begin, 3))  \
NEST2(macro, CONCAT(begin, 4))  \
NEST2(macro, CONCAT(begin, 5))  \
NEST2(macro, CONCAT(begin, 6))  \
NEST2(macro, CONCAT(begin, 7))  \
NEST2(macro, CONCAT(begin, 8))  \
NEST2(macro, CONCAT(begin, 9))  \
NEST2(macro, CONCAT(begin, a))  \
NEST2(macro, CONCAT(begin, b))  \
NEST2(macro, CONCAT(begin, c))  \
NEST2(macro, CONCAT(begin, d))  \
NEST2(macro, CONCAT(begin, e))  \
NEST2(macro, CONCAT(begin, f))  \

/**
 * static ptrdiff_t function0x000() {return 0x000;}
 * static ptrdiff_t function0x004() {return 0x004;}
 * static ptrdiff_t function0x008() {return 0x008;}
 * ...
 */
#define METHOD_DEFINE(hex) static ptrdiff_t CONCAT(function, hex)() {return hex;}

/**
 * virtual ptrdiff_t vfunction0x000() {}
 * virtual ptrdiff_t vfunction0x004() {}
 * virtual ptrdiff_t vfunction0x008() {}
 * ...
 */
#define VMETHOD_DEFINE(hex) virtual void CONCAT(vfunction, hex)() {}

/**
 * (uintptr_t)FunctionScrapper::function0x000,
 * (uintptr_t)FunctionScrapper::function0x004,
 * (uintptr_t)FunctionScrapper::function0x008,
 * ...
 */
#define TABLE_DEFINE(hex) (uintptr_t)CONCAT(FunctionScrapper::function, hex),

/**
 * &FunctionScrapper::vfunction0x000,
 * &FunctionScrapper::vfunction0x004,
 * &FunctionScrapper::vfunction0x008,
 * ...
 */
#define VTABLE_DEFINE(hex) &CONCAT(FunctionScrapper::vfunction, hex),

#define METHOD_SET() NEST3(METHOD_DEFINE, 0x)
#define VMETHOD_SET() NEST3(VMETHOD_DEFINE, 0x)
#define TABLE_SET() NEST3(TABLE_DEFINE, 0x)
#define VTABLE_SET() NEST3(VTABLE_DEFINE, 0x)


class FunctionScrapper {
protected:
	static constexpr ptrdiff_t table_size = 0x1000 / sizeof(uintptr_t);

	using tablemethodptr_t = ptrdiff_t(FunctionScrapper::*)();
	using methodptr_t = void(FunctionScrapper::*)();
	using table_t = uintptr_t[table_size + 0x1]; 
	using tableptr_t = table_t*; 
	using vtable_t = methodptr_t[table_size + 0x1]; 
	using vtableptr_t = vtable_t*; 

public:
	template<typename T>
	static uintptr_t pointerOf(T func) {
		return reinterpret_cast<uintptr_t&>(func);
	}

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

public:

	/**
	 * Specialized functions
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

		// [[this + thunk] + offset] is the function we want
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
	static uintptr_t addressOfVirtual(T* ins, R(T::*func)(Ps...)) {
		auto address = *(uintptr_t*)(*(uintptr_t*)(pointerOf(ins) + thunkOf(func)) + indexOf(func));
		return address;
	}

	template <typename R, typename T, typename ...Ps>
	static uintptr_t addressOfNonVirtual(R(T::*func)(Ps...) const) {
		return addressOfNonVirtual(func);
	}

	template <typename R, typename T, typename ...Ps>
	static uintptr_t addressOfNonVirtual(R(T::*func)(Ps...)) {
		return pointerOf(func);
	}

	template <typename R, typename ...Ps>
	static uintptr_t addressOfNonVirtual(R(*func)(Ps...)) {
		return pointerOf(func);
	}

protected:
	VMETHOD_SET()
	METHOD_SET()
	static ptrdiff_t function() {return -1;}//because c++ cries when there is a trailing comma 
	virtual void vfunction() {}

	inline static table_t table = {
		TABLE_SET()
		(uintptr_t)FunctionScrapper::function
	};
	inline static tableptr_t tableptr = &table;
	inline static FunctionScrapper* instance = reinterpret_cast<FunctionScrapper*>(&tableptr);

	inline static vtable_t vtable = {
		VTABLE_SET()
		&FunctionScrapper::vfunction
	};
};

/**
 * Cleanup
 */
#undef NEST1
#undef NEST2
#undef NEST3
#undef METHOD_DEFINE
#undef TABLE_DEFINE
#undef VMETHOD_DEFINE
#undef VTABLE_DEFINE
#undef METHOD_SET
#undef TABLE_SET
#undef VMETHOD_SET
#undef VTABLE_SET
