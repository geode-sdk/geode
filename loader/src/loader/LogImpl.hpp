#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Types.hpp>
#include <arc/task/Task.hpp>
#include <arc/sync/mpsc.hpp>
#include <arc/task/CancellationToken.hpp>
#include <vector>
#include <deque>
#include <thread>
#include <fstream>
#include <string>
#include <chrono>
#include <semaphore>
#include <atomic>
#include <condition_variable>

namespace geode::log {
    struct BorrowedLog;

    class Log final {
        asp::SystemTime m_time;
        Severity m_severity;
        int32_t m_nestCount;
        std::string m_content;
        std::string m_thread;
        std::string m_source;
        Mod* m_mod = nullptr;

    public:
        friend struct BorrowedLog;
        Log(asp::SystemTime time, Severity severity, int32_t nestCount,
            std::string content, std::string thread, std::string source, Mod* mod);

        [[nodiscard]] std::string toString(bool millis = false) const;
        Severity getSeverity() const;
    };

    class Logger {
    private:
        std::optional<arc::mpsc::Sender<Log>> m_logTx;
        std::optional<arc::mpsc::Receiver<Log>> m_logRx;
        std::atomic<bool> m_initialized = false;
        std::ofstream m_logStream;
        std::filesystem::path m_logPath;
        std::atomic<Severity> m_consoleLevel{Severity::Debug};
        std::atomic<Severity> m_fileLevel{Severity::Debug};

        arc::TaskHandle<void> m_logThread;
        asp::WeakPtr<arc::Runtime> m_runtime;
        arc::CancellationToken m_cancel;
        arc::Notify m_syncFlushNotify;
        std::counting_semaphore<1024> m_syncFlushSemaphore{0};
        std::atomic<bool> m_usingThread = false;

        Logger();
        ~Logger();

        arc::Future<> workerThread();
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

        void shutdownThread();
        void flush();
        void outputLog(BorrowedLog const& log, bool dontFlush = false);
        bool shouldOutputLog(Severity sev, Mod* mod, bool& console, bool& file, bool& listeners, bool& modLevel);
        bool shouldOutputLog(Severity sev, Mod* mod);
        void flushLocked();
        void flushExternal();
    };

    class Nest::Impl {
    public:
        int32_t m_nestLevel;
        int32_t m_nestCountOffset;
        Impl(int32_t nestLevel, int32_t nestCountOffset);
    };
}
