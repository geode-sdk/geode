#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Types.hpp>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>

namespace geode::log {
    class Log final {
        log_clock::time_point m_time;
        Severity m_severity;
        std::string m_thread;
        std::string m_source;
        int32_t m_nestCount;
        std::string m_content;

    public:
        ~Log();
        Log(Severity sev, std::string&& thread, std::string&& source, int32_t nestCount,
            std::string&& content);

        // TODO (v5): remove this and add default arg to the one below
        [[nodiscard]] std::string toString() const;
        [[nodiscard]] std::string toString(bool millis) const;

        [[nodiscard]] Severity getSeverity() const;
    };

    class Logger {
    private:
        bool m_initialized = false;
        std::vector<Log> m_logs;
        std::ofstream m_logStream;
        std::filesystem::path m_logPath;

        Logger() = default;
    public:
        static Logger* get();

        void setup();

        void push(Severity sev, std::string&& thread, std::string&& source, int32_t nestCount,
            std::string&& content);

        std::vector<Log> const& list();
        Severity getConsoleLogLevel();
        Severity getFileLogLevel();
        void clear();

        std::filesystem::path const& getLogPath() const;

        void deleteOldLogs(size_t maxAgeHours);

        template <typename Rep, typename Period>
        void deleteOldLogs(std::chrono::duration<Rep, Period> const& maxAge) {
            this->deleteOldLogs(std::chrono::duration_cast<std::chrono::hours>(maxAge).count());
        }
    };

    class Nest::Impl {
    public:
        int32_t m_nestLevel;
        int32_t m_nestCountOffset;
        Impl(int32_t nestLevel, int32_t nestCountOffset);
    };
}
