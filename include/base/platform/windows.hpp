#pragma once

#pragma warning(default: 4067)
#pragma warning(disable: 4251) // dll-interface
#pragma warning(disable: 4244) // narrowing conversion

#include <Windows.h>
#include <meta/cdecl.hpp>
#include <meta/function.hpp>
#include <meta/optcall.hpp>
#include <meta/membercall.hpp>
#include <meta/thiscall.hpp>

namespace geode {
    struct PlatformInfo {
        HMODULE m_hmod;
    };
}

namespace geode::base {
	GEODE_NOINLINE inline uintptr_t get() {
		static uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));
		return base;
	}
}
