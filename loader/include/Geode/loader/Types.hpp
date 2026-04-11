#pragma once

#include "../DefaultInclude.hpp"
#include "../platform/cplatform.h"
#include <matjson.hpp>

#include <string>

namespace geode {
    /**
     * Describes the severity of the log
     * message.
     * @enum Severity
     */
    struct Severity {
        enum {
            /**
             * This message contains information
             * so verbose that you shouldn't enable
             * it unless you want your console to
             * be spammed with messages, creating
             * gigabytes of log files in the process.
             */
            Trace = -1,

            /**
             * The message contains information
             * only relevant to the developer /
             * other mod developers.
             */
            Debug = 0,

            /**
             * The message contains general
             * information about that logger's
             * state. The logger is still
             * working correctly.
             */
            Info = 1,

            /**
             * The message contains information
             * about the logger's state that
             * is abnormal and may result in
             * errors if not handled properly.
             */
            Warning = 2,

            /**
             * The message contains information
             * about a general error, such as
             * inability to read files.
             */
            Error = 3,
        };

        using type = decltype(Debug);

        type m_value;

        Severity(type t) {
            m_value = t;
        }

        Severity& operator=(type t) {
            m_value = t;
            return *this;
        }

        bool operator==(int other) const {
            return m_value == other;
        }

        bool operator==(Severity const& other) const {
            return m_value == other.m_value;
        }

        operator int() const {
            return m_value;
        }

        template <class T>
        static type cast(T t) {
            return static_cast<type>(t);
        }

        static constexpr char const* toString(type lp) {
            switch (lp) {
                case Trace: return "Trace";
                case Debug: return "Debug";
                case Info: return "Info";
                case Warning: return "Warning";
                case Error: return "Error";
            }
            return "Undefined";
        }
    };

    constexpr std::string_view GEODE_MOD_EXTENSION = ".geode";

    class Mod;
    class Loader;
    class Hook;
    class VersionInfo;

    class Unknown;

    namespace modifier {
        template <class, class>
        class FieldIntermediate;
    }

    using ModJson = matjson::Value;
}

/**
 * The predeclaration of the implicit entry
 */
GEODE_API void GEODE_CALL geode_implicit_load(geode::Mod*);
