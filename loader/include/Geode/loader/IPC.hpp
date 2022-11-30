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
        std::string m_targetModID;
        std::string m_messageID;
        std::string* m_replyString;
        nlohmann::json m_messageData;
        bool m_replied = false;

        friend class IPCFilter;

    public:
        IPCEvent(
            void* rawPipeHandle,
            std::string const& targetModID,
            std::string const& messageID,
            nlohmann::json const& messageData,
            std::string* replyString = nullptr
        );
        virtual ~IPCEvent();

        std::string getTargetModID() const;
        std::string getMessageID() const;
        std::string getReplyString() const;
        void setReplyString(std::string const& reply);
        nlohmann::json getMessageData() const;
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
        Loader::get()->scheduleOnModLoad(getMod(), [=]() {
            new EventListener(
                callback, IPCFilter(getMod()->getID(), messageID)
            );
        });
        return std::monostate();
    }
}
