#pragma once

#include "../DefaultInclude.hpp"
#include <Geode/cocos/base_nodes/CCNode.h>

namespace geode {
    class GEODE_DLL OverlayManager final : public cocos2d::CCNode {
    public:
        static OverlayManager* get();
    };
}
