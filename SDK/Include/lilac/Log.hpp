#pragma once

#include "Macros.hpp"
#include "Types.hpp"
#include <chrono>
#include <sstream>
#include <vector>

namespace cocos2d {
    class CCObject;
    class CCNode;
    class CCPoint;
    class CCSize;
    class CCRect;
}

namespace lilac {
    #pragma warning(disable: 4251)

    class Mod;
    class LogStream;

    using log_clock = std::chrono::system_clock;

    class LILAC_DLL Log {
        protected:
            friend class LogStream;

        public:
            virtual std::string toString() const = 0;

            virtual ~Log();
    };

    class LILAC_DLL LogMod : public Log {
        protected:
            Mod* m_mod;

            friend class LogStream;
        
        public:
            LogMod(Mod* Mod) : m_mod(Mod) {}
            
            Mod* getMod() const;

            std::string toString() const override;
    };

    class LILAC_DLL LogString : public Log {
        protected:
            std::string m_string;

            friend class LogStream;

        public:
            std::string toString() const override;

            LogString() = default;
            LogString(
                std::string const& str
            ) : m_string(str) {}
    };

    class LILAC_DLL LogCCObject : public Log {
        protected:
            cocos2d::CCObject* m_obj;
        
            friend class LogStream;

        public:
            LogCCObject() = default;
            LogCCObject(cocos2d::CCObject* obj);
            ~LogCCObject();

            cocos2d::CCObject* getObject() const;
            
            std::string toString() const override;
    };

    class LILAC_DLL LogMessage {
        protected:
            Mod* m_sender                = nullptr;
            log_clock::time_point m_time = log_clock::now();
            std::vector<Log*> m_data     = {};
            Severity m_severity          = Severity::Debug;

            friend class LogStream;
        
        public:
            LogMessage() = default;

            LogMessage(
                Mod* Mod
            ) : m_sender(Mod) {}

            LogMessage(
                std::string data,
                Mod* Mod
            ) : m_data({ new LogString(data) }),
                m_sender(Mod) {}

            LogMessage(
                std::string data,
                Severity severity,
                Mod* Mod
            ) : m_sender(Mod),
                m_data({ new LogString(data) }),
                m_severity(severity) {}

            LogMessage(
                std::initializer_list<Log*> data,
                Mod* Mod
            ) : m_sender(Mod),
                m_data(data) {}

            ~LogMessage();

            void add(Log* msg);

            log_clock::time_point getTime() const;
            std::string getTimeString() const;
            Mod* getSender() const;
            Severity getSeverity() const;
            std::vector<Log*> const& getData() const;

            std::string toString(bool logTime = true) const;
    };

    /**
     * End logging and print message to 
     * the console.
     */
    struct endl_type {
        constexpr endl_type() {}
    };
    constexpr const auto endl = endl_type();

    /**
     * Print message to the console without 
     * a newline and continue logging to it.
     */
    struct continue_type {
        constexpr continue_type() {}
    };
    constexpr const auto conl = continue_type();

    class LILAC_DLL LogStream {
        protected:
            LogMessage* m_log = nullptr;
            std::stringstream m_stream;

            void init();
            void save();
            void finish();
            void log();

        public:
            LogStream& operator<<(Mod*);
            LogStream& operator<<(void*);
            LogStream& operator<<(Severity);
            LogStream& operator<<(Severity::type);
            LogStream& operator<<(cocos2d::CCObject*);
            LogStream& operator<<(std::string const&);
            LogStream& operator<<(std::string_view const&);
            LogStream& operator<<(const char*);
            LogStream& operator<<(uintptr_t);
            LogStream& operator<<(int);
            LogStream& operator<<(long);
            LogStream& operator<<(float);
            LogStream& operator<<(double);
            LogStream& operator<<(cocos2d::CCPoint const&);
            LogStream& operator<<(cocos2d::CCSize const&);
            LogStream& operator<<(cocos2d::CCRect const&);
            LogStream& operator<<(endl_type);
            LogStream& operator<<(continue_type);

            ~LogStream();
    };
}
