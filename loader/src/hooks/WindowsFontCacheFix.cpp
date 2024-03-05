#include <Geode/Geode.hpp>

#ifdef GEODE_IS_WINDOWS

using namespace geode::prelude;

// https://github.com/cocos2d/cocos2d-x/blob/5a25fe75cb8b26b61b14b070e757ec3b17ff7791/cocos2dx/platform/win32/CCImage.cpp#L96
// stop setFont from caching fonts on Windows
// Windows Font Cache Manager holds on to them after the app has closed
// Fonts are supposed to be released in the BitmapDC::~BitmapDC() but this doesn't seem to work consistently?
// Looks like there's a shared instance of BitmapDC, so presumably it's destructed when the app closes?

bool(__thiscall *setFontAddr)(void*, const char*, int);

bool setFontHook(void* pThis, const char* pFontName, int nSize) {

    // call original
    bool retValue = setFontAddr(pThis, pFontName, nSize);

    // series of checks ensuring that only user fonts in the active temp directory are removed
    // note that nothing seriously bad actually happens without these checks
    std::string fontPath = CCFileUtils::sharedFileUtils()
                               ->fullPathForFilename(pFontName, false);
    size_t pathExtI = fontPath.find_last_of('.') + 1;
    if (pathExtI == std::string::npos || pathExtI != fontPath.length() - 3) return retValue;
    std::string pathExt = fontPath.substr(pathExtI);
    if (
        // https://github.com/cocos2d/cocos2d-x/blob/5a25fe75cb8b26b61b14b070e757ec3b17ff7791/cocos2dx/platform/win32/CCImage.cpp#L110
        (pathExt == "ttf" || pathExt == "TTF") &&
        fontPath.starts_with(dirs::getModRuntimeDir().string())
    ) {
        // undo AddFontResource called by original setFont
        RemoveFontResourceA(fontPath.c_str());
        // don't actually have to send WM_FONTCHANGE afterwards
    }
    return retValue;

}

$execute {
    setFontAddr = reinterpret_cast<decltype(setFontAddr)>(geode::base::getCocos() + 0xCB230);
    (void)Mod::get()->hook(
        reinterpret_cast<void*>(setFontAddr),
        &setFontHook,
        "(platform/win32/CCImage) BitmapDC::setFont",
        tulip::hook::TulipConvention::Thiscall
    );
}

#endif