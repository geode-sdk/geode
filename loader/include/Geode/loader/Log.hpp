#pragma once

#include "Types.hpp"
#include "../utils/Result.hpp"

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/ranges.hpp>
#include <ccTypes.h>
#include <chrono>
#include <ghc/fs_fwd.hpp>
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

        // todo: maybe add a debugParse function for these?

        template <class T>
            requires requires(T t) {
                parse(t);
            }
        std::string parse(std::optional<T> const& thing) {
            if (thing.has_value()) {
                return "opt(" + parse(thing.value()) + ")";
            }
            return "nullopt";
        }

        template <class T>
            requires requires(T t) {
                parse(t);
            }
        std::string parse(std::vector<T> const& thing) {
            std::string res = "[";
            bool first = true;
            for (auto& t : thing) {
                if (!first) {
                    res += ", ";
                }
                first = false;
                res += parse(t);
            }
            res += "]";
            return res;
        }

        template <class A, class B>
            requires requires(A a, B b) {
                parse(a);
                parse(b);
            }
        std::string parse(std::pair<A, B> const& thing) {
            return "(" + parse(thing.first) + ", " + parse(thing.second) + ")";
        }

        template <class... T, std::size_t... Is>
        std::string parseTupleImpl(std::tuple<T...> const& tuple, std::index_sequence<Is...>) {
            std::string ret = "(";
            ((ret += (Is == 0 ? "" : ", ") + parse(std::get<Is>(tuple))), ...);
            ret += ")";
            return ret;
        }

        template <class... T>
            requires requires(T... t) {
                (parse(t), ...);
            }
        std::string parse(std::tuple<T...> const& tuple) {
            return parseTupleImpl(tuple, std::index_sequence_for<T...> {});
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

            [[deprecated("Will be removed in next version")]]
            void addFormat(std::string_view formatStr, std::span<ComponentTrait*> comps);

            Result<> addFormatNew(std::string_view formatStr, std::span<ComponentTrait*> comps);
        };

        class GEODE_DLL Logger {
        private:
            static std::vector<Log>& logs();
            static std::ofstream& logStream();

            Logger() = delete;
            ~Logger() = delete;

            // logs
        public:
            static void setup();

            static void push(Log&& log);

            static void pop(Log* log);

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
            auto res = l.addFormatNew(formatStr, comps);

            if (res.isErr()) {
                internalLog(Severity::Warning, getMod(), "Error parsing log format \"{}\": {}", formatStr, res.unwrapErr());
                return;
            }

            Logger::push(std::move(l));
        }

        template <typename... Args>
        void debug(Args... args) {
            internalLog(Severity::Debug, getMod(), args...);
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
