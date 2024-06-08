
#include <Geode/platform/platform.hpp>
#include <Geode/utils/general.hpp>

using namespace geode::prelude;

PlatformID PlatformID::from(const char* str) {
    switch (hash(str)) {
        case hash("win"):
        case hash("Windows"):
        case hash("windows"): return PlatformID::Windows;
        
        case hash("mac-intel"):
        case hash("imac"):
        case hash("MacIntel"): return PlatformID::MacIntel;

        case hash("m1"):
        case hash("mac-arm"):
        case hash("MacArm"): return PlatformID::MacArm;

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

bool PlatformID::coveredBy(const char* str, PlatformID t) {
    switch (hash(str)) {
        case hash("win"): return t == PlatformID::Windows;

        case hash("mac"): return t == PlatformID::MacIntel || t == PlatformID::MacArm;
        case hash("mac-intel"): return t == PlatformID::MacIntel;
        case hash("mac-arm"): return t == PlatformID::MacArm;

        case hash("ios"): return t == PlatformID::iOS;

        case hash("android"): return t == PlatformID::Android32 || t == PlatformID::Android64;
        case hash("android32"): return t == PlatformID::Android32;
        case hash("android64"): return t == PlatformID::Android64;

        case hash("linux"): return t == PlatformID::Linux;

        default: return false;
    }
}

bool PlatformID::coveredBy(std::string const& str, PlatformID t) {
    return PlatformID::coveredBy(str.c_str(), t);
}

PlatformID PlatformID::from(std::string const& str) {
    return PlatformID::from(str.c_str());
}

