#pragma once

#include <coroutine>
#include <Geode/DefaultInclude.hpp>
#include "Task.hpp"

namespace geode {
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
            Awaiter(Result<T, E>&& res) : result(std::move(res)) {}

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
            Awaiter(Result<void, E> res) : result(res) {}

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
    return geode::geode_internal::Awaiter { std::move(res) };
}

template <typename T, typename E, typename ...Args>
struct std::coroutine_traits<geode::Result<T, E>, Args...> {
    using promise_type = geode::geode_internal::promise_type<T, E>;
};
