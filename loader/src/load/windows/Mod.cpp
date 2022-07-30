#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_WINDOWS

#include <Geode/Geode.hpp>
USE_GEODE_NAMESPACE();

template<typename T>
T findSymbolOrMangled(HMODULE load, const char* name, const char* mangled) {
    auto res = reinterpret_cast<T>(GetProcAddress(load, name));
    if (!res) {
        res = reinterpret_cast<T>(GetProcAddress(load, mangled));
    }
    return res;
}

const char* getUsefulError(int code) {
    switch (code) {
        case ERROR_MOD_NOT_FOUND: return
            "ERROR_MOD_NOT_FOUND; The mod is either missing the DLL "
            "file or some of its dependencies. Make sure to list all "
            "other mods you depend on under dependencies and include "
            "other DLLs under resources in mod.json. "
            "If you are not the developer of this mod, report this error "
            "to them as it is likely not your fault.";

        case ERROR_PROC_NOT_FOUND: return
            "ERROR_PROC_NOT_FOUND; The mod tried to access "
            "a function defined in another DLL, but the specified "
            "function was not found. Make sure the other DLL exports "
            "the given symbol, and that it is defined in the DLL. "
            "If you are not the developer of this mod, report this error "
            "to them as it is likely not your fault.";
                
        case ERROR_DLL_INIT_FAILED: return
            "ERROR_DLL_INIT_FAILED; Likely some global variables "
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
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        err,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        text,
        0,
        nullptr
    );
    std::string msg = "";
    if (len == 0) {
        msg = "Unknown";
    } else {
        if (text != nullptr) {
            msg = std::string(text, len);
            LocalFree(text);
        } else {
            msg = "Very Unknown";
        }
    }
    return msg + " (" + std::to_string(err) + ")";
}

Result<> Mod::loadPlatformBinary() {
    auto load = LoadLibraryW((this->m_tempDirName / this->m_info.m_binaryName).wstring().c_str());
    if (load) {
        this->m_implicitLoadFunc = findSymbolOrMangled<geode_load>(load, "geode_implicit_load", "_geode_implicit_load@4");
        this->m_loadFunc = findSymbolOrMangled<geode_load>(load, "geode_load", "_geode_load@4");
        this->m_unloadFunc = findSymbolOrMangled<geode_unload>(load, "geode_unload", "_geode_unload@0");
        this->m_enableFunc = findSymbolOrMangled<geode_enable>(load, "geode_enable", "_geode_enable@0");
        this->m_disableFunc = findSymbolOrMangled<geode_disable>(load, "geode_disable", "_geode_disable@0");
        this->m_saveDataFunc = findSymbolOrMangled<geode_save_data>(load, "geode_save_data", "_geode_save_data@4");
        this->m_loadDataFunc = findSymbolOrMangled<geode_load_data>(load, "geode_load_data", "_geode_load_data@4");
        this->m_settingUpdatedFunc = findSymbolOrMangled<geode_setting_updated>(load, "geode_setting_updated", "_geode_setting_updated@8");

        if (!this->m_implicitLoadFunc && !this->m_loadFunc) {
            return Err<>("Unable to find mod entry point (lacking both implicit & explicit definition)");
        }

        if (this->m_platformInfo) {
            delete this->m_platformInfo;
        }
        this->m_platformInfo = new PlatformInfo { load };

        return Ok<>();
    }
    return Err<>("Unable to load the DLL: " + getLastWinError());
}

Result<> Mod::unloadPlatformBinary() {
    auto hmod = this->m_platformInfo->m_hmod;
    delete this->m_platformInfo;
    if (FreeLibrary(hmod)) {
        this->m_implicitLoadFunc = nullptr;
        this->m_unloadFunc = nullptr;
        this->m_loadFunc = nullptr;
        return Ok<>();
    } else {
        return Err<>("Unable to free the DLL: " + getLastWinError());
    }
}

#endif
