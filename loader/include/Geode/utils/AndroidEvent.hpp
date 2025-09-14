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

    geode::Result<int> getConnectedControllerCount();
    geode::Result<std::vector<int>> getConnectedDevices();

    /**
     * Input timestamp (in nanoseconds) sent by the launcher just before the next input event is received
     */
    class GEODE_DLL AndroidInputTimestampEvent final : public Event {
    protected:
        long m_timestamp;

    public:
        AndroidInputTimestampEvent(long timestamp);
        long getTimestamp() const;
    };

    class GEODE_DLL AndroidInputTimestampFilter final : public EventFilter<AndroidInputTimestampEvent> {
    public:
        using Callback = void(AndroidInputTimestampEvent*);

        ListenerResult handle(geode::Function<Callback>& fn, AndroidInputTimestampEvent* event);
    };

    class GEODE_DLL AndroidInputDevice final {
    private:
        int m_deviceId;
        jobject m_inputDevice{};

        AndroidInputDevice(int deviceId, jobject inputDevice);

    public:
        static Result<AndroidInputDevice> create(int deviceId);

        AndroidInputDevice(const AndroidInputDevice&) = delete;
        AndroidInputDevice& operator=(const AndroidInputDevice&) = delete;

        AndroidInputDevice(AndroidInputDevice&&);
        AndroidInputDevice& operator=(AndroidInputDevice&&);

        ~AndroidInputDevice();

        std::string getDescriptor();

        std::string getName();
        int getVendorId();
        int getProductId();

        float getBatteryCapacity();
        int getBatteryStatus();
        bool hasBattery();

        int getSources();

        enum class LightType {
            None = 0,
            PlayerNumber = 1,
            Color = 2,
            All = 3
        };

        int getLightCount();
        LightType getLightType();
        Result<> setLights(LightType type, std::uint32_t color);

        int getMotorCount();
        Result<> vibrateDevice(long durationMs, int intensity, int motorIdx = -1);

        int getDeviceId() const;
    };

    class GEODE_DLL AndroidInputDeviceInfoEvent final : public Event {
    protected:
        int m_deviceId;
        int m_eventSource;

    public:
        AndroidInputDeviceInfoEvent(int deviceId, int eventSource);

        int deviceId() const;
        int eventSource() const;
    };

    class GEODE_DLL AndroidInputDeviceInfoFilter final : public EventFilter<AndroidInputDeviceInfoEvent> {
    public:
        using Callback = void(AndroidInputDeviceInfoEvent*);

        ListenerResult handle(std::function<Callback> fn, AndroidInputDeviceInfoEvent* event);
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

        ListenerResult handle(std::function<Callback> fn, AndroidInputDeviceEvent* event);
    };

    class GEODE_DLL AndroidInputJoystickEvent final : public Event {
    protected:
        std::vector<float> m_leftX;
        std::vector<float> m_leftY;

        std::vector<float> m_rightX;
        std::vector<float> m_rightY;

        std::vector<float> m_hatX;
        std::vector<float> m_hatY;

        std::vector<float> m_leftTrigger;
        std::vector<float> m_rightTrigger;

    public:
        AndroidInputJoystickEvent(std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>);

        std::vector<float> leftX() const;
        std::vector<float> leftY() const;

        std::vector<float> rightX() const;
        std::vector<float> rightY() const;

        std::vector<float> hatX() const;
        std::vector<float> hatY() const;

        std::vector<float> leftTrigger() const;
        std::vector<float> rightTrigger() const;
    };

    class GEODE_DLL AndroidInputJoystickFilter final : public EventFilter<AndroidInputJoystickEvent> {
    public:
        using Callback = void(AndroidInputJoystickEvent*);

        ListenerResult handle(std::function<Callback> fn, AndroidInputJoystickEvent* event);
    };
}

#endif
