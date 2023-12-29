#pragma once

#include "Types.hpp"
#include "../utils/Result.hpp"

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/ranges.hpp>
#include <ccTypes.h>
#include <chrono>
#include <ghc/fs_fwd.hpp>
#include <fmt/core.h>
#include <type_traits>

namespace geode {
    // these are here because theyre special :-)
    GEODE_DLL std::string format_as(cocos2d::CCObject const*);
    GEODE_DLL std::string format_as(cocos2d::CCArray*);
    GEODE_DLL std::string format_as(cocos2d::CCNode*);
    class Mod;
    GEODE_DLL std::string format_as(Mod*);
}

namespace geode::log::impl {
    // What is this all for? well, fmtlib disallows writing custom formatters for non-void pointer types.
    // So instead, we just wrap everything and pass it a string instead.
    // WARNING: This code breaks in fmtlib 10.1.1! I have no idea why, so be careful before updating it.
    
    template <class T>
    GEODE_INLINE GEODE_HIDDEN decltype(auto) wrapCocosObj(T&& value) {
        if constexpr (std::is_pointer_v<std::decay_t<T>> && requires(T ptr) { geode::format_as(ptr); }) {
            return geode::format_as(value);
        } else {
            return std::forward<T>(value);
        }
    }

    template <class T>
    using TransformType = decltype(wrapCocosObj<T>(std::declval<T>()));

    template <class... Args>
    using FmtStr = fmt::format_string<TransformType<Args>...>;
}

namespace cocos2d {
    GEODE_DLL std::string format_as(cocos2d::ccColor3B const&);
    GEODE_DLL std::string format_as(cocos2d::ccColor4B const&);
    GEODE_DLL std::string format_as(cocos2d::ccColor4F const&);
    GEODE_DLL std::string format_as(cocos2d::CCPoint const&);
    GEODE_DLL std::string format_as(cocos2d::CCRect const&);
    GEODE_DLL std::string format_as(cocos2d::CCSize const&);
}

namespace gd {
    GEODE_INLINE GEODE_HIDDEN std::string format_as(gd::string const& value) {
        return value;
    }
}

namespace ghc::filesystem {
    GEODE_INLINE GEODE_HIDDEN std::string format_as(ghc::filesystem::path const& value) {
        return value.string();
    }
}

namespace geode {
#pragma warning(disable : 4251)

    class Mod;
    Mod* getMod();

    namespace log {
        using log_clock = std::chrono::system_clock;
        GEODE_DLL std::string generateLogName();

        GEODE_DLL void vlogImpl(Severity, Mod*, fmt::string_view format, fmt::format_args args);

        template <typename... Args>
        inline void logImpl(Severity severity, Mod* mod, impl::FmtStr<Args...> str, Args&&... args) {
            vlogImpl(severity, mod, str, fmt::make_format_args(impl::wrapCocosObj(args)...));
        }

        template <typename... Args>
        inline void debug(impl::FmtStr<Args...> str, Args&&... args) {
            logImpl(Severity::Debug, getMod(), str, std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline void info(impl::FmtStr<Args...> str, Args&&... args) {
            logImpl(Severity::Info, getMod(), str, std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline void warn(impl::FmtStr<Args...> str, Args&&... args) {
            logImpl(Severity::Warning, getMod(), str, std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline void error(impl::FmtStr<Args...> str, Args&&... args) {
            logImpl(Severity::Error, getMod(), str, std::forward<Args>(args)...);
        }

        GEODE_DLL void pushNest();
        GEODE_DLL void popNest();
    }
}
