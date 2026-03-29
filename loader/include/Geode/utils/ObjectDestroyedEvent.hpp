#pragma once

#include "../loader/Event.hpp"

namespace geode {

    /// Fired when any CCObject is destroyed.
    class ObjectDestroyedEvent : public GlobalEvent<ObjectDestroyedEvent, bool(), cocos2d::CCObject*> {
        public:
        using GlobalEvent::GlobalEvent;
    };
}