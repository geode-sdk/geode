#pragma once

#include <Geode/Bindings.hpp>

namespace geode {
    class GEODE_DLL Scrollbar :
        public cocos2d::CCLayer
        // public ExtMouseDelegate
    {
    protected:
        CCScrollLayerExt* m_target = nullptr;
        cocos2d::extension::CCScale9Sprite* m_track;
        cocos2d::extension::CCScale9Sprite* m_thumb;
        cocos2d::CCPoint m_clickOffset;
        float m_width;
        bool m_resizeThumb;
        bool m_trackIsRotated;
        bool m_hoverHighlight;

        // bool mouseDownExt(MouseEvent, cocos2d::CCPoint const&) override;
        // bool mouseUpExt(MouseEvent, cocos2d::CCPoint const&) override;
        // void mouseMoveExt(cocos2d::CCPoint const&) override;
        void scrollWheel(float y, float x) override;
    
        void draw() override;

        bool init(CCScrollLayerExt*);

    public:
        void setTarget(CCScrollLayerExt* list);

        static Scrollbar* create(CCScrollLayerExt* list);
    };
}
