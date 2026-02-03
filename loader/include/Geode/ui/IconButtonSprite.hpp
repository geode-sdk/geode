#pragma once

#include <cocos2d.h>

namespace cocos2d::extension {
    class CCScale9Sprite;
}

namespace geode {
    class GEODE_DLL IconButtonSprite : public cocos2d::CCSprite, public cocos2d::CCLabelProtocol {
        class Impl;
        std::unique_ptr<Impl> m_impl;
    protected:
        bool init(
            char const* bg, bool bgIsFrame, cocos2d::CCNode* icon, char const* text,
            char const* font
        );

        void updateLayout();

        IconButtonSprite();
        ~IconButtonSprite();
        IconButtonSprite(IconButtonSprite&&) = delete;
        IconButtonSprite& operator=(IconButtonSprite&&) = delete;

    public:
        static IconButtonSprite* create(
            char const* bg, cocos2d::CCNode* icon, char const* text, char const* font
        );
        static IconButtonSprite* createWithSpriteFrameName(
            char const* bg, cocos2d::CCNode* icon, char const* text, char const* font
        );

        void setBG(char const* bg, bool isFrame);

        void setIcon(cocos2d::CCNode* icon);
        cocos2d::CCNode* getIcon() const;

        void setString(char const* label) override;
        char const* getString() override;

        void setColor(cocos2d::ccColor3B const& color) override;
        void setOpacity(GLubyte opacity) override;
        cocos2d::extension::CCScale9Sprite* getBg();
        cocos2d::CCLabelBMFont* getLabel();
        cocos2d::CCNode* getIcon();
    };
}
