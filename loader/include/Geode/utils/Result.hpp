#pragma once

#include "../DefaultInclude.hpp"

#include <string>
#include <string_view>
#include <variant>
#include <fmt/format.h>

// clang-format off

namespace geode {
    namespace {
        struct AnyType {
            explicit AnyType() = delete;
        };

        template <class V, class RV>
        concept ConvertibleToResult =
            std::is_convertible_v<std::remove_reference_t<V>, std::remove_reference_t<RV>> ||
            std::is_same_v<std::remove_reference_t<V>, std::remove_reference_t<RV>>;

        using DefaultValue = std::monostate;
        using DefaultError = std::string;

        template<class T, class E>
        struct Storage {
            std::variant<T, E> m_value;

            bool holdsOk() const {
                return std::holds_alternative<T>(m_value);
            }

            T getOk() const requires std::is_copy_constructible_v<T> {
                return std::get<T>(m_value);
            }

            T&& getOk() requires(!std::is_copy_constructible_v<T>) {
                return std::move(std::get<T>(m_value));
            }

            E getErr() const requires std::is_copy_constructible_v<E> {
                return std::get<E>(m_value);
            }

            E&& getErr() requires(!std::is_copy_constructible_v<E>) {
                return std::move(std::get<E>(m_value));
            }

            template<class T2, class E2>
                requires
                    std::is_convertible_v<T2, T> &&
                    std::is_convertible_v<E2, E>
            Storage(Storage<T2, E2> const& other)
                requires
                    std::is_copy_constructible_v<T> &&
                    std::is_copy_constructible_v<E>
            {
                if (other.holdsOk()) {
                    m_value = other.getOk();
                } else {
                    m_value = other.getErr();
                }
            }
            
            Storage(T const& value)
                requires std::is_copy_constructible_v<T>
              : m_value(value) {}

            Storage(T&& value)
                requires std::is_move_constructible_v<T>
              : m_value(std::forward<T>(value)) {}

            Storage(E const& value, std::monostate)
                requires std::is_copy_constructible_v<E>
              : m_value(value) {}

            Storage(E&& value, std::monostate)
                requires std::is_move_constructible_v<E>
              : m_value(std::forward<E>(value)) {}
        };

        template<class T>
        struct Storage<T, T> {
            bool m_holdsOk;
            T m_value;

            bool holdsOk() const {
                return m_holdsOk;
            }

            T getOk() const requires std::is_copy_constructible_v<T> {
                return m_value;
            }

            T&& getOk() requires(!std::is_copy_constructible_v<T>) {
                return std::move(m_value);
            }

            T getErr() const requires std::is_copy_constructible_v<T> {
                return m_value;
            }

            T&& getErr() requires(!std::is_copy_constructible_v<T>) {
                return std::move(m_value);
            }

            template<class T2, class E2>
                requires
                    std::is_convertible_v<T2, T> &&
                    std::is_convertible_v<E2, T>
            Storage(Storage<T2, E2> const& other)
                requires
                    std::is_copy_constructible_v<T> &&
                    std::is_copy_constructible_v<T>
               : m_value(other.holdsOk() ? other.getOk() : other.getErr()),
                 m_holdsOk(other.holdsOk()) {}
            
            Storage(T const& value)
                requires std::is_copy_constructible_v<T>
              : m_value(value),
                m_holdsOk(true) {}

            Storage(T&& value)
                requires std::is_move_constructible_v<T>
              : m_value(std::forward<T>(value)),
                m_holdsOk(true) {}

            Storage(T const& value, std::monostate)
                requires std::is_copy_constructible_v<T>
              : m_value(value),
                m_holdsOk(false) {}

            Storage(T&& value, std::monostate)
                requires std::is_move_constructible_v<T>
              : m_value(std::forward<T>(value)),
                m_holdsOk(false) {}
        };
    }

    template <class T = DefaultValue, class E = DefaultError>
    class [[nodiscard]] Result final {
    public:
        using value_type = std::remove_reference_t<T>;
        using error_type = std::remove_reference_t<E>;

        // for some reason doing requires causes errors with pch...
        static_assert(
            std::is_copy_constructible_v<value_type> || std::is_move_constructible_v<value_type>,
            "T must be copiable or movable!"
        );
        static_assert(
            std::is_copy_constructible_v<error_type> || std::is_move_constructible_v<error_type>,
            "E must be copiable or movable!"
        );

    protected:
        Storage<value_type, error_type> m_value;

    public:
        Storage<value_type, error_type> const& _Raw_Storage() const {
            return m_value;
        }

        bool isOk() const {
            return m_value.holdsOk();
        }

        bool isErr() const {
            return !m_value.holdsOk();
        }

        template<class ... Args>
        Result<T, std::string> expect(const char* str, Args&&... args) {
            if (isErr()) {
                return Result<T, std::string>(fmt::format(
                    str,
                    std::forward<Args>(args)...,
                    fmt::arg("error", unwrapErr())
                ), std::monostate());
            } else {
                return *this;
            }
        }

        explicit Result(value_type const& value)
            requires std::is_copy_constructible_v<value_type>
          : m_value(value) {}

        explicit Result(value_type&& value)
            requires std::is_move_constructible_v<value_type>
          : m_value(std::forward<value_type>(value)) {}

        explicit Result(error_type const& value, std::monostate)
            requires std::is_copy_constructible_v<error_type>
          : m_value(value, std::monostate()) {}

        explicit Result(error_type&& value, std::monostate)
            requires std::is_move_constructible_v<error_type>
          : m_value(std::forward<error_type>(value), std::monostate()) {}

        Result(Result<T, E> const& other)
            requires
                std::is_copy_constructible_v<value_type> &&
                std::is_copy_constructible_v<error_type>
            = default;

        Result(Result<T, E>&& other)
            requires(
                !std::is_copy_constructible_v<value_type> ||
                !std::is_copy_constructible_v<error_type>
            ) = default;

        template<class T2, class E2>
            requires
                std::is_convertible_v<T2, T> &&
                std::is_convertible_v<E2, E>
        Result(Result<T2, E2> const& other)
            requires
                std::is_copy_constructible_v<value_type> &&
                std::is_copy_constructible_v<error_type>
          : m_value(other._Raw_Storage()) {}

        template <class T2>
            requires ConvertibleToResult<T2, T>
        Result(Result<T2, AnyType> const& other)
            requires
                std::is_copy_constructible_v<value_type> &&
                std::is_copy_constructible_v<error_type>
          : Result(value_type(other.unwrap())) {}

        template <class E2>
            requires ConvertibleToResult<E2, E>
        Result(Result<AnyType, E2> const& other)
            requires
                std::is_copy_constructible_v<value_type> &&
                std::is_copy_constructible_v<error_type>
          : m_value(std::forward<E2>(other.unwrapErr()), std::monostate()) {}

        template <class T2>
            requires ConvertibleToResult<T2, T>
        Result(Result<T2, AnyType>&& other)
            requires(
                !std::is_copy_constructible_v<value_type> ||
                !std::is_copy_constructible_v<error_type>
            )
          : m_value(other.unwrap()) {}

        template <class E2>
            requires ConvertibleToResult<E2, E>
        Result(Result<AnyType, E2>&& other)
            requires(
                !std::is_copy_constructible_v<value_type> ||
                !std::is_copy_constructible_v<error_type>
            )
          : Result(std::forward<error_type>(other.unwrapErr()), std::monostate()) {}

        value_type unwrap() const requires std::is_copy_constructible_v<value_type> {
            return m_value.getOk();
        }

        value_type&& unwrap() requires(!std::is_copy_constructible_v<value_type>) {
            return std::forward<value_type>(m_value.getOk());
        }

        error_type unwrapErr() const requires std::is_copy_constructible_v<error_type> {
            return m_value.getErr();
        }

        error_type&& unwrapErr() requires(!std::is_copy_constructible_v<error_type>) {
            return std::forward<error_type>(m_value.getErr());
        }

        explicit operator bool() const requires(
            !std::is_same_v<T, bool> &&
            !std::is_same_v<E, bool>
        ) {
            return this->isOk();
        }
    };

    template <class T = DefaultValue, class E = AnyType>
        requires std::is_copy_constructible_v<T>
    Result<T, E> Ok(T value = T()) {
        return Result<T, E>(value);
    }

    template <class T = DefaultValue, class E = AnyType>
        requires(!std::is_copy_constructible_v<T>)
    Result<T, E> Ok(T&& value) {
        return Result<T, E>(std::forward<T>(value));
    }

    template <class E = DefaultError, class T = AnyType>
        requires std::is_copy_constructible_v<E>
    Result<T, E> Err(E error) {
        return Result<T, E>(error, std::monostate());
    }

    template <class E = DefaultError, class T = AnyType>
        requires(!std::is_copy_constructible_v<E>)
    Result<T, E> Err(E&& error) {
        return Result<T, E>(std::forward<E>(error), std::monostate());
    }

    #define GEODE_UNWRAP_INTO(into, ...)                                            \
        auto GEODE_CONCAT(unwrap_res_, __LINE__) = (__VA_ARGS__);                   \
        if (GEODE_CONCAT(unwrap_res_, __LINE__).isErr()) {                          \
            return Err(std::move(GEODE_CONCAT(unwrap_res_, __LINE__).unwrapErr())); \
        }                                                                           \
        into = std::move(GEODE_CONCAT(unwrap_res_, __LINE__).unwrap())

    #define GEODE_UNWRAP(...)                                                           \
        {                                                                               \
            auto GEODE_CONCAT(unwrap_res_, __LINE__) = (__VA_ARGS__);                   \
            if (GEODE_CONCAT(unwrap_res_, __LINE__).isErr()) {                          \
                return Err(std::move(GEODE_CONCAT(unwrap_res_, __LINE__).unwrapErr())); \
            }                                                                           \
        }
}

// clang-format on
