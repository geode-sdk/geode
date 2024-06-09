#pragma once

#include <Geode/cocos/cocoa/CCGeometry.h>
#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <GUI/CCControlExtension/CCScale9Sprite.h>
#include "ui/mods/popups/DevPopup.hpp"

using namespace geode::prelude;

class ModDeveloperList : public CCNode {
protected:
    ModMetadata m_meta;
    ScrollLayer* m_list;
    CCScale9Sprite* m_bg;
public:
    static ModDeveloperList* create(
        DevListPopup* popup, 
        ModMetadata const& meta,
        CCSize const& size
    );
protected:
    bool init(
        DevListPopup* popup, 
        ModMetadata const& meta,
        CCSize const& size
    );
};
