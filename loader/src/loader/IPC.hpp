#pragma once

#include <string>
#include <matjson.hpp>

namespace geode::ipc {
    void setup();
    matjson::Value processRaw(void* rawHandle, std::string const& buffer);
}
