#pragma once

#include <cocos2d.h>

namespace geode {
    class GEODE_DLL Scrollbar : public cocos2d::CCLayer {
    protected:
        CCScrollLayerExt* m_target = nullptr;
        cocos2d::extension::CCScale9Sprite* m_track;
        cocos2d::extension::CCScale9Sprite* m_thumb;
        cocos2d::CCPoint m_clickOffset;
        float m_width;
        bool m_resizeThumb;
        bool m_trackIsRotated;
        bool m_hoverHighlight;
        bool m_touchDown = false;

        bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        void scrollWheel(float y, float x) override;
        void registerWithTouchDispatcher() override;
    
        void draw() override;

        bool init(CCScrollLayerExt* list);

    public:
        void setTarget(CCScrollLayerExt* list);

        static Scrollbar* create(CCScrollLayerExt* list);
    };
}
