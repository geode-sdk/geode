#pragma once

#include "../DefaultInclude.hpp"
#include "../loader/Log.hpp"
#include <source_location>

namespace geode {
    class Mod;
}

namespace geode::utils {
    namespace detail {
        // This needs to do stuff with `Mod*` which is not included in the file
        GEODE_DLL std::string fmtTerminateError(const char* reason, Mod* mod, std::source_location loc);
    }

    template <class = void>
    [[noreturn]]
    void terminate(
        std::string const& reason,
        Mod* mod = getMod(),
        std::source_location loc = std::source_location::current()
    ) {
        auto fullError = detail::fmtTerminateError(reason.c_str(), mod, loc);

        // Add the error to the logfile
        log::error("{}", fullError);

    #ifdef GEODE_IS_WINDOWS
        // If a debugger is attached, start debugging
        if (IsDebuggerPresent()) {
            OutputDebugStringA(reason.c_str());
            DebugBreak();
        }
        // Otherwise just terminate
        else {
            MessageBoxA(nullptr, "A Mod Crashed", fullError.c_str(), MB_ICONERROR);
            std::terminate();
        }
    #else
        std::terminate();
    #endif
    }
    
    template <class = void>
    [[noreturn]]
    void unreachable(
        std::string const& reason = "Unspecified",
        Mod* mod = getMod(),
        std::source_location loc = std::source_location::current()
    ) {
        return terminate(reason + " (Unreachable code path)", mod, loc);
    }
}
