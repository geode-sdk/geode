#include "Mod.hpp"
#include "Loader.hpp"
#include "Interface.hpp"

#ifdef API_DECL
#undef API_DECL
#endif


#if GEODE_CONCAT(EXPORT_, EXPORT_NAME)
    #define API_DECL(func, ...) ; static inline auto GEODE_CONCAT(_dummy, __LINE__) = (geode::Interface::get()->exportAPIFunction(GEODE_STR(func), func), 0);
#else
    #define API_DECL(func, ...) \
    {\
        static geode::Mod* src = geode::Loader::get()->getLoadedMod(_source);\
    	static auto ptr = geode::Mod::get()->importAPIFunction<decltype(func)>(GEODE_STR(func), src);\
    	return std::invoke(ptr, __VA_ARGS__);\
    }
#endif

#ifndef GEODE_API_HPP
#define GEODE_API_HPP

#define API_INIT(name) static constexpr auto _source = name;

namespace geode {
    class GEODE_DLL ModAPI {};
}
#endif

#undef EXPORT_NAME
