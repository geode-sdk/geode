#pragma once

#include <Geode/utils/ZStringView.hpp>

namespace geode {
    class GEODE_DLL Scale9Sprite : public cocos2d::CCNodeRGBA {
    public:
        struct Insets {
            float top;
            float right;
            float bottom;
            float left;
            bool operator==(const Insets& other) const = default;
        };

        static Scale9Sprite* create(geode::ZStringView file, const cocos2d::CCRect& rect = {}, const Insets& insets = {});
        static Scale9Sprite* createWithSpriteFrameName(geode::ZStringView spriteFrameName, const Insets& insets = {});
        static Scale9Sprite* createWithSpriteFrame(cocos2d::CCSpriteFrame* spriteFrame, const Insets& insets = {});

        /// Sets a multiplier for the slices to be scaled by
        void setScaleMultiplier(float scaleMultiplier);

        /// Sets whether the slices should repeat or scale, by default, it scales
        void setRepeatCenter(bool repeat);
        bool getRepeatCenter() const;

        /// Sets the insets used for the size of each side slice of the sprite
        void setInsets(const Insets& insets);
        Insets getInsets() const;

        void setInsetTop(float top);
        void setInsetRight(float right);
        void setInsetBottom(float bottom);
        void setInsetLeft(float left);

        float getInsetTop() const;
        float getInsetRight() const;
        float getInsetBottom() const;
        float getInsetLeft() const;

        virtual void setContentSize(const cocos2d::CCSize& size) override;
        virtual void visit() override;

        virtual void setColor(const cocos2d::ccColor3B& color) override;
        virtual void setOpacity(GLubyte opacity) override;

    protected:
        Scale9Sprite();
        ~Scale9Sprite();

        bool initWithFile(geode::ZStringView file, const cocos2d::CCRect& rect, const Insets& insets);
        bool initWithSpriteFrameName(geode::ZStringView spriteFrameName, const Insets& insets);
        bool initWithSpriteFrame(cocos2d::CCSpriteFrame* spriteFrame, const Insets& insets);

        void setup(const Insets& insets, const cocos2d::CCRect& rect = {});

        void setSpriteRect(cocos2d::CCSprite* spr, cocos2d::CCRect rect, cocos2d::CCAffineTransform transform);
        void createRepeatingSprites(cocos2d::CCSprite* spr, int horizontalAmount, int verticalAmount, float lastHorizontalFactor, float lastVerticalFactor);

        void createSprites();
        void createSprite(cocos2d::CCSprite*& spr);
        void updateSprites();

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
    };
}