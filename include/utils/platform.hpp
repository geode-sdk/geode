#pragma once

#include <Macros.hpp>
#include "Result.hpp"
#include <string>
#include <vector>
#include <functional>

namespace geode::utils::clipboard {
    GEODE_DLL bool write(std::string const& data);
    GEODE_DLL std::string read();
}
