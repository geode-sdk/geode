#include <crashlog.hpp>

#ifdef GEODE_IS_ANDROID

ghc::filesystem::path crashlog::getCrashLogDirectory() {
    return geode::dirs::getSaveDir();
}

bool crashlog::setupPlatformHandler() {
    return false;
}

bool crashlog::didLastLaunchCrash() {
    return false;
}

#endif
