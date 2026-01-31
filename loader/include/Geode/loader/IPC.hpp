#pragma once

#include "Event.hpp"
#include "Loader.hpp"
#include "Mod.hpp"
#include <matjson.hpp>

namespace geode::ipc {
    #ifdef GEODE_IS_WINDOWS
    constexpr char const* IPC_PIPE_NAME = R"(\\.\pipe\GeodeIPCPipe)";
    #endif

    #ifdef GEODE_IS_MACOS
    constexpr char const* IPC_PORT_NAME = "GeodeIPCPipe";
    #endif

    class IPCFilter;

    // IPC (Inter-Process Communication) provides a way for Geode mods to talk
    // to other programs on the user's computer. If you have, for example, a
    // debugger, or an external modding UI, that application can open up the
    // platform-specific pipe and start sending messages to mods. Mods can
    // listen for messages using the listenForIPC function, and reply to
    // messages the get by using the reply method on the event provided. For
    // example, an external application can query what mods are loaded in Geode
    // by sending the `list-mods` message to `geode.loader`.
    class GEODE_DLL IPCEvent final : public ThreadSafeEvent<IPCEvent, bool(void*, matjson::Value, matjson::Value&), std::string, std::string> {
    public:
        // listener params rawHandle, messageData, replyData
        // filter params targetModID, messageID
        using ThreadSafeEvent::ThreadSafeEvent;
    };

    template <class Callback>
    inline ListenerHandle* listen(std::string messageID, Callback&& callback) {
        return IPCEvent(getMod()->getID(), std::move(messageID)).listen(
            [callback = std::move(callback)](void* rawHandle, matjson::Value data, matjson::Value& reply) {
                reply = callback(data);
                return true;
            }
        ).leak();
    }
}
