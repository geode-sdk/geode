#include <Geode/loader/IPC.hpp>
#include <matjson.hpp>

using namespace geode::prelude;

std::monostate geode::listenForIPC(std::string const& messageID, matjson::Value(*callback)(IPCEvent*)) {
    (void) new EventListener(
        callback, IPCFilter(getMod()->getID(), messageID)
    );
    return std::monostate();
}

IPCEvent::IPCEvent(
    void* rawPipeHandle,
    std::string const& targetModID,
    std::string const& messageID,
    matjson::Value const& messageData,
    matjson::Value& replyData
) : m_rawPipeHandle(rawPipeHandle),
    targetModID(targetModID),
    messageID(messageID),
    replyData(replyData),
    messageData(std::make_unique<matjson::Value>(messageData)) {}

IPCEvent::~IPCEvent() {}

ListenerResult IPCFilter::handle(utils::MiniFunction<Callback> fn, IPCEvent* event) {
    if (event->targetModID == m_modID && event->messageID == m_messageID) {
        event->replyData = fn(event);
        return ListenerResult::Stop;
    }
    return ListenerResult::Propagate;
}

IPCFilter::IPCFilter(std::string const& modID, std::string const& messageID) :
    m_modID(modID), m_messageID(messageID) {}
