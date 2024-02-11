#pragma once

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/utils/MiniFunction.hpp>
#include <Geode/utils/cocos.hpp>

namespace geode {
    template <typename... InitArgs>
    class Popup : public FLAlertLayer {
    protected:
        cocos2d::CCSize m_size;
        cocos2d::extension::CCScale9Sprite* m_bgSprite;
        cocos2d::CCLabelBMFont* m_title = nullptr;
        CCMenuItemSpriteExtra* m_closeBtn;
        bool m_dynamic;

        ~Popup() override {
            cocos2d::CCTouchDispatcher::get()->unregisterForcePrio(this);
        }

        void registerWithTouchDispatcher() override {
            cocos2d::CCTouchDispatcher::get()->addTargetedDelegate(this, -500, true);
        }

    private:
        bool initBase(
            float width, float height, InitArgs... args, char const* bg,
            cocos2d::CCRect bgRect, bool dynamic
        ) {
            m_dynamic = dynamic;

            auto winSize = cocos2d::CCDirector::get()->getWinSize();
            m_size = cocos2d::CCSize { width, height };

            cocos2d::CCTouchDispatcher::get()->registerForcePrio(this, 2);

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

            if (dynamic) {
                m_mainLayer->ignoreAnchorPointForPosition(false);
                m_mainLayer->setPosition(winSize / 2);
                m_mainLayer->setContentSize(m_size);
                m_mainLayer->setLayout(
                    cocos2d::CopySizeLayout::create()
                        ->add(m_buttonMenu)
                        ->add(m_bgSprite)
                );
            }

            this->setTouchEnabled(true);

            auto closeSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
            closeSpr->setScale(.8f);

            m_closeBtn = CCMenuItemSpriteExtra::create(
                closeSpr, this, (cocos2d::SEL_MenuHandler)(&Popup::onClose)
            );
            if (dynamic) {
                m_buttonMenu->addChildAtPosition(m_closeBtn, cocos2d::Anchor::TopLeft, { 3.f, -3.f });
            }
            else {
                m_closeBtn->setPosition(-m_size.width / 2 + 3.f, m_size.height / 2 - 3.f);
                m_buttonMenu->addChild(m_closeBtn);
            }

            if (!setup(std::forward<InitArgs>(args)...)) {
                return false;
            }

            this->setKeypadEnabled(true);
            this->setTouchEnabled(true);

            return true;
        }

    protected:
        [[deprecated("Use Popup::initAnchored instead, as it has more reasonable menu and layer content sizes")]]
        bool init(
            float width, float height, InitArgs... args, char const* bg = "GJ_square01.png",
            cocos2d::CCRect bgRect = { 0, 0, 80, 80 }
        ) {
            return this->initBase(width, height, std::forward<InitArgs>(args)..., bg, bgRect, false);
        }

        /**
         * Init with AnchorLayout and the content size of `m_buttonMenu` and 
         * `m_bgSprite` being tied to the size of `m_mainLayer` (rather than 
         * being the size of the window)
         */
        bool initAnchored(
            float width, float height, InitArgs... args, char const* bg = "GJ_square01.png",
            cocos2d::CCRect bgRect = { 0, 0, 80, 80 }
        ) {
            return this->initBase(width, height, std::forward<InitArgs>(args)..., bg, bgRect, true);
        }

        virtual bool setup(InitArgs... args) = 0;

        void keyDown(cocos2d::enumKeyCodes key) {
            if (key == cocos2d::enumKeyCodes::KEY_Escape) return this->onClose(nullptr);
            if (key == cocos2d::enumKeyCodes::KEY_Space) return;
            return FLAlertLayer::keyDown(key);
        }

        virtual void onClose(cocos2d::CCObject*) {
            this->setKeypadEnabled(false);
            this->setTouchEnabled(false);
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
            }
            else {
                m_title = cocos2d::CCLabelBMFont::create(title.c_str(), font);
                m_title->setZOrder(2);
                if (m_dynamic) {
                    m_mainLayer->addChildAtPosition(m_title, cocos2d::Anchor::Top, ccp(0, -offset));
                }
                else {
                    auto winSize = cocos2d::CCDirector::get()->getWinSize();
                    m_title->setPosition(winSize / 2 + ccp(0, m_size.height / 2 - offset));
                    m_mainLayer->addChild(m_title);
                }
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
