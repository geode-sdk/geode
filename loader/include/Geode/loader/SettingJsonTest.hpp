#include "Setting.hpp"
#include <matjson.hpp>

namespace geode {
    template<class T>
    bool GeodeSettingValue<T>::load(matjson::Value const& json) {
        try {
            m_value = json.as<ValueType>();
            return true;
        }
        catch (...) {
            return false;
        }
    }

    template<class T>
    bool GeodeSettingValue<T>::save(matjson::Value& json) const {
        json = m_value;
        return true;
    }
}