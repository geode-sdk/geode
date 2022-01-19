// 
// Copyright camila314 & alk1m123 2022. 
//
#pragma once

#include <Base.hpp>
#include <MacroBase.hpp>
#include <PlatformBase.hpp>

#include <Gen/Header.hpp>
#include <Cacao.hpp>
#include <lilac.hpp>

#include <FunctionBase.hpp>
#include <unordered_map>

#include <type_traits>

struct TmpModInfo {
    uintptr_t hookAddr;
    uintptr_t funcLocation;
};

struct ModStorage {
    std::vector<TmpModInfo> m_mods;

    static inline ModStorage& create() {
        auto a = new ModStorage;
        return *a;
    }
    inline void registerHook(uintptr_t address, uintptr_t function) {
        m_mods.push_back({address, function});
    }
};
inline auto& modContainer = ModStorage::create();

template<typename T>
struct field_t {
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
};

template<typename T = void*>
struct container_t {
    virtual ~container_t() {
        field.~T();
    }
    T field;
};

template <typename T, typename A>
T& operator->*(A* self, field_t<T>& member) {
    // this replaces the destructor in the vtable
    // only done this way to be performant
    if (A::originalDestructor == 0) {
        auto& dtor = 2[*(size_t**)self]; // i love this
        A::originalDestructor = dtor;
        dtor = (size_t)&A::fieldCleanup;
    }

    // gets the respective field
    container_t<>*& field = A::fields[(size_t)&member];
    // create the container on first use
    if (!field) field = reinterpret_cast<container_t<>*>(new container_t<T>());
    return reinterpret_cast<container_t<T>*>(field)->field;
}

template <typename t = void, auto orig = 0>
class __unitSpec {};

class InterfaceBase {
  public:
    void _apply() {}
    static void fieldCleanup(size_t self) {}
};

template <typename T, typename F>
inline T base_cast(F obj) {
    return static_cast<T>(dynamic_cast<void*>(obj));
}

/**
 * Basic way to make a main function without it being a main
 * function, inject is purposed for that
 */
#define inject() $inject(); static int const _inject = ($inject(), 0); void $inject()

inline lilac::Mod* MyMod;

LILAC_API bool LILAC_CALL lilac_load(lilac::Mod* mod) {
    MyMod = mod;
    for (auto i : modContainer.m_mods) {
        mod->addHook(
            (void*)i.hookAddr,
            (void*)i.funcLocation
        );
    }
    return true;
}

LILAC_API void LILAC_CALL lilac_unload() {

}

/**
 * Main class implementation, it has the structure
 * 
 * class hook0__;
 * template<typename T>
 * struct _hook0 {};
 * namespace {
 *     struct hook0Unique {};
 *     bool hook0Apply = Cacao::interfaces::$MenuLayer<_hook0<hook0Unique>>::_apply();
 * }
 * using hook0 = _hook0<hook0Unique>;
 * template<>
 * struct hidden _hook0<hook0Unique>: public Cacao::interfaces::$MenuLayer<_hook0<hook0Unique>> {
 *     // code stuff idk
 * };
 * 
 * I tried to make the macro as verbose as it can be but
 * I am bad at this stuff
 */

#define PREDECLARE(derived) derived##__; template<typename T, auto _orig> struct _##derived {};
#define APPLY(base, derived) namespace { struct derived##UUID{}; bool derived##Apply = base<derived##UUID, _##derived>::_apply();  }
#define DECLARE(base, derived) using derived = _##derived<derived##UUID, 0>; template <auto _orig> struct hidden _##derived<derived##UUID, _orig>: public base<derived##UUID, _##derived>

#define REDIRECT___(base, derived) PREDECLARE(derived) APPLY(base, derived) DECLARE(base, derived)
#define REDIRECT__(base, derived) REDIRECT___(Cacao::interfaces::$##base, derived)
#define REDIRECT_(base) REDIRECT__(base, CONCAT(hook, __COUNTER__))
#define REDIRECT(base) REDIRECT_(base)

/**
 * Interfaces for the class implementation
 * 
 * $redirect is for when you don't need the name of the class
 * class $redirect(MenuLayer) {};
 * 
 * $implement is for when you need the name of the class
 * class $implement(MenuLayer, MyMenuLayerInterface) {};
 */
#define $redirect(base) REDIRECT(base)
#define $implement(base, derived) REDIRECT__(base, derived)
#define $orig(...) (*this.*_orig)(__VA_ARGS__)
/**
 * Or just use this lol
 */
#define CRTP1(base) $redirect(base)
#define CRTP2(base, derived) $implement(base, derived)
#define $(...) CONCAT(CRTP, NUMBER_OF_ARGS(__VA_ARGS__))(__VA_ARGS__)

namespace Cacao {
    using namespace cocos2d;
    using namespace cocos2d::extension;
    using std::declval;
    struct interfaces { // i find this really funny
        #include <Gen/Interface.hpp>
    };
}
