#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

struct PickupTriggerFix : Modify<PickupTriggerFix, GJBaseGameLayer> {
    void collectItem(int item, int count) {
        item = std::clamp(item, 0, 1099);
        GJBaseGameLayer::collectItem(item, count);
    }
    
    void collectedObject(EffectGameObject* object) {
        object->m_itemBlockAID = std::clamp(object->m_itemBlockAID, 0, 1099);
        GJBaseGameLayer::collectedObject(object);
    }
};
