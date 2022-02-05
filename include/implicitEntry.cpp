#include <Geode.hpp>

GEODE_API bool GEODE_CALL geode_implicit_load(geode::Mod* mod) {
    geode::Interface::get()->init(mod);
    return true;
}
