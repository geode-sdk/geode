#pragma once

#include <cocos2d.h>

namespace geode {
    class GEODE_DLL BreakLine : public cocos2d::CCNode {
        class Impl;
        std::unique_ptr<Impl> m_impl;
    protected:
        BreakLine();
        ~BreakLine();
        void draw() override;
        bool init(float width, float height, cocos2d::ccColor4F color);
    public:
        /**
         * Create a break line to act as a separator
         * @param width Width of the line
         * @param height Height of the line
         * @param color The color of the line
         */
        static BreakLine* create(
            float width,
            float height = 1.f,
            cocos2d::ccColor4F color = {1.f, 1.f, 1.f, .2f}
        );

        cocos2d::ccColor4F getColor();
    };
}
