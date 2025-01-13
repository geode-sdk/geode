#include <Geode/loader/ModEvent.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

ModStateEvent::ModStateEvent(Mod* mod, ModEventType type) : m_mod(mod), m_type(type) {}

ModEventType ModStateEvent::getType() const {
    return m_type;
}

Mod* ModStateEvent::getMod() const {
    return m_mod;
}

ListenerResult ModStateFilter::handle(std::function<Callback> fn, ModStateEvent* event) {
    // log::debug("Event mod filter: {}, {}, {}, {}", m_mod, static_cast<int>(m_type), event->getMod(), static_cast<int>(event->getType()));
    if ((!m_mod || event->getMod() == m_mod) && event->getType() == m_type) {
        fn(event);
    }
    return ListenerResult::Propagate;
}

ModStateFilter::ModStateFilter(Mod* mod, ModEventType type) : m_mod(mod), m_type(type) {}

class DependencyLoadedEvent::Impl final {
public:
    Mod* target;
    Mod* dependency;
};

DependencyLoadedEvent::DependencyLoadedEvent(Mod* target, Mod* dependency)
  : m_impl(std::make_unique<Impl>())
{
    m_impl->target = target;
    m_impl->dependency = dependency;
}

Mod* DependencyLoadedEvent::getTarget() const {
    return m_impl->target;
}
Mod* DependencyLoadedEvent::getDependency() const {
    return m_impl->dependency;
}
matjson::Value DependencyLoadedEvent::getDependencySettings() const {
    return m_impl->dependency->getDependencySettingsFor(m_impl->target->getID());
}

class DependencyLoadedFilter::Impl final {
public:
    Mod* target;
};

ListenerResult DependencyLoadedFilter::handle(std::function<Callback> fn, DependencyLoadedEvent* event) {
    if (event->getTarget() == m_impl->target) {
        fn(event);
    }
    return ListenerResult::Propagate;
}

DependencyLoadedFilter::DependencyLoadedFilter(Mod* target = geode::getMod())
  : m_impl(std::make_unique<Impl>())
{
    m_impl->target = target;
}
DependencyLoadedFilter::DependencyLoadedFilter(DependencyLoadedFilter const&) = default;
