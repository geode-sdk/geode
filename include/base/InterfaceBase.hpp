#pragma once

#include <Macros.hpp>
#include <HeaderBase.hpp>

#include <utils/addresser.hpp>
#include <unordered_map>
#include <type_traits>
#include <Interface.hpp>
#include <stdint.h>

namespace geode::core::meta {}

template<auto F>
struct AddressOf {
	static inline auto value = geode::base::get();
};



