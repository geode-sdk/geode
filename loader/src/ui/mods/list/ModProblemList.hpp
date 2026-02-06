#pragma once

#include <Geode/cocos/cocoa/CCGeometry.h>
#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/ui/NineSlice.hpp>

using namespace geode::prelude;

class ModProblemList : public CCNode {
protected:
    Mod* m_mod;
    ScrollLayer* m_list;
    NineSlice* m_bg;
public:
    static ModProblemList* create(
        Mod* mod,
        CCSize const& size
    );
protected:
    bool init(
        Mod* mod,
        CCSize const& size
    );
};
