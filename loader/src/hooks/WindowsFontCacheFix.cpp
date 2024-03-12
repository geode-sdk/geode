#include <Geode/Geode.hpp>

#ifdef GEODE_IS_WINDOWS

using namespace geode::prelude;

int __stdcall AddFontResourceWHook(LPCWSTR p0);
int __stdcall RemoveFontResourceWHook(LPCWSTR p0);
static void patchCall(uintptr_t addr, uintptr_t newCall);

int __stdcall AddFontResourceWHook(LPCWSTR p0) {
    return AddFontResourceExW(p0, FR_PRIVATE, 0);
}

int __stdcall RemoveFontResourceWHook(LPCWSTR p0) {
    return RemoveFontResourceExW(p0, FR_PRIVATE, 0);
}

static void patchCall(uintptr_t addr, uintptr_t newCall) {
    const int CALL_INSTRUCTION_SIZE = 5;
    ByteVector patch(CALL_INSTRUCTION_SIZE, 0);
    uintptr_t addrCocos = addr + reinterpret_cast<uintptr_t>(geode::base::getCocos());

    uintptr_t callAddr = newCall - (addrCocos + CALL_INSTRUCTION_SIZE);
    for (int i = 0; i < sizeof(int); ++i)
        patch[i] = (callAddr >> (8 * i));

    patch[CALL_INSTRUCTION_SIZE - 1] = 0x90;

    (void)Mod::get()->patch(reinterpret_cast<void*>(addrCocos), patch);
}

$execute {
    patchCall(0xC9A56, reinterpret_cast<uintptr_t>(&RemoveFontResourceWHook));
    patchCall(0xCB5BC, reinterpret_cast<uintptr_t>(&RemoveFontResourceWHook));
    patchCall(0xCB642, reinterpret_cast<uintptr_t>(&AddFontResourceWHook));
};

#endif
