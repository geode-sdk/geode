#pragma once

#include <cocos2d.h>

namespace geode {
    class GEODE_DLL Border : public cocos2d::CCLayerColor {
    public:
        struct Padding {
            float top;
            float right;
            float bottom;
            float left;
        };

        static Border* create(cocos2d::CCNode* node, const cocos2d::ccColor4B& backgroundColor, const cocos2d::CCSize& size = { 0, 0 }, const cocos2d::CCPoint& padding = { 0, 0 });

        void setPadding(const cocos2d::CCPoint& padding);
        void setPadding(float x, float y);
        void setPadding(float padding);
        Padding getPadding();
        void setPaddingX(float x);
        float getPaddingX();
        void setPaddingY(float y);
        float getPaddingY();
        void setPaddingTop(float top);
        float getPaddingTop();
        void setPaddingRight(float right);
        float getPaddingRight();
        void setPaddingBottom(float bottom);
        float getPaddingBottom();
        void setPaddingLeft(float left);
        float getPaddingLeft();
        void setBackgroundColor(const cocos2d::ccColor4B& color);
        cocos2d::ccColor4B getBackgroundColor();
        void setNode(CCNode* node);
        CCNode* getNode();
        void setSize(const cocos2d::CCSize& size);
    protected:
        Padding m_padding;

        Border(const cocos2d::CCPoint& padding = { 0, 0 });
        bool init(const cocos2d::ccColor4B& backgroundColor, const cocos2d::CCSize& size);
        bool init(CCNode* node, const cocos2d::ccColor4B& backgroundColor, const cocos2d::CCSize& size);
    private:
        void updatePadding();
    };
}