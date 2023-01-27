#pragma once

#include "Event.hpp"
#include "Loader.hpp"
#include <json.hpp>

namespace geode {
    #ifdef GEODE_IS_WINDOWS
    constexpr char const* IPC_PIPE_NAME = "\\\\.\\pipe\\GeodeIPCPipe";
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

    class GEODE_DLL IPCEvent : public Event {
    protected:
        void* m_rawPipeHandle;
        bool m_replied = false;
    
    public:
        std::string targetModID;
        std::string messageID;
        std::unique_ptr<json::Value> messageData;
        json::Value& replyData;

        friend class IPCFilter;

        IPCEvent(
            void* rawPipeHandle,
            std::string const& targetModID,
            std::string const& messageID,
            json::Value const& messageData,
            json::Value& replyData
        );
        virtual ~IPCEvent();
    };

    class GEODE_DLL IPCFilter : public EventFilter<IPCEvent> {
    public:
        using Callback = json::Value(IPCEvent*);

    protected:
        std::string m_modID;
        std::string m_messageID;

    public:
        ListenerResult handle(std::function<Callback> fn, IPCEvent* event);
		IPCFilter(
            std::string const& modID,
            std::string const& messageID
        );
    };

    std::monostate listenForIPC(std::string const& messageID, json::Value(*callback)(IPCEvent*));
}
