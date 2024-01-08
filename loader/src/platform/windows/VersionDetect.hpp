#pragma once

#include <unordered_map>
#include <string_view>
#include <Windows.h>

// Original by absolute
// https://gist.github.com/absoIute/ebe5da42d118109a03632c9751d86e19

inline std::string_view detectGDVersion() {
    static const std::unordered_map<uint32_t, std::string_view> buildMap = {
        { 1419173053, "1.900" },
        { 1419880840, "1.910" },
        { 1421745341, "1.920" },
        { 1440638199, "2.000" },
        { 1440643927, "2.001" },
        { 1443053232, "2.010" },
        { 1443077847, "2.011" },
        { 1443077847, "2.020" },
        { 1484612867, "2.100" },
        { 1484626658, "2.101" },
        { 1484737207, "2.102" },
        { 1510526914, "2.110" },
        { 1510538091, "2.111" },
        { 1510619253, "2.112" },
        { 1511220108, "2.113" },
        { 1702921605, "2.200" },
        { 1704582672, "2.201" },
        { 1704601266, "2.202" },
    };

    auto hMod = GetModuleHandleA(NULL);
    auto dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(hMod);

    if (dosHeader->e_magic == IMAGE_DOS_SIGNATURE) {
        auto ntHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uintptr_t>(hMod) + dosHeader->e_lfanew);

        if (ntHeader->Signature == IMAGE_NT_SIGNATURE) {
            auto timeStamp = ntHeader->FileHeader.TimeDateStamp;
            if (auto it = buildMap.find(timeStamp); it != buildMap.end())
                return it->second;
        }
    }

    return "";
}