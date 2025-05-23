
#include <Geode/platform/platform.hpp>
#include <Geode/utils/general.hpp>
#include <Geode/utils/ranges.hpp>

using namespace geode::prelude;

PlatformID PlatformID::from(const char* str) {
    // todo in v5: this should just be
    // "win" -> Windows
    // "mac", "mac-intel", "mac-arm" -> Mac
    // "ios" -> iOS
    // "android", "android32", "android64" -> Android
    // no linux
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

        default: return PlatformID::Unknown;
    }
}

bool PlatformID::coveredBy(const char* str, PlatformID t) {
    // todo in v5: this is ridiculously inefficient currently - in v5 just use a flag check!
    return ranges::contains(getCovered(str), t);
}

bool PlatformID::coveredBy(std::string const& str, PlatformID t) {
    return PlatformID::coveredBy(str.c_str(), t);
}

std::vector<PlatformID> PlatformID::getCovered(std::string_view str) {
    switch (hash(str)) {
        case hash("desktop"): return { PlatformID::Windows, PlatformID::MacArm, PlatformID::MacIntel };
        case hash("mobile"): return { PlatformID::iOS, PlatformID::Android32, PlatformID::Android64 };

        case hash("win"): return { PlatformID::Windows };

        case hash("mac"): return { PlatformID::MacIntel, PlatformID::MacArm };
        case hash("mac-intel"): return { PlatformID::MacIntel };
        case hash("mac-arm"): return { PlatformID::MacArm };

        case hash("ios"): return { PlatformID::iOS };

        case hash("android"): return { PlatformID::Android32, PlatformID::Android64 };
        case hash("android32"): return { PlatformID::Android32 };
        case hash("android64"): return { PlatformID::Android64 };

        default: return {};
    }
}

PlatformID PlatformID::from(std::string const& str) {
    return PlatformID::from(str.c_str());
}

