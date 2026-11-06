#include <Geode/utils/terminate.hpp>
#include <Geode/loader/Mod.hpp>
#include "../loader/Log.hpp"

#ifdef GEODE_IS_WINDOWS
#include <Windows.h>
#endif

using namespace geode::prelude;

void geode::utils::detail::logTerminationError(const char* reason, Mod* mod) {
    // invoke logImpl directly to indicate what mod it came from
    log::logImpl(
        Severity::Error, mod,
        "The mod '{}' by {} has deliberately asked the game to crash.\nReason: {}",
        mod->getID(), fmt::format("{}", fmt::join(mod->getDevelopers(), ", ")), reason
    );
}


#ifdef GEODE_IS_WINDOWS
void geode::utils::detail::handleDebuggerException(ZStringView reason, Mod* mod, size_t platformCode) {
    // If a debugger is attached, start debugging
    if (IsDebuggerPresent()) {
        OutputDebugStringW(utils::string::utf8ToWide(reason).c_str());
        DebugBreak();
    }
    // Otherwise terminate by raising an exception (which is caught by the crashlog handler)
    else {
        std::array<const void*, 2> errorList { reason.c_str(), mod };
        RaiseException(
            platformCode,
            EXCEPTION_NONCONTINUABLE,
            2, reinterpret_cast<ULONG_PTR*>(errorList.data())
        );
    }
}
#endif
