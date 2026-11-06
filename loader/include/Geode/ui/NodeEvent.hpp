#pragma once

#include "../loader/Event.hpp"
#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/cocos/menu_nodes/CCMenuItem.h>

namespace cocos2d {
    class CCNode;
    class CCMenuItem;
}

namespace geode {

    enum class NodeEventType {
        /// Fired when a CCNode is added to the scene tree.
        OnEnter,

        /// Fired when a CCNode is removed from the scene tree.
        OnExit,

        /// Fired when a CCNode is already added to the scene tree but after a scene transition.
        OnEnterTransitionDidFinish,

        /// Fired when a CCNode is about to be removed from the scene tree before a scene transition.
        OnExitTransitionDidStart,

        /// Fired when a CCNode is cleaned up (Schedules removed and Actions stopped).
        OnCleanup,
    };

    /// Fired when a CCNode has a specific Event in NodeEventType.
    class NodeEvent : public GlobalEvent<NodeEvent, bool(), cocos2d::CCNode*, NodeEventType> {
        public:
        using GlobalEvent::GlobalEvent;
    };

    /// Fired when a CCMenuItem is Activated (after the callback).
    class MenuItemActivatedEvent : public Event<MenuItemActivatedEvent, bool(cocos2d::CCMenuItem*), cocos2d::CCMenuItem*> {
        public:
        using Event::Event;
    };
}


namespace cocos2d {
    template <class Callback>
    inline geode::comm::ListenerHandle* CCNode::addOnEnterCallback(Callback&& callback, int priority) {
        return this->addEventListener("", geode::NodeEvent(this, geode::NodeEventType::OnEnter), std::forward<Callback>(callback), priority);
    }

    template <class Callback>
    inline geode::comm::ListenerHandle* CCNode::addOnEnterTransitionDidFinishCallback(Callback&& callback, int priority) {
        return this->addEventListener("", geode::NodeEvent(this, geode::NodeEventType::OnEnterTransitionDidFinish), std::forward<Callback>(callback), priority);
    }

    template <class Callback>
    inline geode::comm::ListenerHandle* CCNode::addOnExitCallback(Callback&& callback, int priority) {
        return this->addEventListener("", geode::NodeEvent(this, geode::NodeEventType::OnExit), std::forward<Callback>(callback), priority);
    }

    template <class Callback>
    inline geode::comm::ListenerHandle* CCNode::addOnExitTransitionDidStartCallback(Callback&& callback, int priority) {
        return this->addEventListener("", geode::NodeEvent(this, geode::NodeEventType::OnExitTransitionDidStart), std::forward<Callback>(callback), priority);
    }

    template <class Callback>
    inline geode::comm::ListenerHandle* CCNode::addCleanupCallback(Callback&& callback, int priority) {
        return this->addEventListener("", geode::NodeEvent(this, geode::NodeEventType::OnCleanup), std::forward<Callback>(callback), priority);
    }

    template <class Callback>
    inline geode::comm::ListenerHandle* CCMenuItem::addActivateCallback(Callback&& callback, int priority) {
        return this->addEventListener("", geode::MenuItemActivatedEvent(this), std::forward<Callback>(callback), priority);
    }
}
