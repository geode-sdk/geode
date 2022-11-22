#include <Geode/loader/IPC.hpp>
#include <InternalLoader.hpp>

USE_GEODE_NAMESPACE();

IPCEvent::IPCEvent(
    void* rawPipeHandle,
    std::string const& targetModID,
    std::string const& messageID,
    std::optional<std::string> const& replyID,
    nlohmann::json const& messageData
) : m_rawPipeHandle(rawPipeHandle),
    m_targetModID(targetModID),
    m_messageID(messageID),
    m_replyID(replyID),
    m_messageData(messageData) {}

IPCEvent::~IPCEvent() {
    this->reply(nullptr);
}

std::optional<std::string> IPCEvent::getReplyID() const {
    return m_replyID;
}

std::string IPCEvent::getTargetModID() const {
    return m_targetModID;
}

std::string IPCEvent::getMessageID() const {
    return m_messageID;
}

nlohmann::json IPCEvent::getMessageData() const {
    return m_messageData;
}

bool IPCEvent::canReply() const {
    return m_replyID.has_value();
}

void IPCEvent::reply(nlohmann::json const& data) {
    if (!m_replied && m_rawPipeHandle && m_replyID.has_value()) {
        InternalLoader::get()->postIPCReply(
            m_rawPipeHandle, m_replyID.value(), data
        );
        m_replied = true;
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
