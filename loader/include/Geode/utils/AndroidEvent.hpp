#pragma once

#include "../platform/cplatform.h"

// this is exclusively for android users if you're not android please look away
#ifdef GEODE_IS_ANDROID

#include "../loader/Event.hpp"
#include <Geode/cocos/platform/android/jni/JniHelper.h>

namespace geode {
    namespace utils {
        /** Gets the current version of the launcher. Returns an error if the method was not found. */
        geode::Result<int> getLauncherVersion();
    }

    class GEODE_DLL AndroidKeyInput final {
    protected:
        int m_keycode;
        int m_modifiers;
        int m_repeatCount;
        bool m_isDown;

    public:
        AndroidKeyInput(int keycode, int modifiers, bool isDown, int repeatCount);

        /** Returns the Android keycode for the KeyEvent */
        int keycode() const;
        bool isDown() const;
        int repeatCount() const;
        int modifiers() const;
    };

    class GEODE_DLL AndroidScrollInput final {
    protected:
        float m_scrollX;
        float m_scrollY;

    public:
        AndroidScrollInput(float scrollX, float scrollY);

        float scrollX() const;
        float scrollY() const;
    };

    class GEODE_DLL AndroidTouchInput final {
    public:
        struct Data final {
            int id;
            float x;
            float y;
        };

        enum class Type {
            Began = 0, Moved = 1, Ended = 2, Cancelled = 3
        };

        AndroidTouchInput(std::vector<Data> touches, Type type);

        /** Touch information for all pointers. For most events, only the first pointer is considered. */
        std::vector<Data> touches() const;
        Type type() const;

    private:
        std::vector<Data> m_touches;
        Type m_type;
    };

    class GEODE_DLL AndroidJoystickInput final {
    public:
        struct Data final {
            float leftX;
            float leftY;

            float rightX;
            float rightY;

            float hatX;
            float hatY;

            float leftTrigger;
            float rightTrigger;
        };

        AndroidJoystickInput(std::vector<Data> packets);

        /** Batched joystick data for the current frame, where the last item in the vector is the current position of each analog input. */
        std::vector<Data> packets() const;

    private:
        std::vector<Data> m_packets;
    };

    using AndroidRichInput = std::variant<AndroidJoystickInput, AndroidTouchInput, AndroidKeyInput, AndroidScrollInput>;

    /**
     * Event that represents extended information for all input events received by the game.
     * The data() member allows for determining the type of event and additional information.
     *
     * If an event is stopped by a listener, the corresponding event within Cocos will not be triggered.
     */
    class GEODE_DLL AndroidRichInputEvent final : public Event {
    protected:
        std::int64_t m_timestamp;
        int m_deviceId;
        int m_eventSource;

        AndroidRichInput m_data;

    public:
        AndroidRichInputEvent(std::int64_t timestamp, int deviceId, int eventSource, AndroidRichInput data);

        /** Timestamp (in NS) of the event. */
        std::int64_t timestamp() const;
        int deviceId() const;
        int eventSource() const;
        AndroidRichInput data() const;
    };

    class GEODE_DLL AndroidRichInputFilter final : public EventFilter<AndroidRichInputEvent> {
    public:
        using Callback = void(AndroidRichInputEvent*);

        ListenerResult handle(geode::Function<Callback>& fn, AndroidRichInputEvent* event);
    };

    /** Event that represents some change in input devices, namely removal/addition. */
    class GEODE_DLL AndroidInputDeviceEvent final : public Event {
    public:
        enum class Status {
            Added, Changed, Removed
        };

    protected:
        int m_deviceId;
        Status m_status;

    public:
        AndroidInputDeviceEvent(int deviceId, Status status);

        int deviceId() const;
        Status status() const;
    };

    class GEODE_DLL AndroidInputDeviceFilter final : public EventFilter<AndroidInputDeviceEvent> {
    public:
        using Callback = void(AndroidInputDeviceEvent*);

        ListenerResult handle(geode::Function<Callback>& fn, AndroidInputDeviceEvent* event);
    };
}

#endif
