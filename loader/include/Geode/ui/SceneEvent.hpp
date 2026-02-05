#pragma once

#include "../loader/Event.hpp"

namespace cocos2d {
    class CCScene;
}

namespace geode {
    class SceneEvent final : public Event<SceneEvent, bool(cocos2d::CCScene*)> {
    public:
        using Event::Event;
    };
}
