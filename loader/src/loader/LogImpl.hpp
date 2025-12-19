#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Types.hpp>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <atomic>

namespace geode::log {
    struct BorrowedLog;

    class Log final {
        log_clock::time_point m_time;
        Severity m_severity;
        std::string m_thread;
        std::string m_source;
        int32_t m_nestCount;
        std::string m_content;

    public:
        friend struct BorrowedLog;
        Log(log_clock::time_point time, Severity severity, std::string thread,
            std::string source, int32_t nestCount, std::string content);

        [[nodiscard]] std::string toString(bool millis = false) const;
        Severity getSeverity() const;
    };

    class Logger {
    private:
        std::atomic<bool> m_initialized = false;
        std::vector<Log> m_logs;
        std::ofstream m_logStream;
        std::filesystem::path m_logPath;

        // for tracking when to flush the log stream
        size_t m_unflushedLogs = 0;
        log_clock::time_point m_lastFlushTime = log_clock::now();

        Logger() = default;
    public:
        static Logger* get();

        void setup();

        void push(Severity sev, std::string_view thread, std::string_view source, int32_t nestCount, std::string_view content);
        void push(BorrowedLog const& log);

        Severity getConsoleLogLevel();
        Severity getFileLogLevel();
        void clear();

        std::filesystem::path const& getLogPath() const;

        void deleteOldLogs(size_t maxAgeHours);

        template <typename Rep, typename Period>
        void deleteOldLogs(std::chrono::duration<Rep, Period> const& maxAge) {
            this->deleteOldLogs(std::chrono::duration_cast<std::chrono::hours>(maxAge).count());
        }
        
        void flush();
        void outputLog(BorrowedLog const& log, bool dontFlush = false);
        void maybeFlushStream();
        void flushLocked();
    };

    class Nest::Impl {
    public:
        int32_t m_nestLevel;
        int32_t m_nestCountOffset;
        Impl(int32_t nestLevel, int32_t nestCountOffset);
    };
}
