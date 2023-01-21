#pragma once

#include "Event.hpp"

#include <optional>

namespace geode {
    class Mod;
    Mod* getMod();

    enum class ModEventType {
        Loaded,
        Unloaded,
        Enabled,
        Disabled,
        DataLoaded,
        DataSaved,
    };

    class GEODE_DLL ModStateEvent : public Event {
    protected:
        ModEventType m_type;
        Mod* m_mod;

    public:
        ModStateEvent(Mod* mod, ModEventType type);
        ModEventType getType() const;
        Mod* getMod() const;
    };

    class GEODE_DLL ModStateFilter : public EventFilter<ModStateEvent> {
    public:
        using Callback = void(ModStateEvent*);

    protected:
        ModEventType m_type;
        Mod* m_mod;

    public:
        ListenerResult handle(std::function<Callback> fn, ModStateEvent* event);
        ModStateFilter(Mod* mod, ModEventType type);
    };
}

// clang-format off
#define $on_mod(type) \
template<class>                                                     \
void GEODE_CONCAT(geodeExecFunction, __LINE__)(ModStateEvent*);     \
namespace {                                                         \
	struct GEODE_CONCAT(ExecFuncUnique, __LINE__) {};               \
}                                                                   \
static inline auto GEODE_CONCAT(Exec, __LINE__) = (new geode::EventListener(                                     \
            &GEODE_CONCAT(geodeExecFunction, __LINE__)<GEODE_CONCAT(ExecFuncUnique, __LINE__)>,\
            geode::ModStateFilter(geode::getMod(), geode::ModEventType::type)\
        ), 0);                                                              \
template<class>                                                     \
void GEODE_CONCAT(geodeExecFunction, __LINE__)(ModStateEvent*)
// clang-format on
