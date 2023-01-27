#include <Geode/utils/JsonValidation.hpp>

USE_GEODE_NAMESPACE();

template <class Json>
Json& JsonMaybeSomething<Json>::json() {
    return m_json;
}

template <class Json>
JsonMaybeSomething<Json>::JsonMaybeSomething(
    JsonChecker<Json>& checker, Json& json, std::string const& hierarchy, bool hasValue
) :
    m_checker(checker),
    m_json(json), m_hierarchy(hierarchy), m_hasValue(hasValue) {}

template <class Json>
bool JsonMaybeSomething<Json>::isError() const {
    return m_checker.isError() || !m_hasValue;
}

template <class Json>
std::string JsonMaybeSomething<Json>::getError() const {
    return m_checker.getError();
}

template <class Json>
JsonMaybeSomething<Json>::operator bool() const {
    return !isError();
}

template <class Json>
void JsonMaybeSomething<Json>::setError(std::string const& error) {
    m_checker.m_result = error;
}

template <class Json>
JsonMaybeValue<Json>::JsonMaybeValue(
    JsonChecker<Json>& checker, Json& json, std::string const& hierarchy, bool hasValue
) :
    JsonMaybeSomething<Json>(checker, json, hierarchy, hasValue) {}

template <class Json>
JsonMaybeSomething<Json>& JsonMaybeValue<Json>::self() {
    return *static_cast<JsonMaybeSomething<Json>*>(this);
}

// template<class Json>
// template<nlohmann::detail::value_t T>
// JsonMaybeValue<Json>& JsonMaybeValue<Json>::as() {
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

template <class Json>
JsonMaybeValue<Json>& JsonMaybeValue<Json>::array() {
    this->as<value_t::Array>();
    return *this;
}

// template<class Json>
// template<nlohmann::detail::value_t... T>
// JsonMaybeValue<Json> JsonMaybeValue<Json>::asOneOf() {
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
// JsonMaybeValue<Json> JsonMaybeValue<Json>::is() {
//     if (this->isError()) return *this;
//     self().m_hasValue = jsonConvertibleTo(self().m_json.type(), T);
//     m_inferType = false;
//     return *this;
// }

// template<class Json>
// template<class T>
// JsonMaybeValue<Json> JsonMaybeValue<Json>::validate(JsonValueValidator<T> validator) {
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
// JsonMaybeValue<Json> JsonMaybeValue<Json>::inferType() {
//     if (this->isError() || !m_inferType) return *this;
//     return this->as<getJsonType<T>()>();
// }

// template<class Json>
// template<class T>
// JsonMaybeValue<Json> JsonMaybeValue<Json>::intoRaw(T& target) {
//     if (this->isError()) return *this;
//     target = self().m_json;
//     return *this;
// }

// template<class Json>
// template<class T>
// JsonMaybeValue<Json> JsonMaybeValue<Json>::into(T& target) {
//     return this->intoAs<T, T>(target);
// }

// template<class Json>
// template<class T>
// JsonMaybeValue<Json> JsonMaybeValue<Json>::into(std::optional<T>& target) {
//     return this->intoAs<T, std::optional<T>>(target);
// }

// template<class Json>
// template<class A, class T>
// JsonMaybeValue<Json> JsonMaybeValue<Json>::intoAs(T& target) {
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
// T JsonMaybeValue<Json>::get() {
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

template <class Json>
JsonMaybeObject<Json> JsonMaybeValue<Json>::obj() {
    this->as<value_t::Object>();
    return JsonMaybeObject(self().m_checker, self().m_json, self().m_hierarchy, self().m_hasValue);
}

// template<class Json>
// template<class T>
// struct JsonMaybeValue<Json>::Iterator {
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

template <class Json>
JsonMaybeValue<Json> JsonMaybeValue<Json>::at(size_t i) {
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
    return JsonMaybeValue<Json>(
        self().m_checker, json.at(i), self().m_hierarchy + "." + std::to_string(i), self().m_hasValue
    );
}

template <class Json>
typename JsonMaybeValue<Json>::template Iterator<JsonMaybeValue<Json>> JsonMaybeValue<Json>::iterate() {
    this->as<value_t::Array>();
    Iterator<JsonMaybeValue<Json>> iter;
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

template <class Json>
typename JsonMaybeValue<Json>::template Iterator<std::pair<std::string, JsonMaybeValue<Json>>> JsonMaybeValue<
    Json>::items() {
    this->as<value_t::Object>();
    Iterator<std::pair<std::string, JsonMaybeValue<Json>>> iter;
    if (this->isError()) return iter;

    for (auto& [k, v] : self().m_json.as_object()) {
        iter.m_values.emplace_back(
            k,
            JsonMaybeValue<Json>(self().m_checker, v, self().m_hierarchy + "." + k, self().m_hasValue)
        );
    }

    return iter;
}

template <class Json>
JsonMaybeObject<Json>::JsonMaybeObject(
    JsonChecker<Json>& checker, Json& json, std::string const& hierarchy, bool hasValue
) :
    JsonMaybeSomething<Json>(checker, json, hierarchy, hasValue) {}

template <class Json>
JsonMaybeSomething<Json>& JsonMaybeObject<Json>::self() {
    return *static_cast<JsonMaybeSomething<Json>*>(this);
}

template <class Json>
void JsonMaybeObject<Json>::addKnownKey(std::string const& key) {
    m_knownKeys.insert(key);
}

template <class Json>
Json& JsonMaybeObject<Json>::json() {
    return self().m_json;
}

template <class Json>
JsonMaybeValue<Json> JsonMaybeObject<Json>::emptyValue() {
    return JsonMaybeValue(self().m_checker, self().m_json, "", false);
}

template <class Json>
JsonMaybeValue<Json> JsonMaybeObject<Json>::has(std::string const& key) {
    this->addKnownKey(key);
    if (this->isError()) return emptyValue();
    if (!self().m_json.contains(key) || self().m_json[key].is_null()) {
        return emptyValue();
    }
    return JsonMaybeValue<Json>(self().m_checker, self().m_json[key], key, true);
}

template <class Json>
JsonMaybeValue<Json> JsonMaybeObject<Json>::needs(std::string const& key) {
    this->addKnownKey(key);
    if (this->isError()) return emptyValue();
    if (!self().m_json.contains(key)) {
        this->setError(self().m_hierarchy + " is missing required key \"" + key + "\"");
        return emptyValue();
    }
    return JsonMaybeValue<Json>(self().m_checker, self().m_json[key], key, true);
}

template <class Json>
void JsonMaybeObject<Json>::checkUnknownKeys() {
    for (auto& [key, _] : self().m_json.as_object()) {
        if (!m_knownKeys.count(key)) {
            log::warn("{} contains unknown key \"{}\"", self().m_hierarchy, key);
        }
    }
}

template <class Json>
JsonChecker<Json>::JsonChecker(Json& json) : m_json(json), m_result(std::monostate()) {}

template <class Json>
bool JsonChecker<Json>::isError() const {
    return std::holds_alternative<std::string>(m_result);
}

template <class Json>
std::string JsonChecker<Json>::getError() const {
    return std::get<std::string>(m_result);
}

template <class Json>
JsonMaybeValue<Json> JsonChecker<Json>::root(std::string const& hierarchy) {
    return JsonMaybeValue(*this, m_json, hierarchy, true);
}

namespace geode {

    template struct JsonMaybeSomething<json::Value>;
    template struct JsonMaybeSomething<json::Value const>;
    //template struct JsonMaybeSomething<ModJson>;

    template struct JsonMaybeValue<json::Value>;
    template struct JsonMaybeValue<json::Value const>;
    //template struct JsonMaybeValue<ModJson>;

    template struct JsonMaybeObject<json::Value>;
    template struct JsonMaybeObject<json::Value const>;
    //template struct JsonMaybeObject<ModJson>;

    template struct JsonChecker<json::Value>;
    template struct JsonChecker<json::Value const>;
    //template struct JsonChecker<ModJson>;

}