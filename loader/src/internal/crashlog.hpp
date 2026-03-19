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

    struct Image {
        uintptr_t address;
        std::string name_;

        std::string_view name() const;
        std::string_view shortName() const;
    };

    struct StackFrame {
        uintptr_t address;
        Image* image;
        std::string symbol;
        uintptr_t offset;

        ptrdiff_t imageOffset() const;
        ptrdiff_t functionOffset() const;
    };

    struct Register {
        std::string_view name;
        size_t value;
    };

    struct CrashContext {
        static constexpr size_t MAX_FRAMES = 128;

        std::vector<Image> images;
        std::vector<StackFrame> frames;
        std::vector<Register> registers;
        geode::Mod* faultyMod;
        void const* crashAddr;
        Buffer infoStream;

        void initialize(void const* crashAddr);

        Image* imageFromAddress(void const* addr);
        geode::Mod* modFromAddress(void const* addr);

        // Formats a memory address into something that can more precisely point its location,
        // i.e. 0x12345678 -> "0x12345678 (GeometryDash + 0x5678)"
        void formatAddress(void const* addr, Buffer& stream);

        /// These functions are implemented differently per-platform and not defined in the common crashlog.cpp
        /// As well, each platform must call initialize() after a crash to initialize the context.

        static std::vector<Image> getImages();
        static std::vector<StackFrame> getStacktrace();
        static std::vector<Register> getRegisters();
        static std::string_view getGeodeBinaryName();
        void writeInfo(Buffer& stream);
    };

    std::string GEODE_DLL writeCrashlog(geode::Mod* faultyMod, std::string_view info, std::string_view stacktrace, std::string_view registers);
    std::string GEODE_DLL writeCrashlog(const CrashContext& ctx);

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