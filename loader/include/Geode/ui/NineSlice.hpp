#pragma once

#include <Geode/utils/ZStringView.hpp>
#include <cocos2d.h>

namespace geode {
    class GEODE_DLL NineSlice : public cocos2d::CCNodeRGBA {
    public:
        struct Insets {
            float top;
            float right;
            float bottom;
            float left;
            bool operator==(Insets const& other) const = default;
        };

        static NineSlice* create(geode::ZStringView file, cocos2d::CCRect const& rect = {}, Insets const& insets = {});
        static NineSlice* createWithSpriteFrameName(geode::ZStringView spriteFrameName, Insets const& insets = {});
        static NineSlice* createWithSpriteFrame(cocos2d::CCSpriteFrame* spriteFrame, Insets const& insets = {});

        /// Sets a multiplier for the slices to be scaled by
        void setScaleMultiplier(float scaleMultiplier);

        /// Sets whether the slices should repeat or scale, by default, it scales
        void setRepeatCenter(bool repeat);
        bool getRepeatCenter() const;

        /// Sets the insets used for the size of each side slice of the sprite
        void setInsets(Insets const& insets);
        Insets getInsets() const;

        void setInsetTop(float top);
        void setInsetRight(float right);
        void setInsetBottom(float bottom);
        void setInsetLeft(float left);

        float getInsetTop() const;
        float getInsetRight() const;
        float getInsetBottom() const;
        float getInsetLeft() const;

        cocos2d::CCSprite* getTopLeft();
        cocos2d::CCSprite* getTopRight();
        cocos2d::CCSprite* getBottomLeft();
        cocos2d::CCSprite* getBottomRight();
        cocos2d::CCSprite* getTop();
        cocos2d::CCSprite* getBottom();
        cocos2d::CCSprite* getLeft();
        cocos2d::CCSprite* getRight();
        cocos2d::CCSprite* getCenter();

        cocos2d::CCSpriteBatchNode* getBatchNode();

        virtual void setContentSize(cocos2d::CCSize const& size) override;
        virtual void visit() override;

        virtual void setColor(cocos2d::ccColor3B const& color) override;
        virtual void setOpacity(GLubyte opacity) override;

    protected:
        NineSlice();
        ~NineSlice();

        bool initWithFile(geode::ZStringView file, cocos2d::CCRect const& rect, const Insets& insets);
        bool initWithSpriteFrameName(geode::ZStringView spriteFrameName, Insets const& insets);
        bool initWithSpriteFrame(cocos2d::CCSpriteFrame* spriteFrame, Insets const& insets);

        void setup(Insets const& insets, cocos2d::CCRect const& rect = {});

        void setSpriteRect(cocos2d::CCSprite* spr, cocos2d::CCRect rect, cocos2d::CCAffineTransform transform);
        void createRepeatingSprites(cocos2d::CCSprite* spr, int horizontalAmount, int verticalAmount, float lastHorizontalFactor, float lastVerticalFactor);

        void createSprites();
        void createSprite(cocos2d::CCSprite*& spr, ZStringView id);
        void updateSprites();

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
    };
}