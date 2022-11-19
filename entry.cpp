// included by default in every geode project

#include <Geode/Loader.hpp>

GEODE_API bool GEODE_CALL geode_implicit_load(geode::Mod* m) {
    geode::Mod::setSharedMod(m);
    InternalLoader::platformMessageBox("Geode 1!", "Boobs i will squash the commits");
    geode::log::releaseSchedules(m);
    InternalLoader::platformMessageBox("Geode 2!", "Boobs i will squash the commits");
    geode::Loader::get()->releaseScheduledFunctions(m);
    InternalLoader::platformMessageBox("Geode 3!", "Boobs i will squash the commits");
    return true;
}
