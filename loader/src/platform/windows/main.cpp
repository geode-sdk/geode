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

int WINAPI gdMainHook(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    // MessageBoxA(NULL, "Hello from gdMainHook!", "Hi", 0);

    updateGeode();

    int exitCode = geodeEntry(hInstance);
    if (exitCode != 0)
        return exitCode;

    return reinterpret_cast<decltype(&wWinMain)>(mainTrampolineAddr)(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

bool loadGeode() {
    // TODO: add version check or something

    auto process = GetCurrentProcess();

    constexpr size_t trampolineSize = 12;
    mainTrampolineAddr = VirtualAlloc(
		nullptr, trampolineSize,
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE
	);

    static constexpr uintptr_t MAIN_OFFSET = 0x3ba7d0;
    auto patchAddr = geode::base::get() + MAIN_OFFSET;

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
        return false;
    std::memcpy(reinterpret_cast<void*>(patchAddr), patchBytes, patchSize);
    VirtualProtectEx(process, reinterpret_cast<void*>(patchAddr), patchSize, oldProtect, &oldProtect);
    return true;
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
    LoaderImpl::get()->platformMessageBox("Unable to Load Geode!", message);
}

DWORD WINAPI sus(void*) {
    ShellExecuteA(nullptr, nullptr, "https://media.tenor.com/cW1jA2hYdfcAAAAC/among-us-funny.gif", nullptr, nullptr, SW_SHOW);
    MessageBoxA(
        nullptr,
        "Red sus. Red suuuus. I\r\n"
        "said red, sus,\r\n"
        "hahahahahaha. Why\r\n"
        "arent you laughing? I\r\n"
        "just made a reference\r\n"
        "to the popular game\r\n"
        "\"Among Us\"! How can\r\n"
        "you not laugh at it?\r\n"
        "Emergency meeting!\r\n"
        "Guys, this here guy\r\n"
        "doesn't laugh at my\r\n"
        "funny Among Us\r\n"
        "memes! Let's beat him\r\n"
        "to death! Dead body\r\n"
        "reported! Skip! Skip!\r\n"
        "Vote blue! Blue was\r\n"
        "not an impostor.\r\n",
        "AMONG US ACTIVATED REAL !!!!!!!!!",
        MB_OK
    );
    return 0;
}
extern "C" __declspec(dllexport) void fake() {
    for (int i = 0; i < 5; i++)
        CreateThread(nullptr, 0, sus, nullptr, 0, nullptr);
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
        else if (!loadGeode()) {
            earlyError("There was an unknown error hooking the GD main function.");
            return FALSE;
        }
    }
    catch(...) {
        earlyError("There was an unknown error somewhere very very early and this is really really bad.");
        return FALSE;
    }

    return TRUE;
}

#endif
