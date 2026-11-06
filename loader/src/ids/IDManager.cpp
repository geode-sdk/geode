#include <Geode/modify/IDManager.hpp>

using namespace geode;

NodeIDs::NodeIDs() = default;
NodeIDs::~NodeIDs() = default;

NodeIDs* NodeIDs::get() {
    static auto inst = new NodeIDs;
    return inst;
}
