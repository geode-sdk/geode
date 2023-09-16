
#include <Geode/loader/Dirs.hpp>
#include <cocos2d.h>
#include <crashlog.hpp>
#include <filesystem>
#include "LoaderImpl.hpp"

using namespace geode::prelude;

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

ghc::filesystem::path dirs::getModConfigDir() {
    return dirs::getGeodeDir() / "config";
}

ghc::filesystem::path dirs::getIndexDir() {
    return dirs::getGeodeDir() / "index";
}

ghc::filesystem::path dirs::getCrashlogsDir() {
    return crashlog::getCrashLogDirectory();
}
