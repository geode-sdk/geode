#pragma once

#include "cplatform.h"
#include <string>
#include <functional>
#include <memory>

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
    #define GEODE_PRETTY_FUNCTION std::string(__FUNCSIG__)
#else
    #define GEODE_PRETTY_FUNCTION std::string(__PRETTY_FUNCTION__)
#endif

// Windows
#ifdef GEODE_IS_WINDOWS

    #define GEODE_HIDDEN
    #define GEODE_INLINE __forceinline
    #define GEODE_VIRTUAL_CONSTEXPR
    #define GEODE_NOINLINE __declspec(noinline)

    #ifdef GEODE_EXPORTING
        #define GEODE_DLL __declspec(dllexport)
    #else
        #define GEODE_DLL __declspec(dllimport)
    #endif

    #define GEODE_API extern "C" __declspec(dllexport)
    #define GEODE_EXPORT __declspec(dllexport)

    #if defined(GEODE_IS_WINDOWS64)
        #define GEODE_IS_X64
        #define GEODE_CDECL_CALL
    #else 
        #define GEODE_IS_X86
        #define GEODE_CDECL_CALL __cdecl
	#endif

    #include "windows.hpp"

#elif defined(GEODE_IS_MACOS)

    #define GEODE_HIDDEN __attribute__((visibility("hidden")))
    #define GEODE_INLINE inline __attribute__((always_inline))
    #define GEODE_VIRTUAL_CONSTEXPR constexpr
    #define GEODE_NOINLINE __attribute__((noinline))

    #ifdef GEODE_EXPORTING
        #define GEODE_DLL __attribute__((visibility("default")))
    #else
        #define GEODE_DLL
    #endif

    #define GEODE_API extern "C" __attribute__((visibility("default")))
    #define GEODE_EXPORT __attribute__((visibility("default")))

    #define GEODE_IS_X64
    #define GEODE_CDECL_CALL

    #include "macos.hpp"

#elif defined(GEODE_IS_IOS)

    #define GEODE_HIDDEN __attribute__((visibility("hidden")))
    #define GEODE_INLINE inline __attribute__((always_inline))
    #define GEODE_VIRTUAL_CONSTEXPR constexpr
    #define GEODE_NOINLINE __attribute__((noinline))

    #ifdef GEODE_EXPORTING
        #define GEODE_DLL __attribute__((visibility("default")))
    #else
        #define GEODE_DLL
    #endif

    #define GEODE_API extern "C" __attribute__((visibility("default")))
    #define GEODE_EXPORT __attribute__((visibility("default")))

    #define GEODE_IS_X64
    #define GEODE_CDECL_CALL

    #include "ios.hpp"

#elif defined(GEODE_IS_ANDROID)

    #define GEODE_HIDDEN __attribute__((visibility("hidden")))
    #define GEODE_INLINE inline __attribute__((always_inline))
    #define GEODE_VIRTUAL_CONSTEXPR constexpr
    #define GEODE_NOINLINE __attribute__((noinline))

    #ifdef GEODE_EXPORTING
        #define GEODE_DLL __attribute__((visibility("default")))
    #else
        #define GEODE_DLL
    #endif

    #define GEODE_API extern "C" __attribute__((visibility("default")))
    #define GEODE_EXPORT __attribute__((visibility("default")))

    #if defined(GEODE_IS_ANDROID64)
        #define GEODE_IS_X64
    #else 
        #define GEODE_IS_X86
    #endif
    #define GEODE_CDECL_CALL

    #include "android.hpp"

#else

    #error "Unsupported Platform!"

#endif

namespace geode {
    class PlatformID {
    public:
        // todo in v4: make these flags and add archless Mac and Android as well as Desktop and Mobile and remove Linux
        enum {
            Unknown   = -1,
            Windows   = 0,
            MacIntel  = 1,
            MacArm    = 2,
            iOS       = 3,
            Android32 = 4,
            Android64 = 5,
            Linux     = 6,
        };

        using Type = decltype(Unknown);

        Type m_value;

        constexpr PlatformID(Type t) {
            m_value = t;
        }

        constexpr PlatformID& operator=(Type t) {
            m_value = t;
            return *this;
        }

        constexpr bool operator==(int other) const {
            return m_value == other;
        }

        constexpr bool operator==(PlatformID const& other) const {
            return m_value == other.m_value;
        }

        constexpr bool operator<(PlatformID const& other) const {
            return m_value < other.m_value;
        }

        constexpr bool operator>(PlatformID const& other) const {
            return m_value > other.m_value;
        }

        constexpr operator int() const {
            return m_value;
        }

        /**
         * Parse string into PlatformID. String should be all-lowercase, for 
         * example "windows" or "linux"
         */
        static GEODE_DLL PlatformID from(const char* str);
        static GEODE_DLL PlatformID from(std::string const& str);

        /**
         * Determines if a given platform string "covers" the given platform.
         * For example, "android" is covered by Platform::Android32 and Platform::Android64.
         * Input string must follow the format in PlatformID::toShortString.
         */
        static GEODE_DLL bool coveredBy(const char* str, PlatformID t);
        static GEODE_DLL bool coveredBy(std::string const& str, PlatformID t);
        /**
         * Returns the list of platforms covered by this string name. For 
         * example, "android" would return both Android32 and Android64
         * todo in v4: deprecate this as the flagged version deals with this
         */
        static GEODE_DLL std::vector<PlatformID> getCovered(std::string_view str);

        // todo in v4: this does not need to be constexpr in the header. dllexport it
        static constexpr char const* toString(Type lp) {
            switch (lp) {
                case Unknown: return "Unknown";
                case Windows: return "Windows";
                case MacIntel: return "MacIntel";
                case MacArm: return "MacArm";
                case iOS: return "iOS";
                case Android32: return "Android32";
                case Android64: return "Android64";
                case Linux: return "Linux";
                default: break;
            }
            return "Undefined";
        }

        // todo in v4: this does not need to be constexpr in the header. dllexport it
        static constexpr char const* toShortString(Type lp, bool ignoreArch = false) {
            switch (lp) {
                case Unknown: return "unknown";
                case Windows: return "win";
                case MacIntel: return ignoreArch ? "mac" : "mac-intel";
                case MacArm: return ignoreArch ? "mac" : "mac-arm";
                case iOS: return "ios";
                case Android32: return ignoreArch ? "android" : "android32";
                case Android64: return ignoreArch ? "android" : "android64";
                case Linux: return "linux";
                default: break;
            }
            return "undefined";
        }

        template <class T>
            requires requires(T t) {
                static_cast<Type>(t);
            }
        constexpr static PlatformID from(T t) {
            return static_cast<Type>(t);
        }

        template <class T>
            requires requires(Type t) {
                static_cast<T>(t);
            }
        constexpr T to() const {
            return static_cast<T>(m_value);
        }
    };
}

namespace std {
    template <>
    struct hash<geode::PlatformID> {
        inline std::size_t operator()(geode::PlatformID const& id) const {
            return std::hash<geode::PlatformID::Type>()(id.m_value);
        }
    };
}

#ifdef GEODE_IS_WINDOWS
    #define GEODE_PLATFORM_TARGET PlatformID::Windows
#elif defined(GEODE_IS_ARM_MAC)
    #define GEODE_PLATFORM_TARGET PlatformID::MacArm
#elif defined(GEODE_IS_INTEL_MAC)
    #define GEODE_PLATFORM_TARGET PlatformID::MacIntel
#elif defined(GEODE_IS_IOS)
    #define GEODE_PLATFORM_TARGET PlatformID::iOS
#elif defined(GEODE_IS_ANDROID32)
    #define GEODE_PLATFORM_TARGET PlatformID::Android32
#elif defined(GEODE_IS_ANDROID64)
    #define GEODE_PLATFORM_TARGET PlatformID::Android64
#endif

// this is cross-platform so not duplicating it across the typeinfo_cast definitions
namespace geode::cast {
    template<class T, class U>
    std::shared_ptr<T> typeinfo_pointer_cast(std::shared_ptr<U> const& r) noexcept {
        // https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast
        auto p = typeinfo_cast<typename std::shared_ptr<T>::element_type*>(r.get());
        return std::shared_ptr<T>(r, p);
    }
}
