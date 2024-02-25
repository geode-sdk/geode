#include <Geode/loader/Dispatch.hpp>

using namespace geode::prelude;

std::unordered_map<std::string, EventListenerPool*>& geode::dispatchPools() {
    static std::unordered_map<std::string, EventListenerPool*> pools;
    return pools;
}