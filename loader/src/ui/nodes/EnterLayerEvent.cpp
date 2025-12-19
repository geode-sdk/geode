#include <Geode/ui/EnterLayerEvent.hpp>

using namespace geode::prelude;

AEnterLayerEvent::AEnterLayerEvent(
    std::string layerID,
    cocos2d::CCNode* layer
) : layerID(std::move(layerID)),
    layer(layer) {}

EventListenerPool* AEnterLayerEvent::getPool() const {
    return DefaultEventListenerPool::getForEvent<AEnterLayerEvent>();
}

ListenerResult AEnterLayerFilter::handle(geode::Function<Callback>& fn, AEnterLayerEvent* event) {
    if (m_targetID.has_value()) {
        if (event->layerID == m_targetID.value()) {
            fn(event);
        }
    }
    else {
        fn(event);
    }
    return ListenerResult::Propagate;
}

AEnterLayerFilter::AEnterLayerFilter(
    std::optional<std::string> id
) : m_targetID(std::move(id)) {}

EventListenerPool* AEnterLayerFilter::getPool() const {
    return DefaultEventListenerPool::getForEvent<AEnterLayerEvent>();
}