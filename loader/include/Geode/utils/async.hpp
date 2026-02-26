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
/// Overload for function objects that return a Future, i.e. `[] -> arc::Future {}`
template <
    typename Lambda,
    typename Func = std::decay_t<Lambda>,
    typename Cb
> requires (arc::ReturnsPollable<Func>)
arc::TaskHandle<void> spawn(Lambda&& lambda, Cb&& cb) {
    using Fut = std::invoke_result_t<Func>;
    using Out = arc::FutureTraits<Fut>::Output;
    constexpr bool Void = std::is_void_v<Out>;

    struct SpawnPollable : arc::Pollable<SpawnPollable, void> {
        explicit SpawnPollable(Func&& f, Cb&& c) : m_func(std::forward<Func>(f)), m_callback(std::forward<Cb>(c)) {}

        bool poll(arc::Context& cx) {
            if (!m_fut) {
                m_fut.emplace(std::invoke(m_func));
            }

            if (!m_fut->m_vtable->poll(&*m_fut, cx)) {
                return false;
            }

            if constexpr (Void) {
                static_assert(std::is_invocable_v<Cb>, "When spawning a void future, callback must be invocable with no arguments");

                // scary things
                geode::queueInMainThread(std::move(*m_callback));
                m_callback.reset();
            } else {
                // more scary things
                geode::queueInMainThread([cb = std::move(*m_callback), result = std::move(m_fut->m_vtable->template getOutput<Out>(&*m_fut))] mutable {
                    if constexpr (std::is_invocable_v<Cb, Out>) {
                        cb(std::move(result));
                    } else if constexpr (std::is_invocable_v<Cb>) {
                        cb();
                    } else {
                        static_assert(!std::is_same_v<Cb, Cb>, "When spawning a future, callback must be invocable with the future's output or with no arguments");
                    }
                });
                m_callback.reset();
            }

            return true;
        }

        SpawnPollable(SpawnPollable&&) = default;
        SpawnPollable& operator=(SpawnPollable&&) = default;

        ~SpawnPollable() {
            if (m_callback) {
                // Task was cancelled before it could finish, we should schedule the callback to be destroyed on main thread,
                // because mods may do things like capture Refs, which must be destroyed on main thread
                geode::queueInMainThread([cb = std::move(*m_callback)] {});
            }
        }

    private:
        Func m_func;
        std::optional<Cb> m_callback;
        std::optional<Fut> m_fut;
    };

    return runtime().spawn(SpawnPollable {
        std::forward<Func>(lambda),
        std::forward<Cb>(cb),
    });
}

/// Asynchronously spawns a future, then invokes the given callback on the main thread when it completes.
template <typename Fut, typename Cb> requires (arc::IsPollable<std::decay_t<Fut>>)
arc::TaskHandle<void> spawn(Fut&& future, Cb&& cb) {
    return spawn([fut = std::forward<Fut>(future)] mutable { return std::move(fut); }, std::forward<Cb>(cb));
}

/// Spawns a future as an async task, can be a function that returns a future.
template <typename F> requires (arc::Spawnable<std::decay_t<F>>)
auto spawn(F&& f) {
    return runtime().spawn(std::forward<F>(f));
}

template <typename F> requires (arc::Spawnable<std::decay_t<F>>)
auto wrapSpawn(F&& f) {
    return [f = std::forward<F>(f)]() mutable { return spawn(std::move(f)); };
}

template <
    typename T,
    typename NonVoidT = std::conditional_t<std::is_void_v<T>, std::monostate, T>,
    typename PollOut = std::conditional_t<std::is_void_v<T>, bool, std::optional<NonVoidT>>
>
struct WaitForMainAwaiter : arc::Pollable<WaitForMainAwaiter<T>, PollOut> {
    template <typename F> requires (!std::is_same_v<std::decay_t<F>, WaitForMainAwaiter>)
    explicit WaitForMainAwaiter(F&& func) {
        m_state = std::make_shared<std::atomic<State>>(State::Pending);
        auto [tx, rx] = arc::oneshot::channel<NonVoidT>();
        m_receiver.emplace(std::move(rx));
        m_recvAwaiter.emplace(m_receiver->recv());

        geode::queueInMainThread([state = m_state, func = std::forward<F>(func), tx = std::move(tx)] mutable {
            auto expected = State::Pending;
            if (!state->compare_exchange_strong(expected, State::Running, std::memory_order::acq_rel)) {
                // cancelled before the function started running, simply exit
                return;
            }

            auto complete = [&]<typename X>(X&& val) {
                // the state must be either Running or RunningCancelled, depending on this we decide whether to post the result or not
                bool shouldPost = State::Running == state->exchange(State::Completed, std::memory_order::acq_rel);

                if (shouldPost) {
                    (void) tx.send(std::forward<X>(val));
                } else {
                    state->notify_one();
                }
            };

            if constexpr (std::is_void_v<T>) {
                func();
                complete(std::monostate{});
            } else {
                complete(func());
            }
        });
    }

    ~WaitForMainAwaiter() {
        if (!m_state || !m_receiver) return;

        auto state = m_state->load(std::memory_order::acquire);
        while (true) {
            switch (state) {
                case State::Pending: {
                    // function hasn't ran yet, all we have to do is cancel it
                    if (m_state->compare_exchange_weak(state, State::Completed, std::memory_order::acq_rel)) {
                        return;
                    }
                } break;

                case State::Running: {
                    // currently running, we need to wait for it to finish
                    // the function may have captured local environment, prevent UB by waiting until it finishes before we destroy anything
                    if (m_state->compare_exchange_weak(state, State::RunningCancelled, std::memory_order::acq_rel)) {
                        m_state->wait(State::RunningCancelled, std::memory_order::acquire);
                        return;
                    }
                } break;

                // nothing to do if completed, and all other states are impossible here
                case State::Completed:
                default: return;
            }
        }
    }

    WaitForMainAwaiter(WaitForMainAwaiter const&) = delete;
    WaitForMainAwaiter& operator=(WaitForMainAwaiter const&) = delete;
    WaitForMainAwaiter(WaitForMainAwaiter&&) = default;
    WaitForMainAwaiter& operator=(WaitForMainAwaiter&&) = delete;

    std::optional<PollOut> poll(arc::Context& cx) {
        auto pres = m_recvAwaiter->poll(cx);
        if (!pres) return std::nullopt;

        // res is RecvResult<T>, return Some(nullopt/false) if it failed
        auto res = std::move(*pres);

        if constexpr (std::is_void_v<T>) {
            return std::optional{res.isOk()};
        } else {
            return std::move(res).ok();
        }
    }

private:
    enum class State : uint8_t {
        /// The function has been queued to run, but is not yet running
        Pending,
        /// The function is currently running in the main thread
        Running,
        /// The function is still running in the main thread, but it has been cancelled
        RunningCancelled,
        /// The function either completed and posted the result, or was cancelled and is not currently running
        Completed,
    };

    std::shared_ptr<std::atomic<State>> m_state;
    std::optional<arc::oneshot::Receiver<NonVoidT>> m_receiver;
    std::optional<arc::oneshot::RecvAwaiter<NonVoidT>> m_recvAwaiter;
};

/// Queues the given function to run in the main thread as soon as possible
/// and waits for it to complete. Returns null/false if the function failed to send the result.
/// (although that usually cannot happen in practice)
/// This pollable is cancel-safe and won't be destroyed until the function finishes running or is confirmed to be aborted.
template <typename T = void, typename F>
auto waitForMainThread(F&& func) {
    return WaitForMainAwaiter<T>(std::forward<F>(func));
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
        // check if the user accidentally provided a future as the callback, it's a bit of a common mistake
        if constexpr (std::is_void_v<Ret>) {
            using CbRet = std::invoke_result_t<Cb>;
            static_assert(!arc::IsPollable<CbRet>, "Callback function in `TaskHolder::spawn` must be a sync function, not a future");
        } else {
            using CbRet = std::invoke_result_t<Cb, NonVoid>;
            static_assert(!arc::IsPollable<CbRet>, "Callback function in `TaskHolder::spawn` must be a sync function, not a future");
        }

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

    template <typename F, typename Cb>
    void spawnInner(std::string name, F&& future, Cb&& cb) {
        using FutureOutput = typename arc::SpawnableOutput<std::decay_t<F>>::type;

        static_assert(
            std::convertible_to<FutureOutput, Ret>,
            "Output of spawned future must be convertible to TaskHolder's expected return type"
        );

        this->cancel();

        auto state = std::make_shared<SpawnedTaskState>();

        if constexpr (std::is_void_v<Ret>) {
            state->m_handle = geode::async::spawn(std::forward<F>(future), [state, cb = std::forward<Cb>(cb)] mutable {
                if (state->isCancelled()) return;
                state->complete();
                cb();
            });
        } else {
            state->m_handle = geode::async::spawn(std::forward<F>(future), [state, cb = std::forward<Cb>(cb)](Ret val) mutable {
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