#pragma once

#include <functional>

#if defined(GEODE_CALL)
	#undef GEODE_CALL
#endif

namespace geode {
	class PlatformID {
	public:
		enum {
			Unknown = -1,
			Windows,
			MacOS,
			iOS,
			Android,
			Linux,
		};
		
		using Type = decltype(Unknown);

		Type m_value;

		PlatformID(Type t) { m_value = t; }
		PlatformID& operator=(Type t) { m_value = t; return *this; }
		bool operator==(int other) const { return m_value == other; }
		bool operator==(PlatformID const& other) const { return m_value == other.m_value; }
		bool operator<(PlatformID const& other) const { return m_value < other.m_value; }
		bool operator>(PlatformID const& other) const { return m_value > other.m_value; }
		operator int() const { return m_value; }

		template<class T>
		static PlatformID from(T t) {
			return static_cast<Type>(t);
		}
		template<class T>
		T to() const {
			return static_cast<T>(m_value);
		}

		static constexpr const char* toString(Type lp) {
			switch (lp) {
				case Unknown: return "Unknown";
				case Windows: return "Windows";
				case MacOS:   return "MacOS";
				case iOS:     return "iOS";
				case Android: return "Android";
				case Linux:   return "Linux";
				default: break;
			}
			return "Undefined";
		}
	};
}

namespace std {
    template<>
    struct hash<geode::PlatformID> {
        inline std::size_t operator()(geode::PlatformID const& id) const {
			return std::hash<geode::PlatformID::Type>()(id.m_value);
		}
    };
}

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
	#define GEODE_PRETTY_FUNCTION std::string_view(__FUNCSIG__)
#else
	#define GEODE_PRETTY_FUNCTION std::string_view(__PRETTY_FUNCTION__)
#endif

// Windows
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

	#define GEODE_WINDOWS(...) __VA_ARGS__
	#define GEODE_IS_WINDOWS
	#define GEODE_IS_DESKTOP
	#define GEODE_PLATFORM_NAME "Windows"
	#define GEODE_PLATFORM_TARGET PlatformID::Windows
	#define GEODE_CALL __stdcall
	#define GEODE_HIDDEN
	#define GEODE_DUPABLE __forceinline
	#define GEODE_VIRTUAL_CONSTEXPR 
	#define GEODE_NOINLINE __declspec(noinline)
	#define GEODE_PLATFORM_EXTENSION ".dll"
	#define GEODE_PLATFORM_SHORT_IDENTIFIER "win"

	#ifdef GEODE_EXPORTING
	    #define GEODE_DLL    __declspec(dllexport)
	#else
	    #define GEODE_DLL    __declspec(dllimport)
	#endif

	#define GEODE_API extern "C" __declspec(dllexport)
	#define GEODE_EXPORT __declspec(dllexport)
	
	#include "windows.hpp"

#else
	#define GEODE_WINDOWS(...)
#endif

// MacOS / iOS
#if defined(__APPLE__)
#include <TargetConditionals.h>

#if TARGET_OS_IPHONE
	#define GEODE_MACOS(...)

	#define GEODE_IOS(...) __VA_ARGS__
	#define GEODE_IS_IOS
	#define GEODE_IS_MOBILE
	#define GEODE_PLATFORM_NAME "iOS"
	#define GEODE_PLATFORM_TARGET PlatformID::iOS
	#define GEODE_CALL
	#define GEODE_HIDDEN __attribute__((visibility("hidden")))
	#define GEODE_DUPABLE __attribute__((always_inline))
	#define GEODE_VIRTUAL_CONSTEXPR constexpr
	#define GEODE_NOINLINE __attribute__((noinline))
	#define GEODE_PLATFORM_EXTENSION ".ios.dylib"
	#define GEODE_PLATFORM_SHORT_IDENTIFIER "ios"

	#ifdef GEODE_EXPORTING
	    #define GEODE_DLL    __attribute__((visibility("default")))
	#else
	    #define GEODE_DLL    
	#endif

	#define GEODE_API extern "C" __attribute__((visibility("default")))
	#define GEODE_EXPORT __attribute__((visibility("default")))

	#include "ios.hpp"

#else
	#define GEODE_IOS(...)

	#define GEODE_MACOS(...) __VA_ARGS__
	#define GEODE_IS_MACOS
	#define GEODE_IS_DESKTOP
	#define GEODE_PLATFORM_NAME "MacOS"
	#define GEODE_PLATFORM_TARGET PlatformID::MacOS
	#define GEODE_CALL
	#define GEODE_HIDDEN __attribute__((visibility("hidden")))
	#define GEODE_DUPABLE __attribute__((always_inline))
	#define GEODE_VIRTUAL_CONSTEXPR constexpr
	#define GEODE_NOINLINE __attribute__((noinline))
	#define GEODE_PLATFORM_EXTENSION ".dylib"
	#define GEODE_PLATFORM_SHORT_IDENTIFIER "mac"

	#ifdef GEODE_EXPORTING
	    #define GEODE_DLL    __attribute__((visibility("default")))
	#else
	    #define GEODE_DLL
	#endif

	#define GEODE_API extern "C" __attribute__((visibility("default")))
	#define GEODE_EXPORT __attribute__((visibility("default")))

	#include "macos.hpp"
#endif

#else 
	#define GEODE_IOS(...)
	#define GEODE_MACOS(...)

#endif

// Android
#if defined(__ANDROID__)
	#define GEODE_ANDROID(...) __VA_ARGS__
	#define GEODE_IS_ANDROID
	#define GEODE_IS_MOBILE
	#define GEODE_PLATFORM_NAME "Android"
	#define GEODE_PLATFORM_TARGET PlatformID::Android
	#define GEODE_CALL
	#define GEODE_HIDDEN __attribute__((visibility("hidden")))
	#define GEODE_DUPABLE __attribute__((always_inline))
	#define GEODE_VIRTUAL_CONSTEXPR constexpr
	#define GEODE_NOINLINE __attribute__((noinline))
	#define GEODE_PLATFORM_EXTENSION ".so"
	#define GEODE_PLATFORM_SHORT_IDENTIFIER "android"

	#ifdef GEODE_EXPORTING
	    #define GEODE_DLL    __attribute__((visibility("default")))
	#else
	    #define GEODE_DLL    
	#endif

	#define GEODE_API extern "C" __attribute__((visibility("default")))
	#define GEODE_EXPORT __attribute__((visibility("default")))

	#include "windows.hpp"

#else
	#define GEODE_ANDROID(...)
#endif

#ifndef GEODE_PLATFORM_NAME
	#error "Unsupported PlatformID!"
#endif
