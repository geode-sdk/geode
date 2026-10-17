#pragma once

#include "../DefaultInclude.hpp"
#include <exception>
#include <Geode/utils/string.hpp>
#include <Geode/utils/ZStringView.hpp>

namespace geode {
    class Mod;
    Mod* getMod();
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
        GEODE_DLL void logTerminationError(const char* reason, Mod* mod);
        #ifdef GEODE_IS_WINDOWS
        GEODE_DLL void handleDebuggerException(ZStringView reason, Mod* mod, size_t platformCode);
        #endif
    }

    template <class = void>
    [[noreturn]]
    void terminate(ZStringView reason, Mod* mod = getMod(), size_t platformCode = GEODE_TERMINATE_EXCEPTION_CODE) {
        // Add the error to the logfile
        detail::logTerminationError(reason.c_str(), mod);

    #ifdef GEODE_IS_WINDOWS
        detail::handleDebuggerException(reason, mod, platformCode);
    #endif

        std::terminate();
    }

    template <class = void>
    [[noreturn]]
    void unreachable(ZStringView reason = "Unspecified", Mod* mod = getMod()) {
        terminate(reason, mod, GEODE_UNREACHABLE_EXCEPTION_CODE);
    }
}
