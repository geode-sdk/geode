#include "Mod.hpp"
#include "../external/json/json.hpp"

namespace geode {
    template <class T>
    T Mod::getSettingValue(std::string const& key) const {
        if (auto sett = this->getSetting(key)) {
            return SettingValueSetter<T>::get(sett);
        }
        return T();
    }

    template <class T>
    T Mod::setSettingValue(std::string const& key, T const& value) {
        if (auto sett = this->getSetting(key)) {
            auto old = this->getSettingValue<T>(sett);
            SettingValueSetter<T>::set(sett, value);
            return old;
        }
        return T();
    }

    template <class T>
    T Mod::getSavedValue(std::string const& key) {
        auto& saved = this->getSaveContainer();
        if (saved.count(key)) {
            try {
                // json -> T may fail
                return saved.at(key);
            }
            catch (...) {
            }
        }
        return T();
    }

    template <class T>
    T Mod::getSavedValue(std::string const& key, T const& defaultValue) {
        auto& saved = this->getSaveContainer();
        if (saved.count(key)) {
            try {
                // json -> T may fail
                return saved.at(key);
            }
            catch (...) {
            }
        }
        saved[key] = defaultValue;
        return defaultValue;
    }

    template<class T>
    T Mod::setSavedValue(std::string const& key, T const& value) {
        auto& saved = this->getSaveContainer();
        auto old = this->getSavedValue<T>(key);
        saved[key] = value;
        return old;
    }
}