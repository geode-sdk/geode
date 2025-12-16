#include <Geode/ui/EnterLayerEvent.hpp>

using namespace geode::prelude;

AEnterLayerEvent::AEnterLayerEvent(
    std::string layerID,
    cocos2d::CCNode* layer
) : layerID(std::move(layerID)),
    layer(layer) {}

ListenerResult AEnterLayerFilter::handle(geode::Function<Callback>& fn, AEnterLayerEvent* event) {
    if (m_targetID == event->layerID) {
        fn(event);
    }
    return ListenerResult::Propagate;
}

AEnterLayerFilter::AEnterLayerFilter(
    std::optional<std::string> id
) : m_targetID(std::move(id)) {}
