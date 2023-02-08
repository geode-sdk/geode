#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_WINDOWS

#include <Geode/loader/Mod.hpp>
#include <loader/ModImpl.hpp>

USE_GEODE_NAMESPACE();

template <typename T>
T findSymbolOrMangled(HMODULE load, char const* name, char const* mangled) {
    auto res = reinterpret_cast<T>(GetProcAddress(load, name));
    if (!res) {
        res = reinterpret_cast<T>(GetProcAddress(load, mangled));
    }
    return res;
}

char const* getUsefulError(int code) {
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
    char* text = nullptr;
    auto len = FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), text, 0, nullptr
    );
    std::string msg = "";
    if (len == 0) {
        msg = "Unknown";
    }
    else {
        if (text != nullptr) {
            msg = std::string(text, len);
            LocalFree(text);
        }
        else {
            msg = "Very Unknown";
        }
    }
    return msg + " (" + std::to_string(err) + ")";
}

Result<> Mod::Impl::loadPlatformBinary() {
    auto load = LoadLibraryW((m_tempDirName / m_info.binaryName()).wstring().c_str());
    if (load) {
        if (m_platformInfo) {
            delete m_platformInfo;
        }
        m_platformInfo = new PlatformInfo { load };
        return Ok();
    }
    return Err("Unable to load the DLL: " + getLastWinError());
}

Result<> Mod::Impl::unloadPlatformBinary() {
    auto hmod = m_platformInfo->m_hmod;
    delete m_platformInfo;
    if (FreeLibrary(hmod)) {
        return Ok();
    }
    else {
        return Err("Unable to free the DLL: " + getLastWinError());
    }
}

#endif
