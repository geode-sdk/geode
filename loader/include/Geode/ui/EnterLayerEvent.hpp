#pragma once

#include "../loader/Event.hpp"
#include <optional>

namespace cocos2d {
    class CCNode;
}

namespace geode {
    template<class T>
    concept InheritsCCNode = std::is_base_of_v<cocos2d::CCNode, T>;

    template <InheritsCCNode T>
    class EnterLayerEvent final : public Event<EnterLayerEvent<T>, bool(cocos2d::CCNode*), std::string> {
    public:
        // listener params layer
        // filter params targetID
        using Event<EnterLayerEvent<T>, bool(cocos2d::CCNode*), std::string>::Event;
    };

    template <InheritsCCNode T>
    class GlobalEnterLayerEvent final : public SimpleEvent<GlobalEnterLayerEvent<T>, std::string_view, cocos2d::CCNode*> {
    public:
        // listener params layerID, layer
        using SimpleEvent<GlobalEnterLayerEvent<T>, std::string_view, cocos2d::CCNode*>::SimpleEvent;
    };
}
