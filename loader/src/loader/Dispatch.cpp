#include <Geode/loader/Dispatch.hpp>

using namespace geode::prelude;

StringMap<EventListenerPool*>& geode::dispatchPools() {
    static StringMap<EventListenerPool*> pools;
    return pools;
}