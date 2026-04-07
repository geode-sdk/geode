#pragma once

#include <Geode/utils/ZStringView.hpp>

namespace geode::utils::thread {
    // the platform-specific methods are needed for the thread names to show up
    // in places like task managers and debuggers
    void platformSetName(ZStringView name);
}
