#include <Geode/DefaultInclude.hpp>

#if defined(GEODE_IS_WINDOWS)

#include "../load.hpp"
#include <Windows.h>

#include "loader/LoaderImpl.hpp"
using namespace geode::prelude;

int updateGeode() {
    auto workingDir = dirs::getGameDir();
    auto updatesDir = workingDir / "geode" / "update";

    auto error = std::error_code();

    if (!ghc::filesystem::is_empty(updatesDir, error) && !error) {
        if (ghc::filesystem::exists(updatesDir / "GeodeUpdater.exe", error) && !error)
            ghc::filesystem::rename(updatesDir / "GeodeUpdater.exe", workingDir / "GeodeUpdater.exe", error);
        if (error)
            return error.value();

        char gdPath[MAX_PATH];
        GetModuleFileName(nullptr, gdPath, MAX_PATH);
        auto gdName = ghc::filesystem::path(gdPath).filename().string();

        // launch updater
        auto updaterPath = (workingDir / "GeodeUpdater.exe").string();
        ShellExecute(NULL, "open", updaterPath.c_str(), gdName.c_str(), workingDir.string().c_str(), FALSE);

        // quit gd before it can even start
        exit(0);
        return 0;
    }
    if (error)
        return error.value();

    return 0;
}

int WINAPI gdMainHook(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    int exitCode = updateGeode();
    if (exitCode != 0)
        return exitCode;

    exitCode = geodeEntry(hInstance);
    if (exitCode != 0)
        return exitCode;

    return reinterpret_cast<decltype(&wWinMain)>(geode::base::get() + 0x260ff8)(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

extern "C" __declspec(dllexport) DWORD WINAPI loadGeode(void* arg) {
    auto process = GetCurrentProcess();

    auto patchAddr = reinterpret_cast<void*>(geode::base::get() + 0x260ff8);
    constexpr size_t patchLength = 13;
    auto detourAddr = reinterpret_cast<uintptr_t>(&gdMainHook) - geode::base::get() - 0x261005;
    auto detourAddrPtr = reinterpret_cast<uint8_t*>(&detourAddr);

    uint8_t patchBytes[patchLength] = {
        0x55,
        0x8b, 0xec,
        0x83, 0xe4, 0xf8,
        0xeb, 0x06,
        0xe9, detourAddrPtr[0], detourAddrPtr[1], detourAddrPtr[2], detourAddrPtr[3]
    };

    DWORD oldProtect;
    BOOL res = TRUE;
    res = res && VirtualProtectEx(process, patchAddr, patchLength, PAGE_EXECUTE_READWRITE, &oldProtect);
    res = res && WriteProcessMemory(process, patchAddr, patchBytes, patchLength, nullptr);
    res = res && VirtualProtectEx(process, patchAddr, patchLength, oldProtect, &oldProtect);

    if (!res) {
        LoaderImpl::get()->platformMessageBox(
            "Unable to Load Geode!",
            "There was an unknown fatal error hooking "
            "the GD main function and Geode can not be loaded."
        );
        return 1;
    }

    return 0;
}

DWORD WINAPI upgradeThread(void*) {
    return updateGeode() == 0;
}
BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module);

        // if we find the old bootstrapper dll, don't load geode, copy new updater and let it do the rest
        auto workingDir = dirs::getGameDir();
        auto error = std::error_code();
        if (ghc::filesystem::exists(workingDir / "GeodeBootstrapper.dll", error) && !error)
            CreateThread(nullptr, 0, upgradeThread, nullptr, 0, nullptr);
        if (error)
            return FALSE;
    }
    return TRUE;
}

#endif