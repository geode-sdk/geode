#include <Geode/DefaultInclude.hpp>

#if defined(GEODE_IS_WINDOWS)

#include "../load.hpp"
#include <Windows.h>

#include "loader/LoaderImpl.hpp"
using namespace geode::prelude;

void updateGeode() {
    const auto workingDir = dirs::getGameDir();
    const auto geodeDir = dirs::getGeodeDir();
    const auto updatesDir = geodeDir / "update";

    if (ghc::filesystem::exists(geodeDir) && ghc::filesystem::exists(updatesDir) && !ghc::filesystem::is_empty(updatesDir)) {
        if (ghc::filesystem::exists(updatesDir / "GeodeUpdater.exe"))
            ghc::filesystem::rename(updatesDir / "GeodeUpdater.exe", workingDir / "GeodeUpdater.exe");

        wchar_t buffer[MAX_PATH];
        GetModuleFileNameW(nullptr, buffer, MAX_PATH);
        const auto gdName = ghc::filesystem::path(buffer).filename().string();

        // launch updater
        const auto updaterPath = (workingDir / "GeodeUpdater.exe").string();
        ShellExecuteA(nullptr, "open", updaterPath.c_str(), gdName.c_str(), workingDir.string().c_str(), false);

        // quit gd before it can even start
        exit(0);
    }
}

int WINAPI gdMainHook(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    updateGeode();

    int exitCode = geodeEntry(hInstance);
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
    bool res = true;
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
    updateGeode();
    return 0;
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