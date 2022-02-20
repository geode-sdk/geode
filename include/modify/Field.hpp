#pragma once
#include "Traits.hpp"

namespace geode::modifier {
	template<typename T>
	struct field {
		uint8_t padding;

	    template<typename Q>
	    void operator=(Q) {
	        static_assert(always_false<T>, "field shouldn't be used directly. it should be used with this->*myMember.");
	    }

	    field<T>* operator->() {
	        static_assert(always_false<T>, "field shouldn't be used directly. it should be used with this->*myMember.");
			return nullptr;
	    }

	    void operator*() {
	        static_assert(always_false<T>, "field shouldn't be used directly. it should be used with this->*myMember.");
	    } 

	    operator void*() {
	    	return reinterpret_cast<void*>(this);
	    }
	};

	template<typename T = void*>
	struct container {
	    virtual ~container() {
	        delete field;
	    }
	    T* field;

	    GEODE_NOINLINE T& get() {
	    	if (!field) field = new T();
	    	return *field;
	    }
	};

	template <typename T>
	GEODE_NOINLINE inline auto& getAdditionalFields() {
    	static std::unordered_map<T*, std::unordered_map<void*, container<void*>>> ret;
    	return ret;
    }

    template <typename T>
    GEODE_NOINLINE inline auto& getOriginalDestructor() {
    	static void(*ret)(void*);
    	return ret;
    }

    template <typename T>
    GEODE_NOINLINE inline void fieldCleanup(T* self) {{
    	getAdditionalFields<T>().erase(self);
    	getOriginalDestructor<T>()(self);
    }}

	template <typename T, typename A>
	GEODE_NOINLINE T& operator->*(A* self, field<T>& member) {
	    // this replaces the destructor in the vtable
	    // only done this way to be performant
	    // because we are still not using inline hooking
	    auto& fields = getAdditionalFields<A>();
	    auto& destructor = getOriginalDestructor<A>();

	    // on first use
	    if (destructor == nullptr) {
	    	// takes the third element because all cocos classes have the destructor at third.
	        auto& vtableDestructor = 2[*(uintptr_t**)self]; // i love this
	        destructor = reinterpret_cast<void(*)(void*)>(vtableDestructor);
	        vtableDestructor = (uintptr_t)&fieldCleanup<A>;
	    }

	    auto field = reinterpret_cast<container<T>*>(&fields[self][member]);

	    // gets the respective field
	    return field->get();
	}
}
