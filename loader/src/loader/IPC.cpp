#include <Geode/loader/IPC.hpp>
#include "IPC.hpp"
#include <matjson.hpp>

using namespace geode::prelude;

std::monostate ipc::listen(std::string const& messageID, matjson::Value(*callback)(IPCEvent*)) {
    (void) new EventListener(
        callback, IPCFilter(getMod()->getID(), messageID)
    );
    return std::monostate();
}

ipc::IPCEvent::IPCEvent(
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

ipc::IPCEvent::~IPCEvent() {}

ListenerResult ipc::IPCFilter::handle(utils::MiniFunction<Callback> fn, IPCEvent* event) {
    if (event->targetModID == m_modID && event->messageID == m_messageID) {
        event->replyData = fn(event);
        return ListenerResult::Stop;
    }
    return ListenerResult::Propagate;
}

ipc::IPCFilter::IPCFilter(std::string const& modID, std::string const& messageID) :
    m_modID(modID), m_messageID(messageID) {}

matjson::Value ipc::processRaw(void* rawHandle, std::string const& buffer) {
    matjson::Value reply;

    matjson::Value json;
    try {
        json = matjson::parse(buffer);
    } catch (...) {
        log::warn("Received IPC message that isn't valid JSON");
        return reply;
    }

    if (!json.contains("mod") || !json["mod"].is_string()) {
        log::warn("Received IPC message without 'mod' field");
        return reply;
    }
    if (!json.contains("message") || !json["message"].is_string()) {
        log::warn("Received IPC message without 'message' field");
        return reply;
    }
    matjson::Value data;
    if (json.contains("data")) {
        data = json["data"];
    }
    // log::debug("Posting IPC event");
    // ! warning: if the event system is ever made asynchronous this will break!
    IPCEvent(rawHandle, json["mod"].as_string(), json["message"].as_string(), data, reply).post();
    return reply;
}
