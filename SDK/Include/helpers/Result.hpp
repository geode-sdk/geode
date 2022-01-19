#pragma once

//#include <Macros.hpp>
#include <string_view>

namespace lilac {
    /**
     * @struct no_result
     * The default value of a Result.
     * Represents a Result that has no
     * data to carry on Success, but
     * may still return extended
     * error information on failure.
     */
    struct no_result {};

    /**
     * @class Result
     * Represents an optional value with an
     * associated error state and -information.
     * Use the Ok<T> and Err<T> classes to
     * create Result(s).
     * @param T Success result value type
     * @param E Error message type
     * @authors Matcool, HJfod
     */
    template <class T = no_result, class E = std::string>
    class Result {
    protected:
        bool success;
        union {
            T my_value;
            E error_msg;
        };
        Result(const T value) : success(true), my_value(value) {}
        Result(const E error, int dummy) : success(false), error_msg(error) {}

    public:
        /**
         * Destructor
         */
        ~Result() {
            if (success) {
                if (std::is_destructible<T>::value) {
                    my_value.~T();
                }
            } else {
                if (std::is_destructible<E>::value) {
                    error_msg.~E();
                }
            }
        }
        
        /**
         * Copy another Result of the same type
         */
        Result(Result<T, E> const& other) {
            if (other.success) {
                this->success = true;
                new (&this->my_value) T(other.value());
            } else {
                this->success = false;
                new (&this->error_msg) E(other.error());
            }
        }

        /**
         * Copy another Result of a convertible type
         */
        template<class T2, class E2>
        Result(Result<T2, E2> const& other) {
            if (other) {
                this->success = true;
                if constexpr (!std::is_same<T, no_result>::value) {
                    this->my_value = other.value();
                }
            } else {
                this->success = false;
                this->error_msg = other.error();
            }
        }

        /**
         * Check if Result was errorful
         * @returns True if errorful
         */
        bool is_error() const { return !success; }

        /**
         * Check if Result was succesful
         * @returns True if succesful
         */
        bool is_value() const { return success; }
        
        /**
         * Get the success value of a Result
         * @returns Value
         */
        auto value() const { return my_value; }
        
        /**
         * Get the error message of a Result
         * @returns Error
         */
        auto error() const { return error_msg; }

        /**
         * Convert to bool
         * @example if (result) { <handle success> } else { <handle failure> }
         */
        operator bool() const { return this->success; }

        /**
         * Create a success result
         * @param value Value
         */
        static auto ok(const T value) { return Result<T>(value); }
        
        /**
         * Create an error result
         * @param error Error information
         */
        static auto err(E error) { return Result<T>(error, 0); }
    };

    /**
     * Create a succesful Result with a value
     * @param value The value. If none is provided, the type will be no_result
     * @returns Successful Result
     */
    template <class T = no_result>
    Result<T> Ok(T value = T()) {
        return Result<T>::ok(value);
    }

    /**
     * @class Err
     * Create an errorful Result with an error message
     * @param value Error message
     * @returns Errorful Result
     */
    template <class E = std::string>
    struct Err {
        const E _value;
        Err(const E value) : _value(value) {}
        template <class T>
        operator Result<T>() const {
            return Result<T>::err(_value);
        }
    };
}
