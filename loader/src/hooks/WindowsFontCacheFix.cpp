#include <Geode/Geode.hpp>

#ifdef GEODE_IS_WINDOWS

using namespace geode::prelude;

// https://github.com/cocos2d/cocos2d-x/blob/5a25fe75cb8b26b61b14b070e757ec3b17ff7791/cocos2dx/platform/win32/CCImage.cpp#L96
// stop setFont from caching fonts on Windows
// Windows Font Cache Manager holds on to them after the app has closed
// Fonts are supposed to be released in the BitmapDC::~BitmapDC() but this doesn't seem to work consistently?
// Looks like there's a shared instance of BitmapDC, so presumably it's destructed when the app closes?

int AddFontResourceWHook(LPCWSTR p0) {
    return AddFontResourceExW(p0, FR_PRIVATE, 0);
}

int RemoveFontResourceWHook(LPCWSTR p0) {
    return RemoveFontResourceExW(p0, FR_PRIVATE, 0);
}

$execute {

    // TODO: one more patch; and make this code look nicer

    void* dst = reinterpret_cast<void*>(geode::base::getCocos() + 0xCB5BC);
    ByteVector p = { 0xE8 };
    unsigned int callAddr = (unsigned int)&RemoveFontResourceWHook - ((unsigned int)dst + 5);
    for (unsigned char i = 0; i < sizeof(int); ++i)
        p.push_back(callAddr >> (8 * i));
    p.push_back(0x90);
    Mod::get()->patch(dst, p);

    dst = reinterpret_cast<void*>(geode::base::getCocos() + 0xCB642);
    p = { 0xE8 };
    callAddr = (unsigned int)&AddFontResourceWHook - ((unsigned int)dst + 5);
    for (unsigned char i = 0; i < sizeof(int); ++i)
        p.push_back(callAddr >> (8 * i));
    p.push_back(0x90);
    Mod::get()->patch(dst, p);

};

#endif