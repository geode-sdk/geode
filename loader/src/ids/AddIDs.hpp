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

static CCMenu* detachIntoOwnMenu(CCNode* parent, CCNode* node, const char* menuID, Layout* layout) {
    auto oldMenu = node->getParent();

    node->retain();
    node->removeFromParent();

    auto newMenu = CCMenu::create();
    newMenu->setPosition(oldMenu->convertToWorldSpace(node->getPosition()));
    newMenu->setID("top-right-menu");
    node->setPosition(0, 0);
    newMenu->addChild(node);
    newMenu->setLayout(layout);
    parent->addChild(newMenu);

    node->release();

    return newMenu;
}
