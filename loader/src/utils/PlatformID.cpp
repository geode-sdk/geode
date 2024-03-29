
#include <Geode/platform/platform.hpp>
#include <Geode/utils/general.hpp>

using namespace geode::prelude;

PlatformID PlatformID::from(const char* str) {
    switch (hash(str)) {
        case hash("win"):
        case hash("Windows"):
        case hash("windows"): return PlatformID::Windows;
        
        case hash("mac"):
        case hash("MacOS"):
        case hash("macos"): return PlatformID::MacOS;

        case hash("iOS"):
        case hash("ios"): return PlatformID::iOS;
        
        case hash("Android32"):
        case hash("android32"): return PlatformID::Android32;
        case hash("Android64"):
        case hash("android64"): return PlatformID::Android64;

        case hash("Linux"):
        case hash("linux"): return PlatformID::Linux;
        default: return PlatformID::Unknown;
    }
}

PlatformID PlatformID::from(std::string const& str) {
    return PlatformID::from(str.c_str());
}

