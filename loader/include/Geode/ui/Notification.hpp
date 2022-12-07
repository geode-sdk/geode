#pragma once

#include "SceneManager.hpp"
#include <cocos2d.h>
#include <cocos-ext.h>
#include <Geode/binding/TextAlertPopup.hpp>
#include "../utils/cocos.hpp"

namespace geode {
    constexpr auto NOTIFICATION_DEFAULT_TIME = 1.f;
    constexpr auto NOTIFICATION_LONG_TIME = 4.f;

    enum class NotificationIcon {
        None,
        Loading,
        Success,
        Warning,
        Error,
    };

    class GEODE_DLL Notification : public cocos2d::CCNodeRGBA {
    protected:
        static Ref<cocos2d::CCArray> s_queue;
        cocos2d::extension::CCScale9Sprite* m_bg;
        cocos2d::CCLabelBMFont* m_label;
        cocos2d::CCSprite* m_icon = nullptr;
        float m_time;
        bool m_showing = false;

        bool init(std::string const& text, cocos2d::CCSprite* icon, float time);
        void updateLayout();

        static cocos2d::CCSprite* createIcon(NotificationIcon icon);

        void animateIn();
        void animateOut();
        void showNextNotification();
        void wait();

    public:
        /**
         * Create a notification, similar to TextAlertPopup but more customizable
         * @param text Notification text
         * @param icon Icon to show in the notification
         * @param time Time to show the notification on screen; pass 0 to show 
         * the notification indefinitely until hide() is called
         * @returns The new notification. Make sure to call show() to show the 
         * notification
         */
        static Notification* create(
            std::string const& text,
            NotificationIcon icon = NotificationIcon::None,
            float time = NOTIFICATION_DEFAULT_TIME
        );
        /**
         * Create a notification with a custom icon
         * @param text Notification text
         * @param icon Icon to show in the notification
         * @param time Time to show the notification on screen; pass 0 to show 
         * the notification indefinitely until hide() is called
         * @returns The new notification. Make sure to call show() to show the 
         * notification
         */
        static Notification* create(
            std::string const& text,
            cocos2d::CCSprite* icon,
            float time
        );

        void setString(std::string const& text);
        void setIcon(NotificationIcon icon);
        void setIcon(cocos2d::CCSprite* icon);
        void setTime(float time);

        /**
         * Set the wait time to default, wait the time and hide the notification. 
         * Equivalent to setTime(NOTIFICATION_DEFAULT_TIME)
        */
        void waitAndHide();

        /**
         * Adds the notification to the current scene if it doesn't have a 
         * parent yet, and displays the show animation. If the time for the 
         * notification was specified, the notification waits that time and 
         * then automatically hides
        */
        void show();

        /**
         * Hide the notification. If you passed a time to the create function, 
         * this function doesn't need to be called manually, unless you want 
         * to prematurily hide the notification
         */
        void hide();
    };
}
