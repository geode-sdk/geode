#include <Geode/ui/OverlayManager.hpp>

using namespace geode::prelude;

OverlayManager* OverlayManager::get()
{
    static OverlayManager* instance;

    if (!instance)
    {
        instance = new OverlayManager();
        
        CCDirector::get()->setNotificationNode(instance);
    }

    return instance;
}

void OverlayManager::addNode(CCNode* node)
{
    this->addChild(node);

    nodes.push_back(node);
}

void OverlayManager::removeNode(CCNode* node)
{
    this->removeChild(node);

    std::erase(nodes, node);
}

int OverlayManager::getHighestOverlayZOrder()
{
    int z = INT_MIN;

    for (auto node : nodes)
    {
        if (node->getZOrder() > z)
            z = node->getZOrder();
    }

    return z;
}

int OverlayManager::getLowestOverlayZOrder()
{
    int z = INT_MAX;

    for (auto node : nodes)
    {
        if (node->getZOrder() < z)
            z = node->getZOrder();
    }

    return z;
}

std::vector<CCNode*> OverlayManager::getOverlays()
{
    return nodes;
}