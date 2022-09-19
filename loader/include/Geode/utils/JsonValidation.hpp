#pragma once

#include "json.hpp"
#include "../loader/Log.hpp"
#include <variant>
#include <set>

namespace geode {
    struct JsonChecker;

    namespace {
        using value_t = nlohmann::detail::value_t;

        constexpr const char* jsonValueTypeToString(value_t type) {
            switch (type) {
                default:
                case value_t::null:            return "null";
                case value_t::object:          return "object";
                case value_t::array:           return "array";
                case value_t::string:          return "string";
                case value_t::boolean:         return "boolean";
                case value_t::binary:          return "binary";
                case value_t::discarded:       return "discarded";
                case value_t::number_integer:  return "integer";
                case value_t::number_unsigned: return "integer";
                case value_t::number_float:    return "number";
            }
        }

        template<class T>
        constexpr value_t getJsonType() {
            if constexpr (std::is_same_v<T, bool>) {
                return value_t::boolean;
            }
            else if constexpr (std::is_floating_point_v<T>) {
                return value_t::number_float;
            }
            else if constexpr (std::is_unsigned_v<T>) {
                return value_t::number_unsigned;
            }
            else if constexpr (std::is_integral_v<T>) {
                return value_t::number_integer;
            }
            else if constexpr (
                std::is_same_v<T, std::string> ||
                std::is_same_v<T, const char*>
            ) {
                return value_t::string;
            }
            return value_t::null;
        }

        bool jsonConvertibleTo(value_t value, value_t to) {
            if (
                value == value_t::number_float ||
                value == value_t::number_integer ||
                value == value_t::number_unsigned
            ) {
                return
                    to == value_t::number_float ||
                    to == value_t::number_integer ||
                    to == value_t::number_unsigned;
            }
            return value == to;
        }
    }

    template<class T>
    using JsonValueValidator = bool(*)(T const&);

    struct JsonMaybeObject;

    struct GEODE_DLL JsonMaybeSomething {
    protected:
        JsonChecker& m_checker;
        nlohmann::json& m_json;
        std::string m_hierarchy;
        bool m_hasValue;

        void setError(std::string const& error);
    
    public:
        nlohmann::json& json() {
            return m_json;
        }

        JsonMaybeSomething(
            JsonChecker& checker,
            nlohmann::json& json,
            std::string const& hierarchy,
            bool hasValue
        ) : m_checker(checker),
            m_json(json),
            m_hierarchy(hierarchy),
            m_hasValue(hasValue) {}

        bool isError() const;

        operator bool() const {
            return !isError();
        }
    };

    struct GEODE_DLL JsonMaybeValue : JsonMaybeSomething {
        bool m_inferType = true;

        JsonMaybeValue(
            JsonChecker& checker,
            nlohmann::json& json,
            std::string const& hierarchy,
            bool hasValue
        ) : JsonMaybeSomething(checker, json, hierarchy, hasValue) {}

        template<nlohmann::detail::value_t T>
        JsonMaybeValue as() {
            if (this->isError()) return *this;
            if (!jsonConvertibleTo(m_json.type(), T)) {
                this->setError(
                    m_hierarchy + ": Invalid type \"" + 
                    m_json.type_name() + "\", expected \"" + 
                    jsonValueTypeToString(T) + "\""
                );
            }
            m_inferType = false;
            return *this;
        }

        template<nlohmann::detail::value_t... T>
        JsonMaybeValue asOneOf() {
            if (this->isError()) return *this;
            bool isOneOf = (... || jsonConvertibleTo(m_json.type(), T));
            if (!isOneOf) {
                this->setError(
                    m_hierarchy + ": Invalid type \"" + 
                    m_json.type_name() + "\", expected one of \"" + 
                    (jsonValueTypeToString(T), ...) + "\""
                );
            }
            m_inferType = false;
            return *this;
        }

        template<nlohmann::detail::value_t T>
        JsonMaybeValue is() {
            if (this->isError()) return *this;
            m_hasValue = jsonConvertibleTo(m_json.type(), T);
            m_inferType = false;
            return *this;
        }

        template<class T>
        JsonMaybeValue validate(JsonValueValidator<T> validator) {
            if (this->isError()) return *this;
            try {
                if (!validator(m_json.get<T>())) {
                    this->setError(m_hierarchy + ": Invalid value format");
                }
            } catch(...) {
                this->setError(
                    m_hierarchy + ": Invalid type \"" +
                    std::string(m_json.type_name()) + "\""
                );
            }
            return *this;
        }

        template<class T>
        JsonMaybeValue inferType() {
            if (this->isError() || !m_inferType) return *this;
            return this->as<getJsonType<T>()>();
        }

        JsonMaybeValue intoRaw(nlohmann::json& target) {
            if (this->isError()) return *this;
            target = m_json;
            return *this;
        }

        template<class T>
        JsonMaybeValue into(T& target) {
            return this->intoAs<T, T>(target);
        }

        template<class A, class T>
        JsonMaybeValue intoAs(T& target) {
            this->inferType<A>();
            if (this->isError()) return *this;
            try {
                target = m_json.get<A>();
            } catch(...) {
                this->setError(
                    m_hierarchy + ": Invalid type \"" + 
                    std::string(m_json.type_name()) + "\""
                );
            }
            return *this;
        }

        template<class T>
        T get() {
            this->inferType<T>();
            if (this->isError()) return T();
            try {
                return m_json.get<T>();
            } catch(...) {
                this->setError(
                    m_hierarchy + ": Invalid type to get \"" + 
                    std::string(m_json.type_name()) + "\""
                );
            }
            return T();
        }

        JsonMaybeObject obj();

        template<class T>
        struct Iterator {
            std::vector<T> m_values;

            using iterator = typename std::vector<T>::iterator;
            using const_iterator = typename std::vector<T>::const_iterator;

            iterator begin() {
                return m_values.begin();
            }
            iterator end() {
                return m_values.end();
            }

            const_iterator begin() const {
                return m_values.begin();
            }
            const_iterator end() const {
                return m_values.end();
            }
        };

        Iterator<JsonMaybeValue> iterate() {
            this->as<value_t::array>();
            Iterator<JsonMaybeValue> iter;
            if (this->isError()) return iter;
            size_t i = 0;
            for (auto& obj : m_json) {
                iter.m_values.emplace_back(
                    m_checker, obj,
                    m_hierarchy + "." + std::to_string(i++),
                    m_hasValue
                );
            }
            return iter;
        }

        Iterator<std::pair<std::string, JsonMaybeValue>> items() {
            this->as<value_t::object>();
            Iterator<std::pair<std::string, JsonMaybeValue>> iter;
            if (this->isError()) return iter;

            for (auto& [k, v] : m_json.items()) {
                iter.m_values.emplace_back(k, JsonMaybeValue(
                    m_checker, v,
                    m_hierarchy + "." + k,
                    m_hasValue
                ));
            }

            return iter;
        }
    };

    struct GEODE_DLL JsonMaybeObject : JsonMaybeSomething {
        std::set<std::string> m_knownKeys;

        JsonMaybeObject(
            JsonChecker& checker,
            nlohmann::json& json,
            std::string const& hierarchy,
            bool hasValue
        ) : JsonMaybeSomething(checker, json, hierarchy, hasValue) {}

        void addKnownKey(std::string const& key) {
            m_knownKeys.insert(key);
        }

        nlohmann::json& json() {
            return m_json;
        }

        JsonMaybeValue emptyValue() {
            return JsonMaybeValue(m_checker, m_json, "", false);
        }

        JsonMaybeValue has(std::string const& key) {
            this->addKnownKey(key);
            if (this->isError()) return emptyValue(); 
            if (!m_json.contains(key) || m_json[key].is_null()) {
                return emptyValue();
            }
            return JsonMaybeValue(m_checker, m_json[key], key, true);
        }

        JsonMaybeValue needs(std::string const& key) {
            this->addKnownKey(key);
            if (this->isError()) return emptyValue(); 
            if (!m_json.contains(key)) {
                this->setError(
                    m_hierarchy + " is missing required key \"" + key + "\""
                );
                return emptyValue();
            }
            return JsonMaybeValue(m_checker, m_json[key], key, true);
        }

        void checkUnknownKeys() {
            for (auto& [key, _] : m_json.items()) {
                if (!m_knownKeys.count(key)) {
                    Log::get() << m_hierarchy + " contains unknown key \"" + key + "\"";
                }
            }
        }
    };
    
    struct GEODE_DLL JsonChecker {
        std::variant<std::monostate, std::string> m_result;
        nlohmann::json& m_json;

        JsonChecker(
            nlohmann::json& json
        ) : m_json(json), m_result(std::monostate()) {}

        bool isError() const {
            return std::holds_alternative<std::string>(m_result);
        }

        std::string getError() const {
            return std::get<std::string>(m_result);
        }

        JsonMaybeValue root(std::string const& hierarchy) {
            return JsonMaybeValue(*this, m_json, hierarchy, true);
        }
    };
}
