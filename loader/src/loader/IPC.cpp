#include <Geode/loader/IPC.hpp>

USE_GEODE_NAMESPACE();

IPCEvent::IPCEvent(
    void* rawPipeHandle,
    std::string const& targetModID,
    std::string const& messageID,
    nlohmann::json const& messageData,
    nlohmann::json& replyData
) : m_rawPipeHandle(rawPipeHandle),
    targetModID(targetModID),
    messageID(messageID),
    replyData(replyData),
    messageData(messageData) {}

IPCEvent::~IPCEvent() {}

ListenerResult IPCFilter::handle(std::function<Callback> fn, IPCEvent* event) {
    if (event->targetModID == m_modID && event->messageID == m_messageID) {
        event->replyData = fn(event);
        return ListenerResult::Stop;
    }
    return ListenerResult::Propagate;
}

IPCFilter::IPCFilter(std::string const& modID, std::string const& messageID) :
    m_modID(modID), m_messageID(messageID) {}
