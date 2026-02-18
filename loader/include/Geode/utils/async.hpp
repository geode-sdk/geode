#pragma once

#include <arc/runtime/Runtime.hpp>
#include <arc/sync/oneshot.hpp>
#include <arc/util/Result.hpp>
#include <arc/task/CancellationToken.hpp>
#include <Geode/utils/function.hpp>
#include <Geode/loader/Loader.hpp>

namespace geode::async {

/// Gets the main arc Runtime, prefer running all async code inside this runtime.
GEODE_DLL arc::Runtime& runtime();

/// Asynchronously spawns a future, then invokes the given callback on the main thread when it completes.
template <
    typename Fut,
    typename Out = arc::FutureTraits<Fut>::Output,
    bool Void = std::is_void_v<Out>,
    typename Callback = std::conditional_t<Void, Function<void()>, Function<void(Out)>>
> requires (arc::IsPollable<Fut>)
arc::TaskHandle<void> spawn(Fut future, Callback cb) {
    return runtime().spawn([](Fut future, Callback cb) mutable -> arc::Future<> {
        if constexpr (Void) {
            co_await std::move(future);
            geode::queueInMainThread([cb = std::move(cb)] mutable {
                cb();
            });
        } else {
            auto result = co_await std::move(future);
            geode::queueInMainThread([cb = std::move(cb), result = std::move(result)] mutable {
                cb(std::move(result));
            });
        }
    }(std::move(future), std::move(cb)));
}

/// Asynchronously spawns a future, then invokes the given callback on the main thread when it completes.
/// Overload for function objects that return a Future, i.e. `[] -> arc::Future {}`
template <
    typename Lambda,
    typename Func = std::decay_t<Lambda>,
    typename Out = arc::FutureTraits<std::invoke_result_t<Func>>::Output,
    bool Void = std::is_void_v<Out>,
    typename Callback = std::conditional_t<Void, Function<void()>, Function<void(Out)>>
> requires (arc::ReturnsPollable<Func>)
arc::TaskHandle<void> spawn(Lambda&& lambda, Callback cb) {
    return runtime().spawn([](Lambda&& lambda, Callback cb) mutable -> arc::Future<> {
        if constexpr (Void) {
            co_await std::invoke(std::forward<Lambda>(lambda));
            geode::queueInMainThread([cb = std::move(cb)] mutable {
                cb();
            });
        } else {
            auto result = co_await std::invoke(std::forward<Lambda>(lambda));
            geode::queueInMainThread([cb = std::move(cb), result = std::move(result)] mutable {
                cb(std::move(result));
            });
        }
    }(std::forward<Lambda>(lambda), std::move(cb)));
}

/// Spawns a future as an async task, can be a function that returns a future.
template <typename F> requires (arc::Spawnable<std::decay_t<F>>)
auto spawn(F&& f) {
    return runtime().spawn(std::forward<F>(f));
}

/// Queues the given function to run in the main thread as soon as possible
/// and waits for it to complete. Returns null if the function failed to send the result.
/// (although that usually cannot happen in practice)
template <typename T = void> requires (!std::is_void_v<T>)
arc::Future<std::optional<T>> waitForMainThread(Function<T()> func) {
    auto [tx, rx] = arc::oneshot::channel<T>();
    auto token = std::make_shared<arc::CancellationToken>();

    auto _ = arc::scopeDtor([&] {
        token->cancel();
    });

    geode::queueInMainThread([func = std::move(func), tx = std::move(tx), token] mutable {
        if (token->isCancelled()) return;
        (void) tx.send(func());
    });

    co_return (co_await rx.recv()).ok();
}

/// Queues the given function to run in the main thread as soon as possible
/// and waits for it to complete. Returns false if the function failed to complete (e.g. due to exception)
template <typename T = void> requires (std::is_void_v<T>)
arc::Future<bool> waitForMainThread(Function<void()> func) {
    auto [tx, rx] = arc::oneshot::channel<std::monostate>();
    auto token = std::make_shared<arc::CancellationToken>();

    auto _ = arc::scopeDtor([&] {
        token->cancel();
    });

    geode::queueInMainThread([func = std::move(func), tx = std::move(tx), token] mutable {
        if (token->isCancelled()) return;
        func();
        (void) tx.send({});
    });

    co_return (co_await rx.recv()).isOk();
}

/// Allows an async task to be spawned and then automatically aborted when the holder goes out of scope.
template <typename Ret = void>
class TaskHolder {
    using NonVoid = std::conditional_t<std::is_void_v<Ret>, std::monostate, Ret>;
    using Callback = std::conditional_t<std::is_void_v<Ret>, Function<void()>, Function<void(NonVoid)>>;
public:
    /// Spawns the given future, invoking the callback in the main thread on completion.
    /// Lambdas that return a future are also accepted.
    template <typename F, typename Cb>
    void spawn(F&& future, Cb&& cb) {
        this->spawn("", std::forward<F>(future), std::forward<Cb>(cb));
    }

    /// Spawns the given future, assigning a name to the task and invoking the callback in the main thread on completion.
    /// Lambdas that return a future are also accepted.
    template <typename F, typename Cb>
    void spawn(std::string name, F&& future, Cb&& cb) {
        this->spawnInner(std::move(name), std::forward<F>(future), std::forward<Cb>(cb));
    }

    ~TaskHolder() {
        this->cancel();
    }

    TaskHolder() = default;

    TaskHolder(TaskHolder const&) = delete;
    TaskHolder& operator=(TaskHolder const&) = delete;

    TaskHolder(TaskHolder&& other) noexcept {
        *this = std::move(other);
    }

    TaskHolder& operator=(TaskHolder&& other) noexcept {
        if (this != &other) {
            this->cancel();
            m_state = std::move(other.m_state);
            other.m_state = nullptr;
        }
        return *this;
    }

    /// Terminates the task as soon as possible and ensures the callback will not be called.
    /// This is optional and called automatically when destroying or spawning another task.
    /// Does nothing if there is no pending task.
    void cancel() {
        if (!m_state) return;

        m_state->cancel();
        m_state.reset();
    }

    /// Sets the name for the currently spawned task, for debugging purposes
    void setName(std::string name) {
        if (m_state && m_state->m_handle) {
            m_state->m_handle.setName(std::move(name));
        }
    }

    /// Checks if the spawned task is still pending. This will return false
    /// if no task is spawned, or if called inside the completion callback.
    bool isPending() const {
        return m_state && m_state->m_handle;
    }

private:
    struct SpawnedTaskState {
        arc::TaskHandle<void> m_handle;
        bool m_cancelled = false;

        void cancel() {
            m_cancelled = true;
            if (m_handle) {
                m_handle.abort();
            }
        }

        bool isCancelled() const {
            return m_cancelled;
        }

        void complete() {
            // reset the task handle to release the (potentially last) reference to the task and clean it up
            m_handle.detach();
        }
    };

    std::shared_ptr<SpawnedTaskState> m_state;

    template <typename F>
    void spawnInner(std::string name, F&& future, Callback cb) {
        using FutureOutput = typename arc::SpawnableOutput<std::decay_t<F>>::type;

        static_assert(
            std::convertible_to<FutureOutput, Ret>,
            "Output of spawned future must be convertible to TaskHolder's expected return type"
        );

        this->cancel();

        auto state = std::make_shared<SpawnedTaskState>();

        if constexpr (std::is_void_v<Ret>) {
            state->m_handle = geode::async::spawn(std::forward<F>(future), [state, cb = std::move(cb)] mutable {
                if (state->isCancelled()) return;
                state->complete();
                cb();
            });
        } else {
            state->m_handle = geode::async::spawn(std::forward<F>(future), [state, cb = std::move(cb)](Ret val) mutable {
                if (state->isCancelled()) return;
                state->complete();
                cb(std::move(val));
            });
        }

        m_state = std::move(state);

        if (!name.empty()) {
            this->setName(std::move(name));
        }
    }
};

}