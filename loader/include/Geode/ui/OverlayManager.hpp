#pragma once

#include "../DefaultInclude.hpp"
#include <cocos2d.h>

namespace geode {
    class GEODE_DLL OverlayManager final : public cocos2d::CCNode {
    public:
        static OverlayManager* get();
    };
}
