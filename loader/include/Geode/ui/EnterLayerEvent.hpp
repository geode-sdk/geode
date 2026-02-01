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
    class EnterLayerEvent final : public GlobalEvent<EnterLayerEvent<T>, bool(std::string_view, cocos2d::CCNode*), bool(cocos2d::CCNode*), std::string> {
    public:
        // listener params layer
        // filter params targetID
        using GlobalEvent<EnterLayerEvent<T>, bool(std::string_view, cocos2d::CCNode*), bool(cocos2d::CCNode*), std::string>::GlobalEvent;
    };
}
