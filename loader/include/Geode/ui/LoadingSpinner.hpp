#pragma once

#include <cocos2d.h>

namespace geode {
    /**
     * An eternally spinning loading circle. Essentially just a more convenient 
     * alternative to RobTop's `LoadingCircle` class, as this one respects its 
     * content size and is a lot more stripped down (not requiring a `show` 
     * method or anything - it just works!)
     */
    class GEODE_DLL LoadingSpinner : public cocos2d::CCNode {
    protected:
        cocos2d::CCSprite* m_spinner;

        bool init(float size);

        void spin();

    public:
        /**
         * Create a loading circle
         * @param size The diameter of the circle in Cocos units
         */
        static LoadingSpinner* create(float size);

        void setVisible(bool visible) override;
    };
}
