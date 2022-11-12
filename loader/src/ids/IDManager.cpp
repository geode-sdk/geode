#include <Geode/modify/IDManager.hpp>

using namespace geode;

NodeIDs* NodeIDs::get() {
    static auto inst = new NodeIDs;
    return inst;
}
