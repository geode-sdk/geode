#pragma once

#include <string>
#include <matjson3.hpp>

namespace geode::ipc {
    void setup();
    matjson::Value processRaw(void* rawHandle, std::string const& buffer);
}
