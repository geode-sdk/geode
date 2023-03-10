
#include <Geode/loader/Dirs.hpp>
#include <cocos2d.h>
#include <crashlog.hpp>
#include <filesystem>

using namespace geode::prelude;

ghc::filesystem::path dirs::getGameDir() {
    return ghc::filesystem::path(CCFileUtils::sharedFileUtils()->getWritablePath2().c_str());
}

ghc::filesystem::path dirs::getSaveDir() {
#ifdef GEODE_IS_MACOS
    // not using ~/Library/Caches
    return ghc::filesystem::path("/Users/Shared/Geode");
#elif defined(GEODE_IS_WINDOWS)
    // this is std::filesystem intentionally because ghc version doesnt want to work with softlinked directories
    return ghc::filesystem::path(
        std::filesystem::weakly_canonical(CCFileUtils::sharedFileUtils()->getWritablePath().c_str())
            .string()
    );
#else
    return ghc::filesystem::path(CCFileUtils::sharedFileUtils()->getWritablePath().c_str());
#endif
}

ghc::filesystem::path dirs::getGeodeDir() {
#ifdef GEODE_IS_MACOS
    return ghc::filesystem::current_path() / "geode";
#else
    return dirs::getGameDir() / "geode";
#endif
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
