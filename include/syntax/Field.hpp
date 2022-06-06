#pragma once
#include "Traits.hpp"
#include "../platform/platform.hpp"

#ifdef GEODE_IS_WINDOWS
// for the manual virtualprotect
#include <memoryapi.h>
#endif

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

	using DestructorType = void(GEODE_WINDOWS(__thiscall)*)(void* GEODE_WINDOWS(, bool));

    template <typename T>
    GEODE_NOINLINE inline auto& getOriginalDestructor() {
    	static DestructorType ret;
    	return ret;
    }

	// this is beautiful
	struct FieldCleanupHook {
		template <typename T>
		static GEODE_NOINLINE inline void GEODE_WINDOWS(__thiscall) fieldCleanup(T* self GEODE_WINDOWS(, bool arg)) {{
			for (auto& [mem, cont] : getAdditionalContainers<T>()[self]) {
				delete cont;
			}
			getAdditionalContainers<T>().erase(self);
			getOriginalDestructor<T>()(self GEODE_WINDOWS(, arg));
		}}
	};

	template <typename T, typename A>
	GEODE_NOINLINE T& operator->*(A* self, field<T>& member) {
	    // this replaces the destructor in the vtable
	    // only done this way to be performant
	    // because we are still not using inline hooking
	    auto& containers = getAdditionalContainers<A>();
	    auto& destructor = getOriginalDestructor<A>();

	    // on first use
	    if (destructor == nullptr) {
			// classes inheriting CCObject always have their destructor(s) as the 2nd (and 3rd on itanium) element in the vtable
			// choose 3rd elemnt (the second destructor) on itanium platforms because the first one gets inlined often
			static constexpr size_t index = GEODE_WINDOWS(1) GEODE_MACOS(2) GEODE_IOS(2) GEODE_ANDROID(2);
	        auto& vtableDestructor = index[*reinterpret_cast<uintptr_t**>(self)]; // i love this
	        destructor = reinterpret_cast<DestructorType>(vtableDestructor);
#ifdef GEODE_IS_WINDOWS
			DWORD old;
			VirtualProtect(reinterpret_cast<void*>(&vtableDestructor), sizeof(void*), PAGE_EXECUTE_READWRITE, &old);
#endif
	        vtableDestructor = reinterpret_cast<uintptr_t>(&FieldCleanupHook::fieldCleanup<A>);
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
