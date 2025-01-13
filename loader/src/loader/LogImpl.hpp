#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <vector>
#include <fstream>
#include <string>

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

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] Severity getSeverity() const;
    };

    class Logger {
    private:
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
        void clear();

        std::filesystem::path const& getLogPath() const;
    };

    class Nest::Impl {
    public:
        int32_t m_nestLevel;
        int32_t m_nestCountOffset;
        Impl(int32_t nestLevel, int32_t nestCountOffset);
    };
}
