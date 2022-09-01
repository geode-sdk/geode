#pragma once

#include <Geode/Bindings.hpp>

namespace geode {
    template<typename... InitArgs>
    class GEODE_DLL Popup : public FLAlertLayer {
    protected:
        cocos2d::CCSize m_size;
        cocos2d::extension::CCScale9Sprite* m_bgSprite;
        cocos2d::CCLabelBMFont* m_title = nullptr;

        bool init(
            float width,
            float height,
            InitArgs... args,
            const char* bg = "GJ_square01.png"
        ) {
            auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
            m_size = cocos2d::CCSize { width, height };

            if (!this->initWithColor({ 0, 0, 0, 105 })) return false;
            m_mainLayer = cocos2d::CCLayer::create();
            this->addChild(m_mainLayer);

            m_bgSprite = cocos2d::extension::CCScale9Sprite::create(bg, { 0, 0, 80, 80 });
            m_bgSprite->setContentSize(m_size);
            m_bgSprite->setPosition(winSize.width / 2, winSize.height / 2);
            m_mainLayer->addChild(m_bgSprite);

            m_buttonMenu = cocos2d::CCMenu::create();
            m_mainLayer->addChild(m_buttonMenu);

            cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);
            this->registerWithTouchDispatcher();

            if (!setup(std::forward<InitArgs>(args)...)) {
                return false;
            }

            auto closeSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
            closeSpr->setScale(.8f);

            auto closeBtn = CCMenuItemSpriteExtra::create(
                closeSpr, this, (cocos2d::SEL_MenuHandler)(&Popup::onClose)
            );
            closeBtn->setPosition(
                -m_size.width / 2 + 3.f,
                m_size.height / 2 - 3.f
            );
            m_buttonMenu->addChild(closeBtn);

            this->setKeypadEnabled(true);
            this->setTouchEnabled(true);

            return true;
        }

        virtual bool setup(InitArgs... args) = 0;

        void keyDown(cocos2d::enumKeyCodes key) {
            if (key == cocos2d::enumKeyCodes::KEY_Escape) return this->onClose(nullptr);
            if (key == cocos2d::enumKeyCodes::KEY_Space) return;
            return FLAlertLayer::keyDown(key);
        }

        virtual void onClose(cocos2d::CCObject*) {
            this->setKeyboardEnabled(false);
            this->removeFromParentAndCleanup(true);
        }

        void setTitle(const char* title, const char* font = "goldFont.fnt") {
            if (m_title) {
                m_title->setString(title);
            } else {
                auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
                m_title = cocos2d::CCLabelBMFont::create(title, font);
                m_title->setPosition(
                    winSize.width / 2,
                    winSize.height / 2 + m_size.height / 2 - 20.f
                );
                m_mainLayer->addChild(m_title, 2);
            }
            m_title->limitLabelWidth(m_size.width - 20.f, .7f, .1f);
        }
    };

    void GEODE_DLL createQuickPopup(
        const char* title,
        std::string const& content,
        const char* btn1,
        const char* btn2,
        std::function<void(FLAlertLayer*, bool)> selected
    );
}


