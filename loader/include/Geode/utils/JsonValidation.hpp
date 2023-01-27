#pragma once

#include <json.hpp>
#include "../loader/Log.hpp"

#include <set>
#include <variant>

namespace geode {
    template <class Json>
    struct JsonChecker;

    template <typename T, typename = void>
    struct is_iterable : std::false_type {};

    template <typename T>
    struct is_iterable<
        T, std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>> :
        std::true_type {};

    template <typename T>
    constexpr bool is_iterable_v = is_iterable<T>::value;

    namespace {
        using value_t = json::Type;

        constexpr char const* jsonValueTypeToString(value_t type) {
            switch (type) {
                default:
                case value_t::Null: return "null";
                case value_t::Object: return "object";
                case value_t::Array: return "array";
                case value_t::String: return "string";
                case value_t::Bool: return "boolean";
                case value_t::Number: return "number";
            }
        }

        template <class T>
        constexpr value_t getJsonType() {
            if constexpr (std::is_same_v<T, bool>) {
                return value_t::Bool;
            }
            else if constexpr (std::is_floating_point_v<T>) {
                return value_t::Number;
            }
            else if constexpr (std::is_unsigned_v<T>) {
                return value_t::Number;
            }
            else if constexpr (std::is_integral_v<T>) {
                return value_t::Number;
            }
            else if constexpr (std::is_constructible_v<T, std::string>) {
                return value_t::String;
            }
            else if constexpr (is_iterable_v<T>) {
                return value_t::Array;
            }
            return value_t::Null;
        }

        bool jsonConvertibleTo(value_t value, value_t to) {
            // if we don't know the type we're passing into,
            // everything's valid
            if (to == value_t::Null) return true;

            if (value == value_t::Number) {
                return to == value_t::Number;
            }

            return value == to;
        }
    }

    template <class T>
    using JsonValueValidator = std::function<bool(T const&)>;

    template <class Json>
    struct JsonMaybeObject;
    template <class Json>
    struct JsonMaybeValue;

    template <class Json>
    struct JsonMaybeSomething {
    protected:
        JsonChecker<Json>& m_checker;
        Json& m_json;
        std::string m_hierarchy;
        bool m_hasValue;

        friend struct JsonMaybeObject<Json>;
        friend struct JsonMaybeValue<Json>;

        GEODE_DLL void setError(std::string const& error);

    public:
        GEODE_DLL Json& json();

        GEODE_DLL JsonMaybeSomething(
            JsonChecker<Json>& checker, Json& json, std::string const& hierarchy, bool hasValue
        );

        GEODE_DLL bool isError() const;
        GEODE_DLL std::string getError() const;

        GEODE_DLL operator bool() const;
    };

    template <class Json>
    struct JsonMaybeValue : public JsonMaybeSomething<Json> {
        bool m_inferType = true;

        GEODE_DLL JsonMaybeValue(
            JsonChecker<Json>& checker, Json& json, std::string const& hierarchy, bool hasValue
        );

        GEODE_DLL JsonMaybeSomething<Json>& self();

        template <json::Type T>
        JsonMaybeValue<Json>& as() {
            if (this->isError()) return *this;
            if (!jsonConvertibleTo(self().m_json.type(), T)) {
                this->setError(
                    self().m_hierarchy + ": Invalid type \"" + jsonValueTypeToString(self().m_json.type()) +
                    "\", expected \"" + jsonValueTypeToString(T) + "\""
                );
            }
            m_inferType = false;
            return *this;
        }

        GEODE_DLL JsonMaybeValue<Json>& array();

        template <json::Type... T>
        JsonMaybeValue<Json>& asOneOf() {
            if (this->isError()) return *this;
            bool isOneOf = (... || jsonConvertibleTo(self().m_json.type(), T));
            if (!isOneOf) {
                this->setError(
                    self().m_hierarchy + ": Invalid type \"" + jsonValueTypeToString(self().m_json.type()) +
                    "\", expected one of \"" + (jsonValueTypeToString(T), ...) + "\""
                );
            }
            m_inferType = false;
            return *this;
        }

        template <json::Type T>
        JsonMaybeValue<Json>& is() {
            if (this->isError()) return *this;
            self().m_hasValue = jsonConvertibleTo(self().m_json.type(), T);
            m_inferType = false;
            return *this;
        }

        template <class T>
        JsonMaybeValue<Json>& validate(JsonValueValidator<T> validator) {
            if (this->isError()) return *this;
            try {
                if (!validator(self().m_json.template as<T>())) {
                    this->setError(self().m_hierarchy + ": Invalid value format");
                }
            }
            catch (...) {
                this->setError(
                    self().m_hierarchy + ": Invalid type \"" +
                    std::string(jsonValueTypeToString(self().m_json.type())) + "\""
                );
            }
            return *this;
        }

        template <class T>
        JsonMaybeValue<Json>& validate(bool (*validator)(T const&)) {
            return this->validate(std::function(validator));
        }

        template <class T>
        JsonMaybeValue<Json>& inferType() {
            if (this->isError() || !m_inferType) return *this;
            return this->as<getJsonType<T>()>();
        }

        template <class T>
        JsonMaybeValue<Json>& intoRaw(T& target) {
            if (this->isError()) return *this;
            target = self().m_json;
            return *this;
        }

        template <class T>
        JsonMaybeValue<Json>& into(T& target) {
            return this->intoAs<T, T>(target);
        }

        template <class T>
        JsonMaybeValue<Json>& into(std::optional<T>& target) {
            return this->intoAs<T, std::optional<T>>(target);
        }

        template <class A, class T>
        JsonMaybeValue<Json>& intoAs(T& target) {
            this->inferType<A>();
            if (this->isError()) return *this;

            try {
                target = self().m_json.template as<A>();
            }
            catch (...) {
                this->setError(
                    self().m_hierarchy + ": Invalid type \"" +
                    jsonValueTypeToString(self().m_json.type()) + "\""
                );
            }

            return *this;
        }

        template <class T>
        T get() {
            this->inferType<T>();
            if (this->isError()) return T();
            try {

                constexpr auto type = getJsonType<T>();

                if constexpr (type == value_t::Number) {
                    return self().m_json.as_double();
                } else if constexpr (type == value_t::Bool) {
                    return self().m_json.as_bool();
                } else if constexpr (type == value_t::String) {
                    return self().m_json.as_string();
                }
            }
            catch (...) {
                this->setError(
                    self().m_hierarchy + ": Invalid type to get \"" +
                    std::string(jsonValueTypeToString(self().m_json.type())) + "\""
                );
            }
            return T();
        }

        GEODE_DLL JsonMaybeObject<Json> obj();

        template <class T>
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

        GEODE_DLL JsonMaybeValue<Json> at(size_t i);

        GEODE_DLL Iterator<JsonMaybeValue<Json>> iterate();

        GEODE_DLL Iterator<std::pair<std::string, JsonMaybeValue<Json>>> items();
    };

    template <class Json>
    struct JsonMaybeObject : JsonMaybeSomething<Json> {
        std::set<std::string> m_knownKeys;

        GEODE_DLL JsonMaybeObject(
            JsonChecker<Json>& checker, Json& json, std::string const& hierarchy, bool hasValue
        );

        GEODE_DLL JsonMaybeSomething<Json>& self();

        GEODE_DLL void addKnownKey(std::string const& key);

        GEODE_DLL Json& json();

        GEODE_DLL JsonMaybeValue<Json> emptyValue();

        GEODE_DLL JsonMaybeValue<Json> has(std::string const& key);

        GEODE_DLL JsonMaybeValue<Json> needs(std::string const& key);

        GEODE_DLL void checkUnknownKeys();
    };

    template <class Json = json::Value>
    struct JsonChecker {
        std::variant<std::monostate, std::string> m_result;
        Json& m_json;

        GEODE_DLL JsonChecker(Json& json);

        GEODE_DLL bool isError() const;

        GEODE_DLL std::string getError() const;

        GEODE_DLL JsonMaybeValue<Json> root(std::string const& hierarchy);
    };

}
