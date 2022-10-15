#pragma once

#include <functional>
#include "cplatform.h"

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
	#define GEODE_PRETTY_FUNCTION std::string(__FUNCSIG__)
#else
	#define GEODE_PRETTY_FUNCTION std::string(__PRETTY_FUNCTION__)
#endif

// Windows
#ifdef GEODE_IS_WINDOWS

	#define GEODE_PLATFORM_TARGET PlatformID::Windows
	#define GEODE_HIDDEN
	#define GEODE_INLINE __forceinline
	#define GEODE_VIRTUAL_CONSTEXPR 
	#define GEODE_NOINLINE __declspec(noinline)

	#ifdef GEODE_EXPORTING
	    #define GEODE_DLL    __declspec(dllexport)
	#else
	    #define GEODE_DLL    __declspec(dllimport)
	#endif

	#define GEODE_API extern "C" __declspec(dllexport)
	#define GEODE_EXPORT __declspec(dllexport)
	
	#include "windows.hpp"

#elif defined(GEODE_IS_MACOS)

	#define GEODE_PLATFORM_TARGET PlatformID::MacOS
	#define GEODE_HIDDEN __attribute__((visibility("hidden")))
	#define GEODE_INLINE inline __attribute__((always_inline))
	#define GEODE_VIRTUAL_CONSTEXPR constexpr
	#define GEODE_NOINLINE __attribute__((noinline))

	#ifdef GEODE_EXPORTING
	    #define GEODE_DLL    __attribute__((visibility("default")))
	#else
	    #define GEODE_DLL
	#endif

	#define GEODE_API extern "C" __attribute__((visibility("default")))
	#define GEODE_EXPORT __attribute__((visibility("default")))

	#include "macos.hpp"

#elif defined(GEODE_IS_IOS)

	#define GEODE_PLATFORM_TARGET PlatformID::iOS
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

	#include "ios.hpp"
	
#elif defined(GEODE_IS_ANDROID)

	#define GEODE_PLATFORM_TARGET PlatformID::Android
	#define GEODE_HIDDEN __attribute__((visibility("hidden")))
	#define GEODE_INLINE inline __attribute__((always_inline))
	#define GEODE_VIRTUAL_CONSTEXPR constexpr
	#define GEODE_NOINLINE __attribute__((noinline))

	#ifdef GEODE_EXPORTING
	    #define GEODE_DLL    __attribute__((visibility("default")))
	#else
	    #define GEODE_DLL    
	#endif

	#define GEODE_API extern "C" __attribute__((visibility("default")))
	#define GEODE_EXPORT __attribute__((visibility("default")))

	#include "android.hpp"

#else

	#error "Unsupported Platform!"

#endif
