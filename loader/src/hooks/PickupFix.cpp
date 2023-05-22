#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

struct PickupTriggerFix : Modify<PickupTriggerFix, GJBaseGameLayer> {
    void collectItem(int item, int count) {
        item = std::clamp(item, 0, 1099);
        GJBaseGameLayer::collectItem(item, count);
    }
};
