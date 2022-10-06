#pragma once

#include <Geode/DefaultInclude.hpp>
#include "Types.hpp"
#include <chrono>
#include <sstream>
#include <vector>
#include <fs/filesystem.hpp>
#include <ccTypes.h>

namespace std {
    // <concepts> isn't working for me lmao
    template <class From, class To>
    concept convertible_to =
      std::is_convertible_v<From, To> &&
      requires {
        static_cast<To>(std::declval<From>());
      };
}

namespace geode {
    #pragma warning(disable: 4251)

    class Mod;
    inline Mod* getMod();

    namespace log {
        using log_clock = std::chrono::system_clock;

        std::string generateLogName();

        struct GEODE_DLL ComponentTrait {
            virtual std::string _toString() = 0;
        };

        template <typename T> requires requires(T b) { std::stringstream() << b; }
        std::string parse(T const& thing) {
            std::stringstream buf;
            buf << thing;
            return buf.str();
        }

        template <typename T>
        struct ComponentBase : public ComponentTrait {
            T m_item;
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
        };

        template <typename ...Args> requires requires(Args... b) { (parse(b), ...); }
        void log(Severity sev, Mod* m, Args... args) {
            Log l(m, sev);
            (l.getComponents().push_back(new ComponentBase(args)), ...);

            l.pushToLoader();
        }

        template <typename ...Args>
        void debug(Args... args) { log(Severity::Debug, getMod(), args...); }

        template <typename ...Args>
        void info(Args... args) { log(Severity::Info, getMod(), args...); }

        template <typename ...Args>
        void notice(Args... args) { log(Severity::Notice, getMod(), args...); }

        template <typename ...Args>
        void warn(Args... args) { log(Severity::Warning, getMod(), args...); }

        template <typename ...Args>
        void error(Args... args) { log(Severity::Error, getMod(), args...); }

        template <typename ...Args>
        void critical(Args... args) { log(Severity::Critical, getMod(), args...); }

        template <typename ...Args>
        void alert(Args... args) { log(Severity::Alert, getMod(), args...); }

        template <typename ...Args>
        void emergency(Args... args) { log(Severity::Emergency, getMod(), args...); }

        // parse overload
        #define FF(x) \
            std::string parse(x const& thing);

        FF(cocos2d::CCObject*)
        FF(cocos2d::CCPoint)
        FF(cocos2d::CCSize)
        FF(cocos2d::CCRect)
        FF(cocos2d::CCArray*)
        FF(cocos2d::ccColor3B)
        FF(cocos2d::ccColor4B)
        FF(cocos2d::ccColor4F)
        FF(Mod*)

        #undef FF
    }
}
