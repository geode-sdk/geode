#pragma once

#include <string>

namespace geode::utils::thread {
    // the platform-specific methods are needed for the thread names to show up
    // in places like task managers and debuggers
    void platformSetName(std::string const& name);
}
