#pragma once

#include <Geode/Loader.hpp>
#include <Geode/utils/StringBuffer.hpp>
#include <filesystem>
#include <string>

/**
 * Platform-specific crashlog functions. Used by the loader
 */
namespace crashlog {
    using Buffer = geode::utils::StringBuffer<1>;

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
    std::filesystem::path GEODE_DLL getCrashLogDirectory();


    std::string GEODE_DLL writeCrashlog(geode::Mod* faultyMod, std::string_view info, std::string_view stacktrace, std::string_view registers);

    std::string writeCrashlog(geode::Mod* faultyMod, std::string_view info, std::string_view stacktrace, std::string_view registers, std::filesystem::path& outCrashlogPath);

    std::string getDateString(bool filesafe);

    void GEODE_DLL printGeodeInfo(Buffer& stream);
    void GEODE_DLL printMods(Buffer& stream);

    struct FunctionBinding {
        std::string name;
        uintptr_t offset;
    };

    void updateFunctionBindings();
    std::string_view GEODE_DLL lookupClosestFunction(uintptr_t& address);
    std::string_view GEODE_DLL lookupFunctionByOffset(uintptr_t address);
}

template <>
struct matjson::Serialize<crashlog::FunctionBinding> {
    static geode::Result<crashlog::FunctionBinding> fromJson(Value const& value) {
        return geode::Ok(crashlog::FunctionBinding{
            GEODE_UNWRAP(value[1].asString()),
            GEODE_UNWRAP(value[0].as<uintptr_t>())
        });
    }

    static Value toJson(crashlog::FunctionBinding const& binding) {
        auto arr = Value::array();
        arr.push(binding.offset);
        arr.push(binding.name);
        return arr;
    }
};