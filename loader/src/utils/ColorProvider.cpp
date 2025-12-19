#include <Geode/utils/ColorProvider.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

ColorProvidedEvent::ColorProvidedEvent(std::string id, cocos2d::ccColor4B const& color)
  : id(std::move(id)), color(color) {}

ListenerResult ColorProvidedFilter::handle(geode::Function<Callback>& fn, ColorProvidedEvent* event) {
    if (event->id == m_id) {
        fn(event);
    }
    return ListenerResult::Propagate;
}

namespace {
    EventListenerPool* getDispatchPool(std::string const& id) {
        static std::unordered_map<std::string, std::shared_ptr<DefaultEventListenerPool>> pools;
        if (pools.count(id) == 0) {
            pools[id] = DefaultEventListenerPool::create();
        }
        return pools[id].get();
    }
}

EventListenerPool* ColorProvidedEvent::getPool() const {
    return getDispatchPool(id);
}

ColorProvidedFilter::ColorProvidedFilter(std::string id) : m_id(std::move(id)) {}

EventListenerPool* ColorProvidedFilter::getPool() const {
    return getDispatchPool(m_id);
}

class ColorProvider::Impl {
public:
    std::unordered_map<std::string, std::pair<ccColor4B, std::optional<ccColor4B>>> colors;
};

ColorProvider::ColorProvider() : m_impl(new Impl()) {}

ColorProvider* ColorProvider::get() {
    static auto inst = new ColorProvider();
    return inst;
}

using is_transparent = void;

ccColor4B ColorProvider::define(std::string id, ccColor4B const& color) {
    // `insert` doesn't override existing keys, which is what we want
    m_impl->colors.insert({ std::move(id), std::pair(color, std::nullopt) });
    return this->color(id);
}
ccColor3B ColorProvider::define(std::string id, ccColor3B const& color) {
    return to3B(this->define(std::move(id), to4B(color)));
}
ccColor4B ColorProvider::override(std::string const& id, ccColor4B const& color) {
    if (m_impl->colors.contains(id)) {
        m_impl->colors.at(id).second = color;
        ColorProvidedEvent(id, color).post();
        return color;
    }
    else {
        log::error("(ColorProvider) Attempted to override color \"{}\", which is not defined", id);
        return to4B(ccWHITE);
    }
}
ccColor3B ColorProvider::override(std::string const& id, ccColor3B const& color) {
    return to3B(this->override(id, to4B(color)));
}
ccColor4B ColorProvider::reset(std::string const& id) {
    if (m_impl->colors.contains(id)) {
        m_impl->colors.at(id).second = std::nullopt;
        auto def = m_impl->colors.at(id).first;
        ColorProvidedEvent(id, def).post();
        return def;
    }
    else {
        log::error("(ColorProvider) Attempted to reset color \"{}\", which is not defined", id);
        return to4B(ccWHITE);
    }
}
ccColor4B ColorProvider::color(std::string const& id) const {
    if (m_impl->colors.contains(id)) {
        return m_impl->colors.at(id).second.value_or(m_impl->colors.at(id).first);
    }
    else {
        log::error("(ColorProvider) Attempted to get color \"{}\", which is not defined", id);
        return to4B(ccWHITE);
    }
}
ccColor3B ColorProvider::color3b(std::string const& id) const {
    return to3B(this->color(id));
}
