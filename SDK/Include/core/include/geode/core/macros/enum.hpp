#pragma once

#include "foreach.hpp"

#define GEODE_FIRST_ARG_(N, ...) N
#define GEODE_FIRST_ARG(args) GEODE_FIRST_ARG_ args

#define GEODE_FIRST_TYPE(...) decltype(GEODE_FIRST_ARG((__VA_ARGS__)))
#define GEODE_SWITCH_CASE(x) case x: return #x;
#define GEODE_SWITCH_CASE_R(x) case geode::utils::hash(#x): return x;

#define GEODE_DECL_ENUMERATIVE(_name_, ...)                             \
    struct _name_ {                                                     \
    enum { __VA_ARGS__ };                                               \
    using Type = GEODE_FIRST_TYPE(__VA_ARGS__);                         \
    Type m_value;                                                       \
    _name_(Type t) { m_value = t; }                                     \
    _name_& operator=(Type t) { m_value = t; }                          \
    bool operator==(int other) const { return m_value == other; }       \
    bool operator==(_name_ const& other) const { return m_value == other.m_value; } \
    operator int() { return m_value; }                                  \
    template<class T>                                                   \
    static Type cast(T t) {                                             \
        return static_cast<Type>(t);                                    \
    }                                                                   \
    static constexpr const char* toString(Type t) {                     \
        switch (t) { GEODE_FOR_EACH(GEODE_SWITCH_CASE, , __VA_ARGS__) } \
        return "Undefined";                                             \
    }                                                                   \
    static constexpr Type fromString(const char* s) {                   \
        switch (geode::utils::hash(s)) { GEODE_FOR_EACH(GEODE_SWITCH_CASE_R, , __VA_ARGS__) } \
        return GEODE_FIRST_ARG((__VA_ARGS__));                          \
    }                                                                   \
    }                                                                   \
