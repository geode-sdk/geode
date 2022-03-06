#pragma once

#include "Mod.hpp"
#include "Loader.hpp"
#include "Interface.hpp"

#define GEODE_EXPORT_FUNC(func, ...) ; static inline auto GEODE_CONCAT(_dummy, __LINE__) = (geode::Interface::get()->exportAPIFunction(GEODE_STR(func), func), 0);

#define GEODE_IMPORT_FUNC(func, ...) \
{\
	static auto ptr = geode::Interface::mod()->importAPIFunction<decltype(func)>(GEODE_STR(func));\
	return std::invoke(ptr, __VA_ARGS__);\
}


// #ifndef API_MOD
//     #error "API_MOD not defined"
// #elif API_MOD == PROJECT_NAME
//     #define API_DECL(ret, name, id, ...) { return bind<ret, __VA_ARGS__>(id); } 
// #else
//     #define API_DECL(ret, name, id, ...) ; \
//         static auto r##__COUNTER__ = Interface::mod()->exportAPIFunction(id, &name);
// #endif

// namespace geode {
//     class GEODE_DLL ModAPI {
//      protected:
//         Mod* m_mod;
//         template <typename R, auto ...Args>
//         R bind(char const* id) {
//             static auto f = m_mod->importAPIFunction<R(decltype(Args)...)>(id);
//             return f(Args...);
//         }
//      public:
//         ModAPI(std::string const& modid) : m_mod(Loader::get()->getLoadedMod(modid)) {}
//     };
// }
