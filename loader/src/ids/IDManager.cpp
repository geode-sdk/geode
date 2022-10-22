#include <Geode/modify/IDManager.hpp>

using namespace geode;

NodeStringIDManager* NodeStringIDManager::get() {
    static auto inst = new NodeStringIDManager;
    return inst;
}
