#include <Geode/utils/JsonValidation.hpp>

USE_GEODE_NAMESPACE();

void JsonMaybeSomething::setError(std::string const& error) {
    m_checker.m_result = error;
}

bool JsonMaybeSomething::isError() const {
    return m_checker.isError() || !m_hasValue;
}

JsonMaybeObject JsonMaybeValue::obj() {
    this->as<value_t::object>();
    return JsonMaybeObject(m_checker, m_json, m_hierarchy, m_hasValue);
}
