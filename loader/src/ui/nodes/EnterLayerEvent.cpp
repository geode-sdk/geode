#include <Geode/ui/EnterLayerEvent.hpp>

USE_GEODE_NAMESPACE();

AEnterLayerEvent::AEnterLayerEvent(
    std::string const& layerID,
    cocos2d::CCNode* layer
) : m_layerID(layerID),
    m_layer(layer) {}

std::string AEnterLayerEvent::getID() const {
    return m_layerID;
}

cocos2d::CCNode* AEnterLayerEvent::getLayer() const {
    return m_layer;
}

ListenerResult AEnterLayerFilter::handle(Callback fn, AEnterLayerEvent* event) {
    if (m_targetID == event->getID()) {
        fn(event);
    }
    return ListenerResult::Propagate;
}

AEnterLayerFilter::AEnterLayerFilter(
    std::optional<std::string> const& id
) : m_targetID(id) {}
