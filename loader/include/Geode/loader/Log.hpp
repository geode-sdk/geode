#pragma once

#include <Geode/DefaultInclude.hpp>
#include "Types.hpp"
#include <chrono>
#include <sstream>
#include <vector>
#include <fs/filesystem.hpp>
#include <ccTypes.h>

#ifndef __cpp_lib_concepts
namespace std {
    // <concepts> isn't working for me lmao
    template <class From, class To>
    concept convertible_to =
      std::is_convertible_v<From, To> &&
      requires {
        static_cast<To>(std::declval<From>());
      };
}
#endif

namespace geode {
    #pragma warning(disable: 4251)

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
        requires (std::convertible_to<T*, cocos2d::CCObject*> && !std::convertible_to<T*, cocos2d::CCNode*>)
        std::string parse(T* node) {
            return parse(static_cast<cocos2d::CCObject*>(node));
        }
        GEODE_DLL std::string parse(Mod*);

        template <typename T>
        requires requires(T b) { std::stringstream() << b; }
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
            inline std::string _toString() override { return parse(m_item); }
        };

        /*template <typename T> requires requires(T b) { std::stringstream() << b; }
        struct Component<T> : ComponentBase<T> {
            inline static std::string toString(T const& thing) {
                std::stringstream buf;
                buf << thing;
                return buf.str();
            }
        };*/

        class GEODE_DLL Log {
        private: 
            static std::vector<std::function<void(Mod*)>>& scheduled();
        protected:
            Mod* m_sender;
            log_clock::time_point m_time;
            std::vector<ComponentTrait*> m_components;
            Severity m_severity;
        
        public:
            Log(Mod* mod, Severity sev);// : m_sender(mod), m_time(log_clock::now()), m_severity(sev) {}
            Log(Log&& l) = default;
            Log& operator=(Log&& l) = default;
            bool operator==(Log const& l);

            std::string toString(bool logTime = true) const;
            void pushToLoader();

            inline std::vector<ComponentTrait*>& getComponents() { return m_components; }
            inline log_clock::time_point getTime() const { return m_time; }
            inline Mod* getSender() const { return m_sender; }
            inline Severity getSeverity() const { return m_severity; }
            inline ~Log() {
                for (auto comp : m_components) {
                    delete comp;
                }
            }

            template <typename ...Args>
            friend void schedule(Severity sev, Args... args);

            friend void GEODE_DLL releaseSchedules(Mod* m);
        };

        void GEODE_DLL vlogImpl(Severity, Mod*, std::string_view, std::function<void(Log&)>*, size_t);

        template <typename... Args>
        requires requires(Args... b) { (parse(b), ...); }
        void log(Severity severity, Mod* mod, std::string_view formatStr, Args... args) {
            static constexpr auto pushSomething = [](Log& log, auto something) {
                // i think this line of code is very sad
                log.getComponents().push_back(new ComponentBase(something));
            };

            std::array<std::function<void(Log&)>, sizeof...(Args)> comps = { [&](Log& log) { pushSomething(log, args); }... };
            // tfw no std::span
            vlogImpl(severity, mod, formatStr, comps.data(), comps.size());
        }

        void GEODE_DLL releaseSchedules(Mod* m);

        template <typename ...Args>
        void schedule(Severity sev, Args... args) {
            auto m = getMod();
            if (m) return log(sev, m, args...);

            Log::scheduled().push_back([=](Mod* m2){
                log(sev, m2, args...);
            });
        }

        template <typename ...Args>
        void debug(Args... args) { 
        #ifdef GEODE_DEBUG
            schedule(Severity::Debug, args...); 
        #endif
        }

        template <typename ...Args>
        void info(Args... args) { schedule(Severity::Info, args...); }

        template <typename ...Args>
        void notice(Args... args) { schedule(Severity::Notice, args...); }

        template <typename ...Args>
        void warn(Args... args) { schedule(Severity::Warning, args...); }

        template <typename ...Args>
        void error(Args... args) { schedule(Severity::Error, args...); }

        template <typename ...Args>
        void critical(Args... args) { schedule(Severity::Critical, args...); }

        template <typename ...Args>
        void alert(Args... args) { schedule(Severity::Alert, args...); }

        template <typename ...Args>
        void emergency(Args... args) { schedule(Severity::Emergency, args...); }
    }
}
