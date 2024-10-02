#pragma once

#include <matjson.hpp>
#include "../loader/Log.hpp"
#include <set>
#include <variant>
#include <Geode/utils/MiniFunction.hpp>
#include <Geode/utils/Result.hpp>

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
        using value_t = matjson::Type;

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

    struct GEODE_DLL 
    [[deprecated("Use JsonExpectedValue via the checkJson function instead")]]
    JsonMaybeSomething {
    protected:
        JsonChecker& m_checker;
        matjson::Value& m_json;
        std::string m_hierarchy;
        bool m_hasValue;

        friend struct JsonMaybeObject;
        friend struct JsonMaybeValue;

        void setError(std::string const& error);

    public:
        matjson::Value& json();

        JsonMaybeSomething(
            JsonChecker& checker, matjson::Value& json, std::string const& hierarchy, bool hasValue
        );

        bool isError() const;
        std::string getError() const;

        operator bool() const;
    };

    struct GEODE_DLL 
    [[deprecated("Use JsonExpectedValue via the checkJson function instead")]]
    JsonMaybeValue : public JsonMaybeSomething {
        bool m_inferType = true;

        JsonMaybeValue(
            JsonChecker& checker, matjson::Value& json, std::string const& hierarchy, bool hasValue
        );

        JsonMaybeSomething& self();

        template <matjson::Type T>
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

        template <matjson::Type... T>
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

        template <class T>
        bool is() {
            if (this->isError()) return false;
            return self().m_json.template is<T>();
        }

        template <class T>
        JsonMaybeValue& validate(JsonValueValidator<T> validator) {
            if (this->isError()) return *this;
            if (self().m_json.template is<T>()) {
                if (!validator(self().m_json.template as<T>())) {
                    this->setError(self().m_hierarchy + ": Invalid value format");
                }
            }
            else {
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

            if (self().m_json.template is<A>()) {
                try {
                    target = self().m_json.template as<A>();
                }
                catch(matjson::JsonException const& e) {
                    this->setError(
                        self().m_hierarchy + ": Error parsing JSON: " + std::string(e.what())
                    );
                }
            }
            else {
                this->setError(
                    self().m_hierarchy + ": Invalid type \"" +
                    std::string(jsonValueTypeToString(self().m_json.type())) + "\""
                );
            }

            return *this;
        }

        template <class T>
        T get() {
            this->inferType<T>();
            if (this->isError()) return T();
            if (self().m_json.template is<T>()) {
                return self().m_json.template as<T>();
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

    struct 
    [[deprecated("Use JsonExpectedValue via the checkJson function instead")]]
    GEODE_DLL JsonMaybeObject : JsonMaybeSomething {
        std::set<std::string> m_knownKeys;

        JsonMaybeObject(
            JsonChecker& checker, matjson::Value& json, std::string const& hierarchy, bool hasValue
        );

        JsonMaybeSomething& self();

        void addKnownKey(std::string const& key);

        matjson::Value& json();

        JsonMaybeValue emptyValue();

        JsonMaybeValue has(std::string const& key);

        JsonMaybeValue needs(std::string const& key);

        void checkUnknownKeys();
    };

    struct
    [[deprecated("Use JsonExpectedValue via the checkJson function instead")]]
    GEODE_DLL JsonChecker {
        std::variant<std::monostate, std::string> m_result;
        matjson::Value& m_json;

        JsonChecker(matjson::Value& json);

        bool isError() const;

        std::string getError() const;

        JsonMaybeValue root(std::string const& hierarchy);
    };

    class GEODE_DLL JsonExpectedValue final {
    protected:
        class Impl;
        std::unique_ptr<Impl> m_impl;

        JsonExpectedValue();
        JsonExpectedValue(Impl* from, matjson::Value& scope, std::string_view key);

        static const char* matJsonTypeToString(matjson::Type ty);

        bool hasError() const;
        void setError(std::string_view error);

        matjson::Value const& getJSONRef() const;

        template <class... Args>
        void setError(fmt::format_string<Args...> error, Args&&... args) {
            this->setError(fmt::format(error, std::forward<Args>(args)...));
        }

        template <class T>
        std::optional<T> tryGet() {
            if (this->hasError()) return std::nullopt;
            if constexpr (std::is_same_v<T, matjson::Value>) {
                return this->getJSONRef();
            }
            else {
                try {
                    if (this->getJSONRef().template is<T>()) {
                        return this->getJSONRef().template as<T>();
                    }
                    else {
                        this->setError(
                            "unexpected type {}",
                            this->matJsonTypeToString(this->getJSONRef().type())
                        );
                    }
                }
                // matjson can throw variant exceptions too so you need to do this
                catch(std::exception const& e) {
                    this->setError("unable to parse json: {}", e);
                }
            }
            return std::nullopt;
        }

    public:
        JsonExpectedValue(matjson::Value const& value, std::string_view rootScopeName);
        ~JsonExpectedValue();

        JsonExpectedValue(JsonExpectedValue&&);
        JsonExpectedValue& operator=(JsonExpectedValue&&);
        JsonExpectedValue(JsonExpectedValue const&) = delete;
        JsonExpectedValue& operator=(JsonExpectedValue const&) = delete;

        /**
         * Get a copy of the underlying raw JSON value
         */
        matjson::Value json() const;
        /**
         * Get the key name of this JSON value. If this is an array index, 
         * returns the index as a string. If this is the root object, 
         * returns the root scope name. 
         */
        std::string key() const;

        /**
         * Check the type of this JSON value. Does not set an error. If an 
         * error is already set, always returns false
         */
        bool is(matjson::Type type) const;
        bool isNull() const;
        bool isBool() const;
        bool isNumber() const;
        bool isString() const;
        bool isArray() const;
        bool isObject() const;
        /**
         * Asserts that this JSON value is of the specified type. If it is 
         * not, an error is set and all subsequent operations are no-ops
         * @returns Itself
         */
        JsonExpectedValue& assertIs(matjson::Type type);
        JsonExpectedValue& assertIsNull();
        JsonExpectedValue& assertIsBool();
        JsonExpectedValue& assertIsNumber();
        JsonExpectedValue& assertIsString();
        JsonExpectedValue& assertIsArray();
        JsonExpectedValue& assertIsObject();
        /**
         * Asserts that this JSON value is one of a list of specified types
         * @returns Itself
         */
        JsonExpectedValue& assertIs(std::initializer_list<matjson::Type> type);

        // -- Dealing with values --

        template <class T>
        T get(T const& defaultValue = T()) {
            if (auto v = this->template tryGet<T>()) {
                return *std::move(v);
            }
            return defaultValue;
        }
        template <class T>
        JsonExpectedValue& into(T& value) {
            if (auto v = this->template tryGet<T>()) {
                value = *std::move(v);
            }
            return *this;
        }
        template <class T>
        JsonExpectedValue& into(std::optional<T>& value) {
            if (auto v = this->template tryGet<T>()) {
                value.emplace(*std::move(v));
            }
            return *this;
        }
        template <class T>
        JsonExpectedValue& mustBe(std::string_view name, auto predicate) requires requires {
            { predicate(std::declval<T>()) } -> std::convertible_to<bool>;
        } {
            if (this->hasError()) return *this;
            if (auto v = this->template tryGet<T>()) {
                if (!predicate(*v)) {
                    this->setError("json value is not {}", name);
                }
            }
            return *this;
        }
        template <class T>
        JsonExpectedValue& mustBe(std::string_view name, auto predicate) requires requires {
            { predicate(std::declval<T>()) } -> std::convertible_to<Result<>>;
        } {
            if (this->hasError()) return *this;
            if (auto v = this->template tryGet<T>()) {
                auto p = predicate(*v);
                if (!p) {
                    this->setError("json value is not {}: {}", name, p.unwrapErr());
                }
            }
            return *this;
        }

        // -- Dealing with objects --

        /**
         * Check if this object has an optional key. Asserts that this JSON 
         * value is an object. If the key doesn't exist, returns a 
         * `JsonExpectValue` that does nothing
         * @returns The key, which is a no-op value if it didn't exist
         */
        JsonExpectedValue has(std::string_view key);
        /**
         * Check if this object has an optional key. Asserts that this JSON 
         * value is an object. If the key doesn't exist, sets an error and 
         * returns a `JsonExpectValue` that does nothing
         * @returns The key, which is a no-op value if it didn't exist
         */
        JsonExpectedValue needs(std::string_view key);
        /**
         * Asserts that this JSON value is an object. Get all object 
         * properties
         */
        std::vector<std::pair<std::string, JsonExpectedValue>> properties();
        /**
         * Asserts that this JSON value is an object. Logs unknown keys to 
         * the console as warnings
         */
        void checkUnknownKeys();

        // -- Dealing with arrays --

        /**
         * Asserts that this JSON value is an array. Returns the length of 
         * the array, or 0 on error
         */
        size_t length();
        /**
         * Asserts that this JSON value is an array. Returns the value at 
         * the specified index. If there is no value at that index, sets an 
         * error
         */
        JsonExpectedValue at(size_t index);
        /**
         * Asserts that this JSON value is an array. Returns the array items
         * @warning The old JsonChecker used `items` for iterating object 
         * properties - on this new API that function is called `properties`!
         */
        std::vector<JsonExpectedValue> items();

        operator bool() const;

        Result<> ok();
        template <class T>
        Result<T> ok(T value) {
            auto ok = this->ok();
            if (!ok) {
                return Err(ok.unwrapErr());
            }
            return Ok(std::forward<T>(value));
        }
    };
    GEODE_DLL JsonExpectedValue checkJson(matjson::Value const& json, std::string_view rootScopeName);
}
