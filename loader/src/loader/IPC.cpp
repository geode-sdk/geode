#include <Geode/loader/IPC.hpp>
#include "IPC.hpp"
#include <matjson.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

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

bool ipc::IPCEvent::filter(std::string_view modID, std::string_view messageID) const {
    return modID == targetModID && messageID == messageID;
}

ipc::IPCEvent::~IPCEvent() {}

ListenerResult ipc::IPCFilter::handle(std::function<Callback> fn, IPCEvent* event) {
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

    auto res = matjson::Value::parse(buffer);
    if (!res) {
        log::warn("Received IPC message that isn't valid JSON: {}", res.unwrapErr());
        return reply;
    }
    matjson::Value json = res.unwrap();

    if (!json.contains("mod") || !json["mod"].isString()) {
        log::warn("Received IPC message without 'mod' field");
        return reply;
    }
    if (!json.contains("message") || !json["message"].isString()) {
        log::warn("Received IPC message without 'message' field");
        return reply;
    }
    matjson::Value data;
    if (json.contains("data")) {
        data = json["data"];
    }
    // log::debug("Posting IPC event");
    // ! warning: if the event system is ever made asynchronous this will break!
    IPCEvent(rawHandle, json["mod"].asString().unwrap(), json["message"].asString().unwrap(), data, reply).post();
    return reply;
}
