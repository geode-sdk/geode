
#include <Geode/loader/Dirs.hpp>
#include <cocos2d.h>
#include <crashlog.hpp>
#include <filesystem>
#include "LoaderImpl.hpp"

using namespace geode::prelude;

namespace {
    ghc::filesystem::path weaklyCanonical(ghc::filesystem::path const& path) {
    #ifdef GEODE_IS_WINDOWS
        // this is std::filesystem intentionally because ghc version doesnt want to work with softlinked directories
        return std::filesystem::weakly_canonical(path.string()).string();
    #else
        return ghc::filesystem::weakly_canonical(path.string()).string();
    #endif
    }
}

ghc::filesystem::path dirs::getSaveDir() {
#ifdef GEODE_IS_MACOS
    // not using ~/Library/Caches
    return ghc::filesystem::path("/Users/Shared/Geode");
#else
    return weaklyCanonical(CCFileUtils::sharedFileUtils()->getWritablePath().c_str());
#endif
}

ghc::filesystem::path dirs::getGeodeDir() {
    return dirs::getGameDir() / "geode";
}

ghc::filesystem::path dirs::getGeodeSaveDir() {
    return dirs::getSaveDir() / "geode";
}

ghc::filesystem::path dirs::getGeodeResourcesDir() {
    return dirs::getGeodeDir() / "resources";
}

ghc::filesystem::path dirs::getGeodeLogDir() {
    return dirs::getGeodeDir() / "logs";
}

ghc::filesystem::path dirs::getTempDir() {
    return getGeodeDir() / "temp";
}

ghc::filesystem::path dirs::getModsDir() {
    return getGeodeDir() / "mods";
}

ghc::filesystem::path dirs::getModsSaveDir() {
    return getGeodeSaveDir() / "mods";
}

ghc::filesystem::path dirs::getModRuntimeDir() {
    return dirs::getGeodeDir() / "unzipped";
}

ghc::filesystem::path dirs::getModConfigDir() {
    return dirs::getGeodeDir() / "config";
}

ghc::filesystem::path dirs::getIndexDir() {
    return dirs::getGeodeDir() / "index";
}

ghc::filesystem::path dirs::getCrashlogsDir() {
    return crashlog::getCrashLogDirectory();
}
