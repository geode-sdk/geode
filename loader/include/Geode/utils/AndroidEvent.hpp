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

    /**
     * An input representing some kind of key input (whether it is controller or )
     */
    class GEODE_DLL AndroidKeyInput final {
    protected:
        int m_keycode;
        int m_modifiers;
        int m_repeatCount;
        bool m_isDown;

    public:
        AndroidKeyInput(int keycode, int modifiers, bool isDown, int repeatCount);

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

    // this type is intended to be used for variant discrimination, for now. i'm lazy
    class GEODE_DLL AndroidTouchInput final {
    public:
        struct Data {
            int m_id;
            float m_x;
            float m_y;
        };

        enum class Type {
            Began = 0, Moved = 1, Ended = 2, Cancelled = 3
        };

        AndroidTouchInput(std::vector<Data> touches, Type type);

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

        std::vector<Data> packets() const;

    private:
        std::vector<Data> m_packets;
    };

    using AndroidRichInput = std::variant<AndroidJoystickInput, AndroidTouchInput, AndroidKeyInput, AndroidScrollInput>;

    class GEODE_DLL AndroidRichInputEvent final : public Event {
    protected:
        std::int64_t m_timestamp;
        int m_deviceId;
        int m_eventSource;

        AndroidRichInput m_data;

    public:
        AndroidRichInputEvent(std::int64_t timestamp, int deviceId, int eventSource, AndroidRichInput data);

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
