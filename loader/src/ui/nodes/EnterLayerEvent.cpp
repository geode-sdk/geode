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

PassThrough AEnterLayerEventHandler::handle(AEnterLayerEvent* event) {
    if (m_targetID == event->getID()) {
        m_consumer(event);
    }
    return PassThrough::Propagate;
}

AEnterLayerEventHandler::AEnterLayerEventHandler(
    std::optional<std::string> const& id,
    Consumer handler
) : m_targetID(id),
    m_consumer(handler) {}
