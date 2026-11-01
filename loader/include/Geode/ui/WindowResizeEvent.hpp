#pragma once
#include "../loader/Event.hpp"
#include <Geode/cocos/cocoa/CCGeometry.h>

namespace geode {
    class WindowResizeEvent final : public Event<WindowResizeEvent, bool(const cocos2d::CCSize&)> {
        using Event::Event;
    };
}
