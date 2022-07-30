#pragma once
#include "Types.hpp"
#include <Geode/utils/addresser.hpp>

namespace geode::modifier {
	struct addresses {
		#include <codegenned/GeneratedAddress.hpp>
	};
}
