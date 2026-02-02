#pragma once

#include <cocos2d.h>
#include <cocos-ext.h>
#include <Geode/binding/CCScrollLayerExt.hpp>
#include <memory>

namespace geode {
    class GEODE_DLL Scrollbar : public cocos2d::CCLayer {
    protected:
        Scrollbar();
        ~Scrollbar() override;

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

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
