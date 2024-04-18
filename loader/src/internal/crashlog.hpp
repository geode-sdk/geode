#pragma once

#include <Geode/Loader.hpp>
#include <ghc/fs_fwd.hpp>
#include <string>

/**
 * Platform-specific crashlog functions. Used by the loader
 */
namespace crashlog {
    /**
     * Setup platform-specific crashlog handler
     * @returns True if the handler was successfully installed, false otherwise
     */
    bool GEODE_DLL setupPlatformHandler();

    /**
     * Setup platform-specific crashlog handler for post-launch
     */
    void GEODE_DLL setupPlatformHandlerPost();

    /**
     * Check if previous launch of GD crashed unexpectedly
     * @returns True if the launch crashed, false otherwise or if indeterminate
     */
    bool GEODE_DLL didLastLaunchCrash();
    /**
     * Get the directory where crashlogs are saved on the current platform
     * @returns Path to the directory, or an empty string if the platform does
     * not support crash logs
     */
    ghc::filesystem::path GEODE_DLL getCrashLogDirectory();

    std::string GEODE_DLL writeCrashlog(geode::Mod* faultyMod, std::string const& info, std::string const& stacktrace, std::string const& registers);

    std::string getDateString(bool filesafe);

    void GEODE_DLL printGeodeInfo(std::stringstream& stream);
    void GEODE_DLL printMods(std::stringstream& stream);


}
