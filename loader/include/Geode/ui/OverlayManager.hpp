#pragma once

#include "../DefaultInclude.hpp"
#include <cocos2d.h>

namespace geode {
    class GEODE_DLL OverlayManager final : public cocos2d::CCNode {
    protected:
        bool init() override;
    public:
        static OverlayManager* get();
    };
}
