#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/ui/ScrollLayer.hpp>

using namespace geode::prelude;

class ModProblemItemList : public CCNode {
protected:
    ScrollLayer* m_scrollLayer;

    bool init(float height);

public:
    static ModProblemItemList* create(float height);
};
