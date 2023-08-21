#pragma once

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/utils/MiniFunction.hpp>

namespace geode {
    template <typename... InitArgs>
    class Popup : public FLAlertLayer {
    protected:
        cocos2d::CCSize m_size;
        cocos2d::extension::CCScale9Sprite* m_bgSprite;
        cocos2d::CCLabelBMFont* m_title = nullptr;
        CCMenuItemSpriteExtra* m_closeBtn;

        bool init(
            float width, float height, InitArgs... args, char const* bg = "GJ_square01.png",
            cocos2d::CCRect bgRect = { 0, 0, 80, 80 }
        ) {
            auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
            m_size = cocos2d::CCSize { width, height };

            if (!this->initWithColor({ 0, 0, 0, 105 })) return false;
            m_mainLayer = cocos2d::CCLayer::create();
            this->addChild(m_mainLayer);

            m_bgSprite = cocos2d::extension::CCScale9Sprite::create(bg, bgRect);
            m_bgSprite->setContentSize(m_size);
            m_bgSprite->setPosition(winSize.width / 2, winSize.height / 2);
            m_mainLayer->addChild(m_bgSprite);

            m_buttonMenu = cocos2d::CCMenu::create();
            m_buttonMenu->setZOrder(100);
            m_mainLayer->addChild(m_buttonMenu);

            this->setTouchEnabled(true);

            auto closeSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
            closeSpr->setScale(.8f);

            m_closeBtn = CCMenuItemSpriteExtra::create(
                closeSpr, this, (cocos2d::SEL_MenuHandler)(&Popup::onClose)
            );
            m_closeBtn->setPosition(-m_size.width / 2 + 3.f, m_size.height / 2 - 3.f);
            m_buttonMenu->addChild(m_closeBtn);

            if (!setup(std::forward<InitArgs>(args)...)) {
                return false;
            }

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

        void setTitle(
            std::string const& title,
            const char* font = "goldFont.fnt",
            float scale = .7f,
            float offset = 20.f
        ) {
            if (m_title) {
                m_title->setString(title.c_str());
            } else {
                auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
                m_title = cocos2d::CCLabelBMFont::create(title.c_str(), font);
                m_title->setPosition(
                    winSize.width / 2, winSize.height / 2 + m_size.height / 2 - offset
                );
                m_mainLayer->addChild(m_title, 2);
            }
            m_title->limitLabelWidth(m_size.width - 20.f, scale, .1f);
        }
    };

    GEODE_DLL FLAlertLayer* createQuickPopup(
        char const* title, std::string const& content, char const* btn1, char const* btn2,
        utils::MiniFunction<void(FLAlertLayer*, bool)> selected, bool doShow = true
    );

    GEODE_DLL FLAlertLayer* createQuickPopup(
        char const* title, std::string const& content, char const* btn1, char const* btn2,
        float width, utils::MiniFunction<void(FLAlertLayer*, bool)> selected, bool doShow = true
    );

    GEODE_DLL FLAlertLayer* createQuickPopup(
        char const* title, std::string const& content, char const* btn1, char const* btn2,
        utils::MiniFunction<void(FLAlertLayer*, bool)> selected, bool doShow, bool cancelledByEscape
    );

    GEODE_DLL FLAlertLayer* createQuickPopup(
        char const* title, std::string const& content, char const* btn1, char const* btn2,
        float width, utils::MiniFunction<void(FLAlertLayer*, bool)> selected, bool doShow, bool cancelledByEscape
    );
}
