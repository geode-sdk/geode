#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Types.hpp>
#include <vector>
#include <deque>
#include <thread>
#include <fstream>
#include <string>
#include <chrono>
#include <atomic>
#include <condition_variable>

namespace geode::log {
    struct BorrowedLog;

    class Log final {
        log_clock::time_point m_time;
        Severity m_severity;
        int32_t m_nestCount;
        std::string m_content;
        std::string m_thread;
        std::string m_source;
        Mod* m_mod = nullptr;

    public:
        friend struct BorrowedLog;
        Log(log_clock::time_point time, Severity severity, int32_t nestCount,
            std::string content, std::string thread, std::string source, Mod* mod);

        [[nodiscard]] std::string toString(bool millis = false) const;
        Severity getSeverity() const;
    };

    class Logger {
    private:
        std::atomic<bool> m_initialized = false;
        std::atomic<bool> m_terminating = false;
        std::deque<Log> m_logs;
        std::vector<LogCallback> m_callbacks;
        std::ofstream m_logStream;
        std::thread m_logThread;
        std::filesystem::path m_logPath;
        std::condition_variable m_logCv;
        bool m_usingThread = false;

        // for tracking when to flush the log stream
        size_t m_unflushedLogs = 0;
        log_clock::time_point m_lastFlushTime = log_clock::now();

        Logger() = default;
        ~Logger();

        void workerThread();
    public:
        static Logger* get();

        void setup();

        void push(Severity sev, int32_t nestCount, std::string content, std::string_view thread, std::string_view source, Mod* mod);

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

        void addLogCallback(LogCallback callback);
    };

    class Nest::Impl {
    public:
        int32_t m_nestLevel;
        int32_t m_nestCountOffset;
        Impl(int32_t nestLevel, int32_t nestCountOffset);
    };
}
