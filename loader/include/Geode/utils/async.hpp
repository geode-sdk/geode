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


/// Invokes a function that returns a future, asynchronously spawns it,
/// and then invokes the given callback on the main thread when it completes.
template <typename F, typename Cb> requires (arc::ReturnsPollable<std::decay_t<F>>)
arc::TaskHandle<void> spawn(F&& f, Cb&& cb) {
    geode::async::spawn(std::invoke(std::forward<F>(f)), std::forward<Cb>(cb));
}

/// Spawns a future as an async task, can be a function that returns a future.
template <typename F> requires (arc::Spawnable<std::decay_t<F>>)
arc::TaskHandle<void> spawn(F&& f) {
    runtime().spawn(std::forward<F>(f));
}

/// Allows an async task to be spawned and then automatically aborted when the holder goes out of scope.
template <typename Ret = void>
class TaskHolder {
public:
    // spawn a nameless task, delegates the call to one of the two functions below
    template <typename S, typename Cb>
    void spawn(S&& func, Cb&& cb) {
        this->spawn("", std::forward<S>(func), std::forward<Cb>(cb));
    }

    /// Calls the given function (which returns a future) and spawns the resulting future
    template <typename L, typename Cb>
    void spawn(std::string name, L&& func, Cb&& cb) requires arc::ReturnsPollable<std::decay_t<L>> {
        this->spawn(std::move(name), std::invoke(std::forward<L>(func)), std::forward<Cb>(cb));
    }

    /// Spawns the given future
    template <typename F, typename Cb>
    void spawn(std::string name, F&& future, Cb&& cb) requires arc::IsPollable<std::decay_t<F>> {
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
    > requires (arc::IsPollable<std::decay_t<F>>)
    void spawnInner(std::string name, F&& future, Callback cb) {
        static_assert(
            std::convertible_to<typename arc::FutureTraits<std::decay_t<F>>::Output, Ret>,
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