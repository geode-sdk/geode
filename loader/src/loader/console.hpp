#pragma once

#include <string>

namespace geode::console {
    void open();
    void close();
    void log(std::string const& msg, Severity severity);
    void messageBox(char const* title, std::string const& info, Severity severity = Severity::Error);
}
