#pragma once

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/ui/Label.hpp>
#include <Geode/ui/Layout.hpp>
#include <Geode/ui/NineSlice.hpp>
#include <Geode/utils/ZStringView.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/function.hpp>

namespace geode {
    class GEODE_DLL Popup : public FLAlertLayer {
    public:
        /**
         * Event posted when this popup is being closed
         */
        class CloseEvent final : public ::geode::Event<CloseEvent, bool(), Popup*> {
            // filter params popup
            using Event::Event;
        };

        virtual void onClose(CCObject*);

    protected:
        cocos2d::CCSize m_size;
        NineSlice* m_bgSprite;
        geode::Label* m_title = nullptr;
        CCMenuItemSpriteExtra* m_closeBtn;

        ~Popup();
        void registerWithTouchDispatcher() override;

        /**
         * Init with AnchorLayout and the content size of `m_buttonMenu` and
         * `m_bgSprite` being tied to the size of `m_mainLayer` (rather than
         * being the size of the window)
         */
        bool init(float width, float height, ZStringView bg = "GJ_square01.png", cocos2d::CCRect bgRect = {});
        bool init(cocos2d::CCSize size, ZStringView bg = "GJ_square01.png", cocos2d::CCRect bgRect = {});

        void keyBackClicked() override;
        void keyDown(cocos2d::enumKeyCodes key, double p1) override;

        void setTitle(ZStringView title, ZStringView font = "goldFont.fnt", float scale = .7f, float offset = 20.f);

        void setCloseButtonSpr(cocos2d::CCSprite* spr, float scale = 1.f);

        /**
         * Returns an event filter that listens for when this popup is closed
         */
        CloseEvent listenForClose();
    };

    GEODE_DLL FLAlertLayer* createQuickPopup(ZStringView title, std::string content, ZStringView btn1, ZStringView btn2 = "", geode::Function<void(FLAlertLayer*, bool)> selected = nullptr, bool doShow = true, bool cancelledByEscape = false);

    GEODE_DLL FLAlertLayer* createQuickPopup(ZStringView title, std::string content, ZStringView btn1, ZStringView btn2, float width, geode::Function<void(FLAlertLayer*, bool)> selected, bool doShow = true, bool cancelledByEscape = false);
}
