#include <Geode/modify/IDManager.hpp>

using namespace geode;

NodeStringIDManager* NodeStringIDManager::get() {
    static auto inst = new NodeStringIDManager;
    return inst;
}

bool NodeStringIDManager::provide(std::string const& id, cocos2d::CCNode* layer) {
    if (m_providers.count(id)) {
        m_providers.at(id)(layer);
        return true;
    }
    return false;
}
