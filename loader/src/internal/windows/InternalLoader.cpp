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

void InternalLoader::postIPCReply(
    void* rawPipeHandle,
    std::string const& replyID,
    nlohmann::json const& data
) {
    auto msgJson = nlohmann::json::object();
    msgJson["reply"] = replyID;
    msgJson["data"] = data;
    auto msg = msgJson.dump();

    DWORD written;
    WriteFile(rawPipeHandle, msg.c_str(), msg.size(), &written, nullptr);
}

// todo: multiple connections
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
                    try {
                        auto json = nlohmann::json::parse(buffer);
                        if (!json.contains("mod") || !json["mod"].is_string()) {
                            log::warn("Received IPC message without 'mod' field");
                            continue;
                        }
                        if (!json.contains("message") || !json["message"].is_string()) {
                            log::warn("Received IPC message without 'message' field");
                            continue;
                        }
                        std::optional<std::string> reply = std::nullopt;
                        if (json.contains("reply") && json["reply"].is_string()) {
                            reply = json["reply"];
                        }
                        nlohmann::json data;
                        if (json.contains("data")) {
                            data = json["data"];
                        }
                        IPCEvent(pipe, json["mod"], json["message"], reply, data).post();
                    } catch(...) {
                        log::warn("Received IPC message that isn't valid JSON");
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
