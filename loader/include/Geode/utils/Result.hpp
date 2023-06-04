#pragma once

#include "../DefaultInclude.hpp"
#include "../external/result/result.hpp"

#include <fmt/format.h>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
#include <optional>

namespace geode {
    namespace impl {
        using DefaultValue = std::monostate;
        using DefaultError = std::string;
        template <class T>
        using WrappedResult = std::conditional_t<
            std::is_lvalue_reference_v<T>, std::reference_wrapper<std::remove_reference_t<T>>,
            std::remove_const_t<T>>;

        template <class E = impl::DefaultError>
        class [[nodiscard]] Failure {
        public:
            WrappedResult<E> m_error;

            Failure() = default;

            template <class E2>
                requires(std::is_constructible_v<E, E2 const&>)
            explicit constexpr Failure(E2 const& e) : m_error(e) {}

            template <class E2>
                requires(std::is_constructible_v<E, E2 &&>)
            explicit constexpr Failure(E2&& e) : m_error(std::move(e)) {}

            E& error() & noexcept {
                return m_error;
            }

            E const& error() const& noexcept {
                return m_error;
            }

            E&& error() && noexcept {
                return static_cast<E&&>(m_error);
            }

            E const&& error() const&& noexcept {
                return static_cast<E&&>(m_error);
            }
        };

        template <class T = impl::DefaultValue>
        class [[nodiscard]] Success {
        public:
            WrappedResult<T> m_value;

            Success() = default;

            template <class T2>
                requires(std::is_constructible_v<T, T2 const&>)
            explicit constexpr Success(T2 const& v) : m_value(v) {}

            template <class T2>
                requires(std::is_constructible_v<T, T2 &&>)
            explicit constexpr Success(T2&& v) : m_value(std::forward<T2>(v)) {}

            T& value() & noexcept {
                return m_value;
            }

            T const& value() const& noexcept {
                return m_value;
            }

            T&& value() && noexcept {
                return static_cast<T&&>(m_value);
            }

            T const&& value() const&& noexcept {
                return static_cast<T&&>(m_value);
            }
        };
    }

    template <class T = impl::DefaultValue, class E = impl::DefaultError>
    class [[nodiscard]] Result : public cpp::result<T, E> {
    public:
        using Base = cpp::result<T, E>;
        using ValueType = typename Base::value_type;
        using ErrorType = typename Base::error_type;

        using Base::result;

        template <class U>
            requires(cpp::detail::result_is_implicit_value_convertible<T, U>::value)
        constexpr Result(U&& value) = delete;

        template <class E2>
            requires(std::is_constructible_v<E, E2 const&>)
        constexpr Result(impl::Failure<E2> const& e) : Base(cpp::failure<E>(e.error())) {}

        template <class E2>
            requires(std::is_constructible_v<E, E2 &&>)
        constexpr Result(impl::Failure<E2>&& e) : Base(cpp::failure<E>(std::move(e.error()))) {}

        template <class T2>
            requires(std::is_constructible_v<T, T2 const&>)
        constexpr Result(impl::Success<T2> const& s) : Base(s.value()) {}

        template <class T2>
            requires(std::is_constructible_v<T, T2 &&>)
        constexpr Result(impl::Success<T2>&& s) : Base(std::move(s.value())) {}

        [[nodiscard]] constexpr explicit operator bool() const noexcept {
            return this->Base::operator bool();
        }

        [[nodiscard]] constexpr bool isOk() const noexcept {
            return this->Base::has_value();
        }

        [[nodiscard]] constexpr bool isErr() const noexcept {
            return this->Base::has_error();
        }

        [[nodiscard]] constexpr decltype(auto) unwrap() & {
            return this->Base::value();
        }

        [[nodiscard]] constexpr decltype(auto) unwrap() const& {
            return this->Base::value();
        }

        [[nodiscard]] constexpr decltype(auto) unwrap() && {
            return this->Base::value();
        }

        [[nodiscard]] constexpr decltype(auto) unwrap() const&& {
            return this->Base::value();
        }

        [[nodiscard]] constexpr decltype(auto) unwrapErr() & {
            return this->Base::error();
        }

        [[nodiscard]] constexpr decltype(auto) unwrapErr() const& {
            return this->Base::error();
        }

        [[nodiscard]] constexpr decltype(auto) unwrapErr() && {
            return this->Base::error();
        }

        [[nodiscard]] constexpr decltype(auto) unwrapErr() const&& {
            return this->Base::error();
        }

        template <class... Args>
            requires(std::is_constructible_v<T, T &&>)
        [[nodiscard]] Result<T, std::string> expect(std::string const& format, Args&&... args) {
            if (this->isErr()) {
                return impl::Failure<std::string>(fmt::format(
                    fmt::runtime(format), std::forward<Args>(args)...,
                    fmt::arg("error", this->unwrapErr())
                ));
            }
            else {
                return std::move(*this);
            }
        }

        template <class... Args>
            requires(std::is_constructible_v<T, T const&>)
        [[nodiscard]] Result<T, std::string> expect(std::string const& format, Args&&... args)
            const {
            if (this->isErr()) {
                return impl::Failure<std::string>(fmt::format(
                    fmt::runtime(format), std::forward<Args>(args)...,
                    fmt::arg("error", this->unwrapErr())
                ));
            }
            else {
                return *this;
            }
        }

        template <class U>
        [[nodiscard]] constexpr decltype(auto) unwrapOr(U&& val) && {
            return this->Base::value_or(std::forward<U>(val));
        }

        template <class U>
        [[nodiscard]] constexpr decltype(auto) unwrapOr(U&& val) const& {
            return this->Base::value_or(std::forward<U>(val));
        }

        template <class U>
        [[nodiscard]] constexpr decltype(auto) errorOr(U&& val) && {
            return this->Base::error_or(std::forward<U>(val));
        }

        template <class U>
        [[nodiscard]] constexpr decltype(auto) errorOr(U&& val) const& {
            return this->Base::error_or(std::forward<U>(val));
        }

        /**
         * Convert the result into an optional containing the value if Ok, and 
         * nullopt if Err
         */
        [[nodiscard]] constexpr std::optional<T> ok() const& {
            if (this->isOk()) {
                return this->unwrap();
            }
            return std::nullopt;
        }

        /**
         * Convert the result into an optional containing the value if Ok, and 
         * nullopt if Err
         */
        [[nodiscard]] constexpr std::optional<T> ok() && {
            if (this->isOk()) {
                return this->unwrap();
            }
            return std::nullopt;
        }

        /**
         * Convert the result into an optional containing the error if Err, and 
         * nullopt if Ok
         */
        [[nodiscard]] constexpr std::optional<E> err() const& {
            if (this->isErr()) {
                return this->unwrapErr();
            }
            return std::nullopt;
        }

        /**
         * Convert the result into an optional containing the error if Err, and 
         * nullopt if Ok
         */
        [[nodiscard]] constexpr std::optional<E> err() && {
            if (this->isErr()) {
                return this->unwrapErr();
            }
            return std::nullopt;
        }
    };

    template <class T = impl::DefaultValue>
    constexpr impl::Success<T> Ok() {
        return impl::Success<T>();
    }

    template <class T>
    constexpr impl::Success<T> Ok(T&& value) {
        return impl::Success<T>(std::forward<T>(value));
    }

    template <class E>
    constexpr impl::Failure<E> Err(E&& error) {
        return impl::Failure<E>(std::forward<E>(error));
    }

    template <class... Args>
    inline impl::Failure<std::string> Err(std::string const& format, Args&&... args) {
        return impl::Failure<std::string>(
            fmt::format(fmt::runtime(format), std::forward<Args>(args)...)
        );
    }

#define GEODE_UNWRAP_INTO(into, ...)                                            \
    auto GEODE_CONCAT(unwrap_res_, __LINE__) = (__VA_ARGS__);                   \
    if (GEODE_CONCAT(unwrap_res_, __LINE__).isErr()) {                          \
        return geode::Err(std::move(GEODE_CONCAT(unwrap_res_, __LINE__).unwrapErr())); \
    }                                                                           \
    into = std::move(GEODE_CONCAT(unwrap_res_, __LINE__).unwrap())

#define GEODE_UNWRAP(...)                                                           \
    {                                                                               \
        auto GEODE_CONCAT(unwrap_res_, __LINE__) = (__VA_ARGS__);                   \
        if (GEODE_CONCAT(unwrap_res_, __LINE__).isErr()) {                          \
            return geode::Err(std::move(GEODE_CONCAT(unwrap_res_, __LINE__).unwrapErr())); \
        }                                                                           \
    }
}
