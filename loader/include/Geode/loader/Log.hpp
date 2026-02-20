#pragma once

#include "Types.hpp"
#include "../platform/cplatform.h"

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/function.hpp>
#include <Geode/utils/StringBuffer.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/Result.hpp>
#include <ccTypes.h>
#include <chrono>
#include <filesystem>
#include <matjson.hpp>
#include <type_traits>
#include <asp/time/SystemTime.hpp>
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

        class Log;

        // Borrowed log
        struct GEODE_DLL BorrowedLog final {
        private:
            friend class Logger;
            friend struct Log;

            // these are private since we want to disallow users to deal with a BorrowedLog directly,
            // only allow them to use it through a reference
            BorrowedLog(Severity severity, int32_t nestCount, std::string_view content, std::string_view thread, std::string_view source, Mod* mod);
            BorrowedLog(Log const& log);
            BorrowedLog(BorrowedLog const&) = default;
            BorrowedLog& operator=(BorrowedLog const&) = default;
            ~BorrowedLog() = default;

            Log intoLog() const;

        public:
            asp::SystemTime m_time;
            Severity m_severity;
            int32_t m_nestCount;
            std::string_view m_content;
            std::string_view m_thread;
            std::string_view m_source;
            Mod* m_mod = nullptr;

            std::tuple<std::string_view, std::string_view, int32_t> truncateWithNest() const;

            template <size_t N>
            void formatTo(utils::StringBuffer<N>& buf, bool millis = false) const;
        };

        struct LogEvent final : ThreadSafeEvent<LogEvent, bool(BorrowedLog const&)> {
            using ThreadSafeEvent::ThreadSafeEvent;
        };
    }
}

template <size_t N>
inline void geode::log::BorrowedLog::formatTo(utils::StringBuffer<N>& buf, bool millis) const {
    auto ms = m_time.timeSinceEpoch().millis() % 1000;
    auto local = asp::localtime(m_time.to_time_t());

    if (millis) {
        buf.append("{:%H:%M:%S}.{:03}", local, ms);
    } else {
        buf.append("{:%H:%M:%S}", local);
    }

    buf.append(' ');

    switch (m_severity.m_value) {
        case Severity::Debug: buf.append("DEBUG"); break;
        case Severity::Info: buf.append("INFO "); break;
        case Severity::Warning: buf.append("WARN "); break;
        case Severity::Error: buf.append("ERROR"); break;
        default: buf.append("?????"); break;
    }

    buf.append(' ');

    auto [source, thread, nestCount] = this->truncateWithNest();
    bool sourceTrunc = source.size() != m_source.size();
    bool threadTrunc = thread.size() != m_thread.size();

    if (!thread.empty()) {
        buf.append('[');
        buf.append(thread);
        if (threadTrunc) {
            buf.append('>');
        }
        buf.append("] ");
    }

    buf.append('[');
    buf.append(source);
    if (sourceTrunc) {
        buf.append('>');
    }
    buf.append("]: ");

    for (int32_t i = 0; i < nestCount; i++) {
        buf.append(' ');
    }

    buf.append(m_content);
}
