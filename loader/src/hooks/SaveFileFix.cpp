#include <Geode/loader/Loader.hpp>

#if defined(GEODE_IS_WINDOWS) || defined(GEODE_IS_ANDROID)

using namespace geode::prelude;

#include <Geode/cocos/support/base64.h>
#include "../loader/LoaderImpl.hpp"

void panic(std::string reason) {
    LoaderImpl::get()->platformMessageBox("Critical", fmt::format(
        "Your save file failed to load (reason: {})\n"
        "As to not lose all of your data, the game will now abort.\n"
        "Please backup your save files and try opening the game again, it might work.\n"
        "Please contact the Geode Team about this", reason
    ));
    std::abort();
}

// This function is well known for crashing on certain save files,
// causing the game to crash at startup, known as the infamous save file bug.
//
// Rob ends up relying on strlen for knowing the size of `data`, instead of just using the passed in `size`.
// Its a miracle this works most of the time, considering `data` is just binary data
// 
// To fix this, we just rewrite the function.
gd::string decompressString2(unsigned char* data, bool decrypt, int size, int decryptionKey) {
    log::debug("decompressString2 data={} size={}", reinterpret_cast<const void*>(data), size);
    if (data == nullptr || size == 0) {
        return {};
    }

    std::vector<unsigned char> copiedData(data, data + size);
    if (decrypt) {
        for (int i = 0; i < size; i++) {
            copiedData[i] ^= decryptionKey;
        }
    }

    // TODO: maybe not use cocos's base64 and inflateMemory..

    unsigned char* out = nullptr;
    auto const decodedSize = cocos2d::base64Decode(copiedData.data(), size, &out);
    std::unique_ptr<unsigned char> b64decoded { out };

    if (decodedSize <= 0) {
        panic(fmt::format("base64 (size={}) (data={} size={})", decodedSize, reinterpret_cast<const void*>(data), size));
        return {};
    }

    out = nullptr;
    auto const inflatedSize = cocos2d::ZipUtils::ccInflateMemory(b64decoded.get(), decodedSize, &out);

    std::unique_ptr<unsigned char> inflated { out };

    if (inflatedSize <= 0) {
        panic(fmt::format("inflate (size={}) (data={} size={})", inflatedSize, reinterpret_cast<const void*>(data), size));
        return {};
    }

    return std::string(reinterpret_cast<char*>(inflated.get()), inflatedSize);
}

// Modify doesnt want to work for some reason!
$execute {
    (void) Mod::get()->addHook(
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