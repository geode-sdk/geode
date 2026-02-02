
#include <Geode/platform/platform.hpp>
#include <Geode/utils/general.hpp>

using namespace geode::prelude;

PlatformID PlatformID::from(std::string_view str) {
    switch (hash(str)) {
        case hash("desktop"): return PlatformID::Desktop;
        case hash("mobile"): return PlatformID::Mobile;

        case hash("win"):
        case hash("Windows"):
        case hash("windows"): return PlatformID::Windows;

        case hash("mac"): return PlatformID::Mac;

        case hash("mac-intel"):
        case hash("imac"):
        case hash("MacIntel"): return PlatformID::MacIntel;

        case hash("m1"):
        case hash("mac-arm"):
        case hash("MacArm"): return PlatformID::MacArm;

        case hash("iOS"):
        case hash("ios"): return PlatformID::iOS;

        case hash("android"): return PlatformID::Android;

        case hash("Android32"):
        case hash("android32"): return PlatformID::Android32;
        case hash("Android64"):
        case hash("android64"): return PlatformID::Android64;

        default: return PlatformID::Unknown;
    }
}

bool PlatformID::coveredBy(std::string_view str, PlatformID t) {
    switch (hash(str)) {
        case hash("desktop"): return t & PlatformID::Desktop;
        case hash("mobile"): return t & PlatformID::Mobile;

        case hash("win"): return t & PlatformID::Windows;

        case hash("mac"): return t & PlatformID::Mac;
        case hash("mac-intel"): return t & PlatformID::MacIntel;
        case hash("mac-arm"): return t & PlatformID::MacArm;

        case hash("ios"): return t & PlatformID::iOS;

        case hash("android"): return t & PlatformID::Android;
        case hash("android32"): return t & PlatformID::Android32;
        case hash("android64"): return t & PlatformID::Android64;

        default: return false;
    }
}

std::string_view PlatformID::toString(PlatformID::Type lp) {
    switch (lp) {
        case Unknown: return "Unknown";
        case Windows: return "Windows";
        case MacIntel: return "MacIntel";
        case MacArm: return "MacArm";
        case iOS: return "iOS";
        case Android32: return "Android32";
        case Android64: return "Android64";
        default: return "Undefined";
    }
}

std::string_view PlatformID::toShortString(PlatformID::Type lp, bool ignoreArch) {
    switch (lp) {
        case Unknown: return "unknown";
        case Windows: return "win";
        case MacIntel: return ignoreArch ? "mac" : "mac-intel";
        case MacArm: return ignoreArch ? "mac" : "mac-arm";
        case iOS: return "ios";
        case Android32: return ignoreArch ? "android" : "android32";
        case Android64: return ignoreArch ? "android" : "android64";
        default: return "undefined";
    }
}

