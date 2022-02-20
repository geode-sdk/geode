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
struct address_of_t {
	static inline auto value = geode::base::get();
};

template<auto F>
inline auto address_of = address_of_t<F>::value;





