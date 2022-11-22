#include "../InternalLoader.hpp"
#include <Geode/loader/Log.hpp>
#include <Geode/loader/IPC.hpp>
#include <iostream>
#include "../InternalMod.hpp"

USE_GEODE_NAMESPACE();

#ifdef GEODE_IS_WINDOWS

void InternalLoader::platformMessageBox(char const* title, std::string const& info) {
    MessageBoxA(nullptr, info.c_str(), title, MB_ICONERROR);
}

void InternalLoader::openPlatformConsole() {
    if (m_platformConsoleOpen) return;
    if (AllocConsole() == 0) return;
    SetConsoleCP(CP_UTF8);
    // redirect console output
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
    freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);

    m_platformConsoleOpen = true;

    for (auto const& log : Loader::get()->getLogs()) {
        std::cout << log->toString(true) << "\n";
    }
}

void InternalLoader::closePlatformConsole() {
    if (!m_platformConsoleOpen) return;

    fclose(stdin);
    fclose(stdout);
    FreeConsole();

    m_platformConsoleOpen = false;
}

void InternalLoader::postIPCMessage(
    void* rawPipeHandle,
    std::string const& senderID,
    std::string const& data
) {
    std::string msg = senderID + "/" + data;
    log::debug("Replying msg: {}", msg);
    DWORD written;
    WriteFile(rawPipeHandle, msg.c_str(), msg.size(), &written, nullptr);
}

void InternalLoader::setupIPC() {
    auto pipe = CreateNamedPipeA(
        IPC_PIPE_NAME,
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        1,
        1024 * 16,
        1024 * 16,
        NMPWAIT_USE_DEFAULT_WAIT,
        nullptr
    );
    std::thread([pipe]() {
        while (pipe != INVALID_HANDLE_VALUE) {
            log::debug("Waiting for connection");
            if (ConnectNamedPipe(pipe, nullptr)) {
                log::debug("Got connection");
                char buffer[1024];
                DWORD read;
                while (ReadFile(pipe, buffer, sizeof(buffer) - 1, &read, nullptr)) {
                    buffer[read] = '\0';

                    // format of the message should be modID/senderID/msgID/data
                    std::string modID;
                    std::string senderID;
                    std::string msgID;
                    std::string data;
                    size_t collectPart = 0;
                    for (size_t i = 0; i < read; i++) {
                        if (buffer[i] == '/' && collectPart < 3) {
                            collectPart++;
                        } else {
                            switch (collectPart) {
                                case 0: modID += buffer[i]; break;
                                case 1: senderID += buffer[i]; break;
                                case 2: msgID += buffer[i]; break;
                                default: data += buffer[i]; break;
                            }
                        }
                    }
                    if (modID.size() && senderID.size() && msgID.size()) {
                        IPCEvent(pipe, modID, senderID, msgID, data).post();
                    } else {
                        log::warn("Received invalid IPC message: '{}'", buffer);
                    }
                }
                log::debug("Connection done");
            }
            DisconnectNamedPipe(pipe);
            log::debug("Disconnected pipe");
        }
        log::debug("IPC ended");
    }).detach();
    if (pipe != INVALID_HANDLE_VALUE) {
        log::log(Severity::Debug, InternalMod::get(), "IPC set up");
    } else {
        log::log(Severity::Error, InternalMod::get(), "Unable to set up IPC");
    }
}

#endif
