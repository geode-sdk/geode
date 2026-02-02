#pragma once

#include <coroutine>
#include <Geode/DefaultInclude.hpp>
#include "Task.hpp"
#include <concepts>
#include <arc/future/Future.hpp>

namespace geode::utils::coro {
    /**
     * A simple generator class that allows yielding values from a coroutine.
     * Compatible with references and copy-constructible types.
     */
    template <typename T> requires (std::copy_constructible<T> || std::is_reference_v<T>)
    class Generator final {
        using StoredT = std::conditional_t<std::is_reference_v<T>, std::reference_wrapper<std::remove_reference_t<T>>, T>;
    public:
        struct promise_type {
            std::optional<StoredT> m_value;

            Generator get_return_object() {
                return Generator(std::coroutine_handle<promise_type>::from_promise(*this));
            }

            // lazy. don't run anything until you really need to
            std::suspend_always initial_suspend() noexcept { return {}; }
            std::suspend_always final_suspend() noexcept { return {}; }
            void unhandled_exception() {}

            // end of iteration
            void return_void() {
                m_value = {};
            }

            std::suspend_always yield_value(T value) {
                m_value = value;
                return {};
            }
        };

        class iterator_type {
            std::coroutine_handle<promise_type> m_handle;
            std::ptrdiff_t m_count;

            iterator_type(std::coroutine_handle<promise_type> handle, std::ptrdiff_t count)
              : m_handle(handle), m_count(count) {}
            friend class Generator;
        public:
            using iterator_category = std::input_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = T;
            using pointer           = StoredT*;
            using reference         = T&;

            // wait for next yield
            iterator_type& operator++() {
                m_handle.resume();
                if (m_handle.done()) {
                    m_handle.promise().m_value = {};
                }
                m_count++;
                return *this;
            }

            // m_count is -1 for end iterator
            bool operator==(iterator_type const& it) const {
                if (it.m_count == -1)
                    return !m_handle || m_handle.promise().m_value == std::nullopt;
                return m_handle == it.m_handle && m_count == it.m_count;
            }

            // better have the value
            T operator*() const {
                return m_handle.promise().m_value.value();
            }
        };
    private:
        std::coroutine_handle<promise_type> m_handle;
        Generator(std::coroutine_handle<promise_type> handle) : m_handle(handle) {}
    public:
        Generator() = delete;
        Generator(Generator&& other) : m_handle(std::exchange(other.m_handle, nullptr)) {}
        Generator(Generator const&) = delete;
        Generator& operator=(Generator&& other) {
            if (this != &other) {
                if (m_handle) m_handle.destroy();
                m_handle = std::exchange(other.m_handle, nullptr);
            }
            return *this;
        }
        Generator& operator=(Generator const&) = delete;

        ~Generator() {
            if (m_handle) m_handle.destroy();
        }

        iterator_type begin() {
            if (m_handle) {
                m_handle.resume();
                if (m_handle.done()) {
                    m_handle.promise().m_value = {};
                }
            }
            return iterator_type(m_handle, 0);
        }
        iterator_type end() { return iterator_type({}, -1); }


        template <std::invocable<T&&> U>
        Generator<std::invoke_result_t<U, T&&>> map(U&& func) && {
            for (auto&& value : *this) {
                co_yield func(value);
            }
        }

        template <std::predicate<T const&> U>
        Generator<T> filter(U&& func) && {
            for (auto&& value : *this) {
                if (func(value)) {
                    co_yield value;
                }
            }
        }
    };

    template <typename T>
    Generator<T> makeGenerator(std::vector<T> const& vec) {
        for (auto const& item : vec) {
            co_yield item;
        }
    }
    template <typename T>
    Generator<T> makeGenerator(std::vector<T>&& vec) {
        for (auto&& item : vec) {
            co_yield std::move(item);
        }
    }
    template <typename T, typename E>
    Generator<T> makeGenerator(Result<T, E> const& res) {
        if (res.isOk()) {
            co_yield res;
        }
    }
    template <typename T>
    Generator<T> makeGenerator(cocos2d::CCArray* arr) {
        if (!arr)
            co_return;

        for (int i = 0; i < arr->count(); ++i) {
            if (auto obj = typeinfo_cast<T*>(arr->objectAtIndex(i)))
                co_yield obj;
        }
    }

    GEODE_DLL arc::Future<void> nextFrame();
    GEODE_DLL arc::Future<void> skipFrames(int frames);

    template <typename T, typename E>
    struct BaseResultPromise {
        std::optional<Result<T, E>>* result;

        struct return_object {
            std::unique_ptr<std::optional<Result<T, E>>> ptr;
            operator Result<T, E>() noexcept {
                return ptr->value();
            }
        };

        std::suspend_never initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        return_object get_return_object() noexcept {
            auto ptr = std::make_unique<std::optional<Result<T, E>>>();
            result = &*ptr;
            return return_object {std::move(ptr)};
        }

        void unhandled_exception() const noexcept {}
    };

    template <typename T, typename E>
    struct ResultPromise : public BaseResultPromise<T, E> {
        void return_value(Result<T, E>&& value) noexcept {
            *this->result = std::move(value);
        }
    };

    template <typename E>
    struct TryResultPromise : public BaseResultPromise<void, E> {
        void return_void() noexcept {
            *this->result = Ok();
        }
    };

    template <typename T, typename E>
    struct ResultAwaiter {
        Result<T, E> result;

        bool await_ready() const noexcept { return result.isOk(); }
        T&& await_resume() noexcept { return std::move(result.unwrap()); }

        template <std::convertible_to<Result<T, E>> Q>
        ResultAwaiter(Q&& res) : result(std::forward<Q>(res)) {}

        template <typename U>
        void await_suspend(std::coroutine_handle<U> handle) noexcept {
            *handle.promise().result = Err(result.unwrapErr());
            handle.destroy();
        }
    };

    template <typename E>
    struct ResultAwaiter<void, E> {
        Result<void, E> result;

        bool await_ready() const noexcept { return result.isOk(); }
        void await_resume() const noexcept { return; }

        template <std::convertible_to<Result<void, E>> Q>
        ResultAwaiter(Q&& res) : result(std::forward<Q>(res)) {}

        template <typename U>
        void await_suspend(std::coroutine_handle<U> handle) noexcept {
            *handle.promise().result = Err(result.unwrapErr());
            handle.destroy();
        }
    };

    template <typename T>
    concept ConvertibleToTask = requires(T t) { Task(t); };

    /// Utility to spawn coroutines from non-coroutine code.
    static struct {
        template <std::invocable F>
        decltype(auto) operator<<(F&& fn) {
            return fn();
        }

        template <std::invocable F> requires (ConvertibleToTask<std::invoke_result_t<F>> && std::copy_constructible<F>)
        decltype(auto) operator<<(F&& fn) {
            auto ptr_fn = new F(fn);

            auto task = (*ptr_fn)();

            task.listen([ptr_fn](auto const&){
                delete ptr_fn;
            });

            return std::make_tuple(std::move(task));
        }

        template <typename T> requires ConvertibleToTask<T>
        decltype(auto) operator<<(T&& item) {
            auto task = Task(std::forward<T>(item));
            task.listen([](auto const&){});

            return std::make_tuple(std::move(task));
        }

        template <typename T>
        decltype(auto) operator()(T&& item) {
            return *this << std::forward<T>(item);
        }
    } spawn;

    template <typename T = void, typename E = std::string>
    using TryResult = std::conditional_t<std::same_as<T, void>, std::tuple<Result<void, E>>, Result<T, E>>;

    #define $async(...) geode::utils::coro::spawn << [__VA_ARGS__]() -> geode::Task<void>
    #define $try geode::utils::coro::spawn << [&]() -> geode::utils::coro::TryResult
};

template <typename T = void, typename E = std::string>
auto operator co_await(geode::Result<T, E>&& res) noexcept {
    return geode::utils::coro::ResultAwaiter<T, E> { std::move(res) };
}
template <typename T = void, typename E = std::string>
auto operator co_await(geode::Result<T, E> const& res) noexcept {
    return geode::utils::coro::ResultAwaiter<T, E> { res };
}

template <typename T, typename E, typename ...Args>
struct std::coroutine_traits<geode::Result<T, E>, Args...> {
    using promise_type = geode::utils::coro::ResultPromise<T, E>;
};

template <typename E, typename ...Args>
struct std::coroutine_traits<std::tuple<geode::Result<void, E>>, Args...> {
    using promise_type = geode::utils::coro::TryResultPromise<E>;
};

template <typename T, typename ...Args>
struct std::coroutine_traits<geode::utils::coro::Generator<T>, Args...> {
    using promise_type = geode::utils::coro::Generator<T>::promise_type;
};
