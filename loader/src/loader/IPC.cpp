#include <Geode/loader/IPC.hpp>
#include <InternalLoader.hpp>

USE_GEODE_NAMESPACE();

IPCEvent::IPCEvent(
    void* rawPipeHandle,
    std::string const& targetModID,
    std::string const& messageID,
    nlohmann::json const& messageData,
    std::string* replyString
) : m_rawPipeHandle(rawPipeHandle),
    m_targetModID(targetModID),
    m_messageID(messageID),
    m_replyString(replyString),
    m_messageData(messageData) {}

IPCEvent::~IPCEvent() {}



std::string IPCEvent::getTargetModID() const {
    return m_targetModID;
}

std::string IPCEvent::getMessageID() const {
    return m_messageID;
}

std::string IPCEvent::getReplyString() const {
    return *m_replyString;
}

void IPCEvent::setReplyString(std::string const& reply) {
    *m_replyString = reply;
}

nlohmann::json IPCEvent::getMessageData() const {
    return m_messageData;
}

ListenerResult IPCFilter::handle(std::function<Callback> fn, IPCEvent* event) {
    if (
        event->getTargetModID() == m_modID &&
        event->getMessageID() == m_messageID
    ) {
        event->setReplyString(fn(event));
        return ListenerResult::Stop;
    }
    return ListenerResult::Propagate;
}

IPCFilter::IPCFilter(
    std::string const& modID,
    std::string const& messageID
) : m_modID(modID),
    m_messageID(messageID) {}
