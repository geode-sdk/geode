#pragma once

#include <cocos2d.h>

namespace cocos2d::extension {
    class CCScale9Sprite;
}

namespace geode {
    class GEODE_DLL IconButtonSprite :
        public cocos2d::CCSprite,
        public cocos2d::CCLabelProtocol
    {
    protected:
        cocos2d::extension::CCScale9Sprite* m_bg = nullptr;
        cocos2d::CCLabelBMFont* m_label = nullptr;
        cocos2d::CCNode* m_icon = nullptr;

        bool init(
            const char* bg,
            bool bgIsFrame,
            cocos2d::CCNode* icon,
            const char* text,
            const char* font
        );

        void updateLayout();

        IconButtonSprite() = default;
        IconButtonSprite(IconButtonSprite&&) = delete;
        IconButtonSprite& operator=(IconButtonSprite&&) = delete;
    
    public:
        static IconButtonSprite* create(
            const char* bg,
            cocos2d::CCNode* icon,
            const char* text,
            const char* font
        );
        static IconButtonSprite* createWithSpriteFrameName(
            const char* bg,
            cocos2d::CCNode* icon,
            const char* text,
            const char* font
        );

        void setBG(const char* bg, bool isFrame);

        void setIcon(cocos2d::CCNode* icon);
        cocos2d::CCNode* getIcon() const;

        void setString(const char* label) override;
        const char* getString() override;
    };
}
