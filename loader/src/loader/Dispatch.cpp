#include <Geode/loader/Dispatch.hpp>

using namespace geode::prelude;

<<<<<<< HEAD
EventListenerPool* geode::getDispatchPool(std::string const& id) {
    static std::unordered_map<std::string, std::shared_ptr<DefaultEventListenerPool>> pools;
    if (pools.count(id) == 0) {
        pools[id] = DefaultEventListenerPool::create();
    }
    return pools[id].get();
=======
StringMap<EventListenerPool*>& geode::dispatchPools() {
    static StringMap<EventListenerPool*> pools;
    return pools;
>>>>>>> v5
}