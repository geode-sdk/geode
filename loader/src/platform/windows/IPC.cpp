#include <Geode/loader/IPC.hpp>
#include <loader/IPC.hpp>

#include <thread>
#include <optional>
#include <string>

using namespace geode::prelude;

static constexpr auto IPC_BUFFER_SIZE = 512;

void ipcPipeThread(HANDLE pipe) {
    thread::setName("Geode IPC Pipe");

    char buffer[IPC_BUFFER_SIZE * sizeof(TCHAR)];
    DWORD read;

    std::optional<std::string> replyID;

    // log::debug("Waiting for I/O");
    if (ReadFile(pipe, buffer, sizeof(buffer) - 1, &read, nullptr)) {
        buffer[read] = '\0';

        std::string reply = ipc::processRaw((void*)pipe, buffer).dump();

        DWORD written;
        WriteFile(pipe, reply.c_str(), reply.size(), &written, nullptr);
    }
    // log::debug("Connection done");

    FlushFileBuffers(pipe);
    DisconnectNamedPipe(pipe);
    CloseHandle(pipe);

    // log::debug("Disconnected pipe");
}

void ipc::setup() {
    std::thread ipcThread([]() {
        thread::setName("Geode Main IPC");
        while (true) {
            auto pipe = CreateNamedPipeW(
                utils::string::utf8ToWide(IPC_PIPE_NAME).c_str(),
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
                std::thread pipeThread(&ipcPipeThread, pipe);
                pipeThread.detach();
            }
            else {
                // log::debug("No connection, cleaning pipe");
                CloseHandle(pipe);
            }
        }
    });
    ipcThread.detach();

    log::debug("IPC set up");
}
