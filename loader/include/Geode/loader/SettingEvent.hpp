#pragma once

#include "Event.hpp"
#include "Loader.hpp"
#include "Setting.hpp"
#include "Mod.hpp"
#include "SettingV3.hpp"
#include <optional>

namespace geode {
    struct GEODE_DLL [[deprecated("Use SettingChangedEventV3 from SettingV3.hpp instead")]] SettingChangedEvent : public Event {
        Mod* mod;
        SettingValue* value;

        SettingChangedEvent(Mod* mod, SettingValue* value);
    };

    class GEODE_DLL [[deprecated("Use SettingChangedEventV3 from SettingV3.hpp instead")]] SettingChangedFilter : public EventFilter<SettingChangedEvent> {
    protected:
        std::string m_modID;
        std::optional<std::string> m_targetKey;

    public:
        using Callback = void(SettingValue*);

        ListenerResult handle(utils::MiniFunction<Callback> fn, SettingChangedEvent* event);
        /**
         * Listen to changes on a setting, or all settings
         * @param modID Mod whose settings to listen to
         * @param settingID Setting to listen to, or all settings if nullopt
         */
        SettingChangedFilter(
            std::string const& modID,
            std::optional<std::string> const& settingKey
        );
        SettingChangedFilter(SettingChangedFilter const&) = default;
    };

    /**
     * Listen for built-in setting changes
     */
    template<class T>
    class [[deprecated("Use SettingChangedEventV3 from SettingV3.hpp instead")]] GeodeSettingChangedFilter : public SettingChangedFilter {
    public:
        using Callback = void(T);

        ListenerResult handle(utils::MiniFunction<Callback> fn, SettingChangedEvent* event) {
            if (
                m_modID == event->mod->getID() &&
                (!m_targetKey || m_targetKey.value() == event->value->getKey())
            ) {
                fn(SettingValueSetter<T>::get(event->value));
            }
            return ListenerResult::Propagate;
        }

        GeodeSettingChangedFilter(
            std::string const& modID,
            std::string const& settingID
        ) : SettingChangedFilter(modID, settingID) {}
        GeodeSettingChangedFilter(GeodeSettingChangedFilter const&) = default;
    };
}
