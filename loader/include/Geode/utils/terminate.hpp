#pragma once

#include "../DefaultInclude.hpp"
#include "../loader/Log.hpp"
#include <source_location>

namespace geode {
    class Mod;
}

namespace geode::utils {
#ifdef GEODE_IS_WINDOWS
    static constexpr size_t GEODE_TERMINATE_EXCEPTION_CODE   = 0x4000;
    static constexpr size_t GEODE_UNREACHABLE_EXCEPTION_CODE = 0x4001;

    static constexpr bool isGeodeExceptionCode(size_t code) {
        return GEODE_TERMINATE_EXCEPTION_CODE <= code && code <= GEODE_UNREACHABLE_EXCEPTION_CODE;
    }
#else
    static constexpr size_t GEODE_TERMINATE_EXCEPTION_CODE = 0;
    static constexpr size_t GEODE_UNREACHABLE_EXCEPTION_CODE = 0;

    static constexpr bool isGeodeExceptionCode(size_t code) {
        return false;
    }
#endif

    namespace detail {
        // This needs to do stuff with `Mod*` which is not included in the file
        GEODE_DLL std::string fmtTerminateError(const char* reason, Mod* mod, std::source_location loc);
    }

    template <class = void>
    [[noreturn]]
    void terminate(
        std::string const& reason,
        Mod* mod = getMod(),
        std::source_location loc = std::source_location::current(),
        size_t platformCode = GEODE_TERMINATE_EXCEPTION_CODE
    ) {
        // Add the error to the logfile
        log::error("{}", detail::fmtTerminateError(reason.c_str(), mod, loc));

    #ifdef GEODE_IS_WINDOWS
        // If a debugger is attached, start debugging
        if (IsDebuggerPresent()) {
            OutputDebugStringA(reason.c_str());
            DebugBreak();
        }
        // Otherwise terminate by raising an exception (which is caught by the crashlog handler)
        else {
            std::array<const void*, 2> errorList { static_cast<const void*>(reason.c_str()), mod };
            RaiseException(
                platformCode,
                EXCEPTION_NONCONTINUABLE,
                2, reinterpret_cast<ULONG_PTR*>(errorList.data())
            );
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
        terminate(reason, mod, loc, GEODE_UNREACHABLE_EXCEPTION_CODE);
    }
}
