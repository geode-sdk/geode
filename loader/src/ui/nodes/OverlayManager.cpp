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
    node->retain();

    nodes.push_back(node);
}

void OverlayManager::removeNode(CCNode* node)
{
    node->release();

    std::erase(nodes, node);
}

void OverlayManager::visit()
{
    std::sort(nodes.begin(), nodes.end(), [](CCNode* a, CCNode* b)
    {
        return a->getZOrder() < b->getZOrder();
    });

    for (auto node : nodes)
    {
        node->visit();
    }
}

int OverlayManager::getHighestOverlayZOrder()
{
    int z = 0;

    for (auto node : nodes)
    {
        if (node->getZOrder() > z)
            z = node->getZOrder();
    }

    return z;
}

int OverlayManager::getLowestOverlayZOrder()
{
    int z = 0;

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