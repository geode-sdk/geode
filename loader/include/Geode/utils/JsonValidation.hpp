#pragma once

#include "json.hpp"
#include "../loader/Log.hpp"
#include <variant>
#include <set>

namespace geode {
    template<class Json>
    struct JsonChecker;

    template <typename T, typename = void>
    struct is_iterable : std::false_type {};

    template <typename T>
    struct is_iterable<T,
        std::void_t<
            decltype(std::begin(std::declval<T>())),
            decltype(std::end(std::declval<T>()))
        >
    > : std::true_type {};

    template <typename T>
    constexpr bool is_iterable_v = is_iterable<T>::value;

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
                std::is_constructible_v<T, std::string>
            ) {
                return value_t::string;
            }
            else if constexpr (is_iterable_v<T>) {
                return value_t::array;
            }
            return value_t::null;
        }

        bool jsonConvertibleTo(value_t value, value_t to) {
            // if we don't know the type we're passing into, 
            // everything's valid
            if (to == value_t::null) return true;
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

    template<class Json>
    struct JsonMaybeObject;
    template<class Json>
    struct JsonMaybeValue;

    template<class Json>
    struct JsonMaybeSomething {
    protected:
        JsonChecker<Json>& m_checker;
        Json& m_json;
        std::string m_hierarchy;
        bool m_hasValue;

        friend struct JsonMaybeObject<Json>;
        friend struct JsonMaybeValue<Json>;

        void setError(std::string const& error);
    
    public:
        Json& json() {
            return m_json;
        }

        JsonMaybeSomething(
            JsonChecker<Json>& checker,
            Json& json,
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

    template<class Json>
    struct JsonMaybeValue : public JsonMaybeSomething<Json> {
        bool m_inferType = true;

        JsonMaybeValue(
            JsonChecker<Json>& checker,
            Json& json,
            std::string const& hierarchy,
            bool hasValue
        ) : JsonMaybeSomething<Json>(checker, json, hierarchy, hasValue) {}

        JsonMaybeSomething<Json>& self() {
            return *static_cast<JsonMaybeSomething<Json>*>(this);
        }

        template<nlohmann::detail::value_t T>
        JsonMaybeValue<Json> as() {
            if (this->isError()) return *this;
            if (!jsonConvertibleTo(self().m_json.type(), T)) {
                this->setError(
                    self().m_hierarchy + ": Invalid type \"" + 
                    self().m_json.type_name() + "\", expected \"" + 
                    jsonValueTypeToString(T) + "\""
                );
            }
            m_inferType = false;
            return *this;
        }

        JsonMaybeValue<Json> array() {
            this->as<value_t::array>();
            return *this;
        }

        template<nlohmann::detail::value_t... T>
        JsonMaybeValue<Json> asOneOf() {
            if (this->isError()) return *this;
            bool isOneOf = (... || jsonConvertibleTo(self().m_json.type(), T));
            if (!isOneOf) {
                this->setError(
                    self().m_hierarchy + ": Invalid type \"" + 
                    self().m_json.type_name() + "\", expected one of \"" + 
                    (jsonValueTypeToString(T), ...) + "\""
                );
            }
            m_inferType = false;
            return *this;
        }

        template<nlohmann::detail::value_t T>
        JsonMaybeValue<Json> is() {
            if (this->isError()) return *this;
            self().m_hasValue = jsonConvertibleTo(self().m_json.type(), T);
            m_inferType = false;
            return *this;
        }

        template<class T>
        JsonMaybeValue<Json> validate(JsonValueValidator<T> validator) {
            if (this->isError()) return *this;
            try {
                if (!validator(self().m_json.template get<T>())) {
                    this->setError(self().m_hierarchy + ": Invalid value format");
                }
            } catch(...) {
                this->setError(
                    self().m_hierarchy + ": Invalid type \"" +
                    std::string(self().m_json.type_name()) + "\""
                );
            }
            return *this;
        }

        template<class T>
        JsonMaybeValue<Json> inferType() {
            if (this->isError() || !m_inferType) return *this;
            return this->as<getJsonType<T>()>();
        }

        template<class T>
        JsonMaybeValue<Json> intoRaw(T& target) {
            if (this->isError()) return *this;
            target = self().m_json;
            return *this;
        }

        template<class T>
        JsonMaybeValue<Json> into(T& target) {
            return this->intoAs<T, T>(target);
        }

        template<class T>
        JsonMaybeValue<Json> into(std::optional<T>& target) {
            return this->intoAs<T, std::optional<T>>(target);
        }

        template<class A, class T>
        JsonMaybeValue<Json> intoAs(T& target) {
            this->inferType<A>();
            if (this->isError()) return *this;
            try {
                target = self().m_json.template get<A>();
            } catch(...) {
                this->setError(
                    self().m_hierarchy + ": Invalid type \"" + 
                    std::string(self().m_json.type_name()) + "\""
                );
            }
            return *this;
        }

        template<class T>
        T get() {
            this->inferType<T>();
            if (this->isError()) return T();
            try {
                return self().m_json.template get<T>();
            } catch(...) {
                this->setError(
                    self().m_hierarchy + ": Invalid type to get \"" + 
                    std::string(self().m_json.type_name()) + "\""
                );
            }
            return T();
        }

        JsonMaybeObject<Json> obj();

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

        JsonMaybeValue<Json> at(size_t i) {
            this->as<value_t::array>();
            if (this->isError()) return *this;
            if (self().m_json.size() <= i) {
                this->setError(
                    self().m_hierarchy + ": has " +
                    std::to_string(self().m_json.size()) + "items "
                    ", expected to have at least " + std::to_string(i + 1)
                );
                return *this;
            }
            return JsonMaybeValue<Json>(
                self().m_checker, self().m_json.at(i),
                self().m_hierarchy + "." + std::to_string(i),
                self().m_hasValue
            );
        }

        Iterator<JsonMaybeValue<Json>> iterate() {
            this->as<value_t::array>();
            Iterator<JsonMaybeValue<Json>> iter;
            if (this->isError()) return iter;
            size_t i = 0;
            for (auto& obj : self().m_json) {
                iter.m_values.emplace_back(
                    self().m_checker, obj,
                    self().m_hierarchy + "." + std::to_string(i++),
                    self().m_hasValue
                );
            }
            return iter;
        }

        Iterator<std::pair<std::string, JsonMaybeValue<Json>>> items() {
            this->as<value_t::object>();
            Iterator<std::pair<std::string, JsonMaybeValue<Json>>> iter;
            if (this->isError()) return iter;

            for (auto& [k, v] : self().m_json.items()) {
                iter.m_values.emplace_back(k, JsonMaybeValue<Json>(
                    self().m_checker, v,
                    self().m_hierarchy + "." + k,
                    self().m_hasValue
                ));
            }

            return iter;
        }
    };

    template<class Json>
    struct JsonMaybeObject : JsonMaybeSomething<Json> {
        std::set<std::string> m_knownKeys;

        JsonMaybeObject(
            JsonChecker<Json>& checker,
            Json& json,
            std::string const& hierarchy,
            bool hasValue
        ) : JsonMaybeSomething<Json>(checker, json, hierarchy, hasValue) {}

        JsonMaybeSomething<Json>& self() {
            return *static_cast<JsonMaybeSomething<Json>*>(this);
        }

        void addKnownKey(std::string const& key) {
            m_knownKeys.insert(key);
        }

        Json& json() {
            return self().m_json;
        }

        JsonMaybeValue<Json> emptyValue() {
            return JsonMaybeValue(self().m_checker, self().m_json, "", false);
        }

        JsonMaybeValue<Json> has(std::string const& key) {
            this->addKnownKey(key);
            if (this->isError()) return emptyValue(); 
            if (!self().m_json.contains(key) || self().m_json[key].is_null()) {
                return emptyValue();
            }
            return JsonMaybeValue<Json>(self().m_checker, self().m_json[key], key, true);
        }

        JsonMaybeValue<Json> needs(std::string const& key) {
            this->addKnownKey(key);
            if (this->isError()) return emptyValue(); 
            if (!self().m_json.contains(key)) {
                this->setError(
                    self().m_hierarchy + " is missing required key \"" + key + "\""
                );
                return emptyValue();
            }
            return JsonMaybeValue<Json>(self().m_checker, self().m_json[key], key, true);
        }

        void checkUnknownKeys() {
            for (auto& [key, _] : self().m_json.items()) {
                if (!m_knownKeys.count(key)) {
                    // log::debug(self().m_hierarchy + " contains unknown key \"" + key + "\"");
                    log::debug("{} contains unknown key \"{}\"", self().m_hierarchy, key);
                }
            }
        }
    };
    
    template<class Json = nlohmann::json>
    struct JsonChecker {
        std::variant<std::monostate, std::string> m_result;
        Json& m_json;

        JsonChecker(Json& json) : m_json(json), m_result(std::monostate()) {}

        bool isError() const {
            return std::holds_alternative<std::string>(m_result);
        }

        std::string getError() const {
            return std::get<std::string>(m_result);
        }

        JsonMaybeValue<Json> root(std::string const& hierarchy) {
            return JsonMaybeValue(*this, m_json, hierarchy, true);
        }
    };
    
    template<class Json>
    void JsonMaybeSomething<Json>::setError(std::string const& error) {
        m_checker.m_result = error;
    }

    template<class Json>
    bool JsonMaybeSomething<Json>::isError() const {
        return m_checker.isError() || !m_hasValue;
    }

    template<class Json>
    JsonMaybeObject<Json> JsonMaybeValue<Json>::obj() {
        this->as<value_t::object>();
        return JsonMaybeObject(
            self().m_checker, self().m_json,
            self().m_hierarchy, self().m_hasValue
        );
    }
}
