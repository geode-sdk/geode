#include <crashlog.hpp>

#ifdef GEODE_IS_IOS

bool crashlog::setupPlatformHandler() {
    return false;
}

bool crashlog::didLastLaunchCrash() {
    return false;
}

ghc::filesystem::path crashlog::getCrashLogDirectory() {
    return "";
}

#endif
