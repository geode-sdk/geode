#include <crashlog.hpp>

bool crashlog::setupPlatformHandler() {
    return false;
}

bool crashlog::didLastLaunchCrash() {
    return false;
}

ghc::filesystem::path crashlog::getCrashLogDirectory() {
    return "";
}
