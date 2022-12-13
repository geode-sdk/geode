#pragma once

#include "Event.hpp"
#include "Loader.hpp"

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
        nlohmann::json messageData;
        nlohmann::json& replyData;

        friend class IPCFilter;

        IPCEvent(
            void* rawPipeHandle,
            std::string const& targetModID,
            std::string const& messageID,
            nlohmann::json const& messageData,
            nlohmann::json& replyData
        );
        virtual ~IPCEvent();
    };

    class GEODE_DLL IPCFilter : public EventFilter<IPCEvent> {
    public:
        using Callback = nlohmann::json(IPCEvent*);

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

    template<class = void>
    std::monostate listenForIPC(std::string const& messageID, nlohmann::json(*callback)(IPCEvent*)) {
        (void) new EventListener(
            callback, IPCFilter(getMod()->getID(), messageID)
        );
        return std::monostate();
    }
}
