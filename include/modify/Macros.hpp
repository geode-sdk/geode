/**
 * Main class implementation, it has the structure
 * 
 * class hook0Dummy;
 * template<typename>
 * struct _hook0 final {};
 * namespace {
 *     struct hook0UUID {};
 *     Modify<_hook0<hook0UUID>, MenuLayer> hook0Apply;
 * }
 * using hook0 = _hook0<hook0UUID>;
 * template<>
 * struct GEODE_HIDDEN _hook0<hook0UUID>: public MenuLayer {
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
 * class $modify(MenuLayer) {};
 * class $modify(MyMenuLayerInterface, MenuLayer) {};
 */

#define GEODE_CRTP1(base) GEODE_MODIFY_REDIRECT1(base)
#define GEODE_CRTP2(derived, base) GEODE_MODIFY_REDIRECT3(base, derived)
#define $modify(...) GEODE_INVOKE(GEODE_CONCAT(GEODE_CRTP, GEODE_NUMBER_OF_ARGS(__VA_ARGS__)), __VA_ARGS__)
#define $(...) $modify(__VA_ARGS__)

#define GEODE_ONLY_FIELD(type, field) private: field<type> field; public:
#define GEODE_INTERNAL_FIELD(type, field, name) inline type& name() { return this->*field; }
#define GEODE_EXTERNAL_FIELD(type, field, name) static inline type& name##From(void* self) { return reinterpret_cast<decltype(this)>(self)->*field; }
#define GEODE_FIELD(type, field, name) GEODE_ONLY_FIELD(type, field) GEODE_INTERNAL_FIELD(type, field, name) GEODE_EXTERNAL_FIELD(type, field, name)
