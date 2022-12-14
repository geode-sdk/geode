#pragma once

#include "Types.hpp"

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/ranges.hpp>
#include <ccTypes.h>
#include <chrono>
#include <ghc/filesystem.hpp>
#include <sstream>
#include <vector>
#include <span>

namespace geode {
#pragma warning(disable : 4251)

    class Mod;
    Mod* getMod();

    namespace log {
        using log_clock = std::chrono::system_clock;
        GEODE_DLL std::string generateLogName();

        // Parse overloads
        GEODE_DLL std::string parse(cocos2d::CCArray*);
        GEODE_DLL std::string parse(cocos2d::ccColor3B const&);
        GEODE_DLL std::string parse(cocos2d::ccColor4B const&);
        GEODE_DLL std::string parse(cocos2d::ccColor4F const&);
        GEODE_DLL std::string parse(cocos2d::CCNode*);
        GEODE_DLL std::string parse(cocos2d::CCObject*);
        GEODE_DLL std::string parse(cocos2d::CCPoint const&);
        GEODE_DLL std::string parse(cocos2d::CCRect const&);
        GEODE_DLL std::string parse(cocos2d::CCSize const&);
        GEODE_DLL std::string parse(Mod*);


        template <class T>
            requires std::convertible_to<T*, cocos2d::CCNode*>
        std::string parse(T* node) {
            return parse(static_cast<cocos2d::CCNode*>(node));
        }
        template <class T>
            requires(
                std::convertible_to<T*, cocos2d::CCObject*> &&
                !std::convertible_to<T*, cocos2d::CCNode*>
            )
        std::string parse(T* node) {
            return parse(static_cast<cocos2d::CCObject*>(node));
        }

        template <typename T>
            requires requires(T b) {
                std::stringstream() << b;
            }
        std::string parse(T const& thing) {
            std::stringstream buf;
            buf << thing;
            return buf.str();
        }

        // Log component system

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

        // Log

        class GEODE_DLL Log final {
            Mod* m_sender;
            log_clock::time_point m_time;
            std::vector<ComponentTrait*> m_components;
            Severity m_severity;

            friend class Logger;
        public:
            ~Log();
            Log(Mod* mod, Severity sev);
            Log(Log&& l) = default;
            Log& operator=(Log&& l) = default;
            bool operator==(Log const& l);

            std::string toString(bool logTime = true) const;

            std::vector<ComponentTrait*>& getComponents();
            log_clock::time_point getTime() const;
            Mod* getSender() const;
            Severity getSeverity() const;

            void addFormat(std::string_view formatStr, std::span<ComponentTrait*> comps);
        };

        class GEODE_DLL Logger {
        private:
            static inline std::vector<Log> s_logs;
            static inline std::ofstream s_logStream;

            Logger() = delete;
            ~Logger() = delete;

            // logs
            static void _push(Log&& log);

        public:
            static void setup();

            static inline void push(Log&& log) {
                Logger::_push(std::move(log));
            }

            static inline void pop(Log* log) {
                geode::utils::ranges::remove(Logger::s_logs, *log);
            }

            static std::vector<Log*> list();
            static void clear();
        };

        template <typename... Args>
            requires requires(Args... b) {
                (parse(b), ...);
            }
        void internalLog(Severity sev, Mod* m, std::string_view formatStr, Args... args) {
            Log l(m, sev);

            std::array<ComponentTrait*, sizeof...(Args)> comps = { static_cast<ComponentTrait*>(new ComponentBase(args))... };
            l.addFormat(formatStr, comps);

            Logger::push(std::move(l));
        }

        template <typename... Args>
        void debug(Args... args) {
#ifdef GEODE_DEBUG
            internalLog(Severity::Debug, getMod(), args...);
#endif
        }

        template <typename... Args>
        void info(Args... args) {
            internalLog(Severity::Info, getMod(), args...);
        }

        template <typename... Args>
        void warn(Args... args) {
            internalLog(Severity::Warning, getMod(), args...);
        }

        template <typename... Args>
        void error(Args... args) {
            internalLog(Severity::Error, getMod(), args...);
        }
    }
}
