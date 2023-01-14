#include <Geode/loader/IPC.hpp>
#include <Geode/external/json/json.hpp>

USE_GEODE_NAMESPACE();

std::monostate geode::listenForIPC(std::string const& messageID, nlohmann::json(*callback)(IPCEvent*)) {
    (void) new EventListener(
        callback, IPCFilter(getMod()->getID(), messageID)
    );
    return std::monostate();
}

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
