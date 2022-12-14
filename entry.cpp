#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Loader.hpp>

namespace {
    // to make sure the instance is set into the sharedMod<> in load time
    static auto mod = geode::getMod(); 
}