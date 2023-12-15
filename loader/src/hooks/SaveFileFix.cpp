#include <Geode/loader/Loader.hpp>

#if defined(GEODE_IS_WINDOWS) || defined(GEODE_IS_ANDROID)

using namespace geode::prelude;

#include <Geode/cocos/support/base64.h>

// This function is well known for crashing on certain save files,
// causing the game to crash at startup, known as the infamous save file bug.
//
// The exact reason for why this fails is still a bit fuzzy, but I believe it's because
// rob doesn't use the base64/inflated size correctly, and ends up relying on a null
// terminator being there when it shouldnt.
// 
// To fix this, we just rewrite the function.
gd::string decompressString2(unsigned char* data, bool decrypt, int size, int decryptionKey) {
    if (data == nullptr || size == 0 || *data == 0) {
        return {};
    }

    if (decrypt) {
        for (int i = 0; i < size; i++) {
            data[i] ^= decryptionKey;
        }
    }

    // TODO: maybe not use cocos's base64 and inflateMemory..

    unsigned char* out = nullptr;
    auto const decodedSize = cocos2d::base64Decode(data, size, &out);
    std::unique_ptr<unsigned char> b64decoded { out };

    if (decodedSize <= 0) {
        return {};
    }

    out = nullptr;
    auto const inflatedSize = cocos2d::ZipUtils::ccInflateMemory(b64decoded.get(), decodedSize, &out);

    std::unique_ptr<unsigned char> inflated { out };

    if (inflatedSize <= 0) {
        return {};
    }

    return std::string(reinterpret_cast<char*>(inflated.get()), inflatedSize);
}

// Modify doesnt want to work for some reason!
$execute {
    Mod::get()->addHook(
        reinterpret_cast<void*>(
            geode::addresser::getNonVirtual(
                &cocos2d::ZipUtils::decompressString2
            )
        ),
        &decompressString2,
        "cocos2d::ZipUtils::decompressString2",
        tulip::hook::TulipConvention::Cdecl
    );
}

#endif