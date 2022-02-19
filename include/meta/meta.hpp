#ifndef GEODE_CORE_META_META_HPP
#define GEODE_CORE_META_META_HPP

namespace geode::core::meta {
	namespace x86 {
		
	}
}

#ifdef GEODE_IS_WINDOWS
	#include "function.hpp"
	// #include "hook.hpp"
	#include "cdecl.hpp"
	#include "optcall.hpp"
	#include "thiscall.hpp"
	#include "membercall.hpp"
#endif
#include "defaultconv.hpp"

#endif /* GEODE_CORE_META_META_HPP */
