#pragma once

#include <Geode/binding/CCScrollLayerExt.hpp>
#include <Geode/ui/NineSlice.hpp>
#include <memory>

namespace geode {
    class GEODE_DLL Scrollbar : public cocos2d::CCLayer {
    public:
        enum class Style {
            Geode = 0,
            GDSlider = 1,
            GDScrollbar = 2,
        };

    protected:
        Scrollbar();
        ~Scrollbar() override;

        bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        bool scrollWheel(float y, float x) override;
        void registerWithTouchDispatcher() override;

        void draw() override;

        bool init(CCScrollLayerExt* list, Style style);

    public:
        bool isTouching();
        CCScrollLayerExt* getTarget();
        NineSlice* getTrack();
        NineSlice* getThumb();

        void setTarget(CCScrollLayerExt* list);
        void setTrack(NineSlice* track);
        void setThumb(NineSlice* thumb);

        void setShrinkWithOverscroll(bool shrink);
        bool getShrinkWithOverscroll() const;

        void setTouchInThumb(bool onlyInThumb);
        bool getTouchInThumb() const;

        Style getStyle() const;
        void setStyle(Style style);

        void setThumbColors(cocos2d::ccColor4B normal, cocos2d::ccColor4B selected);

        static Scrollbar* create(CCScrollLayerExt* list, Style style = Style::Geode);

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
