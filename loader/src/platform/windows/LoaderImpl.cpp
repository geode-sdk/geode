#include <Geode/loader/IPC.hpp>
#include <Geode/loader/Log.hpp>
#include <loader/ModImpl.hpp>
#include <iostream>
#include <loader/LoaderImpl.hpp>
#include <Geode/utils/string.hpp>

USE_GEODE_NAMESPACE();

#ifdef GEODE_IS_WINDOWS

#include <Psapi.h>

static constexpr auto IPC_BUFFER_SIZE = 512;

void Loader::Impl::platformMessageBox(char const* title, std::string const& info) {
    MessageBoxA(nullptr, info.c_str(), title, MB_ICONERROR);
}

void Loader::Impl::openPlatformConsole() {
    if (m_platformConsoleOpen) return;
    if (AllocConsole() == 0) return;
    SetConsoleCP(CP_UTF8);
    // redirect console output
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
    freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);

    m_platformConsoleOpen = true;

    for (auto const& log : log::Logger::list()) {
        std::cout << log->toString(true) << "\n";
    }
}

void Loader::Impl::closePlatformConsole() {
    if (!m_platformConsoleOpen) return;

    fclose(stdin);
    fclose(stdout);
    FreeConsole();

    m_platformConsoleOpen = false;
}

void ipcPipeThread(HANDLE pipe) {
    char buffer[IPC_BUFFER_SIZE * sizeof(TCHAR)];
    DWORD read;

    std::optional<std::string> replyID = std::nullopt;

    // log::debug("Waiting for I/O");
    if (ReadFile(pipe, buffer, sizeof(buffer) - 1, &read, nullptr)) {
        buffer[read] = '\0';

        std::string reply = LoaderImpl::get()->processRawIPC((void*)pipe, buffer).dump();

        DWORD written;
        WriteFile(pipe, reply.c_str(), reply.size(), &written, nullptr);
    }
    // log::debug("Connection done");

    FlushFileBuffers(pipe);
    DisconnectNamedPipe(pipe);
    CloseHandle(pipe);

    // log::debug("Disconnected pipe");
}

void Loader::Impl::setupIPC() {
    std::thread([]() {
        while (true) {
            auto pipe = CreateNamedPipeA(
                IPC_PIPE_NAME,
                PIPE_ACCESS_DUPLEX,
                PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                PIPE_UNLIMITED_INSTANCES,
                IPC_BUFFER_SIZE,
                IPC_BUFFER_SIZE,
                NMPWAIT_USE_DEFAULT_WAIT,
                nullptr
            );
            if (pipe == INVALID_HANDLE_VALUE) {
                // todo: Rn this quits IPC, but we might wanna change that later
                // to just continue trying. however, I'm assuming that if
                // CreateNamedPipeA fails, then it will probably fail again if
                // you try right after, so changing the break; to continue; might
                // just result in the console getting filled with error messages
                log::warn("Unable to create pipe, quitting IPC");
                break;
            }
            // log::debug("Waiting for pipe connections");
            if (ConnectNamedPipe(pipe, nullptr)) {
                // log::debug("Got connection, creating thread");
                std::thread(&ipcPipeThread, pipe).detach();
            }
            else {
                // log::debug("No connection, cleaning pipe");
                CloseHandle(pipe);
            }
        }
    }).detach();

    log::debug("IPC set up");
}

bool Loader::Impl::userTriedToLoadDLLs() const {
    static std::unordered_set<std::string> KNOWN_MOD_DLLS {
        "betteredit-v4.0.5.dll",
        "betteredit-v4.0.5-min.dll",
        "betteredit-v4.0.3.dll",
        "betteredit.dll",
        "gdshare-v0.3.4.dll",
        "gdshare.dll",
        "hackpro.dll",
        "hackproldr.dll",
        "quickldr.dll",
        "minhook.x32.dll",
        "iconsave.dll",
        "menuanim.dll",
        "volumecontrol.dll",
        "customsplash.dll",
        "scrollanyinput-v1.1.dll",
        "alttabfix-v1.0.dll",
        "sceneswitcher-v1.1.dll",
        "gdantialiasing.dll",
        "textureldr.dll",
        "run-info.dll",
    };

    bool triedToLoadDLLs = false;

    // Check for .DLLs in mods dir
    if (auto files = file::listFiles(dirs::getModsDir(), true)) {
        for (auto& file : files.unwrap()) {
            if (file.extension() == ".dll") {
                triedToLoadDLLs = true;
            }
        }
    }

    // Check all loaded DLLs in the process
    std::array<HMODULE, 1024> mods;
    DWORD needed;
    auto process = GetCurrentProcess();

    if (EnumProcessModules(process, mods.data(), mods.size(), &needed)) {
        for (auto i = 0; i < (needed / sizeof(HMODULE)); i++) {
            std::array<char, MAX_PATH> modName;
            if (GetModuleFileNameExA(process, mods[i], modName.data(), modName.size())) {
                if (KNOWN_MOD_DLLS.count(string::trim(string::toLower(
                    ghc::filesystem::path(modName.data()).filename().string()
                )))) {
                    triedToLoadDLLs = true;
                }
            }
        }
    }

    return triedToLoadDLLs;
}

#endif
