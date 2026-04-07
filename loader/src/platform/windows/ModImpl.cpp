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
            return "ERROR_MOD_NOT_FOUND;\n\nThe mod is either missing the DLL "
                   "file or some of its dependencies. Make sure to list all "
                   "other mods you depend on under dependencies in mod.json  "
                   "and that the mod is not built in Debug. "
                   "\n\nIf you are not the developer of this mod, report this error "
                   "to them as it is likely not your fault. Also try [updating "
                   "Microsoft Visual C++ Redistributable 2026 (64-bit).](https://aka.ms/vc14/vc_redist.x64.exe)";

        case ERROR_PROC_NOT_FOUND:
            return "ERROR_PROC_NOT_FOUND;\n\nThe mod tried to access "
                   "a function defined in another DLL, but the specified "
                   "function was not found. Make sure the other DLL exports "
                   "the given symbol, and that it is defined in the DLL.\n\n"
                   "If you are not the developer of this mod, report this error "
                   "to them as it is likely not your fault.";

        case ERROR_DLL_INIT_FAILED:
            return "ERROR_DLL_INIT_FAILED;\n\nLikely some global variables "
                   "in the mod threw an exception or otherwise failed. "
                   "ALSO MAKE SURE YOU ARE COMPILING IN RELEASE MODE.\n\n"
                   "If you are not the developer of this mod, report this error "
                   "to them as it is likely not your fault.";

        case 4551:
            return "Blocked by Windows Smart App Control (4551);\n\nThe mod was blocked "
                    "from loading by Windows Smart App Control. This is a security feature "
                    "in Windows that blocks unrecognized applications including most "
                    "Geometry Dash mods.";

        case 0xc0e90002:
            return "Blocked by Windows Smart App Control (0xc0e90002);\n\nThe mod was blocked "
                    "from loading by Windows Smart App Control. This is a security feature "
                    "in Windows that blocks unrecognized applications including most "
                    "Geometry Dash mods.";

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
    auto load = LoadLibraryW(this->getBinaryPath().c_str());
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
