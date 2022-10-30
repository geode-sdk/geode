#pragma once

#include <cocos2d.h>

namespace cocos2d::extension {
    class CCScale9Sprite;
}

namespace geode {
    class GEODE_DLL IconButtonSprite : public cocos2d::CCSprite, public cocos2d::CCLabelProtocol {
    protected:
        cocos2d::extension::CCScale9Sprite* m_bg = nullptr;
        cocos2d::CCLabelBMFont* m_label = nullptr;
        cocos2d::CCNode* m_icon = nullptr;

        bool init(
            char const* bg, bool bgIsFrame, cocos2d::CCNode* icon, char const* text,
            char const* font
        );

        void updateLayout();

        IconButtonSprite() = default;
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
    };
}
