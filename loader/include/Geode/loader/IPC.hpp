#pragma once

#include "Event.hpp"
#include "Loader.hpp"

namespace geode {
    #ifdef GEODE_IS_WINDOWS
    constexpr const char* IPC_PIPE_NAME = "\\\\.\\pipe\\GeodeIPCPipe";
    #endif

    // message ID may be anything that doesn't contain /, but to be safe, you 
    // should only define message IDs that match the following: [a-z\-_\.]+

    class GEODE_DLL IPCEvent : public Event {
    protected:
        void* m_rawPipeHandle;
        std::string m_targetModID;
        std::string m_senderID;
        std::string m_messageID;
        std::string m_messageData;

    public:
        IPCEvent(
            void* rawPipeHandle,
            std::string targetModID,
            std::string senderID,
            std::string messageID,
            std::string messageData
        );

        std::string getSenderID() const;
        std::string getTargetModID() const;
        std::string getMessageID() const;
        std::string getMessageData() const;

        /**
         * Reply to the message. You can only reply once
         */
        void reply(std::string const& data);
    };

    class GEODE_DLL IPCFilter : public EventFilter<IPCEvent> {
    public:
        using Callback = void(IPCEvent*);

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
    std::monostate listenForIPC(std::string const& messageID, void(*callback)(IPCEvent*)) {
        Loader::get()->scheduleOnModLoad(getMod(), [=]() {
            new EventListener(
                callback, IPCFilter(getMod()->getID(), messageID)
            );
        });
        return std::monostate();
    }
}
