#include "Setting.hpp"
#include "../external/json/json.hpp"

namespace geode {
    template<class T>
    bool GeodeSettingValue<T>::load(nlohmann::json const& json) override {
        try {
            m_value = json.get<ValueType>();
            return true;
        }
        catch (...) {
            return false;
        }
    }

    template<class T>
    bool GeodeSettingValue<T>::save(nlohmann::json& json) const {
        json = m_value;
        return true;
    }
}