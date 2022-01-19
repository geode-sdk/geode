#pragma once

#include <Windows.h>

#define GEODE_API extern "C" __declspec(dllexport)

#ifdef GEODE_EXPORTING
    #define GEODE_DLL    __declspec(dllexport)
#else
    #define GEODE_DLL    __declspec(dllimport)
#endif

namespace geode {
    struct PlatformInfo {
        HMODULE m_hmod;
    };
}
