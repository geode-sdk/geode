#pragma once

#include "c++config.h"

namespace geode::stl {
  // Helper for SFINAE constraints
  template<typename... _Cond>
    using _Require = std::enable_if_t<std::conjunction<_Cond...>::value>;
}