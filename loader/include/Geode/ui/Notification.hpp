#pragma once

#include <cocos2d.h>
#include <cocos-ext.h>
#include <Geode/binding/TextAlertPopup.hpp>
#include <Geode/ui/NineSlice.hpp>

namespace geode {
    constexpr auto NOTIFICATION_DEFAULT_TIME = 1.8f;
    constexpr auto NOTIFICATION_LONG_TIME = 4.f;

    enum class NotificationIcon {
        None,
        Loading,
        Success,
        Warning,
        Error,
        Info,
    };

    class GEODE_DLL Notification : public cocos2d::CCNodeRGBA {
        class Impl;
        std::unique_ptr<Impl> m_impl;
    protected:
        Notification();
        ~Notification();

        bool init(ZStringView text, cocos2d::CCNode* icon, float time);
        void updateLayout();

        static cocos2d::CCNode* createIcon(NotificationIcon icon);

        void showNextNotification();
        void waitThenHide();

        NineSlice* getBG();
        cocos2d::CCLabelBMFont* getLabel();
        cocos2d::CCNodeRGBA* getContent();

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
            ZStringView text,
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
            ZStringView text,
            cocos2d::CCNode* icon,
            float time = NOTIFICATION_DEFAULT_TIME
        );

        void setString(ZStringView text);
        void setIcon(NotificationIcon icon);
        void setIcon(cocos2d::CCNode* icon);
        cocos2d::CCNode* getIcon();
        void setTime(float time);

        float getTime();
        bool isShowing();

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
         * to prematurely hide the notification
         */
        void hide();

        /**
         * Cancels the showing of the notification if it's in the queue.
         * Otherwise, it hides the notification if it's currently showing.
         */
        void cancel();
    };
}

