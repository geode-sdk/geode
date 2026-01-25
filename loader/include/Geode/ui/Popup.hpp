#pragma once

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/ZStringView.hpp>
#include <Geode/utils/function.hpp>
#include <Geode/ui/Layout.hpp>

namespace geode {
    class GEODE_DLL Popup : public FLAlertLayer {
    public:
        /**
         * Event posted when this popup is being closed
         */
        class CloseEvent final : public ::geode::Event {
        private:
            class Impl;
            std::shared_ptr<Impl> m_impl;

            friend class Popup;

            CloseEvent(Popup* popup);

        public:
            Popup* getPopup() const;
            bool filter(Popup* popup) const;
        };
        
        class CloseEventFilter final : public ::geode::EventFilter<CloseEvent> {
        public:
		    using Callback = void(CloseEvent*);

        private:
            class Impl;
            std::shared_ptr<Impl> m_impl;

            friend class Popup;

            CloseEventFilter(Popup* popup);

        public:
            ListenerResult handle(geode::Function<Callback>& fn, CloseEvent* event);
        };

    protected:
        cocos2d::CCSize m_size;
        cocos2d::extension::CCScale9Sprite* m_bgSprite;
        cocos2d::CCLabelBMFont* m_title = nullptr;
        CCMenuItemSpriteExtra* m_closeBtn;

        ~Popup();
        void registerWithTouchDispatcher() override;
        
        /**
         * Init with AnchorLayout and the content size of `m_buttonMenu` and
         * `m_bgSprite` being tied to the size of `m_mainLayer` (rather than
         * being the size of the window)
         */
        bool init(
            float width, float height, char const* bg = "GJ_square01.png",
            cocos2d::CCRect bgRect = {}
        );

        void keyBackClicked() override;
        void keyDown(cocos2d::enumKeyCodes key, double p1) override;
        virtual void onClose(CCObject*);

        void setTitle(
            ZStringView title,
            const char* font = "goldFont.fnt",
            float scale = .7f,
            float offset = 20.f
        );

        void setCloseButtonSpr(cocos2d::CCSprite* spr, float scale = 1.f);

        /**
         * Returns an event filter that listens for when this popup is closed
         */
        CloseEventFilter listenForClose();
    };

    GEODE_DLL FLAlertLayer* createQuickPopup(
        char const* title, std::string content, char const* btn1, char const* btn2,
        geode::Function<void(FLAlertLayer*, bool)> selected, bool doShow = true, bool cancelledByEscape = false
    );

    GEODE_DLL FLAlertLayer* createQuickPopup(
        char const* title, std::string content, char const* btn1, char const* btn2,
        float width, geode::Function<void(FLAlertLayer*, bool)> selected, bool doShow = true, bool cancelledByEscape = false
    );
}
