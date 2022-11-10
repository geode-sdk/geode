#pragma once

#include "Event.hpp"
#include "Loader.hpp"
#include "Setting.hpp"

#include <optional>

namespace geode {
    class GEODE_DLL SettingChangedEvent : public Event {
    protected:
        std::string m_modID;
        std::shared_ptr<Setting> m_setting;

    public:
        SettingChangedEvent(std::string const& modID, std::shared_ptr<Setting> setting);
        std::string getModID() const;
        std::shared_ptr<Setting> getSetting() const;
    };

    template <typename T = Setting, typename = std::enable_if_t<std::is_base_of_v<Setting, T>>>
    class SettingChangedFilter : public EventFilter<SettingChangedEvent> {
    public:
        using Callback = void(std::shared_ptr<T>);
        using Event = SettingChangedEvent;

        ListenerResult handle(std::function<Callback> fn, SettingChangedEvent* event) {
            if (m_modID == event->getModID() &&
                (!m_targetKey || m_targetKey.value() == event->getSetting()->getKey())) {
                fn(std::static_pointer_cast<T>(event->getSetting()));
            }
            return ListenerResult::Propagate;
        }

        /**
         * Listen to changes on a specific setting
         */
        SettingChangedFilter(
            std::string const& modID, std::string const& settingID
        ) :
            m_modID(modID),
            m_targetKey(settingID) {}

        /**
         * Listen to changes on all of a mods' settings
         */
        SettingChangedFilter(std::string const& modID) :
            m_modID(modID), m_targetKey(std::nullopt) {}
    protected:
        std::string m_modID;
        std::optional<std::string> m_targetKey;
    };

    template <class T>

    requires std::is_base_of_v<Setting, T> std::monostate listenForSettingChanges(
        std::string const& settingID, void (*callback)(std::shared_ptr<T>)
    ) {
        Loader::get()->scheduleOnModLoad(getMod(), [=]() {
            static auto _ = EventListener(callback, SettingChangedFilter<T>(getMod()->getID(), settingID));
        });
        return std::monostate();
    }

    static std::monostate listenForAllSettingChanges(void (*callback)(std::shared_ptr<Setting>)) {
        Loader::get()->scheduleOnModLoad(getMod(), [=]() {
            static auto _ = EventListener(callback, SettingChangedFilter(getMod()->getID()));
        });
        return std::monostate();
    }
}
