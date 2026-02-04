#pragma once

#include <arc/runtime/Runtime.hpp>
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

/// Allows an async task to be spawned and then automatically aborted when the holder goes out of scope.
template <typename Ret = void>
class TaskHolder {
public:
    /// Spawns the given future, invoking the callback on completion.
    /// Lambdas that return a future are also accepted.
    template <typename F, typename Cb>
    void spawn(F&& future, Cb&& cb) {
        this->spawn("", std::forward<F>(future), std::forward<Cb>(cb));
    }

    /// Spawns the given future, assigning a name to the task and invoking the callback on completion.
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

            m_handle = std::move(other.m_handle);
            m_cancel = std::move(other.m_cancel);
            other.m_cancel.reset();
        }
        return *this;
    }

    void cancel() {
        if (m_handle) {
            m_cancel->cancel();
            m_cancel.reset();

            m_handle->abort();
            m_handle.reset();
        }
    }

    void setName(std::string name) {
        if (m_handle) {
            m_handle->setName(std::move(name));
        }
    }

private:
    std::optional<arc::TaskHandle<void>> m_handle;
    std::shared_ptr<arc::CancellationToken> m_cancel;
    
    template <
        typename F,
        typename NonVoid = std::conditional_t<std::is_void_v<Ret>, std::monostate, Ret>,
        typename Callback = std::conditional_t<std::is_void_v<Ret>, Function<void()>, Function<void(NonVoid)>>
    >
    void spawnInner(std::string name, F&& future, Callback cb) {
        using FutureOutput = typename arc::SpawnableOutput<std::decay_t<F>>::type;

        static_assert(
            std::convertible_to<FutureOutput, Ret>,
            "Output of spawned future must be convertible to TaskHolder's expected return type"
        );

        this->cancel();

        m_cancel = std::make_shared<arc::CancellationToken>();
        
        if constexpr (std::is_void_v<Ret>) {
            m_handle = geode::async::spawn(std::forward<F>(future), [cancel = m_cancel, cb = std::move(cb)] mutable {
                if (cancel->isCancelled()) return;
                cb();
            });
        } else {
            m_handle = geode::async::spawn(std::forward<F>(future), [cancel = m_cancel, cb = std::move(cb)](Ret val) mutable {
                if (cancel->isCancelled()) return;
                cb(std::move(val));
            });
        }   

        if (!name.empty()) {
            m_handle->setName(std::move(name));
        }
    }

};

}