#include <Geode/DefaultInclude.hpp>

#include "../load.hpp"
#include <Windows.h>

#include "loader/LoaderImpl.hpp"
#include "loader/console.hpp"

using namespace geode::prelude;

void updateGeode() {
    const auto workingDir = dirs::getGameDir();
    const auto geodeDir = dirs::getGeodeDir();
    const auto updatesDir = geodeDir / "update";

    bool bootstrapperExists = ghc::filesystem::exists(workingDir / "GeodeBootstrapper.dll");
    bool updatesDirExists = ghc::filesystem::exists(geodeDir) && ghc::filesystem::exists(updatesDir);

    if (!bootstrapperExists && !updatesDirExists)
        return;

    // update updater
    if (ghc::filesystem::exists(updatesDir) &&
        ghc::filesystem::exists(updatesDir / "GeodeUpdater.exe"))
        ghc::filesystem::rename(updatesDir / "GeodeUpdater.exe", workingDir / "GeodeUpdater.exe");

    utils::game::restart();
}

void* mainTrampolineAddr;

#include "gdTimestampMap.hpp"
unsigned int gdTimestamp = 0;

int WINAPI gdMainHook(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    // MessageBoxA(NULL, "Hello from gdMainHook!", "Hi", 0);

    updateGeode();

    if (versionToTimestamp(GEODE_STR(GEODE_GD_VERSION)) > gdTimestamp) {
        console::messageBox(
            "Unable to Load Geode!",
            fmt::format(
                "This version of Geode is made for Geometry Dash {} "
                "but you're trying to play with GD {}.\n"
                "Please, update your game.",
                GEODE_STR(GEODE_GD_VERSION),
                LoaderImpl::get()->getGameVersion()
            )
        );
        // TODO: should geode FreeLibrary itself here?
    } else {
        int exitCode = geodeEntry(hInstance);
        if (exitCode != 0)
            return exitCode;
    }

    return reinterpret_cast<decltype(&wWinMain)>(mainTrampolineAddr)(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

std::string loadGeode() {
    auto process = GetCurrentProcess();
    auto dosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(geode::base::get());
    auto ntHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(geode::base::get() + dosHeader->e_lfanew);

    gdTimestamp = ntHeader->FileHeader.TimeDateStamp;

    constexpr size_t trampolineSize = 12;
    mainTrampolineAddr = VirtualAlloc(
        nullptr, trampolineSize,
        MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE
    );

    auto entryAddr = geode::base::get() + ntHeader->OptionalHeader.AddressOfEntryPoint;
    // function that calls main
    auto preWinMainAddr = entryAddr + 5 + *reinterpret_cast<uintptr_t*>(entryAddr + 6) + 5;

    // 6a 00           push 0
    // 68 00 00 40 00  push geode::base::get()
    // e8 ...          call ...
    uint64_t mainSearchBytes = 0xe80000000068006a;
    mainSearchBytes |= static_cast<uint64_t>(geode::base::get()) << 24;

    uintptr_t patchAddr = 0;
    // 0x10000 should be enough of a limit here..
    for (auto searchAddr = preWinMainAddr; searchAddr < preWinMainAddr + 0x10000; searchAddr++) {
        if (*reinterpret_cast<uint64_t*>(searchAddr) != mainSearchBytes)
            continue;
        // follow near call address, this is the call to main
        patchAddr = searchAddr + 12 + *reinterpret_cast<ptrdiff_t*>(searchAddr + 8);
        break;
    }

    if (patchAddr == 0)
        return "Geode could not find the main function, not loading Geode.";

    constexpr size_t patchSize = 6;

#define JMP_ADDR(from, to) (std::bit_cast<uintptr_t>(to) - std::bit_cast<uintptr_t>(from) - 5)
#define JMP_BYTES(from, to) \
    static_cast<uint8_t>((JMP_ADDR(from, to) >>  0) & 0xFF), \
    static_cast<uint8_t>((JMP_ADDR(from, to) >>  8) & 0xFF), \
    static_cast<uint8_t>((JMP_ADDR(from, to) >> 16) & 0xFF), \
    static_cast<uint8_t>((JMP_ADDR(from, to) >> 24) & 0xFF)

    uint8_t trampolineBytes[trampolineSize] = {
        // push ebp
        0x55,
        // mov ebp, esp
        0x8b, 0xec,
        // and esp, ...
        0x83, 0xe4, 0xf8, 
        // jmp main + 6 (after our jmp detour)
        0xe9, JMP_BYTES(reinterpret_cast<uintptr_t>(mainTrampolineAddr) + 6, patchAddr + patchSize)
    };

    std::memcpy(mainTrampolineAddr, trampolineBytes, trampolineSize);

    uint8_t patchBytes[patchSize] = {
        // jmp gdMainHook
        0xe9, JMP_BYTES(patchAddr, &gdMainHook),
        // nop to pad it out, helps the asm to show up properly on debuggers
        0x90
    };

    DWORD oldProtect;
    if (!VirtualProtectEx(process, reinterpret_cast<void*>(patchAddr), patchSize, PAGE_EXECUTE_READWRITE, &oldProtect))
        return "Geode could not hook the main function, not loading Geode.";
    std::memcpy(reinterpret_cast<void*>(patchAddr), patchBytes, patchSize);
    VirtualProtectEx(process, reinterpret_cast<void*>(patchAddr), patchSize, oldProtect, &oldProtect);
    return "";
}

DWORD WINAPI upgradeThread(void*) {
    updateGeode();
    return 0;
}

void earlyError(std::string message) {
    // try to write a file and display a message box
    // wine might not display the message box but *should* write a file
    std::ofstream fout("_geode_early_error.txt");
    fout << message;
    fout.close();
    console::messageBox("Unable to Load Geode!", message);
}

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID) {
    if (reason != DLL_PROCESS_ATTACH)
        return TRUE;
    // Prevents threads from notifying this DLL on creation or destruction.
    // Kind of redundant for a game that isn't multi-threaded but will provide
    // some slight optimizations if a mod frequently creates and deletes threads.
    DisableThreadLibraryCalls(module);

    try {
        // if we find the old bootstrapper dll, don't load geode, copy new updater and let it do the rest
        auto workingDir = dirs::getGameDir();
        std::error_code error;
        bool oldBootstrapperExists = ghc::filesystem::exists(workingDir / "GeodeBootstrapper.dll", error);
        if (error) {
            earlyError("There was an error checking whether the old GeodeBootstrapper.dll exists: " + error.message());
            return FALSE;
        }
        else if (oldBootstrapperExists)
            CreateThread(nullptr, 0, upgradeThread, nullptr, 0, nullptr);
        else if (auto error = loadGeode(); !error.empty()) {
            earlyError(error);
            return TRUE;
        }
    }
    catch(...) {
        earlyError("There was an unknown error somewhere very very early and this is really really bad.");
        return FALSE;
    }

    return TRUE;
}
