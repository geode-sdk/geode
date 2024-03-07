#include <Geode/Geode.hpp>

#ifdef GEODE_IS_WINDOWS

using namespace geode::prelude;

// https://github.com/cocos2d/cocos2d-x/blob/5a25fe75cb8b26b61b14b070e757ec3b17ff7791/cocos2dx/platform/win32/CCImage.cpp#L96
// stop setFont from caching fonts on Windows
// Windows Font Cache Manager holds on to them after the app has closed
// Fonts are supposed to be released in the BitmapDC::~BitmapDC() but this doesn't seem to work consistently?
// Looks like there's a shared instance of BitmapDC, so presumably it's destructed when the app closes?

int __stdcall AddFontResourceWHook(LPCWSTR p0) {
    return AddFontResourceExW(p0, FR_PRIVATE, 0);
}

int __stdcall RemoveFontResourceWHook(LPCWSTR p0) {
    return RemoveFontResourceExW(p0, FR_PRIVATE, 0);
}

/*
 * addr is relative to cocos base
 * patches x86 CALL at addr to call function at newCall
 */
static void patchCall(uintptr_t addr, uintptr_t newCall) {
    ByteVector patch = { 0xE8 }; // CALL near & relative
    addr += (uintptr_t)geode::base::getCocos();
    uintptr_t callAddr = newCall - (addr + 5);
    for (auto i = 0; i < sizeof(int); ++i)
        patch.push_back(callAddr >> (8 * i));
    patch.push_back(0x90); // every overwritten instruction happens to be 6 bytes wide
    (void)Mod::get()->patch(reinterpret_cast<void*>(addr), patch);
}

$execute {

    // BitmapDC::~BitmapDC
    patchCall(0xC9A56, (uintptr_t)&RemoveFontResourceWHook);

    // BitmapDC::setFont
    patchCall(0xCB5BC, (uintptr_t)&RemoveFontResourceWHook);
    patchCall(0xCB642, (uintptr_t)&AddFontResourceWHook);

};

#endif