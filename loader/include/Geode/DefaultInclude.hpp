#pragma once

#include <Geode/Prelude.hpp>
#include <Geode/c++stl/gdstdlib.hpp>
#include <Geode/platform/platform.hpp>
#include <variant>

#define GEODE_STATIC_PTR(type, name)          \
    static type* s_##name;                    \
    inline type* name() {                     \
        if (!s_##name) s_##name = new type(); \
        return s_##name;                      \
    }

#define GEODE_STATIC_VAR(type, name) \
    inline type& name() {            \
        static type s_##name;        \
        return s_##name;             \
    }

#define GEODE_WRAPPER_CONCAT(x, y) x##y
#define GEODE_CONCAT(x, y) GEODE_WRAPPER_CONCAT(x, y)

#define GEODE_WRAPPER_STR(...) #__VA_ARGS__
#define GEODE_STR(...) GEODE_WRAPPER_STR(__VA_ARGS__)

#define GEODE_PAD(size) uint8_t GEODE_CONCAT(__pad, __LINE__)[size]
#define GEODE_UNIMPLEMENTED_PAD private:

#define GEODE_NONINHERITED_MEMBERS private:

#define GEODE_EXPAND(x) x
#define GEODE_INVOKE(macro, ...) GEODE_EXPAND(macro(__VA_ARGS__))

namespace geode {
    struct ZeroConstructorType {};

    static constexpr auto ZeroConstructor = ZeroConstructorType();

    struct CutoffConstructorType {};

    static constexpr auto CutoffConstructor = CutoffConstructorType();
}

#define GEODE_CUSTOM_CONSTRUCTOR_BEGIN(Class_) \
    GEODE_ZERO_CONSTRUCTOR_BEGIN(Class_)       \
    GEODE_CUTOFF_CONSTRUCTOR_BEGIN(Class_)

#define GEODE_CUSTOM_CONSTRUCTOR_COCOS(Class_, Base_) \
    GEODE_ZERO_CONSTRUCTOR(Class_, Base_)             \
    GEODE_CUTOFF_CONSTRUCTOR_COCOS(Class_, Base_)

#define GEODE_CUSTOM_CONSTRUCTOR_GD(Class_, Base_) \
    GEODE_ZERO_CONSTRUCTOR(Class_, Base_)          \
    GEODE_CUTOFF_CONSTRUCTOR_GD(Class_, Base_)

#define GEODE_CUSTOM_CONSTRUCTOR_CUTOFF(Class_, Base_) \
    GEODE_ZERO_CONSTRUCTOR(Class_, Base_)              \
    GEODE_CUTOFF_CONSTRUCTOR_CUTOFF(Class_, Base_)

#define GEODE_ZERO_CONSTRUCTOR_BEGIN(Class_)                                              \
    Class_(geode::ZeroConstructorType, void*) {}                                          \
    Class_(geode::ZeroConstructorType, size_t fill) :                                     \
        Class_(geode::ZeroConstructor, std::memset(static_cast<void*>(this), 0, fill)) {} \
    Class_(geode::ZeroConstructorType) : Class_(geode::ZeroConstructor, nullptr) {}

#define GEODE_ZERO_CONSTRUCTOR(Class_, Base_)                                                \
    Class_(geode::ZeroConstructorType, size_t fill) : Base_(geode::ZeroConstructor, fill) {} \
    Class_(geode::ZeroConstructorType) : Base_(geode::ZeroConstructor, sizeof(Class_)) {}

#define GEODE_FILL_CONSTRUCTOR(Class_, Offset_)                                          \
    Class_(geode::CutoffConstructorType, size_t fill) :                                  \
        Class_(                                                                          \
            geode::CutoffConstructor,                                                    \
            std::memset(reinterpret_cast<std::byte*>(this) + Offset_, 0, fill - Offset_) \
        ) {}                                                                             \
    Class_(geode::CutoffConstructorType, void*)

#define GEODE_CUTOFF_CONSTRUCTOR_BEGIN(Class_)                      \
    GEODE_MACOS(GEODE_FILL_CONSTRUCTOR(Class_, 0){})                \
    GEODE_IOS(GEODE_FILL_CONSTRUCTOR(Class_, 0){})                  \
    GEODE_WINDOWS(Class_(geode::CutoffConstructorType, size_t fill) \
                  : Class_() {})                                    \
    GEODE_ANDROID(GEODE_FILL_CONSTRUCTOR(Class_, 0){})

#define GEODE_CUTOFF_CONSTRUCTOR_COCOS(Class_, Base_)               \
    GEODE_MACOS(Class_(geode::CutoffConstructorType, size_t fill)   \
                : Base_(geode::CutoffConstructor, fill){})          \
    GEODE_IOS(Class_(geode::CutoffConstructorType, size_t fill)     \
              : Base_(geode::CutoffConstructor, fill){})            \
    GEODE_WINDOWS(Class_(geode::CutoffConstructorType, size_t fill) \
                  : Class_() {})                                    \
    GEODE_ANDROID(Class_(geode::CutoffConstructorType, size_t fill)   \
                : Base_(geode::CutoffConstructor, fill){})

#define GEODE_CUTOFF_CONSTRUCTOR_GD(Class_, Base_)                  \
    GEODE_WINDOWS(Class_(geode::CutoffConstructorType, size_t fill) \
                  : Base_(geode::CutoffConstructor, fill){})        \
    GEODE_MACOS(Class_(geode::CutoffConstructorType, size_t fill)   \
                : Base_(geode::CutoffConstructor, fill){})          \
    GEODE_IOS(Class_(geode::CutoffConstructorType, size_t fill)     \
              : Base_(geode::CutoffConstructor, fill){})            \
    GEODE_ANDROID(Class_(geode::CutoffConstructorType, size_t fill) \
              : Base_(geode::CutoffConstructor, fill){})

#define GEODE_CUTOFF_CONSTRUCTOR_CUTOFF(Class_, Base_)                       \
    GEODE_WINDOWS(GEODE_FILL_CONSTRUCTOR(Class_, sizeof(Base_)) : Base_(){}) \
    GEODE_ANDROID(Class_(geode::CutoffConstructorType, size_t fill)          \
                : Base_(geode::CutoffConstructor, fill){})                   \
    GEODE_MACOS(Class_(geode::CutoffConstructorType, size_t fill)            \
                : Base_(geode::CutoffConstructor, fill){})                   \
    GEODE_IOS(Class_(geode::CutoffConstructorType, size_t fill)              \
              : Base_(geode::CutoffConstructor, fill){})

#define GEODE_NUMBER_OF_ARGS(...) \
    GEODE_EXPAND(GEODE_NUMBER_OF_ARGS_(__VA_ARGS__, GEODE_NUMBER_SEQUENCE(), ))
#define GEODE_NUMBER_OF_ARGS_(...) GEODE_EXPAND(GEODE_NUMBER_OF_ARGS_N(__VA_ARGS__))
#define GEODE_NUMBER_OF_ARGS_N( \
    _1,                         \
    _2,                         \
    _3,                         \
    _4,                         \
    _5,                         \
    _6,                         \
    _7,                         \
    _8,                         \
    _9,                         \
    _10,                        \
    _11,                        \
    _12,                        \
    _13,                        \
    _14,                        \
    _15,                        \
    _16,                        \
    _17,                        \
    _18,                        \
    _19,                        \
    _20,                        \
    _21,                        \
    _22,                        \
    _23,                        \
    _24,                        \
    _25,                        \
    _26,                        \
    _27,                        \
    _28,                        \
    _29,                        \
    _30,                        \
    _31,                        \
    _32,                        \
    _33,                        \
    _34,                        \
    _35,                        \
    _36,                        \
    _37,                        \
    _38,                        \
    _39,                        \
    _40,                        \
    _41,                        \
    _42,                        \
    _43,                        \
    _44,                        \
    _45,                        \
    _46,                        \
    _47,                        \
    _48,                        \
    _49,                        \
    _50,                        \
    _51,                        \
    _52,                        \
    _53,                        \
    _54,                        \
    _55,                        \
    _56,                        \
    _57,                        \
    _58,                        \
    _59,                        \
    _60,                        \
    _61,                        \
    _62,                        \
    _63,                        \
    N,                          \
    ...                         \
)                               \
    N
#define GEODE_NUMBER_SEQUENCE()                                                                 \
    63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, \
        40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, \
        18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define $execute                                                                                  \
    template <class>                                                                              \
    void GEODE_CONCAT(geodeExecFunction, __LINE__)();                                             \
    namespace {                                                                                   \
        struct GEODE_CONCAT(ExecFuncUnique, __LINE__) {};                                         \
    }                                                                                             \
    static inline auto GEODE_CONCAT(Exec, __LINE__) =                                             \
        (GEODE_CONCAT(geodeExecFunction, __LINE__) < GEODE_CONCAT(ExecFuncUnique, __LINE__) > (), \
         0);                                                                                      \
    template <class>                                                                              \
    void GEODE_CONCAT(geodeExecFunction, __LINE__)()

#define GEODE_FORWARD_COMPAT_DISABLE_HOOKS_INNER(message) \
    if (Loader::get()->isForwardCompatMode()) {           \
        if (strlen(message)) {                            \
            log::warn("[Forward Compat] " message);       \
        }                                                 \
        for (const auto& [_, hook] : self.m_hooks) {      \
            hook->setAutoEnable(false);                   \
        }                                                 \
    }
#define GEODE_FORWARD_COMPAT_ENABLE_HOOKS_INNER(message)  \
    if (!Loader::get()->isForwardCompatMode()) {          \
        if (strlen(message)) {                            \
            log::warn("[Forward Compat] " message);       \
        }                                                 \
        for (const auto& [_, hook] : self.m_hooks) {      \
            hook->setAutoEnable(false);                   \
        }                                                 \
    }
#define GEODE_FORWARD_COMPAT_DISABLE_HOOKS(message)       \
    static void onModify(const auto& self) {              \
        GEODE_FORWARD_COMPAT_DISABLE_HOOKS_INNER(message) \
    }
#define GEODE_FORWARD_COMPAT_ENABLE_HOOKS(message)        \
    static void onModify(const auto& self) {              \
        GEODE_FORWARD_COMPAT_ENABLE_HOOKS_INNER(message)  \
    }

// #define GEODE_NEST1(macro, begin)           \
// macro(GEODE_CONCAT(begin, 0)),                        \
// macro(GEODE_CONCAT(begin, 1)),                        \
// macro(GEODE_CONCAT(begin, 2)),                        \
// macro(GEODE_CONCAT(begin, 3)),                        \
// macro(GEODE_CONCAT(begin, 4)),                        \
// macro(GEODE_CONCAT(begin, 5)),                        \
// macro(GEODE_CONCAT(begin, 6)),                        \
// macro(GEODE_CONCAT(begin, 7)),                        \
// macro(GEODE_CONCAT(begin, 8)),                        \
// macro(GEODE_CONCAT(begin, 9)),                        \
// macro(GEODE_CONCAT(begin, a)),                        \
// macro(GEODE_CONCAT(begin, b)),                        \
// macro(GEODE_CONCAT(begin, c)),                        \
// macro(GEODE_CONCAT(begin, d)),                        \
// macro(GEODE_CONCAT(begin, e)),                        \
// macro(GEODE_CONCAT(begin, f))

// #define GEODE_NEST2(macro, begin)           \
// GEODE_NEST1(macro, GEODE_CONCAT(begin, 0)), \
// GEODE_NEST1(macro, GEODE_CONCAT(begin, 1)), \
// GEODE_NEST1(macro, GEODE_CONCAT(begin, 2)), \
// GEODE_NEST1(macro, GEODE_CONCAT(begin, 3)), \
// GEODE_NEST1(macro, GEODE_CONCAT(begin, 4)), \
// GEODE_NEST1(macro, GEODE_CONCAT(begin, 5)), \
// GEODE_NEST1(macro, GEODE_CONCAT(begin, 6)), \
// GEODE_NEST1(macro, GEODE_CONCAT(begin, 7)), \
// GEODE_NEST1(macro, GEODE_CONCAT(begin, 8)), \
// GEODE_NEST1(macro, GEODE_CONCAT(begin, 9)), \
// GEODE_NEST1(macro, GEODE_CONCAT(begin, a)), \
// GEODE_NEST1(macro, GEODE_CONCAT(begin, b)), \
// GEODE_NEST1(macro, GEODE_CONCAT(begin, c)), \
// GEODE_NEST1(macro, GEODE_CONCAT(begin, d)), \
// GEODE_NEST1(macro, GEODE_CONCAT(begin, e)), \
// GEODE_NEST1(macro, GEODE_CONCAT(begin, f))

// #define GEODE_NEST3(macro, begin)           \
// GEODE_NEST2(macro, GEODE_CONCAT(begin, 0)), \
// GEODE_NEST2(macro, GEODE_CONCAT(begin, 1)), \
// GEODE_NEST2(macro, GEODE_CONCAT(begin, 2)), \
// GEODE_NEST2(macro, GEODE_CONCAT(begin, 3)), \
// GEODE_NEST2(macro, GEODE_CONCAT(begin, 4)), \
// GEODE_NEST2(macro, GEODE_CONCAT(begin, 5)), \
// GEODE_NEST2(macro, GEODE_CONCAT(begin, 6)), \
// GEODE_NEST2(macro, GEODE_CONCAT(begin, 7)), \
// GEODE_NEST2(macro, GEODE_CONCAT(begin, 8)), \
// GEODE_NEST2(macro, GEODE_CONCAT(begin, 9)), \
// GEODE_NEST2(macro, GEODE_CONCAT(begin, a)), \
// GEODE_NEST2(macro, GEODE_CONCAT(begin, b)), \
// GEODE_NEST2(macro, GEODE_CONCAT(begin, c)), \
// GEODE_NEST2(macro, GEODE_CONCAT(begin, d)), \
// GEODE_NEST2(macro, GEODE_CONCAT(begin, e)), \
// GEODE_NEST2(macro, GEODE_CONCAT(begin, f))

// #define GEODE_NEST4(macro, begin)           \
// GEODE_NEST3(macro, GEODE_CONCAT(begin, 0)), \
// GEODE_NEST3(macro, GEODE_CONCAT(begin, 1)), \
// GEODE_NEST3(macro, GEODE_CONCAT(begin, 2)), \
// GEODE_NEST3(macro, GEODE_CONCAT(begin, 3)), \
// GEODE_NEST3(macro, GEODE_CONCAT(begin, 4)), \
// GEODE_NEST3(macro, GEODE_CONCAT(begin, 5)), \
// GEODE_NEST3(macro, GEODE_CONCAT(begin, 6)), \
// GEODE_NEST3(macro, GEODE_CONCAT(begin, 7)), \
// GEODE_NEST3(macro, GEODE_CONCAT(begin, 8)), \
// GEODE_NEST3(macro, GEODE_CONCAT(begin, 9)), \
// GEODE_NEST3(macro, GEODE_CONCAT(begin, a)), \
// GEODE_NEST3(macro, GEODE_CONCAT(begin, b)), \
// GEODE_NEST3(macro, GEODE_CONCAT(begin, c)), \
// GEODE_NEST3(macro, GEODE_CONCAT(begin, d)), \
// GEODE_NEST3(macro, GEODE_CONCAT(begin, e)), \
// GEODE_NEST3(macro, GEODE_CONCAT(begin, f))

// #define GEODE_ENUM_OFFSETS_DEFINE(hex) GEODE_CONCAT($, hex)
// #define GEODE_ENUM_OFFSETS_SET() GEODE_NEST4(GEODE_ENUM_OFFSETS_DEFINE, 0x)

// enum class PrinterOffsets {
//    GEODE_ENUM_OFFSETS_SET()
// };
