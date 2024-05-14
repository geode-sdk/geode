#pragma once

#include <GUI/CCControlExtension/CCScale9Sprite.h>
#include <Geode/loader/Loader.hpp>
#include <Geode/cocos/cocoa/CCGeometry.h>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/cocos/base_nodes/CCNode.h>
#include <vector>

using namespace geode::prelude;

class ModProblemList : public CCNode {
protected:
    std::vector<LoadProblem> m_problems;
    ScrollLayer* m_list;
    CCScale9Sprite* m_bg;
public:
    static ModProblemList* create(
        std::vector<LoadProblem> problems,
        CCSize const& size
    );
protected:
    bool init(
        std::vector<LoadProblem> problems,
        CCSize const& size
    );
};
