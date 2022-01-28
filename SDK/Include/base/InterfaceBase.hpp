#pragma once

#include <Macros.hpp>
#include <HeaderBase.hpp>

#include <utils/addresser.hpp>
#include <unordered_map>
#include <type_traits>
#include <Interface.hpp>
#include <stdint.h>

template<auto F>
struct address_of_t {
	static inline auto value = geode::base::get();
};

template<auto F>
inline auto address_of = address_of_t<F>::value;

/**
 * Main class implementation, it has the structure
 * 
 * class hook0__;
 * template<typename T>
 * struct _hook0 {};
 * namespace {
 *     struct hook0Unique {};
 *     bool hook0Apply = geode::modify::$MenuLayer<_hook0<hook0Unique>>::_apply();
 * }
 * using hook0 = _hook0<hook0Unique>;
 * template<>
 * struct hidden _hook0<hook0Unique>: public geode::modify::$MenuLayer<_hook0<hook0Unique>> {
 *     // code stuff idk
 * };
 * 
 * I tried to make the macro as verbose as it can be but
 * I am bad at this stuff
 */

#define GEODE_MODIFY_PREDECLARE(derived) 								\
derived##Dummy; 														\
template<auto, typename> struct _##derived final {};
#define GEODE_MODIFY_APPLY(base, derived) 								\
namespace { 															\
	struct derived##UUID {}; 											\
	bool derived##Apply = base<_##derived, derived##UUID>::_apply();  	\
}
#define GEODE_MODIFY_DECLARE(base, derived) 							\
using derived = _##derived<0, derived##UUID>; 							\
template <auto _orig> 													\
struct GEODE_HIDDEN _##derived<_orig, derived##UUID> final 				\
	: public base<_##derived, derived##UUID> 								

#define GEODE_MODIFY_REDIRECT4(base, derived) GEODE_MODIFY_PREDECLARE(derived) GEODE_MODIFY_APPLY(base, derived) GEODE_MODIFY_DECLARE(base, derived)
#define GEODE_MODIFY_REDIRECT3(base, derived) GEODE_MODIFY_REDIRECT4(geode::modify::$##base, derived)
#define GEODE_MODIFY_REDIRECT2(base) GEODE_MODIFY_REDIRECT3(base, GEODE_CONCAT(hook, __COUNTER__))
#define GEODE_MODIFY_REDIRECT1(base) GEODE_MODIFY_REDIRECT2(base)

/**
 * Interfaces for the class implementation
 * 
 * $redirect is for when you don't need the name of the class
 * class $modify(MenuLayer) {};
 * 
 * $implement is for when you need the name of the class
 * class $modify(MyMenuLayerInterface, MenuLayer) {};
 */

#define GEODE_CRTP1(base) GEODE_MODIFY_REDIRECT1(base)
#define GEODE_CRTP2(derived, base) GEODE_MODIFY_REDIRECT3(base, derived)
#define $modify(...) GEODE_INVOKE(GEODE_CONCAT(GEODE_CRTP, GEODE_NUMBER_OF_ARGS(__VA_ARGS__)), __VA_ARGS__)
#define $(...) $modify(__VA_ARGS__)


namespace geode {
	struct GEODE_DLL temp_name_find_better {
        #include <Gen/TempName.hpp>
    };

    struct modify {
    	template <auto orig = 0, typename uuid = void>
		class BlankBase {};

		using Addresser = addresser::Addresser;

		class ModifierBase {
		public:
		    void _apply() {}
		    static void fieldCleanup(size_t self) {}
		};

		template<typename T = void*>
		struct container_t {
			GEODE_NOINLINE container_t(T value) : field(value) {}

		    virtual ~container_t() {
		        field.~T();
		    }
		    T field;
		};

		GEODE_NOINLINE static inline auto& fieldQueue() {
			static std::unordered_map<uintptr_t, container_t<>*> ret;
			return ret;
		}

        #include <Gen/Interface.hpp>
    };
}

template<typename T>
struct field_t {
	uint64_t padding;
	GEODE_NOINLINE field_t() {
		geode::modify::fieldQueue()[(uintptr_t)this] = 
			new geode::modify::container_t(T());
	}

	GEODE_NOINLINE field_t(T value) {
		geode::modify::fieldQueue()[(uintptr_t)this] = 
			reinterpret_cast<geode::modify::container_t<>*>
			(new geode::modify::container_t(value));
	}

    template<typename Q>
    void operator=(Q) {
        static_assert(!std::is_same_v<T, T>, "field_t shouldn't be used directly. it should be used with this->*myMember.");
    }

    void operator->() {
        static_assert(!std::is_same_v<T, T>, "field_t shouldn't be used directly. it should be used with this->*myMember.");
    }

    void operator*() {
        static_assert(!std::is_same_v<T, T>, "field_t shouldn't be used directly. it should be used with this->*myMember.");
    } 

    operator uintptr_t() {
    	return reinterpret_cast<uintptr_t>(this);
    }
};



template <typename T, typename A>
T& operator->*(A* self, field_t<T>& member) {
    // this replaces the destructor in the vtable
    // only done this way to be performant
    auto& destructor = A::getOriginalDestructor();
    if (destructor == 0) {
        auto& dtor = 2[*(size_t**)self]; // i love this
        destructor = dtor;
        dtor = (size_t)&A::fieldCleanup;
    }

    using container_t = geode::modify::container_t<T>;

    auto& fields = A::getAdditionalFields();

    // set the default values on first use
    if (fields.find(member) == fields.end()) {
    	// add the default values to queue
    	auto dummy = A();
    	auto& queue = geode::modify::fieldQueue();

    	for (auto& [k, v] : queue) {
    		fields[k - (uintptr_t)&dummy + (uintptr_t)self] = v;
    	}
    	queue.clear();
    	// this is here to silence the warnings about 
    	// unused variables, if there is any
    	(void)dummy;
    }
    // gets the respective field
    return reinterpret_cast<container_t*>(fields[member])->field;
}
