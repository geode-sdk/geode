#pragma once

#include <json.hpp>
#include "../loader/Log.hpp"

#include <set>
#include <variant>

namespace geode {
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
    using JsonValueValidator = utils::MiniFunction<bool(T const&)>;

    struct JsonMaybeObject;
    struct JsonMaybeValue;

    struct GEODE_DLL JsonMaybeSomething {
    protected:
        JsonChecker& m_checker;
        json::Value& m_json;
        std::string m_hierarchy;
        bool m_hasValue;

        friend struct JsonMaybeObject;
        friend struct JsonMaybeValue;

        void setError(std::string const& error);

    public:
        json::Value& json();

        JsonMaybeSomething(
            JsonChecker& checker, json::Value& json, std::string const& hierarchy, bool hasValue
        );

        bool isError() const;
        std::string getError() const;

        operator bool() const;
    };

    struct GEODE_DLL JsonMaybeValue : public JsonMaybeSomething {
        bool m_inferType = true;

        JsonMaybeValue(
            JsonChecker& checker, json::Value& json, std::string const& hierarchy, bool hasValue
        );

        JsonMaybeSomething& self();

        template <json::Type T>
        JsonMaybeValue& as() {
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

        JsonMaybeValue& array();

        template <json::Type... T>
        JsonMaybeValue& asOneOf() {
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
        JsonMaybeValue& is() {
            if (this->isError()) return *this;
            self().m_hasValue = jsonConvertibleTo(self().m_json.type(), T);
            m_inferType = false;
            return *this;
        }

        template <class T>
        JsonMaybeValue& validate(JsonValueValidator<T> validator) {
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
        JsonMaybeValue& inferType() {
            if (this->isError() || !m_inferType) return *this;
            return this->as<getJsonType<T>()>();
        }

        template <class T>
        JsonMaybeValue& intoRaw(T& target) {
            if (this->isError()) return *this;
            target = self().m_json;
            return *this;
        }

        template <class T>
        JsonMaybeValue& into(T& target) {
            return this->intoAs<T, T>(target);
        }

        template <class T>
        JsonMaybeValue& into(std::optional<T>& target) {
            return this->intoAs<T, std::optional<T>>(target);
        }

        template <class A, class T>
        JsonMaybeValue& intoAs(T& target) {
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

        JsonMaybeObject obj();

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

        JsonMaybeValue at(size_t i);

        Iterator<JsonMaybeValue> iterate();

        Iterator<std::pair<std::string, JsonMaybeValue>> items();
    };

    struct GEODE_DLL JsonMaybeObject : JsonMaybeSomething {
        std::set<std::string> m_knownKeys;

        JsonMaybeObject(
            JsonChecker& checker, json::Value& json, std::string const& hierarchy, bool hasValue
        );

        JsonMaybeSomething& self();

        void addKnownKey(std::string const& key);

        json::Value& json();

        JsonMaybeValue emptyValue();

        JsonMaybeValue has(std::string const& key);

        JsonMaybeValue needs(std::string const& key);

        void checkUnknownKeys();
    };

    struct GEODE_DLL JsonChecker {
        std::variant<std::monostate, std::string> m_result;
        json::Value& m_json;

        JsonChecker(json::Value& json);

        bool isError() const;

        std::string getError() const;

        JsonMaybeValue root(std::string const& hierarchy);
    };

}
