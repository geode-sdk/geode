#ifndef GEODE_CORE_META_META_HPP
#define GEODE_CORE_META_META_HPP

#include "../platform/platform.hpp"
#include "callconv.hpp"
#include "common.hpp"
#include "defaultconv.hpp"
#include "function.hpp"

namespace geode::core::meta {}

namespace geode::core::meta::x86 {}

#if defined(GEODE_IS_WINDOWS)
    #include "cdecl.hpp"
    #include "membercall.hpp"
    #include "optcall.hpp"
    #include "thiscall.hpp"
    #include "stdcall.hpp"
#endif

#endif /* GEODE_CORE_META_META_HPP */
