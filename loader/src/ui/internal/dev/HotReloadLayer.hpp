#pragma once

#include <cocos2d.h>

using namespace geode::prelude;

class HotReloadLayer : public CCLayer {
protected:
    bool init(std::string const& name);

public:
    static HotReloadLayer* create(std::string const& name);
    static HotReloadLayer* scene(std::string const& name);
};
