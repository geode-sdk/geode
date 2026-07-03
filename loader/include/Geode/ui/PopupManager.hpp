#pragma once

#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/ZStringView.hpp>
#include <Geode/ui/Notification.hpp>

#include <deque>

namespace geode {

class [[nodiscard("call showQueue or showInstant to show the popup")]] GEODE_DLL ManagedPopup {
    friend class PopupManager;
    struct Data;

public:
    ManagedPopup(FLAlertLayer* layer);
    ManagedPopup();

    ManagedPopup(const ManagedPopup& other) = default;
    ManagedPopup& operator=(const ManagedPopup& other) = default;
    ManagedPopup(ManagedPopup&& other) noexcept = default;
    ManagedPopup& operator=(ManagedPopup&& other) noexcept = default;

    operator FLAlertLayer*() const;
    FLAlertLayer* operator->() const;
    FLAlertLayer* getInner() const;

    // Set whether the popup should follow the user if they transition to another scene,
    // instead of disappearing. By default is disabled.
    void setPersistent(bool state = true);

    // Makes the popup prioritized, ensuring that it will be put in front of the queue,
    // and shown even if the user is playing a level (the level will be paused).
    // Only effective if `showQueue()` is used, if `showInstant()` is used the popup already shows immediately.
    void setPriority(bool state = true);

    // Makes it impossible to accidentally close the popup (via esc or back button)
    // for a given period of time after it is shown.
    void blockClosingFor(const asp::time::Duration& dur);

    // Makes it impossible to accidentally close the popup (via esc or back button)
    // for a given period of time (in seconds) after it is shown.
    void blockClosingFor(float dur);

    // Shows the popup to the user immediately, does nothing if already shown.
    void showInstant();

    // Queues the popup to be shown to the user when it's appropriate,
    // e.g. not while they're in a level and unpaused or in the middle of a transition
    // This should be the preferred way of showing a popup, and will work as intended when inside e.g. an `init` method of a layer.
    void showQueue();

    bool isShown();

    bool shouldPreventClosing();

private:
    geode::Ref<FLAlertLayer> inner;

    Data& getFields();
    bool hasFields() const;
    void doShow(bool reshowing = false);
};

class GEODE_DLL PopupManager final : public cocos2d::CCObject {
    friend class SingletonNodeBase;
    friend class ManagedPopup;
    PopupManager();

public:
    constexpr static float DEFAULT_WIDTH = 370.f;

    PopupManager(const PopupManager&) = delete;
    PopupManager& operator=(const PopupManager&) = delete;
    PopupManager(PopupManager&&) = delete;
    PopupManager& operator=(PopupManager&&) = delete;

    static PopupManager& get();

    // Creates a popup with the given title and content (optionally button 1, 2 and width). Does not show the popup to the user.
    ManagedPopup alert(
        ZStringView title,
        const std::string& content,
        ZStringView btn1 = "Ok",
        ZStringView btn2 = nullptr,
        float width = DEFAULT_WIDTH
    );

    // Creates a popup with the given title and content (optionally button 1, 2 and width). Does not show the popup to the user.
    // The callback is involved when the user presses either of the buttons in the popup.
    ManagedPopup quickPopup(
        ZStringView title,
        const std::string& content,
        ZStringView btn1 = "Ok",
        ZStringView btn2 = nullptr,
        geode::Function<void (FLAlertLayer*, bool)> callback = {},
        float width = DEFAULT_WIDTH
    );

    // Creates a popup with the given title and content as a formatted string. Does not show the popup to the user.
    template <class... Args>
    ManagedPopup alertFormat(
        ZStringView title,
        fmt::format_string<Args...> fmt,
        Args&&... args
    ) {
        return alert(title, fmt::format(fmt, std::forward<Args>(args)...));
    }

    // Create a ManagedPopup for this custom popup that can be used to manage it.
    // Don't call this if the popup has already been shown.
    ManagedPopup manage(FLAlertLayer* alert);

    bool isManaged(FLAlertLayer* alert);

    // Returns whether there are currently any queued popups that can't be shown,
    // either because the player is transitioning or in a level and unpaused.
    bool hasPendingPopups() const;

private:
    cocos2d::CCScene* m_prevScene = nullptr;
    bool m_isTransitioning = false;
    std::vector<Ref<FLAlertLayer>> m_savedAlerts;
    std::deque<ManagedPopup> m_queuedPopups;

    void changedScene(cocos2d::CCScene* newScene);
    void queuePopup(const ManagedPopup& popup, bool back = true);
    void update(float dt);
};

}
