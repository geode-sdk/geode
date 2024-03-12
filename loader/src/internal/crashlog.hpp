#pragma once

#include <ghc/fs_fwd.hpp>
#include <string>
#include <sstream>
#include <Geode/Loader.hpp>

namespace crashlog {
    bool setupPlatformHandler();
    void setupPlatformHandlerPost();
    bool didLastLaunchCrash();
    ghc::filesystem::path getCrashLogDirectory();
    std::string writeCrashlog(geode::Mod* faultyMod, const std::string& info, const std::string& stacktrace, const std::string& registers);
    std::string getDateString(bool filesafe);
    void printGeodeInfo(std::stringstream& stream);
    void printMods(std::stringstream& stream);
}
