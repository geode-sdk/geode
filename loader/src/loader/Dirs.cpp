
#include <Geode/loader/Dirs.hpp>
#include <cocos2d.h>
#include <crashlog.hpp>
#include <filesystem>
#include "LoaderImpl.hpp"

using namespace geode::prelude;

std::filesystem::path dirs::getGeodeDir() {
    return dirs::getGameDir() / "geode";
}

std::filesystem::path dirs::getGeodeSaveDir() {
    return dirs::getSaveDir() / "geode";
}

std::filesystem::path dirs::getGeodeResourcesDir() {
    return dirs::getGeodeDir() / "resources";
}

std::filesystem::path dirs::getGeodeLogDir() {
    return dirs::getGeodeDir() / "logs";
}

std::filesystem::path dirs::getTempDir() {
    return getGeodeDir() / "temp";
}

std::filesystem::path dirs::getModsDir() {
    return getGeodeDir() / "mods";
}

std::filesystem::path dirs::getModsSaveDir() {
    return getGeodeSaveDir() / "mods";
}

std::filesystem::path dirs::getModConfigDir() {
    return dirs::getGeodeDir() / "config";
}

std::filesystem::path dirs::getIndexDir() {
    return dirs::getGeodeDir() / "index";
}

std::filesystem::path dirs::getCrashlogsDir() {
    return crashlog::getCrashLogDirectory();
}
