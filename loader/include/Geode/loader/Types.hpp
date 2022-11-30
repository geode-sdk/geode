#pragma once

#include "../DefaultInclude.hpp"
#include "../platform/cplatform.h"
#include <string>

class InternalLoader;
class InternalMod;

namespace geode {
    /**
     * Describes the severity of the log
     * message.
     * @enum Severity
     */
    struct Severity {
        enum {
            /**
             * The message contains information
             * only relevant to the developer /
             * other mod developers.
             */
            Debug,

            /**
             * The message contains general
             * information about that logger's
             * state. The logger is still
             * working correctly.
             */
            Info,

            /**
             * The message contains information
             * about the logger's state that
             * may require special attention.
             */
            Notice,

            /**
             * The message contains information
             * about the logger's state that
             * is abnormal and may result in
             * errors if not handled properly.
             */
            Warning,

            /**
             * The message contains information
             * about a general error, such as
             * inability to read files.
             */
            Error,

            /**
             * The message contains information
             * about a severe error that may
             * cause the logger to be partly or
             * fully practically unusable.
             */
            Critical,

            /**
             * The message contains information
             * about a very severe error that
             * requires immediate attention,
             * and renders the logger completely
             * unusable.
             */
            Alert,

            /**
             * The logger has encountered an
             * error so severe it is utterly
             * unusable and can not proceed
             * its execution. This error level
             * should never be seen in-game,
             * as an error this severe will
             * most likely cause an immediate
             * unrequested exit of the
             * application, also known as a
             * crash.
             */
            Emergency,
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

        operator int() {
            return m_value;
        }

        template <class T>
        static type cast(T t) {
            return static_cast<type>(t);
        }

        static constexpr char const* toString(type lp) {
            switch (lp) {
                case Debug: return "Debug";
                case Info: return "Info";
                case Notice: return "Notice";
                case Warning: return "Warning";
                case Error: return "Error";
                case Critical: return "Critical";
                case Alert: return "Alert";
                case Emergency: return "Emergency";
            }
            return "Undefined";
        }
    };

    class Mod;
    class Setting;

    /**
     * Represents if a mod has been loaded &
     * its dependencies resolved
     */
    enum class ModResolveState {
        // Mod has not been loaded at all
        Unloaded,
        // Mod has unresolved dependencies
        Unresolved,
        // Mod has all dependencies resolved,
        // but is not loaded yet
        Resolved,
        // Mod is loaded
        Loaded,
        // Mod is loaded, however it is also
        // disabled and therefore can't be used
        Disabled,
    };

    static constexpr std::string_view GEODE_DIRECTORY = "geode";
    static constexpr std::string_view GEODE_MOD_DIRECTORY = "mods";
    static constexpr std::string_view GEODE_LOG_DIRECTORY = "log";
    static constexpr std::string_view GEODE_RESOURCE_DIRECTORY = "resources";
    static constexpr std::string_view GEODE_CONFIG_DIRECTORY = "config";
    static constexpr std::string_view GEODE_TEMP_DIRECTORY = "temp";
    static constexpr std::string_view GEODE_MOD_EXTENSION = ".geode";
    static constexpr std::string_view GEODE_INDEX_DIRECTORY = "index";

    class Mod;
    class Loader;
    class Hook;
    struct ModInfo;
    class VersionInfo;

    class Unknown;
    using unknownmemfn_t = void (Unknown::*)();
    using unknownfn_t = void (*)();

    namespace modifier {
        template <class, class>
        class FieldIntermediate;
    }
    
}

/**
 * The predeclaration of the implicit entry
 */
GEODE_API void GEODE_CALL geode_implicit_load(geode::Mod*);
