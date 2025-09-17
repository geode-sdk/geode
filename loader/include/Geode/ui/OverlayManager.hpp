#pragma once

#include "../DefaultInclude.hpp"

#include <cocos2d.h>
#include <Geode/utils/cocos.hpp>

namespace geode {
    class GEODE_DLL OverlayManager final : public cocos2d::CCNode {
    public:
        static OverlayManager* get();
    };
}
