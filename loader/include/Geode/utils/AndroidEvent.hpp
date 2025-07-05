#pragma once

#include "../platform/cplatform.h"

// this is exclusively for android users if you're not android please look away
#ifdef GEODE_IS_ANDROID

#include "../loader/Event.hpp"

namespace geode {
    namespace utils {
        /** Gets the current version of the launcher. Returns an error if the method was not found. */
        geode::Result<int> getLauncherVersion();
    }

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

        ListenerResult handle(std::function<Callback> fn, AndroidInputTimestampEvent* event);
    };
}

#endif
