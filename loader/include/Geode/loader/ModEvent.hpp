#pragma once

#include "Event.hpp"
#include <matjson.hpp>
#include <optional>

namespace geode {
    class Mod;
    Mod* getMod();

    enum class ModEventType {
        Loaded,
        DataLoaded,
        DataSaved,
    };

    /**
     * Event that is fired when a mod is loaded / unloaded / enabled / disabled
     */
    class ModStateEvent final : public GlobalEvent<ModStateEvent, bool(), ModEventType, Mod*> {
    public:
        // filter params type, targetMod
        using GlobalEvent::GlobalEvent;
    };

    /**
     * Event posted to a mod when another mod that depends on it is loaded
     */
    class DependencyLoadedEvent final : public Event<DependencyLoadedEvent, bool(Mod*), Mod*> {
    public:
        // listener params dependency
        // filter params target
        using Event::Event;
    };
}

#define $on_mod(type) $execute_base({                                  \
    geode::ModStateEvent(geode::ModEventType::type, geode::getMod())   \
    .listen(&GEODE_CONCAT(geodeExecFunctionI, __LINE__)).leak();       \
})
