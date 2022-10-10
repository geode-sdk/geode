#pragma once

#include <Geode/Bindings.hpp>
#include <Geode/utils/cocos.hpp>

USE_GEODE_NAMESPACE();

void addIDsToMenuLayer(MenuLayer* layer);

static CCNode* setIDSafe(CCNode* node, int index, const char* id) {
    if (auto child = getChild(node, index)) {
        child->setID(id);
        return child;
    }
    return nullptr;
}
