#pragma once

#include <Geode/Bindings.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

static constexpr int32_t GEODE_ID_PRIORITY = 0x100000;

template <class T = CCNode>
    requires std::is_base_of_v<CCNode, T>
T* setIDSafe(CCNode* node, int index, char const* id) {
    if constexpr (std::is_same_v<CCNode, T>) {
        if (auto child = getChild(node, index)) {
            child->setID(id);
            return child;
        }
    } else {
        if (auto child = getChildOfType<T>(node, index)) {
            child->setID(id);
            return child;
        }
    }
    return nullptr;
}

template <typename ...Args>
void setIDs(CCNode* node, int startIndex, Args... args) {
    for (auto i : { args... }) {
        setIDSafe(node, startIndex, i);
        ++startIndex;
    }
}

static void switchToMenu(CCNode* node, CCMenu* menu) {
    if (!node || !menu) return;
    
    auto worldPos = node->getParent()->convertToWorldSpace(node->getPosition());

    node->retain();
    node->removeFromParent();
    node->setZOrder(0);

    menu->addChild(node);
    node->setPosition(menu->convertToNodeSpace(worldPos));
}

static void switchChildToMenu(CCNode* parent, int idx, CCMenu* menu) {
    switchToMenu(static_cast<CCNode*>(parent->getChildren()->objectAtIndex(idx)), menu);
}

template <typename ...Args>
static void switchChildrenToMenu(CCNode* parent, CCMenu* menu, Args... args) {
    for (auto i : { args... }) {
        switchChildToMenu(parent, i, menu);
    }
}

template <typename T, typename... Args>
static CCMenu* detachAndCreateMenu(
    CCNode* parent, char const* menuID, Layout* layout, T first, Args... args
) {
    if (!first) {
        auto menu = CCMenu::create();
        menu->setID(menuID);
        menu->setLayout(layout);
        parent->addChild(menu);
        return menu;
    }

    auto oldMenu = first->getParent();

    first->retain();
    first->removeFromParent();

    auto newMenu = CCMenu::create();
    newMenu->setPosition(parent->convertToNodeSpace(oldMenu->convertToWorldSpace(first->getPosition())));
    newMenu->setID(menuID);
    newMenu->setZOrder(oldMenu->getZOrder());
    parent->addChild(newMenu);

    first->setPosition(0, 0);
    first->setZOrder(0);
    newMenu->addChild(first);
    first->release();

    (switchToMenu(args, newMenu), ...);
    
    newMenu->setLayout(layout);

    return newMenu;
}

static CCSize getSizeSafe(CCNode* node) {
    if (node) {
        return node->getScaledContentSize();
    }
    else {
        return CCSizeZero;
    }
}
