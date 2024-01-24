#include "Setting.hpp"
#include <matjson.hpp>

namespace geode {
    template<class T>
    bool GeodeSettingValue<T>::load(matjson::Value const& json) {
        if (!json.is<ValueType>()) return false;
        m_value = json.as<ValueType>();
        return true;
    }

    template<class T>
    bool GeodeSettingValue<T>::save(matjson::Value& json) const {
        json = m_value;
        return true;
    }
}