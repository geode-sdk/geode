#pragma once
#include "Traits.hpp"

namespace geode::modifier {
	template<typename T = void*>
	struct container; 
	template<typename T>
	struct field;

	template<typename T>
	struct container {
	    T* field;
		container() {
			field = new T();
		}
		container(T value) {
			field = new T();
			*field = value;
		}
	    virtual ~container() {
	        delete field;
	    }
	    T& get() {
	    	return *field;
	    }
	};

	using FieldDefault = std::pair<void*, container<void*>*>;

	GEODE_NOINLINE inline auto& fieldDefaultQueue() {
    	static std::vector<FieldDefault> ret;
    	return ret;
    }

	template<typename T>
	struct field {
		uint8_t padding;

		field() {
			auto cont = new container<T>();
			fieldDefaultQueue().push_back(
				{this, (container<void*>*)(cont)}
			);
		}

		field(T defaultValue) {
			auto cont = new container<T>(defaultValue);
			fieldDefaultQueue().push_back(
				{this, (container<void*>*)(cont)}
			);
		}

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

	    using type = T;
	};

	template <typename T>
	GEODE_NOINLINE inline auto& getAdditionalContainers() {
    	static std::unordered_map<T*, std::unordered_map<void*, container<void*>*>> ret;
    	return ret;
    }

    template <typename T>
    GEODE_NOINLINE inline auto& getOriginalDestructor() {
    	static void(*ret)(void*);
    	return ret;
    }

    template <typename T>
    GEODE_NOINLINE inline void fieldCleanup(T* self) {{
    	for (auto& [mem, cont] : getAdditionalContainers<T>()[self]) {
    		delete cont;
    	}
    	getAdditionalContainers<T>().erase(self);
    	getOriginalDestructor<T>()(self);
    }}

	template <typename T, typename A>
	GEODE_NOINLINE T& operator->*(A* self, field<T>& member) {
	    // this replaces the destructor in the vtable
	    // only done this way to be performant
	    // because we are still not using inline hooking
	    auto& containers = getAdditionalContainers<A>();
	    auto& destructor = getOriginalDestructor<A>();

	    // on first use
	    if (destructor == nullptr) {
	    	// takes the third element because all cocos classes have the destructor at third.
	        auto& vtableDestructor = 2[*(uintptr_t**)self]; // i love this
	        destructor = reinterpret_cast<void(*)(void*)>(vtableDestructor);
	        vtableDestructor = (uintptr_t)&fieldCleanup<A>;
	    }
	    if (containers[self].size() == 0) {
	    	void* obj = new A();
	    	operator delete(obj);
			for (auto& [fie, cont] : fieldDefaultQueue()) {
				containers[self].insert({member, cont});
			}
			fieldDefaultQueue().clear();
	    }

	    // gets the respective field
	    return reinterpret_cast<container<T>*>(containers[self][member])->get();
	}
}
