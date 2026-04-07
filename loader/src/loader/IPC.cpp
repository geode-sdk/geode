#include <Geode/loader/IPC.hpp>
#include "IPC.hpp"
#include <matjson.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

matjson::Value ipc::processRaw(void* rawHandle, std::string_view buffer) {
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
    IPCEvent(json["mod"].asString().unwrap(), json["message"].asString().unwrap()).send(std::move(rawHandle), std::move(data), reply);
    return reply;
}
