#include <type_traits>

/**
 * Main class implementation, it has the structure
 * 
 * class hook0Dummy;
 * template<typename>
 * struct hook0 {};
 * namespace {
 *     struct hook0Parent {};
 *     Modify<hook0<hook0Parent>, MenuLayer> hook0Apply;
 *     struct GEODE_HIDDEN hook0Intermediate: public MenuLayer {
 *         geode::modifier::FieldIntermediate<MenuLayer,
 *             hook0<hook0Intermediate>, hook0<hook0Parent>
 *         > m_fields;
 *     };
 * }
 * template<>
 * struct GEODE_HIDDEN hook0<hook0Parent>: hook0Intermediate {
 *     // code stuff idk
 * };
 * 
 * I tried to make the macro as verbose as it can be but
 * I am bad at this stuff
 */

#define GEODE_MODIFY_DECLARE_ANONYMOUS(base, derived)                             \
derived##Dummy;                                                                   \
template<class> struct derived {};                                                \
namespace {                                                                       \
	struct derived##Parent {};                                                    \
	Modify<derived<derived##Parent>, base> derived##Apply;                        \
	struct GEODE_HIDDEN derived##Intermediate : base {                            \
		geode::modifier::FieldIntermediate<base,                                  \
			derived##Intermediate,                                                \
			derived<derived##Parent>                                              \
		> m_fields;                                                               \
	};                                                                            \
}                                                                                 \
template <> struct GEODE_HIDDEN derived<derived##Parent> : derived##Intermediate  \
   
#define GEODE_MODIFY_DECLARE(base, derived)                                       \
derived##Dummy;                                                                   \
struct derived;                                                                   \
namespace {                                                                       \
	Modify<derived, base> derived##Apply;                                         \
	struct GEODE_HIDDEN derived##Intermediate : base {                            \
		geode::modifier::FieldIntermediate<base,                                  \
			derived##Intermediate,                                                \
			derived                                                               \
		> m_fields;                                                               \
	};                                                                            \
}                                                                                 \
struct GEODE_HIDDEN derived : derived##Intermediate                               \

#define GEODE_MODIFY_REDIRECT4(base, derived) GEODE_MODIFY_DECLARE(base, derived)
#define GEODE_MODIFY_REDIRECT3(base, derived) GEODE_MODIFY_DECLARE_ANONYMOUS(base, derived)
#define GEODE_MODIFY_REDIRECT2(base) GEODE_MODIFY_REDIRECT3(base, GEODE_CONCAT(hook, __LINE__))
#define GEODE_MODIFY_REDIRECT1(base) GEODE_MODIFY_REDIRECT2(base)

/**
 * Interfaces for the class implementation
 * 
 * class $modify(MenuLayer) {};
 * class $modify(MyMenuLayerInterface, MenuLayer) {};
 */

#define GEODE_CRTP1(base) GEODE_MODIFY_REDIRECT1(base)
#define GEODE_CRTP2(derived, base) GEODE_MODIFY_REDIRECT4(base, derived)
#define $modify(...) GEODE_INVOKE(GEODE_CONCAT(GEODE_CRTP, GEODE_NUMBER_OF_ARGS(__VA_ARGS__)), __VA_ARGS__)
#define $(...) $modify(__VA_ARGS__)


/**
 * Get current hook class without needing to name it.
 * Useful for callbacks
 */
#define $cls std::remove_pointer<decltype(this)>::type

#define GEODE_ONLY_FIELD(type, field_, default_) private: field<type> field_ = default_; public:
#define GEODE_INTERNAL_FIELD(type, field, name) inline type& name() { return this->*field; }
//#define GEODE_EXTERNAL_FIELD(type, field, name) static inline type& name##From(void* self) { return reinterpret_cast<decltype(this)>(self)->*field; }
#define GEODE_FIELD(type, field, name, default_) GEODE_ONLY_FIELD(type, field, default_) GEODE_INTERNAL_FIELD(type, field, name) //GEODE_EXTERNAL_FIELD(type, field, name)


#define GEODE_EXECUTE_FUNC(Line_)                                 \
template<class>                                                   \
void _##Line_##Function();                                        \
namespace {                                                       \
	struct _##Line_##Unique {};                                   \
}                                                                 \
static inline auto _line = (Interface::get()->scheduleOnLoad(     \
	&_##Line_##Function<_##Line_##Unique>                         \
), 0);                                                            \
template<class>                                                   \
void _##Line_##Function()

#define $execute GEODE_EXECUTE_FUNC(__LINE__)
