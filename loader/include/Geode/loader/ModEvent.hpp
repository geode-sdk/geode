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
    class GEODE_DLL ModStateEvent final : public GlobalEvent<ModStateEvent, bool(), ModEventType, Mod*> {
    public:
        // filter params type, targetMod
        using GlobalEvent::GlobalEvent;
    };

    /**
     * Event posted to a mod when another mod that depends on it is loaded
     */
    class GEODE_DLL DependencyLoadedEvent final : public Event<DependencyLoadedEvent, bool(Mod*), Mod*> {
    public:
        // listener params dependency
        // filter params target
        using Event::Event;
    };
}

// clang-format off
#define $on_mod(type) \
template<class>                                                        \
void GEODE_CONCAT(geodeExecFunction, __LINE__)();                      \
namespace {                                                            \
	struct GEODE_CONCAT(ExecFuncUnique, __LINE__) {};                  \
}                                                                      \
static inline auto GEODE_CONCAT(Exec, __LINE__) =                      \
    geode::ModStateEvent(geode::ModEventType::type, geode::getMod())   \
    .listen(&GEODE_CONCAT(geodeExecFunction, __LINE__)                 \
        <GEODE_CONCAT(ExecFuncUnique, __LINE__)>).leak();              \
template<class>                                                        \
void GEODE_CONCAT(geodeExecFunction, __LINE__)()
// clang-format on
