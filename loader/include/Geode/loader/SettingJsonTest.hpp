#include "Setting.hpp"
#include <json.hpp>

namespace geode {
    template<class T>
    bool GeodeSettingValue<T>::load(json::Value const& json) {
        try {
            m_value = json.as<ValueType>();
            return true;
        }
        catch (...) {
            return false;
        }
    }

    template<class T>
    bool GeodeSettingValue<T>::save(json::Value& json) const {
        json = m_value;
        return true;
    }
}