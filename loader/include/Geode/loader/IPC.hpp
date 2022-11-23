#pragma once

#include "Event.hpp"
#include "Loader.hpp"

namespace geode {
    #ifdef GEODE_IS_WINDOWS
    constexpr const char* IPC_PIPE_NAME = "\\\\.\\pipe\\GeodeIPCPipe";
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
        std::optional<std::string> m_replyID;
        std::string m_messageID;
        nlohmann::json m_messageData;
        bool m_replied = false;

        /**
         * Reply to the message. Will post a message back to the application 
         * the sent this message with the reply ID and provided data.
         * You can only reply once; after the other application has received 
         * the reply, it can assume the reply ID can be freed and reused for 
         * other messages. Calling reply again on this message will not cause 
         * a new response to be sent.
         * If reply is not explicitly called, a default response of null will 
         * be posted back.
         * @param data The data to send back; will be the under the "data" key 
         * in the response JSON. The structure may be anything; however, you 
         * should document what kind of JSON structures applications may expect 
         * from your mod.
         */
        void reply(nlohmann::json const& data);

        friend class IPCFilter;

    public:
        IPCEvent(
            void* rawPipeHandle,
            std::string const& targetModID,
            std::string const& messageID,
            std::optional<std::string> const& replyID,
            nlohmann::json const& messageData
        );
        virtual ~IPCEvent();

        std::optional<std::string> getReplyID() const;
        std::string getTargetModID() const;
        std::string getMessageID() const;
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
