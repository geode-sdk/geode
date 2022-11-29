#pragma once

#include "Types.hpp"

#include <Geode/DefaultInclude.hpp>
#include <ccTypes.h>
#include <chrono>
#include <fs/filesystem.hpp>
#include <sstream>
#include <vector>

#ifndef __cpp_lib_concepts
namespace std {
    // <concepts> isn't working for me lmao
    template <class From, class To>
    concept convertible_to = std::is_convertible_v<From, To> && requires {
        static_cast<To>(std::declval<From>());
    };
}
#endif

namespace geode {
#pragma warning(disable : 4251)

    class Mod;
    inline Mod* getMod();

    namespace log {
        using log_clock = std::chrono::system_clock;

        GEODE_DLL std::string generateLogName();
        GEODE_DLL std::string parse(cocos2d::CCNode*);

        template <class T>
            requires std::convertible_to<T*, cocos2d::CCNode*>
        std::string parse(T* node) {
            return parse(static_cast<cocos2d::CCNode*>(node));
        }

        GEODE_DLL std::string parse(cocos2d::CCPoint const&);
        GEODE_DLL std::string parse(cocos2d::CCSize const&);
        GEODE_DLL std::string parse(cocos2d::CCRect const&);
        GEODE_DLL std::string parse(cocos2d::CCArray*);
        GEODE_DLL std::string parse(cocos2d::ccColor3B const&);
        GEODE_DLL std::string parse(cocos2d::ccColor4B const&);
        GEODE_DLL std::string parse(cocos2d::ccColor4F const&);
        GEODE_DLL std::string parse(cocos2d::CCObject*);

        template <class T>
            requires(
                std::convertible_to<T*, cocos2d::CCObject*> &&
                !std::convertible_to<T*, cocos2d::CCNode*>
            )
        std::string parse(T* node) {
            return parse(static_cast<cocos2d::CCObject*>(node));
        }

        GEODE_DLL std::string parse(Mod*);

        template <typename T>
            requires requires(T b) {
                std::stringstream() << b;
            }
        std::string parse(T const& thing) {
            std::stringstream buf;
            buf << thing;
            return buf.str();
        }

        struct GEODE_DLL ComponentTrait {
            virtual ~ComponentTrait() {}

            virtual std::string _toString() = 0;
        };

        template <typename T>
        struct ComponentBase : public ComponentTrait {
            T m_item;

            inline ~ComponentBase() override {}

            inline ComponentBase(T const& item) : m_item(item) {}

            // specialization must implement
            inline std::string _toString() override {
                return parse(m_item);
            }
        };

        /*template <typename T> requires requires(T b) { std::stringstream() << b; }
        struct Component<T> : ComponentBase<T> {
            inline static std::string toString(T const& thing) {
                std::stringstream buf;
                buf << thing;
                return buf.str();
            }
        };*/

        class GEODE_DLL Log final {
        private:
            static std::vector<std::function<void(Mod*)>>& scheduled();

        protected:
            Mod* m_sender;
            log_clock::time_point m_time;
            std::vector<ComponentTrait*> m_components;
            Severity m_severity;

        public:
            Log(Mod* mod, Severity sev);
            Log(Log&& l) = default;
            Log& operator=(Log&& l) = default;
            bool operator==(Log const& l);

            std::string toString(bool logTime = true) const;

            std::vector<ComponentTrait*>& getComponents();
            log_clock::time_point getTime() const;
            Mod* getSender() const;
            Severity getSeverity() const;

            ~Log();

            template <typename... Args>
            friend void schedule(Severity sev, Args... args);

            friend void GEODE_DLL releaseSchedules(Mod* m);
        };

        class GEODE_DLL Logs {
        private:
            static inline std::vector<Log> s_logs;
            static inline std::ofstream s_logStream;
        
            Logs() = delete;
            ~Logs() = delete;

        public:
            static void setup();
            static void push(Log&& log);
            static void pop(Log* log);
            static std::vector<Log*> list();
            static void clear();
        };

        void GEODE_DLL vlogImpl(
            Severity, Mod*, std::string_view, std::function<void(Log&)>*, size_t
        );

        template <typename... Args>
            requires requires(Args... b) {
                (parse(b), ...);
            }
        void log(Severity severity, Mod* mod, std::string_view formatStr, Args... args) {
            static constexpr auto pushSomething = [](Log& log, auto something) {
                // i think this line of code is very sad
                log.getComponents().push_back(new ComponentBase(something));
            };

            std::array<std::function<void(Log&)>, sizeof...(Args)> comps = { [&](Log& log) {
                pushSomething(log, args);
            }... };
            // tfw no std::span
            vlogImpl(severity, mod, formatStr, comps.data(), comps.size());
        }

        void GEODE_DLL releaseSchedules(Mod* m);

        template <typename... Args>
        void schedule(Severity sev, Args... args) {
            auto m = getMod();
            if (m) return log(sev, m, args...);

            Log::scheduled().push_back([=](Mod* m2) {
                log(sev, m2, args...);
            });
        }

        template <typename... Args>
        void debug(Args... args) {
#ifdef GEODE_DEBUG
            schedule(Severity::Debug, args...);
#endif
        }

        template <typename... Args>
        void info(Args... args) {
            schedule(Severity::Info, args...);
        }

        template <typename... Args>
        void notice(Args... args) {
            schedule(Severity::Notice, args...);
        }

        template <typename... Args>
        void warn(Args... args) {
            schedule(Severity::Warning, args...);
        }

        template <typename... Args>
        void error(Args... args) {
            schedule(Severity::Error, args...);
        }

        template <typename... Args>
        void critical(Args... args) {
            schedule(Severity::Critical, args...);
        }

        template <typename... Args>
        void alert(Args... args) {
            schedule(Severity::Alert, args...);
        }

        template <typename... Args>
        void emergency(Args... args) {
            schedule(Severity::Emergency, args...);
        }
    }
}
