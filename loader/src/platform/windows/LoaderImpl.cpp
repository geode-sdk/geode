#include <Geode/loader/IPC.hpp>
#include <Geode/loader/Log.hpp>
#include <loader/ModImpl.hpp>
#include <iostream>
#include <loader/LoaderImpl.hpp>

USE_GEODE_NAMESPACE();

#ifdef GEODE_IS_WINDOWS

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

        std::string reply = LoaderImpl::get()->processRawIPC((void*)pipe, buffer);

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

#endif
