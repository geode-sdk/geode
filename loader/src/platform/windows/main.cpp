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

    constexpr size_t trampolineSize = GEODE_WINDOWS64(32) GEODE_WINDOWS32(12);    
    mainTrampolineAddr = VirtualAlloc(
        nullptr, trampolineSize,
        MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE
    );

    const uintptr_t entryAddr = geode::base::get() + ntHeader->OptionalHeader.AddressOfEntryPoint;
    // function that calls main
    // 32 bit: 
    // e8 xx xx xx xx call (security)
    // e9 xx xx xx xx jmp
    // 64 bit:
    // 48 83 ec 28    sub rsp, 0x28
    // e8 xx xx xx xx call (security)
    // 48 83 c4 28    add rsp, 0x28
    // e9 xx xx xx xx jmp

    constexpr ptrdiff_t mainCallJmpOffset = GEODE_WINDOWS64(13) GEODE_WINDOWS32(5);
    const uintptr_t mainCallJmpAddress = entryAddr + mainCallJmpOffset;
    const int32_t mainCallJmpValue = *reinterpret_cast<int32_t*>(mainCallJmpAddress + 1);
    const uintptr_t preWinMainAddr = mainCallJmpAddress + mainCallJmpValue + 5;

    // the search bytes for the main function
    // 32 bit:
    // 6a 00                  push 0
    // 68 00 00 40 00         push geode::base::get()
    // e8 ...                 call ...
    // 64 bit:
    // 44 8b cb               mov r9d, ebx
    // 4c 8b c0               mov r8, rax
    // 33 d2                  xor edx, edx
    // 48 8d 0d xx xx xx xx   lea rcx, [rip + ...]
    // e8 ...                 call ...

    constexpr uint64_t mainSearchBytes = GEODE_WINDOWS64(0xd233c08b4ccb8b44) GEODE_WINDOWS32(0x004000006a006800);
    constexpr ptrdiff_t mainSearchCallOffset = GEODE_WINDOWS64(15) GEODE_WINDOWS32(7);

#ifdef GEODE_IS_WINDOWS32
    mainSearchBytes |= static_cast<uint64_t>(geode::base::get()) << 24;
#endif

    uintptr_t patchAddr = 0;
    // 0x1000 should be enough of a limit here..
    for (auto searchAddr = preWinMainAddr; searchAddr < preWinMainAddr + 0x1000; searchAddr++) {
        if (*reinterpret_cast<uint64_t*>(searchAddr) != mainSearchBytes)
            continue;
        // follow near call address, this is the call to main
        const uintptr_t callAddress = searchAddr + mainSearchCallOffset;
        const int32_t callValue = *reinterpret_cast<int32_t*>(callAddress + 1);
        patchAddr = callAddress + callValue + 5;
        break;
    }

    if (patchAddr == 0)
        return "Geode could not find the main function, not loading Geode.";

#define JMP_ADDR(from, to) (std::bit_cast<uintptr_t>(to) - std::bit_cast<uintptr_t>(from) - 5)
#define JMP_BYTES(from, to) \
    static_cast<uint8_t>((JMP_ADDR(from, to) >>  0) & 0xFF), \
    static_cast<uint8_t>((JMP_ADDR(from, to) >>  8) & 0xFF), \
    static_cast<uint8_t>((JMP_ADDR(from, to) >> 16) & 0xFF), \
    static_cast<uint8_t>((JMP_ADDR(from, to) >> 24) & 0xFF)

#ifdef GEODE_IS_WINDOWS64
    constexpr size_t patchSize = 15;

    uintptr_t jumpAddr = patchAddr + patchSize;
    uint8_t trampolineBytes[trampolineSize] = {
        // mov [rsp + 8], rbx
        0x48, 0x89, 0x5c, 0x24, 0x08,
        // mov [rsp + 10], rsi
        0x48, 0x89, 0x74, 0x24, 0x10,
        // mov [rsp + 18], rdi
        0x48, 0x89, 0x7c, 0x24, 0x18,
        // jmp [rip + 0] 
        0xff, 0x25, 0x00, 0x00, 0x00, 0x00,
        // pointer to main + 15
        static_cast<uint8_t>((jumpAddr >> 0) & 0xFF), static_cast<uint8_t>((jumpAddr >> 8) & 0xFF), static_cast<uint8_t>((jumpAddr >> 16) & 0xFF), static_cast<uint8_t>((jumpAddr >> 24) & 0xFF),
        static_cast<uint8_t>((jumpAddr >> 32) & 0xFF), static_cast<uint8_t>((jumpAddr >> 40) & 0xFF), static_cast<uint8_t>((jumpAddr >> 48) & 0xFF), static_cast<uint8_t>((jumpAddr >> 56) & 0xFF),
        // nop to pad it out, helps the asm to show up properly on debuggers
        0x90, 0x90, 0x90
    };

    std::memcpy(mainTrampolineAddr, trampolineBytes, trampolineSize);

    auto jmpAddr = reinterpret_cast<uintptr_t>(&gdMainHook);
    uint8_t patchBytes[patchSize] = {
        // jmp [rip + 0]
        0xff, 0x25, 0x00, 0x00, 0x00, 0x00,
        // pointer to gdMainHook
        static_cast<uint8_t>((jmpAddr >> 0) & 0xFF), static_cast<uint8_t>((jmpAddr >> 8) & 0xFF), static_cast<uint8_t>((jmpAddr >> 16) & 0xFF), static_cast<uint8_t>((jmpAddr >> 24) & 0xFF),
        static_cast<uint8_t>((jmpAddr >> 32) & 0xFF), static_cast<uint8_t>((jmpAddr >> 40) & 0xFF), static_cast<uint8_t>((jmpAddr >> 48) & 0xFF), static_cast<uint8_t>((jmpAddr >> 56) & 0xFF),
        // nop to pad it out, helps the asm to show up properly on debuggers
        0x90
    };
#else 
    constexpr size_t patchSize = 6;

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
#endif

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
