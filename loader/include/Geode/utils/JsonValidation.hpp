#pragma once

#include "../loader/Log.hpp"
#include "../external/json/json.hpp"

#include <set>
#include <variant>

namespace geode {
    template <class Json>
    struct JsonChecker;

    template <typename T, typename = void>
    struct is_iterable : std::false_type {};

    template <typename T>
    struct is_iterable<
        T,
        std::void_t<
            decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>> :
        std::true_type {};

    template <typename T>
    constexpr bool is_iterable_v = is_iterable<T>::value;

    namespace {
        using value_t = nlohmann::detail::value_t;

        constexpr char const* jsonValueTypeToString(value_t type) {
            switch (type) {
                default:
                case value_t::null: return "null";
                case value_t::object: return "object";
                case value_t::array: return "array";
                case value_t::string: return "string";
                case value_t::boolean: return "boolean";
                case value_t::binary: return "binary";
                case value_t::discarded: return "discarded";
                case value_t::number_integer: return "integer";
                case value_t::number_unsigned: return "integer";
                case value_t::number_float: return "number";
            }
        }

        template <class T>
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
            else if constexpr (std::is_constructible_v<T, std::string>) {
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
            if (value == value_t::number_float || value == value_t::number_integer ||
                value == value_t::number_unsigned) {
                return to == value_t::number_float || to == value_t::number_integer ||
                    to == value_t::number_unsigned;
            }
            return value == to;
        }
    }

    template <class T>
    using JsonValueValidator = bool (*)(T const&);

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

        GEODE_DLL operator bool() const;
    };

    template <class Json>
    struct JsonMaybeValue : public JsonMaybeSomething<Json> {
        bool m_inferType = true;

        GEODE_DLL JsonMaybeValue(
            JsonChecker<Json>& checker, Json& json, std::string const& hierarchy, bool hasValue
        );

        GEODE_DLL JsonMaybeSomething<Json>& self();

        template <nlohmann::detail::value_t T>
        JsonMaybeValue<Json>& as() {
            if (this->isError()) return *this;
            if (!jsonConvertibleTo(self().m_json.type(), T)) {
                this->setError(
                    self().m_hierarchy + ": Invalid type \"" + self().m_json.type_name() +
                    "\", expected \"" + jsonValueTypeToString(T) + "\""
                );
            }
            m_inferType = false;
            return *this;
        }

        GEODE_DLL JsonMaybeValue<Json>& array();

        template <nlohmann::detail::value_t... T>
        JsonMaybeValue<Json>& asOneOf() {
            if (this->isError()) return *this;
            bool isOneOf = (... || jsonConvertibleTo(self().m_json.type(), T));
            if (!isOneOf) {
                this->setError(
                    self().m_hierarchy + ": Invalid type \"" + self().m_json.type_name() +
                    "\", expected one of \"" + (jsonValueTypeToString(T), ...) + "\""
                );
            }
            m_inferType = false;
            return *this;
        }

        template <nlohmann::detail::value_t T>
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
                if (!validator(self().m_json.template get<T>())) {
                    this->setError(self().m_hierarchy + ": Invalid value format");
                }
            }
            catch (...) {
                this->setError(
                    self().m_hierarchy + ": Invalid type \"" +
                    std::string(self().m_json.type_name()) + "\""
                );
            }
            return *this;
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
                target = self().m_json.template get<A>();
            }
            catch (...) {
                this->setError(
                    self().m_hierarchy + ": Invalid type \"" +
                    std::string(self().m_json.type_name()) + "\""
                );
            }
            return *this;
        }

        template <class T>
        T get() {
            this->inferType<T>();
            if (this->isError()) return T();
            try {
                return self().m_json.template get<T>();
            }
            catch (...) {
                this->setError(
                    self().m_hierarchy + ": Invalid type to get \"" +
                    std::string(self().m_json.type_name()) + "\""
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

    template <class Json = nlohmann::json>
    struct JsonChecker {
        std::variant<std::monostate, std::string> m_result;
        Json& m_json;

        GEODE_DLL JsonChecker(Json& json);

        GEODE_DLL bool isError() const;

        GEODE_DLL std::string getError() const;

        GEODE_DLL JsonMaybeValue<Json> root(std::string const& hierarchy);
    };

}
