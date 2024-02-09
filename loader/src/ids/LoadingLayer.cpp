#include <Geode/modify/IDManager.hpp>
#include <Geode/utils/NodeIDs.hpp>
#include <Geode/binding/LoadingLayer.hpp>

using namespace geode::prelude;
using namespace geode::node_ids;

$register_ids(LoadingLayer) {
    setIDs(
        this, 0,
        "bg-texture",
        "gd-logo",
        "robtop-logo",
        "cocos2d-logo",
        "fmod-logo",
        "loading-text",
        "text-area",
        "progress-slider"
    );
}
