
#include <Geode/loader/Dirs.hpp>
#include <cocos2d.h>
#include <crashlog.hpp>
#include <asp/fs/Path.hpp>
#include "LoaderImpl.hpp"

using namespace geode::prelude;

asp::fs::Path dirs::getGeodeDir() {
    return dirs::getGameDir() / "geode";
}

asp::fs::Path dirs::getGeodeSaveDir() {
    return dirs::getSaveDir() / "geode";
}

asp::fs::Path dirs::getGeodeResourcesDir() {
    return dirs::getGeodeDir() / "resources";
}

asp::fs::Path dirs::getGeodeLogDir() {
    return dirs::getGeodeDir() / "logs";
}

asp::fs::Path dirs::getTempDir() {
    return dirs::getGeodeDir() / "temp";
}

asp::fs::Path dirs::getModsDir() {
    return dirs::getGeodeDir() / "mods";
}

asp::fs::Path dirs::getModsSaveDir() {
    return dirs::getGeodeSaveDir() / "mods";
}

asp::fs::Path dirs::getModConfigDir() {
    return dirs::getGeodeDir() / "config";
}

asp::fs::Path dirs::getIndexDir() {
    return dirs::getGeodeDir() / "index";
}

asp::fs::Path dirs::getCrashlogsDir() {
    return crashlog::getCrashLogDirectory();
}

asp::fs::Path dirs::getModPersistentDir() {
    return dirs::getSaveDir() / "geode-persistent";
}

// asp::fs::Path dirs::getModBinariesDir() {
//     return dirs::getModRuntimeDir() / "binaries";
// }