#include <Geode/utils/JsonValidation.hpp>

USE_GEODE_NAMESPACE();


json::Value& JsonMaybeSomething::json() {
    return m_json;
}


JsonMaybeSomething::JsonMaybeSomething(
    JsonChecker& checker, json::Value& json, std::string const& hierarchy, bool hasValue
) :
    m_checker(checker),
    m_json(json), m_hierarchy(hierarchy), m_hasValue(hasValue) {}


bool JsonMaybeSomething::isError() const {
    return m_checker.isError() || !m_hasValue;
}


std::string JsonMaybeSomething::getError() const {
    return m_checker.getError();
}


JsonMaybeSomething::operator bool() const {
    return !isError();
}


void JsonMaybeSomething::setError(std::string const& error) {
    m_checker.m_result = error;
}


JsonMaybeValue::JsonMaybeValue(
    JsonChecker& checker, json::Value& json, std::string const& hierarchy, bool hasValue
) :
    JsonMaybeSomething(checker, json, hierarchy, hasValue) {}


JsonMaybeSomething& JsonMaybeValue::self() {
    return *static_cast<JsonMaybeSomething*>(this);
}

// template<class Json>
// template<nlohmann::detail::value_t T>
// JsonMaybeValue& JsonMaybeValue::as() {
//     if (this->isError()) return *this;
//     if (!jsonConvertibleTo(self().m_json.type(), T)) {
//         this->setError(
//             self().m_hierarchy + ": Invalid type \"" +
//             self().m_json.type_name() + "\", expected \"" +
//             jsonValueTypeToString(T) + "\""
//         );
//     }
//     m_inferType = false;
//     return *this;
// }


JsonMaybeValue& JsonMaybeValue::array() {
    this->as<value_t::Array>();
    return *this;
}

// template<class Json>
// template<nlohmann::detail::value_t... T>
// JsonMaybeValue JsonMaybeValue::asOneOf() {
//     if (this->isError()) return *this;
//     bool isOneOf = (... || jsonConvertibleTo(self().m_json.type(), T));
//     if (!isOneOf) {
//         this->setError(
//             self().m_hierarchy + ": Invalid type \"" +
//             self().m_json.type_name() + "\", expected one of \"" +
//             (jsonValueTypeToString(T), ...) + "\""
//         );
//     }
//     m_inferType = false;
//     return *this;
// }

// template<class Json>
// template<nlohmann::detail::value_t T>
// JsonMaybeValue JsonMaybeValue::is() {
//     if (this->isError()) return *this;
//     self().m_hasValue = jsonConvertibleTo(self().m_json.type(), T);
//     m_inferType = false;
//     return *this;
// }

// template<class Json>
// template<class T>
// JsonMaybeValue JsonMaybeValue::validate(JsonValueValidator<T> validator) {
//     if (this->isError()) return *this;
//     try {
//         if (!validator(self().m_json.template get<T>())) {
//             this->setError(self().m_hierarchy + ": Invalid value format");
//         }
//     } catch(...) {
//         this->setError(
//             self().m_hierarchy + ": Invalid type \"" +
//             std::string(self().m_json.type_name()) + "\""
//         );
//     }
//     return *this;
// }

// template<class Json>
// template<class T>
// JsonMaybeValue JsonMaybeValue::inferType() {
//     if (this->isError() || !m_inferType) return *this;
//     return this->as<getJsonType<T>()>();
// }

// template<class Json>
// template<class T>
// JsonMaybeValue JsonMaybeValue::intoRaw(T& target) {
//     if (this->isError()) return *this;
//     target = self().m_json;
//     return *this;
// }

// template<class Json>
// template<class T>
// JsonMaybeValue JsonMaybeValue::into(T& target) {
//     return this->intoAs<T, T>(target);
// }

// template<class Json>
// template<class T>
// JsonMaybeValue JsonMaybeValue::into(std::optional<T>& target) {
//     return this->intoAs<T, std::optional<T>>(target);
// }

// template<class Json>
// template<class A, class T>
// JsonMaybeValue JsonMaybeValue::intoAs(T& target) {
//     this->inferType<A>();
//     if (this->isError()) return *this;
//     try {
//         target = self().m_json.template get<A>();
//     } catch(...) {
//         this->setError(
//             self().m_hierarchy + ": Invalid type \"" +
//             std::string(self().m_json.type_name()) + "\""
//         );
//     }
//     return *this;
// }

// template<class Json>
// template<class T>
// T JsonMaybeValue::get() {
//     this->inferType<T>();
//     if (this->isError()) return T();
//     try {
//         return self().m_json.template get<T>();
//     } catch(...) {
//         this->setError(
//             self().m_hierarchy + ": Invalid type to get \"" +
//             std::string(self().m_json.type_name()) + "\""
//         );
//     }
//     return T();
// }


JsonMaybeObject JsonMaybeValue::obj() {
    this->as<value_t::Object>();
    return JsonMaybeObject(self().m_checker, self().m_json, self().m_hierarchy, self().m_hasValue);
}

// template<class Json>
// template<class T>
// struct JsonMaybeValue::Iterator {
//     std::vector<T> m_values;

//     using iterator = typename std::vector<T>::iterator;
//     using const_iterator = typename std::vector<T>::const_iterator;

//     iterator begin() {
//         return m_values.begin();
//     }
//     iterator end() {
//         return m_values.end();
//     }

//     const_iterator begin() const {
//         return m_values.begin();
//     }
//     const_iterator end() const {
//         return m_values.end();
//     }
// };


JsonMaybeValue JsonMaybeValue::at(size_t i) {
    this->as<value_t::Array>();
    if (this->isError()) return *this;

    auto& json = self().m_json.as_array();
    if (json.size() <= i) {
        this->setError(
            self().m_hierarchy + ": has " + std::to_string(json.size()) +
            "items "
            ", expected to have at least " +
            std::to_string(i + 1)
        );
        return *this;
    }
    return JsonMaybeValue(
        self().m_checker, json.at(i), self().m_hierarchy + "." + std::to_string(i), self().m_hasValue
    );
}


typename JsonMaybeValue::template Iterator<JsonMaybeValue> JsonMaybeValue::iterate() {
    this->as<value_t::Array>();
    Iterator<JsonMaybeValue> iter;
    if (this->isError()) return iter;

    auto& json = self().m_json.as_array();
    size_t i = 0;
    for (auto& obj : json) {
        iter.m_values.emplace_back(
            self().m_checker, obj, self().m_hierarchy + "." + std::to_string(i++), self().m_hasValue
        );
    }
    return iter;
}


typename JsonMaybeValue::template Iterator<std::pair<std::string, JsonMaybeValue>> JsonMaybeValue::items() {
    this->as<value_t::Object>();
    Iterator<std::pair<std::string, JsonMaybeValue>> iter;
    if (this->isError()) return iter;

    for (auto& [k, v] : self().m_json.as_object()) {
        iter.m_values.emplace_back(
            k,
            JsonMaybeValue(self().m_checker, v, self().m_hierarchy + "." + k, self().m_hasValue)
        );
    }

    return iter;
}


JsonMaybeObject::JsonMaybeObject(
    JsonChecker& checker, json::Value& json, std::string const& hierarchy, bool hasValue
) :
    JsonMaybeSomething(checker, json, hierarchy, hasValue) {}


JsonMaybeSomething& JsonMaybeObject::self() {
    return *static_cast<JsonMaybeSomething*>(this);
}


void JsonMaybeObject::addKnownKey(std::string const& key) {
    m_knownKeys.insert(key);
}


json::Value& JsonMaybeObject::json() {
    return self().m_json;
}


JsonMaybeValue JsonMaybeObject::emptyValue() {
    return JsonMaybeValue(self().m_checker, self().m_json, "", false);
}


JsonMaybeValue JsonMaybeObject::has(std::string const& key) {
    this->addKnownKey(key);
    if (this->isError()) return emptyValue();
    if (!self().m_json.contains(key) || self().m_json[key].is_null()) {
        return emptyValue();
    }
    return JsonMaybeValue(self().m_checker, self().m_json[key], key, true);
}


JsonMaybeValue JsonMaybeObject::needs(std::string const& key) {
    this->addKnownKey(key);
    if (this->isError()) return emptyValue();
    if (!self().m_json.contains(key)) {
        this->setError(self().m_hierarchy + " is missing required key \"" + key + "\"");
        return emptyValue();
    }
    return JsonMaybeValue(self().m_checker, self().m_json[key], key, true);
}


void JsonMaybeObject::checkUnknownKeys() {
    for (auto& [key, _] : self().m_json.as_object()) {
        if (!m_knownKeys.count(key)) {
            log::warn("{} contains unknown key \"{}\"", self().m_hierarchy, key);
        }
    }
}


JsonChecker::JsonChecker(json::Value& json) : m_json(json), m_result(std::monostate()) {}


bool JsonChecker::isError() const {
    return std::holds_alternative<std::string>(m_result);
}


std::string JsonChecker::getError() const {
    return std::get<std::string>(m_result);
}


JsonMaybeValue JsonChecker::root(std::string const& hierarchy) {
    return JsonMaybeValue(*this, m_json, hierarchy, true);
}
