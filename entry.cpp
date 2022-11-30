// included by default in every geode project
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>

GEODE_API void GEODE_CALL geode_implicit_load(geode::Mod* m) {
    geode::Mod::setSharedMod(m);
    geode::log::releaseSchedules(m);
    geode::Loader::get()->dispatchScheduledFunctions(m);
}
