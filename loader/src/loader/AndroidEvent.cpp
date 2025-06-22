#include <Geode/loader/AndroidEvent.hpp>

#ifdef GEODE_IS_ANDROID

using namespace geode::prelude;

AndroidInputTimestampEvent::AndroidInputTimestampEvent(long timestamp) : m_timestamp(timestamp) {}

long AndroidInputTimestampEvent::getTimestamp() const { return m_timestamp; }

ListenerResult AndroidInputTimestampFilter::handle(std::function<Callback> fn, AndroidInputTimestampEvent* event)  {
    fn(event);
    return ListenerResult::Propagate;
}

#endif
