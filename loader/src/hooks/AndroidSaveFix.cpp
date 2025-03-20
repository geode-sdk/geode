#include <Geode/platform/cplatform.h>

#ifdef GEODE_IS_ANDROID
#include <loader/LoaderImpl.hpp>

using namespace geode::prelude;

uintptr_t g_decodeAddress = 0;

int base64DecodeHook(uint8_t* input, uint32_t length, uint8_t** output, bool urlSafe) {
    *output = new uint8_t[(size_t)(length * 3) / 4 + 4];

    uint32_t outLength = 0;
    int ret = reinterpret_cast<int(*)(uint8_t*, uint32_t, uint8_t*, uint32_t*, bool)>(g_decodeAddress)(input, length, *output, &outLength, urlSafe);
    if (ret > 0) {
        delete[] *output;
        *output = nullptr;
        outLength = 0;
    }

    return outLength;
}

$execute {
    if (LoaderImpl::get()->isForwardCompatMode()) return;

    // This fixes a crash when using the base64Decode function in cocos2d-x, due to
    // floating-point precision errors causing the output buffer to be too small.
    // This issue can cause large save files to crash the game.

    auto handle = dlopen("libcocos2dcpp.so", RTLD_LAZY | RTLD_NOLOAD);
    g_decodeAddress = reinterpret_cast<uintptr_t>(dlsym(handle, "_ZN7cocos2d13_base64DecodeEPKhjPhPjb"));
    auto decodeAddress = dlsym(handle, "base64Decode");

    (void) Mod::get()->hook(
        reinterpret_cast<void*>(decodeAddress),
        &base64DecodeHook,
        "base64Decode"
    );
}
#endif
