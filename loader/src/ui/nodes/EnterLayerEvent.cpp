#include <Geode/ui/EnterLayerEvent.hpp>

USE_GEODE_NAMESPACE();

AEnterLayerEvent::AEnterLayerEvent(
    std::string const& layerID,
    cocos2d::CCNode* layer
) : layerID(layerID),
    layer(layer) {}

ListenerResult AEnterLayerFilter::handle(Callback fn, AEnterLayerEvent* event) {
    if (m_targetID == event->layerID) {
        fn(event);
    }
    return ListenerResult::Propagate;
}

AEnterLayerFilter::AEnterLayerFilter(
    std::optional<std::string> const& id
) : m_targetID(id) {}
