#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <vector>
#include <fstream>
#include <string>

namespace geode::log {
    class Log final {
        Mod* m_sender;
        log_clock::time_point m_time;
        Severity m_severity;
        std::string m_content;

    public:
        ~Log();
        Log(Severity sev, Mod* mod, std::string&& content);

        std::string toString(bool logTime = true) const;
        std::string toString(bool logTime, uint32_t nestLevel) const;

        std::string_view getContent() const;
        log_clock::time_point getTime() const;
        Mod* getSender() const;
        Severity getSeverity() const;
    };

    class Logger {
    private:
        std::vector<Log> m_logs;
        std::ofstream m_logStream;
        int m_nestLevel;

        Logger() {}
    public:
        static Logger* get();

        void setup();

        void push(Severity sev, Mod* mod, std::string&& content);
        // why would you need this lol
        // void pop(Log* log);

        void pushNest();
        void popNest();

        std::vector<Log> const& list();
        void clear();
    };
}
