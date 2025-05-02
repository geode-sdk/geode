#include <Geode/DefaultInclude.hpp>

#include <Geode/loader/Mod.hpp>
#include <loader/ModImpl.hpp>

using namespace geode::prelude;

template <typename T>
T findSymbolOrMangled(HMODULE load, char const* name, char const* mangled) {
    auto res = reinterpret_cast<T>(GetProcAddress(load, name));
    if (!res) {
        res = reinterpret_cast<T>(GetProcAddress(load, mangled));
    }
    return res;
}

char const* getUsefulError(DWORD code) {
    switch (code) {
        case ERROR_MOD_NOT_FOUND:
            return "ERROR_MOD_NOT_FOUND; The mod is either missing the DLL "
                   "file or some of its dependencies. Make sure to list all "
                   "other mods you depend on under dependencies and include "
                   "other DLLs under resources in mod.json. "
                   "If you are not the developer of this mod, report this error "
                   "to them as it is likely not your fault.";

        case ERROR_PROC_NOT_FOUND:
            return "ERROR_PROC_NOT_FOUND; The mod tried to access "
                   "a function defined in another DLL, but the specified "
                   "function was not found. Make sure the other DLL exports "
                   "the given symbol, and that it is defined in the DLL. "
                   "If you are not the developer of this mod, report this error "
                   "to them as it is likely not your fault.";

        case ERROR_DLL_INIT_FAILED:
            return "ERROR_DLL_INIT_FAILED; Likely some global variables "
                   "in the mod threw an exception or otherwise failed. "
                   "ALSO MAKE SURE YOU ARE COMPILING IN RELEASE MODE. "
                   "If you are not the developer of this mod, report this error "
                   "to them as it is likely not your fault.";

        default: break;
    }
    return nullptr;
}

std::string getLastWinError() {
    auto err = GetLastError();
    if (!err) return "None (0)";
    auto useful = getUsefulError(err);
    if (useful) return useful;

    return formatSystemError(err);
}

Result<> Mod::Impl::loadPlatformBinary() {
    auto load = LoadLibraryW((m_tempDirName / m_metadata.getBinaryName()).wstring().c_str());
    if (load) {
        if (m_platformInfo) {
            delete m_platformInfo;
        }
        m_platformInfo = new PlatformInfo { load };

        auto geodeImplicitEntry = findSymbolOrMangled<void(*)()>(load, "geodeImplicitEntry", "_geodeImplicitEntry@0");
        if (geodeImplicitEntry) {
            geodeImplicitEntry();
        }

        auto geodeCustomEntry = findSymbolOrMangled<void(*)()>(load, "geodeCustomEntry", "_geodeCustomEntry@0");
        if (geodeCustomEntry) {
            geodeCustomEntry();
        }
        return Ok();
    }
    return Err("Unable to load the DLL: " + getLastWinError());
}
