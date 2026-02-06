#pragma once

#include "../loader/Event.hpp"

namespace cocos2d {
    class CCScene;
}

namespace geode {
    /// Triggered before the scene is actually switched
    class SceneEvent final : public Event<SceneEvent, bool(cocos2d::CCScene*)> {
    public:
        using Event::Event;
    };
}
