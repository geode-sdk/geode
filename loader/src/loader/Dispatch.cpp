#include <Geode/loader/Dispatch.hpp>

using namespace geode::prelude;

EventListenerPool* geode::getDispatchPool(std::string const& id) {
    static std::unordered_map<std::string, std::shared_ptr<DefaultEventListenerPool>> pools;
    if (pools.count(id) == 0) {
        pools[id] = DefaultEventListenerPool::create();
    }
    return pools[id].get();
}