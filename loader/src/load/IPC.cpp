#include <Geode/loader/IPC.hpp>
#include <InternalLoader.hpp>

USE_GEODE_NAMESPACE();

IPCEvent::IPCEvent(
    void* rawPipeHandle,
    std::string targetModID,
    std::string senderID,
    std::string messageID,
    std::string messageData
) : m_rawPipeHandle(rawPipeHandle),
    m_targetModID(targetModID),
    m_senderID(senderID),
    m_messageID(messageID),
    m_messageData(messageData) {}

std::string IPCEvent::getSenderID() const {
    return m_senderID;
}

std::string IPCEvent::getTargetModID() const {
    return m_targetModID;
}

std::string IPCEvent::getMessageID() const {
    return m_messageID;
}

std::string IPCEvent::getMessageData() const {
    return m_messageData;
}

void IPCEvent::reply(std::string const& data) {
    if (m_rawPipeHandle) {
        InternalLoader::get()->postIPCMessage(m_rawPipeHandle, m_senderID, data);
    }
}

ListenerResult IPCFilter::handle(std::function<Callback> fn, IPCEvent* event) {
    if (
        event->getTargetModID() == m_modID &&
        event->getMessageID() == m_messageID
    ) {
        fn(event);
        return ListenerResult::Stop;
    }
    return ListenerResult::Propagate;
}

IPCFilter::IPCFilter(
    std::string const& modID,
    std::string const& messageID
) : m_modID(modID),
    m_messageID(messageID) {}
