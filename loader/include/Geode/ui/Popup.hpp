#pragma once

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/utils/MiniFunction.hpp>
#include <Geode/utils/cocos.hpp>

namespace geode {
    template <class... InitArgs>
    class Popup : public FLAlertLayer {
    public:
        /**
         * Event posted when this popup is being closed
         */
        class CloseEvent final : public ::geode::Event {
        private:
            class Impl final {
            private:
                Popup* popup;
                friend class CloseEvent;
            };
            std::shared_ptr<Impl> m_impl;

            friend class Popup;

            CloseEvent(Popup* popup) : m_impl(std::make_shared<Impl>()) {
                m_impl->popup = popup;
            }

        public:
            Popup* getPopup() const {
                return m_impl->popup;
            }
        };
        class CloseEventFilter final : public ::geode::EventFilter<CloseEvent> {
        public:
		    using Callback = void(CloseEvent*);

        private:
            class Impl final {
            private:
                Popup* popup;
                friend class CloseEventFilter;
            };
            std::shared_ptr<Impl> m_impl;

            friend class Popup;

            CloseEventFilter(Popup* popup) : m_impl(std::make_shared<Impl>()) {
                m_impl->popup = popup;
            }

        public:
            ListenerResult handle(utils::MiniFunction<Callback> fn, CloseEvent* event) {
                if (event->getPopup() == m_impl->popup) {
                    fn(event);
                }
                return ListenerResult::Propagate;
            }
        };

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

        void keyDown(cocos2d::enumKeyCodes key) override {
            if (key == cocos2d::enumKeyCodes::KEY_Escape) return this->onClose(nullptr);
            if (key == cocos2d::enumKeyCodes::KEY_Space) return;
            return FLAlertLayer::keyDown(key);
        }

        virtual void onClose(cocos2d::CCObject*) {
            CloseEvent(this).post();
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

        void setCloseButtonSpr(cocos2d::CCSprite* spr, float scale = 1.f) {
            // Store original attributes of the close button
            auto origSize = m_closeBtn->getContentSize();
            auto orig = Ref(m_closeBtn->getNormalImage());
            
            // Replace the close button sprite
            m_closeBtn->setNormalImage(spr);

            // Restore size and position
            spr->setScale(scale);
            spr->setPosition(orig->getPosition());
            spr->setAnchorPoint(orig->getAnchorPoint());
            m_closeBtn->setContentSize(origSize);
        }

        /**
         * Returns an event filter that listens for when this popup is closed
         */
        CloseEventFilter listenForClose() {
            return CloseEventFilter(this);
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
