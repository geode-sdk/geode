#pragma once

#include <string>

namespace geode::utils::thread {
    void platformSetNameOrdinary(std::string const& name);
    void platformSetNameWide(std::wstring const& wName);
}
