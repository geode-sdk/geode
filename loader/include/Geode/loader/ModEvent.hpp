#pragma once

#include "Event.hpp"
#include <optional>
#include "Mod.hpp"

namespace geode {
    enum class ModEventType {
        Load,
        Unload,
        Enable,
        Disable,
        LoadData,
        SaveData,
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

	class GEODE_DLL ModStateEventHandler : public EventHandler<ModStateEvent> {
	public:
		using Consumer = void(*)();
        
	protected:
        ModEventType m_type;
        Mod* m_mod;
		Consumer m_consumer;
	
	public:
        PassThrough handle(ModStateEvent* event) override;
		ModStateEventHandler(Mod* mod, ModEventType type, Consumer handler);
	};
}

#define $on(type) \
template<class>                                                   \
void GEODE_CONCAT(geodeExecFunction, __LINE__)();                 \
namespace {                                                       \
	struct GEODE_CONCAT(ExecFuncUnique, __LINE__) {};             \
}                                                                 \
static inline auto GEODE_CONCAT(Exec, __LINE__) = (Loader::get()->scheduleOnModLoad(\
	nullptr, []() {                                               \
        static ModStateEventHandler _(\
            Mod::get(),\
            ModEventType::type,\
            &GEODE_CONCAT(geodeExecFunction, __LINE__)<GEODE_CONCAT(ExecFuncUnique, __LINE__)>\
        );\
    }                                                             \
), 0);                                                            \
template<class>                                                   \
void GEODE_CONCAT(geodeExecFunction, __LINE__)()
