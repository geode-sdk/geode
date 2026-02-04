#include <Geode/loader/IPC.hpp>
#include <loader/IPC.hpp>

#include <arc/iocp/IocpPipe.hpp>
#include <string>

using namespace geode::prelude;

static constexpr auto IPC_BUFFER_SIZE = 512;

static arc::Future<> ipcPipeTask(arc::IocpPipe pipe) {
    char buffer[IPC_BUFFER_SIZE * sizeof(TCHAR)];

    if (auto res = co_await pipe.read(buffer, sizeof(buffer) - 1)) {
        auto read = res.unwrap();
        buffer[read] = '\0';
        
        std::string reply = ipc::processRaw(pipe.handle(), buffer).dump();
        auto res2 = co_await pipe.write(reply.data(), reply.size());
        if (!res2) {
            log::warn("Error writing to IPC pipe: {}", res2.unwrapErr());
        }
    } else {
        log::warn("Error reading from IPC pipe: {}", res.unwrapErr());
    }

    // IocpPipe will automatically flush and disconnect in destructor
}

void ipc::setup() {
    async::spawn([] -> arc::Future<> {
        while (true) {
            auto handle = CreateNamedPipeW(
                utils::string::utf8ToWide(IPC_PIPE_NAME).c_str(),
                PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
                PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                PIPE_UNLIMITED_INSTANCES,
                IPC_BUFFER_SIZE,
                IPC_BUFFER_SIZE,
                NMPWAIT_USE_DEFAULT_WAIT,
                nullptr
            );
            if (handle == INVALID_HANDLE_VALUE) {
                // todo: Rn this quits IPC, but we might wanna change that later
                // to just continue trying. however, I'm assuming that if
                // CreateNamedPipeA fails, then it will probably fail again if
                // you try right after, so changing the break; to continue; might
                // just result in the console getting filled with error messages
                log::warn("Unable to create pipe, quitting IPC");
                break;
            }

            auto result = co_await arc::IocpPipe::listen(handle);
            if (!result) {
                log::warn("Unable to accept IPC connection: {}", result.unwrapErr());
            } else {
                async::spawn(ipcPipeTask(std::move(result.unwrap())));
            }
        }
    }).setName("Geode IPC Listener");

    log::debug("IPC set up");

}
