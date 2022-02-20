#pragma once

#include <Macros.hpp>
#include <HeaderBase.hpp>

#include <utils/addresser.hpp>
#include <unordered_map>
#include <type_traits>
#include <Interface.hpp>
#include <stdint.h>

namespace geode::core::meta {}

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
template<typename> struct _##derived final {};
#define GEODE_MODIFY_APPLY(base, derived) 								\
namespace { 															\
	struct derived##UUID {}; 											\
	Modify<_##derived<derived##UUID>, base> derived##Apply;             \
}
#define GEODE_MODIFY_DECLARE(base, derived) 							\
using derived = _##derived<derived##UUID>; 							    \
template <> 													        \
struct GEODE_HIDDEN _##derived<derived##UUID> final 				    \
	: public base

#define GEODE_MODIFY_REDIRECT4(base, derived) GEODE_MODIFY_PREDECLARE(derived) GEODE_MODIFY_APPLY(base, derived) GEODE_MODIFY_DECLARE(base, derived)
#define GEODE_MODIFY_REDIRECT3(base, derived) GEODE_MODIFY_REDIRECT4(base, derived)
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




