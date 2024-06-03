#include <crashlog.hpp>

bool crashlog::setupPlatformHandler() {
    return false;
}

bool crashlog::didLastLaunchCrash() {
    return false;
}

std::filesystem::path crashlog::getCrashLogDirectory() {
    return "";
}
