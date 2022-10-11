#pragma once

#include "Event.hpp"
#include <optional>
#include "Setting.hpp"
#include "Loader.hpp"

namespace geode {
    class GEODE_DLL SettingChangedEvent : public Event {
    protected:
        std::string m_modID;
        std::shared_ptr<Setting> m_setting;
    
    public:
        SettingChangedEvent(
            std::string const& modID,
            std::shared_ptr<Setting> setting
        );
        std::string getModID() const;
        std::shared_ptr<Setting> getSetting() const;
    };

    template<class T>
	class SettingChangedEventHandler : public EventHandler<SettingChangedEvent> {
	public:
		using Consumer = void(*)(std::shared_ptr<T>);
        
        static_assert(
            std::is_base_of_v<Setting, T>,
            "Setting must inherit from the Setting class"
        );

	protected:
		Consumer m_consumer;
		std::string m_modID;
		std::optional<std::string> m_targetKey;
	
	public:
        PassThrough handle(SettingChangedEvent* event) override {
            if (
                m_modID == event->getModID() && (
                !m_targetKey ||
                m_targetKey.value() == event->getSetting()->getKey()
            )) {
                m_consumer(std::static_pointer_cast<T>(event->getSetting()));
            }
			return PassThrough::Propagate;
		}

        /**
         * Listen to changes on a specific setting
         */
		SettingChangedEventHandler(
			std::string const& modID,
			std::string const& settingID,
			Consumer handler
		) : m_modID(modID),
            m_targetKey(settingID),
            m_consumer(handler) {}

        /**
         * Listen to changes on all of a mods' settings
         */
		SettingChangedEventHandler(
			std::string const& modID,
			Consumer handler
		) : m_modID(modID),
            m_targetKey(std::nullopt),
            m_consumer(handler) {}
	};

    template<class T>
        requires std::is_base_of_v<Setting, T>
    std::monostate listenForSettingChanges(
        std::string const& settingID,
        void(*callback)(std::shared_ptr<T>)
    ) {
        Loader::get()->scheduleOnModLoad(getMod(), [=]() {
            static SettingChangedEventHandler<T> _(
                getMod()->getID(), settingID, callback
            );
        });
        return std::monostate();
    }

    static std::monostate listenForAllSettingChanges(
        void(*callback)(std::shared_ptr<Setting>)
    ) {
        Loader::get()->scheduleOnModLoad(getMod(), [=]() {
            static SettingChangedEventHandler<Setting> _(
                getMod()->getID(), callback
            );
        });
        return std::monostate();
    }
}

