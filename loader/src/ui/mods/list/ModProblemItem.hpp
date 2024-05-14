#pragma once

#include <Geode/loader/Loader.hpp>
#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/cocos/cocoa/CCGeometry.h>
#include <GUI/CCControlExtension/CCScale9Sprite.h>

using namespace geode::prelude;

class ModProblemItem : public CCNode {
protected:
    LoadProblem m_problem;
    CCScale9Sprite* m_bg;
public:
    static ModProblemItem* create(LoadProblem problem, CCSize const& size);
protected:
    bool init(LoadProblem problem, CCSize const& size);
};
