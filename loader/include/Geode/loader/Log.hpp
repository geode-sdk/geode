#pragma once

#include "Types.hpp"
#include "../platform/cplatform.h"

#include <Geode/DefaultInclude.hpp>
#include <ccTypes.h>
#include <chrono>
#include <filesystem>
#include <matjson.hpp>
#include <type_traits>
#include <fmt/core.h>
// for formatting std::vector and such
#include <fmt/ranges.h>
// for std::optional
#include <fmt/std.h>

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

namespace std::filesystem {
    GEODE_INLINE GEODE_HIDDEN std::string format_as(std::filesystem::path const& value) {
        return value.string();
    }
}

namespace matjson {
    GEODE_INLINE GEODE_HIDDEN std::string format_as(matjson::Value const& value) {
        return value.dump(matjson::NO_INDENTATION);
    }
}

namespace geode {

    class Mod;
    Mod* getMod();

    namespace log {
        using log_clock = std::chrono::system_clock;
        GEODE_DLL std::string generateLogName();

        GEODE_DLL void vlogImpl(Severity, Mod*, fmt::string_view format, fmt::format_args args);

        template <typename... Args>
        inline void logImpl(Severity severity, Mod* mod, impl::FmtStr<Args...> str, Args&&... args) {
            [&]<typename... Ts>(Ts&&... args) {
                vlogImpl(severity, mod, str, fmt::make_format_args(args...));
            }(impl::wrapCocosObj(args)...);
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

        GEODE_DLL void pushNest(Mod* mod);
        GEODE_DLL void popNest(Mod* mod);

        inline void pushNest() {
            pushNest(getMod());
        }

        inline void popNest() {
            popNest(getMod());
        }

        class Nest final {
        private:
            class Impl;
            std::shared_ptr<Nest::Impl> m_impl;
            friend GEODE_DLL std::shared_ptr<Nest> saveNest();
            friend GEODE_DLL void loadNest(std::shared_ptr<Nest> const& nest);
        public:
            explicit Nest(std::shared_ptr<Nest::Impl> impl);
        };

        [[nodiscard]] GEODE_DLL std::shared_ptr<Nest> saveNest();
        GEODE_DLL void loadNest(std::shared_ptr<Nest> const& nest);
    }
}
