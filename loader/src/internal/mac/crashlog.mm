#include "../crashlog.hpp"
#include <CoreFoundation/CoreFoundation.h>
#include <signal.h>

#ifdef GEODE_IS_MACOS

bool crashlog::setupPlatformHandler() {
    return true;
}

bool crashlog::didLastLaunchCrash() {
    return false;
}

std::string crashlog::getCrashLogDirectory() {
    std::array<char, 1024> path;
    CFStringGetCString((CFStringRef)NSHomeDirectory(), path.data(), path.size(), kCFStringEncodingUTF8);
    auto crashlogDir = ghc::filesystem::path(path.data()) / "Library" / "Logs" / "DiagnosticReports";
    return crashlogDir.string();
}

#endif