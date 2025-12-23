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
    class GEODE_DLL ModStateEvent final : public Event {
    protected:
        ModEventType m_type;
        Mod* m_mod;

    public:
        ModStateEvent(Mod* mod, ModEventType type);
        ModEventType getType() const;
        Mod* getMod() const;
        bool filter(ModEventType type, Mod* mod) const;
    };

    /**
     * Listener for mod load/enable/disable/unload/data save events
     */
    class GEODE_DLL ModStateFilter final : public EventFilter<ModStateEvent> {
    public:
        using Callback = void(ModStateEvent*);

    protected:
        ModEventType m_type;
        Mod* m_mod;

    public:
        ListenerResult handle(std::function<Callback> fn, ModStateEvent* event);

        /**
         * Create a mod state listener
         * @param mod The mod whose events to listen to, or nullptr to listen to
         * all mods' all state events
         * @param type Type of event to listen to. Ignored if mod is nullptr
         */
        ModStateFilter(Mod* mod, ModEventType type);
        ModStateFilter(ModStateFilter const&) = default;
    };

    /**
     * Event posted to a mod when another mod that depends on it is loaded
     */
    class GEODE_DLL DependencyLoadedEvent final : public Event {
    protected:
        class Impl;
        std::unique_ptr<Impl> m_impl;

    public:
        DependencyLoadedEvent(Mod* target, Mod* dependency);
        virtual ~DependencyLoadedEvent();

        Mod* getTarget() const;
        Mod* getDependency() const;
        matjson::Value getDependencySettings() const;

        bool filter(Mod* target) const;
    };

    /**
     * Listen for in a mod when a mod that depends on it is loaded
     */
    class GEODE_DLL DependencyLoadedFilter final : public EventFilter<DependencyLoadedEvent> {
    public:
        using Callback = void(DependencyLoadedEvent*);

    protected:
        class Impl;
        std::unique_ptr<Impl> m_impl;

    public:
        ListenerResult handle(std::function<Callback> fn, DependencyLoadedEvent* event);

        DependencyLoadedFilter(Mod* target = geode::getMod());
        DependencyLoadedFilter(DependencyLoadedFilter&&);
        DependencyLoadedFilter(DependencyLoadedFilter const&);
        DependencyLoadedFilter& operator=(DependencyLoadedFilter const&);
        DependencyLoadedFilter& operator=(DependencyLoadedFilter&&);
        ~DependencyLoadedFilter();
    };
}

// clang-format off
#define $on_mod(type) \
template<class>                                                        \
void GEODE_CONCAT(geodeExecFunction, __LINE__)(geode::ModStateEvent*); \
namespace {                                                            \
	struct GEODE_CONCAT(ExecFuncUnique, __LINE__) {};                  \
}                                                                      \
static inline auto GEODE_CONCAT(Exec, __LINE__) = (new geode::EventListener(  \
            &GEODE_CONCAT(geodeExecFunction, __LINE__)<GEODE_CONCAT(ExecFuncUnique, __LINE__)>, \
            geode::ModStateFilter(geode::getMod(), geode::ModEventType::type) \
        ), 0);                                                         \
template<class>                                                        \
void GEODE_CONCAT(geodeExecFunction, __LINE__)(geode::ModStateEvent*)
// clang-format on
