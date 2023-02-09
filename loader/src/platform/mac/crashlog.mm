#include <crashlog.hpp>

#ifdef GEODE_IS_MACOS

#include <array>
#include <ghc/fs_fwd.hpp>

#import <Foundation/Foundation.h>

bool crashlog::setupPlatformHandler() {
    return true;
}

bool crashlog::didLastLaunchCrash() {
    return false;
}

ghc::filesystem::path crashlog::getCrashLogDirectory() {
    std::array<char, 1024> path;
    CFStringGetCString(
        (CFStringRef)NSHomeDirectory(), path.data(), path.size(), kCFStringEncodingUTF8
    );
    auto crashlogDir =
        ghc::filesystem::path(path.data()) / "Library" / "Logs" / "DiagnosticReports";
    return crashlogDir.string();
}

#endif