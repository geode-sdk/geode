#pragma once

#include <coroutine>
#include <Geode/DefaultInclude.hpp>
#include "Task.hpp"
#include <concepts>

namespace geode {
    /**
     * This is a geode utility that allows Tasks to be used with
     * C++'s own coroutine handling.
     * 
     * @tparam T The task type it will finish to. See `Task` for 
     * more information.
     * @tparam P The progress type the task posts. See `Task` for 
     * more information.
     */
    template <is_task_type T = void, typename P = std::monostate>
    class CoTask final {
        using Type = Task<T, P>::Type;
        Task<T, P> m_task;
     public:
        using promise_type = std::coroutine_traits<Task<T, P>>::promise_type;
        auto operator<=>(const CoTask&) const = default;
        operator Task<T, P>() { return std::move(m_task); }

        CoTask() = default;
        CoTask(CoTask&&) = default;
        CoTask(Task<T, P>&& task) : m_task(std::move(task)) {}


        Type* getFinishedValue() { return m_task.getFinishedValue(); }
        void cancel() { return m_task.cancel(); }
        void shallowCancel() { m_task.shallowCancel(); }
        bool isPending() const { return m_task.isPending(); }
        bool isFinished() const { return m_task.isFinished(); }
        bool isCancelled() const { return m_task.isCancelled(); }
        bool isNull() const { return m_task.isNull(); }

        Task<T, P> task() { return std::move(m_task); }

        static inline struct {
            template <typename F>
            decltype(auto) operator<<(F fn) { return fn(); }
        } invoke;
    };


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
            using pointer           = T*;
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
    };

    namespace geode_internal {
        template <typename T, typename E>
        struct promise_type {
            using Inner = std::optional<Result<T, E>>;

            struct: public std::shared_ptr<Inner> {
                using std::shared_ptr<Inner>::shared_ptr;
                operator Result<T>() {
                    return (*this->get()).value();
                }
            } result{new Inner()};

            std::suspend_never initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            auto get_return_object() { return result; }
            void unhandled_exception() {}
            void return_value(Result<T, E>&& value) { *result = std::move(value); }
        };

        template <typename T, typename E>
        struct Awaiter {
            Result<T, E> result;

            bool await_ready() { return result.isOk(); }
            T&& await_resume() { return std::move(result.unwrap()); }

            template <std::convertible_to<Result<T, E>> Q>
            Awaiter(Q&& res) : result(std::forward<Q>(res)) {}

            template <typename U>
            void await_suspend(std::coroutine_handle<U> handle) {
                handle.promise().return_value(Err(result.unwrapErr()));
                handle.destroy();
            }
        };

        template <typename E>
        struct Awaiter<void, E> {
            Result<void, E> result;

            bool await_ready() { return result.isOk(); }
            void await_resume() { return; }

            template <std::convertible_to<Result<void, E>> Q>
            Awaiter(Q&& res) : result(std::forward<Q>(res)) {}

            template <typename U>
            void await_suspend(std::coroutine_handle<U> handle) {
                handle.promise().return_value(Err(result.unwrapErr()));
                handle.destroy();
            }
        };
    }

    #define $async(...) geode::CoTask<>::invoke << [__VA_ARGS__]() -> geode::CoTask<>
    #define $try geode::CoTask<>::invoke << [&]() -> geode::Result
};

template <typename T = void, typename E = std::string>
auto operator co_await(geode::Result<T, E>&& res) {
    return geode::geode_internal::Awaiter<T, E> { std::move(res) };
}
template <typename T = void, typename E = std::string>
auto operator co_await(geode::Result<T, E> const& res) {
    return geode::geode_internal::Awaiter<T, E> { res };
}

template <typename T, typename E, typename ...Args>
struct std::coroutine_traits<geode::Result<T, E>, Args...> {
    using promise_type = geode::geode_internal::promise_type<T, E>;
};

template <typename T, typename ...Args>
struct std::coroutine_traits<geode::Generator<T>, Args...> {
    using promise_type = geode::Generator<T>::promise_type;
};

