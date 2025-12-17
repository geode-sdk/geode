#pragma once

#include <matjson.hpp>
#include "../loader/Log.hpp"
#include <set>
#include <variant>
#include <Geode/Result.hpp>
#include <Geode/utils/function.hpp>

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
    using JsonValueValidator = geode::Function<bool(T const&)>;

    struct JsonMaybeObject;
    struct JsonMaybeValue;

    class GEODE_DLL JsonExpectedValue final {
    protected:
        class Impl;
        std::unique_ptr<Impl> m_impl;

        JsonExpectedValue();
        JsonExpectedValue(Impl* from, matjson::Value& scope, std::string key);

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
                auto res = this->getJSONRef().as<T>();
                if (res) {
                    return res.unwrap();
                }
                this->setError(
                    "unexpected type {}",
                    this->matJsonTypeToString(this->getJSONRef().type())
                );
            }
            return std::nullopt;
        }

    public:
        JsonExpectedValue(matjson::Value value, std::string rootScopeName);
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
            if (auto v = this->tryGet<T>()) {
                return *std::move(v);
            }
            return defaultValue;
        }
        template <class T>
        JsonExpectedValue& into(T& value) {
            if (auto v = this->tryGet<T>()) {
                value = *std::move(v);
            }
            return *this;
        }
        template <class T>
        JsonExpectedValue& into(std::optional<T>& value) {
            if (this->isNull()) return *this;
            if (auto v = this->tryGet<T>()) {
                value.emplace(*std::move(v));
            }
            return *this;
        }
        template <class T>
        JsonExpectedValue& mustBe(std::string_view name, auto predicate) requires requires {
            { predicate(std::declval<T>()) } -> std::convertible_to<bool>;
        } {
            if (this->hasError()) return *this;
            if (auto v = this->tryGet<T>()) {
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
            if (auto v = this->tryGet<T>()) {
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
        JsonExpectedValue has(std::string key);
        /**
         * Check if this object has an optional key. Asserts that this JSON
         * value is an object. If the key doesn't exist, or the value is null, returns a
         * `JsonExpectValue` that does nothing
         * @returns The key, which is a no-op value if it didn't exist, or was null
         */
        JsonExpectedValue hasNullable(std::string key);
        /**
         * Check if this object has an optional key. Asserts that this JSON
         * value is an object. If the key doesn't exist, sets an error and
         * returns a `JsonExpectValue` that does nothing
         * @returns The key, which is a no-op value if it didn't exist
         */
        JsonExpectedValue needs(std::string key);
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
    GEODE_DLL JsonExpectedValue checkJson(matjson::Value json, std::string rootScopeName);
}
