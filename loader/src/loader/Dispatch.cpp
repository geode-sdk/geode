#include <Geode/loader/Event.hpp>
#include <Geode/utils/ranges.hpp>
#include <mutex>
#include <unordered_map>

using namespace geode::prelude;

static std::unordered_map<std::string, EventListenerPool*> s_pools;

EventListenerPool* DispatchFilter::getPool() const {
    if (s_pools.count(m_id) == 0) {
        s_pools[m_id] = new DefaultEventListenerPool();
    }
    return s_pools[m_id];
}