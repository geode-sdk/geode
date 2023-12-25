#pragma once

#include "Types.hpp"
#include "../utils/Result.hpp"

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/ranges.hpp>
#include <ccTypes.h>
#include <chrono>
#include <ghc/fs_fwd.hpp>
#include <sstream>
#include <vector>
#include <span>

#include <fmt/core.h>

namespace cocos2d {
    GEODE_DLL std::string format_as(cocos2d::CCArray*);
    GEODE_DLL std::string format_as(cocos2d::ccColor3B const&);
    GEODE_DLL std::string format_as(cocos2d::ccColor4B const&);
    GEODE_DLL std::string format_as(cocos2d::ccColor4F const&);
    GEODE_DLL std::string format_as(cocos2d::CCNode*);
    GEODE_DLL std::string format_as(cocos2d::CCObject*);
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

        // Log

        class GEODE_DLL Log final {
            Mod* m_sender;
            log_clock::time_point m_time;
            Severity m_severity;
            std::string m_content;

            friend class Logger;
        public:
            ~Log();
            Log(Severity sev, Mod* mod, std::string content);

            std::string toString(bool logTime = true) const;
            std::string toString(bool logTime, uint32_t nestLevel) const;

            std::string const& getContent() const;
            log_clock::time_point getTime() const;
            Mod* getSender() const;
            Severity getSeverity() const;
        };

        class GEODE_DLL Logger {
        private:
            static std::vector<Log>& logs();
            static std::ofstream& logStream();
            static uint32_t& nestLevel();

            Logger() = delete;
            ~Logger() = delete;

            // logs
        public:
            static void setup();

            static void push(Log&& log);
            static void pop(Log* log);

            static void pushNest();
            static void popNest();

            static std::vector<Log> const& list();
            static void clear();
        };

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

        static void pushNest() {
            Logger::pushNest();
        }
        static void popNest() {
            Logger::popNest();
        }
    }
}
