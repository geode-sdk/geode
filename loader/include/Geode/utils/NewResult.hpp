#pragma once

#include "../DefaultInclude.hpp"
#include <string_view>
#include <string>
#include <variant>

namespace geode {
    namespace {
        struct AnyType {
            explicit AnyType() = delete;
        };

        template<class V, class RV>
        concept ConvertibleToResult =
            std::is_convertible_v<
                std::remove_reference_t<V>,
                std::remove_reference_t<RV>
            > || std::is_same_v<
                std::remove_reference_t<V>,
                std::remove_reference_t<RV>
            >;
        
        using DefaultValue = std::monostate;
        using DefaultError = std::string;
    }

    template<class T = DefaultValue, class E = DefaultError>
    class [[nodiscard]] NewResult {
    public:
        using value_type = std::remove_reference_t<T>;
        using error_type = std::remove_reference_t<E>;

        // for some reason doing requires causes errors with pch...
        static_assert(
            std::is_copy_constructible_v<value_type> || 
            std::is_move_constructible_v<value_type>,
            "T must be copiable or movable!"
        );
        static_assert(
            std::is_copy_constructible_v<error_type> || 
            std::is_move_constructible_v<error_type>,
            "E must be copiable or movable!"
        );

    protected:
        std::variant<value_type, error_type> m_value;
    
    public:
        bool isOk() const {
            return std::holds_alternative<value_type>(m_value);
        }

        bool isErr() const {
            return std::holds_alternative<error_type>(m_value);
        }

        explicit NewResult(value_type const& value)
            requires std::is_copy_constructible_v<value_type>
          : m_value(value) {}
        explicit NewResult(value_type&& value)
            requires std::is_move_constructible_v<value_type>
          : m_value(std::forward<value_type>(value)) {}

        explicit NewResult(error_type const& value)
            requires std::is_copy_constructible_v<error_type>
          : m_value(value) {}
        explicit NewResult(error_type&& value)
            requires std::is_move_constructible_v<error_type>
          : m_value(std::forward<error_type>(value)) {}
        
        NewResult(NewResult<T, E> const& other)
            requires
                std::is_copy_constructible_v<value_type> &&
                std::is_copy_constructible_v<error_type>
          = default;
        
        NewResult(NewResult<T, E>&& other)
            requires (
                !std::is_copy_constructible_v<value_type> ||
                !std::is_copy_constructible_v<error_type>
            ) = default;
        
        template<class T2, class E2>
            requires ConvertibleToResult<T2, T> && ConvertibleToResult<E2, E>
        NewResult(NewResult<T2, E2> const& other)
            requires
                std::is_copy_constructible_v<value_type> &&
                std::is_copy_constructible_v<error_type>
          : m_value(other.isOk() ? other.unwrap() : other.unwrapErr()) {}
        
        template<class T2, class E2>
            requires ConvertibleToResult<T2, T> && ConvertibleToResult<E2, E>
        NewResult(NewResult<T2, E2>&& other)
            requires (
                !std::is_copy_constructible_v<value_type> ||
                !std::is_copy_constructible_v<error_type>
            ) : m_value(other.isOk() ? other.unwrap() : other.unwrapErr()) {}
        
        template<class T2>
            requires ConvertibleToResult<T2, T>
        NewResult(NewResult<T2, AnyType> const& other)
            requires
                std::is_copy_constructible_v<value_type> &&
                std::is_copy_constructible_v<error_type>
          : NewResult(value_type(other.unwrap())) {}
        
        template<class E2>
            requires ConvertibleToResult<E2, E>
        NewResult(NewResult<AnyType, E2> const& other)
            requires
                std::is_copy_constructible_v<value_type> &&
                std::is_copy_constructible_v<error_type>
          : m_value(std::forward<E2>(other.unwrapErr())) {}
        
        template<class T2>
            requires ConvertibleToResult<T2, T>
        NewResult(NewResult<T2, AnyType>&& other)
            requires (
                !std::is_copy_constructible_v<value_type> ||
                !std::is_copy_constructible_v<error_type>
            ) : m_value(other.unwrap()) {}
        
        template<class E2>
            requires ConvertibleToResult<E2, E>
        NewResult(NewResult<AnyType, E2>&& other)
            requires (
                !std::is_copy_constructible_v<value_type> ||
                !std::is_copy_constructible_v<error_type>
            ) : NewResult(std::forward<error_type>(other.unwrapErr())) {}

        value_type unwrap() const
            requires std::is_copy_constructible_v<value_type>
        {
            return std::get<value_type>(m_value);
        }

        value_type&& unwrap()
            requires (!std::is_copy_constructible_v<value_type>)
        {
            return std::move(std::get<value_type>(m_value));
        }

        error_type unwrapErr() const
            requires std::is_copy_constructible_v<error_type>
        {
            return std::get<error_type>(m_value);
        }

        error_type&& unwrapErr()
            requires (!std::is_copy_constructible_v<error_type>)
        {
            return std::move(std::get<error_type>(m_value));
        }

        explicit operator bool() const requires (
            !std::is_same_v<T, bool> &&
            !std::is_same_v<E, bool>
        ) {
            return this->isOk();
        }
    };

    template<class T>
    class [[nodiscard]] NewResult<T, T> {
    public:
        using value_type = std::remove_reference_t<T>;
        using error_type = std::remove_reference_t<T>;

        // for some reason doing requires causes errors with pch...
        static_assert(
            std::is_copy_constructible_v<value_type> || 
            std::is_move_constructible_v<value_type>,
            "T must be copiable or movable!"
        );

    protected:
        bool m_success;
        value_type m_value;
    
    public:
        bool isOk() const {
            return m_success;
        }

        bool isErr() const {
            return !m_success;
        }

        explicit NewResult(value_type const& value, bool success)
            requires std::is_copy_constructible_v<value_type>
          : m_value(value), m_success(success) {}

        explicit NewResult(value_type&& value, bool success)
            requires std::is_move_constructible_v<value_type>
          : m_value(std::forward<value_type>(value)), m_success(success) {}

        NewResult(NewResult<T, T> const& other)
            requires std::is_copy_constructible_v<value_type> = default;
        
        NewResult(NewResult<T, T>&& other)
            requires (!std::is_copy_constructible_v<value_type>) = default;
        
        template<class T2, class E2>
            requires ConvertibleToResult<T2, T> && ConvertibleToResult<E2, T>
        NewResult(NewResult<T2, E2> const& other)
            requires
                std::is_copy_constructible_v<value_type>
          : m_value(other.isOk() ? other.unwrap() : other.unwrapErr()),
            m_success(other.isOk()) {}
        
        template<class T2, class E2>
            requires ConvertibleToResult<T2, T> && ConvertibleToResult<E2, T>
        NewResult(NewResult<T2, E2>&& other)
            requires (
                !std::is_copy_constructible_v<value_type>
            ) : m_value(other.isOk() ? other.unwrap() : other.unwrapErr()),
                m_success(other.isOk()) {}
        
        template<class T2>
            requires ConvertibleToResult<T2, T>
        NewResult(NewResult<T2, AnyType> const& other)
            requires
                std::is_copy_constructible_v<value_type>
          : NewResult(value_type(other.unwrap()), true) {}
        
        template<class T2>
            requires ConvertibleToResult<T2, T>
        NewResult(NewResult<T2, AnyType>&& other)
            requires (
                !std::is_copy_constructible_v<value_type>
            ) : NewResult(std::forward<value_type>(other.unwrap()), true) {}
        
        template<class E2>
            requires ConvertibleToResult<E2, T>
        NewResult(NewResult<AnyType, E2> const& other)
            requires
                std::is_copy_constructible_v<value_type>
          : NewResult(error_type(other.unwrapErr()), false) {}
        
        template<class E2>
            requires ConvertibleToResult<E2, T>
        NewResult(NewResult<AnyType, E2>&& other)
            requires (
                !std::is_copy_constructible_v<value_type>
            ) : NewResult(std::forward<error_type>(other.unwrapErr()), false) {}

        value_type unwrap() const
            requires std::is_copy_constructible_v<value_type>
        {
            return m_value;
        }

        value_type&& unwrap()
            requires (!std::is_copy_constructible_v<value_type>)
        {
            return std::move(m_value);
        }

        error_type unwrapErr() const
            requires std::is_copy_constructible_v<error_type>
        {
            return m_value;
        }

        error_type&& unwrapErr()
            requires (!std::is_copy_constructible_v<error_type>)
        {
            return std::move(m_value);
        }

        explicit operator bool() const requires (
            !std::is_same_v<T, bool>
        ) {
            return this->isOk();
        }
    };

    template<class T = DefaultValue, class E = AnyType>
        requires std::is_copy_constructible_v<T>
    NewResult<T, E> NewOk(T value = T()) {
        return NewResult<T, E>(value);
    }

    template<class T = DefaultValue, class E = AnyType>
        requires (!std::is_copy_constructible_v<T>)
    NewResult<T, E> NewOk(T&& value) {
        return NewResult<T, E>(std::forward<T>(value));
    }

    template<class E = DefaultError, class T = AnyType>
        requires std::is_copy_constructible_v<E>
    NewResult<T, E> NewErr(E error = E()) {
        return NewResult<T, E>(error);
    }

    template<class E = DefaultError, class T = AnyType>
        requires (!std::is_copy_constructible_v<E>)
    NewResult<T, E> NewErr(E&& error) {
        return NewResult<T, E>(std::forward<E>(error));
    }

    #define GEODE_UNWRAP_INTO(into, ...) \
        auto GEODE_CONCAT(res_, __LINE__) = (__VA_ARGS__);\
        if (GEODE_CONCAT(res_, __LINE__).isErr()) {\
            return Err(std::move(GEODE_CONCAT(res_, __LINE__).unwrapErr()));\
        }\
        into = std::move(GEODE_CONCAT(res_, __LINE__).unwrap())

    #define GEODE_UNWRAP(...) \
        { auto GEODE_CONCAT(res_, __LINE__) = (__VA_ARGS__);\
        if (GEODE_CONCAT(res_, __LINE__).isErr()) {\
            return Err(std::move(GEODE_CONCAT(res_, __LINE__).unwrapErr()));\
        } }
}
