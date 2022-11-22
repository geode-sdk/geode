#pragma once

#include "Event.hpp"
#include "Loader.hpp"
#include "Setting.hpp"

#include <optional>

namespace geode {
    class GEODE_DLL SettingChangedEvent : public Event {
    protected:
        std::string m_modID;
        Setting* m_setting;

    public:
        SettingChangedEvent(std::string const& modID, Setting* setting);
        std::string getModID() const;
        Setting* getSetting() const;
    };

    template <typename T = Setting, typename = std::enable_if_t<std::is_base_of_v<Setting, T>>>
    class SettingChangedFilter : public EventFilter<SettingChangedEvent> {
    public:
        using Callback = void(T*);
        using Event = SettingChangedEvent;

        ListenerResult handle(std::function<Callback> fn, SettingChangedEvent* event) {
            if (m_modID == event->getModID() &&
                (!m_targetKey || m_targetKey.value() == event->getSetting()->getKey())) {
                fn(static_cast<T*>(event->getSetting()));
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
    requires std::is_base_of_v<Setting, T>
    std::monostate listenForSettingChanges(
        std::string const& settingID, void (*callback)(T*)
    ) {
        Loader::get()->scheduleOnModLoad(getMod(), [=]() {
            new EventListener(
                callback, SettingChangedFilter<T>(getMod()->getID(), settingID)
            );
        });
        return std::monostate();
    }

    static std::monostate listenForAllSettingChanges(void (*callback)(Setting*)) {
        Loader::get()->scheduleOnModLoad(getMod(), [=]() {
            new EventListener(
                callback, SettingChangedFilter(getMod()->getID())
            );
        });
        return std::monostate();
    }
}
