#ifndef LILAC_CORE_META_META_HPP
#define LILAC_CORE_META_META_HPP

#include "callconv.hpp"
#include "common.hpp"
#include "function.hpp"
#include "hook.hpp"
#include <Geode/platform/platform.hpp>

#if defined(LILAC_IS_WINDOWS)
    #include "cdecl.hpp"
    #include "membercall.hpp"
    #include "optcall.hpp"
    #include "thiscall.hpp"
#endif

#endif /* LILAC_CORE_META_META_HPP */