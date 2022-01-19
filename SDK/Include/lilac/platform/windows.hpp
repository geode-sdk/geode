#pragma once

#include <Windows.h>

#define LILAC_API extern "C" __declspec(dllexport)

#ifdef LILAC_EXPORTING
    #define LILAC_DLL    __declspec(dllexport)
#else
    #define LILAC_DLL    __declspec(dllimport)
#endif

namespace lilac {
    struct PlatformInfo {
        HMODULE m_hmod;
    };
}