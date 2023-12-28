#pragma once

#include "Types.hpp"
#include "../utils/Result.hpp"

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/ranges.hpp>
#include <ccTypes.h>
#include <chrono>
#include <ghc/fs_fwd.hpp>
#include <fmt/core.h>

namespace cocos2d {
    // ive had enough, fmtlib wont let me do what i want
    // GEODE_DLL std::string format_as(cocos2d::CCObject*);
    // GEODE_DLL std::string format_as(cocos2d::CCArray*);
    // GEODE_DLL std::string format_as(cocos2d::CCNode*);
    GEODE_DLL std::string format_as(cocos2d::ccColor3B const&);
    GEODE_DLL std::string format_as(cocos2d::ccColor4B const&);
    GEODE_DLL std::string format_as(cocos2d::ccColor4F const&);
    GEODE_DLL std::string format_as(cocos2d::CCPoint const&);
    GEODE_DLL std::string format_as(cocos2d::CCRect const&);
    GEODE_DLL std::string format_as(cocos2d::CCSize const&);
}

namespace gd {
    inline std::string format_as(gd::string const& value) {
        return value;
    }
}

namespace ghc::filesystem {
    inline std::string format_as(ghc::filesystem::path const& value) {
        return value.string();
    }
}

namespace geode {
#pragma warning(disable : 4251)

    class Mod;
    Mod* getMod();

    GEODE_DLL std::string format_as(Mod*);

    namespace log {
        using log_clock = std::chrono::system_clock;
        GEODE_DLL std::string generateLogName();

        GEODE_DLL void vlogImpl(Severity, Mod*, fmt::string_view format, fmt::format_args args);

        template <typename... Args>
        inline void logImpl(Severity severity, Mod* mod, fmt::format_string<Args...> str, Args&&... args) {
            vlogImpl(severity, mod, str, fmt::make_format_args(args...));
        }

        template <typename... Args>
        inline void debug(fmt::format_string<Args...> str, Args&&... args) {
            logImpl(Severity::Debug, getMod(), str, std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline void info(fmt::format_string<Args...> str, Args&&... args) {
            logImpl(Severity::Info, getMod(), str, std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline void warn(fmt::format_string<Args...> str, Args&&... args) {
            logImpl(Severity::Warning, getMod(), str, std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline void error(fmt::format_string<Args...> str, Args&&... args) {
            logImpl(Severity::Error, getMod(), str, std::forward<Args>(args)...);
        }

        GEODE_DLL void pushNest();
        GEODE_DLL void popNest();
    }
}
