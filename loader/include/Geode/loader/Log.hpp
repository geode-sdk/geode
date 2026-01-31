#pragma once

#include "Types.hpp"
#include "../platform/cplatform.h"

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/function.hpp>
#include <Geode/Result.hpp>
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

template <typename T, typename E>
struct fmt::formatter<geode::Result<T, E>> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) noexcept { return ctx.begin(); }

    template <typename FormatContext>
    auto format(geode::Result<T, E> const& result, FormatContext& ctx) const noexcept {
        std::string out;

        auto formatValue = [&](auto&& value) {
            using V = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<V, std::string>) {
                fmt::format_to(std::back_inserter(out), "\"{}\"", value);
            } else if constexpr (std::is_void_v<V>) {
                // nothing
            } else if constexpr (requires { geode::format_as(value); }) {
                fmt::format_to(std::back_inserter(out), "{}", geode::format_as(value));
            } else if constexpr (std::is_pointer_v<V>) {
                fmt::format_to(std::back_inserter(out), "{}", (void*)value);
            } else {
                fmt::format_to(std::back_inserter(out), "{}", value);
            }
        };

        if (result) {
            std::string_view quotes = std::is_same_v<T, std::string> ? "\"" : "";
            if constexpr (!std::is_void_v<T>) {
                return fmt::format_to(ctx.out(), "Ok({}{}{})", quotes, result.unwrap(), quotes);
            }
            else {
                return fmt::format_to(ctx.out(), "Ok()");
            }
        } else {
            std::string_view quotes = std::is_same_v<E, std::string> ? "\"" : "";
            if constexpr (!std::is_void_v<E>) {
                return fmt::format_to(ctx.out(), "Err({}{}{})", quotes, result.unwrapErr(), quotes);
            }
            else {
                return fmt::format_to(ctx.out(), "Err()");
            }
        }
    }
};

namespace geode::log::impl {
    // What is this all for? well, fmtlib disallows writing custom formatters for non-void pointer types.
    // So instead, we just wrap everything and pass it a string instead.

    template <class T>
    concept IsWrappedCocos = std::is_pointer_v<std::decay_t<T>> && requires(T ptr) { geode::format_as(ptr); };

    template <class T>
    inline decltype(auto) wrapCocosObj(T&& value) {
        if constexpr (IsWrappedCocos<T>) {
            return geode::format_as(value);
        } else {
            return std::forward<T>(value);
        }
    }

    template <class T>
    using TransformType = std::conditional_t<
        IsWrappedCocos<T>,
        decltype(wrapCocosObj<T>(std::declval<T>())),
        T
    >;

    template <class... Args>
    using FmtStr = fmt::format_string<TransformType<Args>...>;
}

template <>
struct fmt::formatter<cocos2d::ccColor3B> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) noexcept { return ctx.begin(); }

    template <typename FormatContext>
    auto format(cocos2d::ccColor3B const& col, FormatContext& ctx) const noexcept {
        return fmt::format_to(ctx.out(), "rgb({}, {}, {})", col.r, col.g, col.b);
    }
};

template <>
struct fmt::formatter<cocos2d::ccColor4B> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) noexcept { return ctx.begin(); }

    template <typename FormatContext>
    auto format(cocos2d::ccColor4B const& col, FormatContext& ctx) const noexcept {
        return fmt::format_to(ctx.out(), "rgba({}, {}, {}, {})", col.r, col.g, col.b, col.a);
    }
};

template <>
struct fmt::formatter<cocos2d::ccColor4F> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) noexcept { return ctx.begin(); }

    template <typename FormatContext>
    auto format(cocos2d::ccColor4F const& col, FormatContext& ctx) const noexcept {
        return fmt::format_to(ctx.out(), "rgba({}, {}, {}, {})", col.r, col.g, col.b, col.a);
    }
};

template <>
struct fmt::formatter<cocos2d::CCPoint> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) noexcept { return ctx.begin(); }

    template <typename FormatContext>
    auto format(cocos2d::CCPoint const& pt, FormatContext& ctx) const noexcept {
        return fmt::format_to(ctx.out(), "{}, {}", pt.x, pt.y);
    }
};

template <>
struct fmt::formatter<cocos2d::CCSize> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) noexcept { return ctx.begin(); }

    template <typename FormatContext>
    auto format(cocos2d::CCSize const& sz, FormatContext& ctx) const noexcept {
        return fmt::format_to(ctx.out(), "{} : {}", sz.width, sz.height);
    }
};

template <>
struct fmt::formatter<cocos2d::CCRect> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) noexcept { return ctx.begin(); }

    template <typename FormatContext>
    auto format(cocos2d::CCRect const& rect, FormatContext& ctx) const noexcept {
        return fmt::format_to(ctx.out(), "{} | {}", rect.origin, rect.size);
    }
};

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
            }(impl::wrapCocosObj(std::forward<Args>(args))...);
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

        /// Returns the path to the current log file
        GEODE_DLL std::filesystem::path const& getCurrentLogPath();

        using LogCallback = Function<void(
            std::string_view content,
            Severity sev,
            Mod* mod,
            std::string_view source,
            std::string_view thread
        )>;
        /// Adds a callback that will be invoked whenever a line is logged.
        /// Callback parameters:
        /// * `std::string_view content` - the content of the log
        /// * `Severity sev` - the severity of the log
        /// * `Mod* mod` - the mod that logged this log, if any (for example for stdout/stderr redirection this is null)
        /// * `std::string_view source` - the source of the log (usually either mod name or "stdout" / "stderr")
        /// * `std::string_view thread` - the name of the thread that logged this log
        ///
        /// Note: logging inside the callback will cause the log to be lost and not logged anywhere.
        GEODE_DLL void addLogCallback(LogCallback callback);

        GEODE_DLL void pushNest(Mod* mod);
        GEODE_DLL void popNest(Mod* mod);

        /// Blocks the current thread until all logs are printed to console/file, recommended only for debugging purposes
        GEODE_DLL void flush();

        inline void pushNest() {
            pushNest(getMod());
        }

        inline void popNest() {
            popNest(getMod());
        }

        struct NestScope {
        private:
            bool m_active = true;
        public:
            NestScope() {
                pushNest();
            }

            NestScope(NestScope const&) {
                pushNest();
            }

            NestScope(NestScope&& other) {
                other.m_active = false;
            }

            NestScope& operator=(NestScope const&) {
                pushNest();
                return *this;
            }

            NestScope& operator=(NestScope&& other) {
                other.m_active = false;
                return *this;
            }

            ~NestScope() {
                if (m_active) popNest();
            }
        };

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
