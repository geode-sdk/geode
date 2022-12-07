#include <type_traits>

/**
 * Main class implementation, it has the structure
 *
 * class hook0Dummy;
 * template<typename>
 * struct hook0 {};
 * namespace {
 *     struct hook0Parent {};
 * }
 * template<>
 * struct GEODE_HIDDEN hook0<hook0Parent> : Modify<hook0<hook0Parent>, MenuLayer> {
 *     // code stuff idk
 * };
 *
 * I tried to make the macro as verbose as it can be but
 * I am bad at this stuff
 */

#define GEODE_MODIFY_DECLARE_ANONYMOUS(base, derived) \
    derived##Dummy;                                   \
    template <class>                                  \
    struct derived {};                                \
    namespace {                                       \
        struct derived##Parent {};                    \
    }                                                 \
    template <>                                       \
    struct GEODE_HIDDEN derived<derived##Parent> : geode::Modify<derived<derived##Parent>, base>

#define GEODE_MODIFY_DECLARE(base, derived) \
    derived##Dummy;                         \
    struct GEODE_HIDDEN derived : geode::Modify<derived, base>

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
#define $modify(...) \
    GEODE_INVOKE(GEODE_CONCAT(GEODE_CRTP, GEODE_NUMBER_OF_ARGS(__VA_ARGS__)), __VA_ARGS__)
#define $(...) $modify(__VA_ARGS__)

/**
 * Get current hook class without needing to name it.
 * Useful for callbacks
 */
#define $cls std::remove_pointer<decltype(this)>::type
