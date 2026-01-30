#pragma once

#include <arc/runtime/Runtime.hpp>
#include <arc/util/Result.hpp>
#include <arc/task/CancellationToken.hpp>
#include <Geode/utils/function.hpp>
#include <Geode/loader/Loader.hpp>

namespace geode::async {

/**
 * Gets the main arc Runtime, prefer running all async code inside this runtime.
 */
arc::Runtime& runtime();

/**
 * Asynchronously spawns a function, then invokes the given callback on the main thread when it completes.
 */
template <
    typename Fut,
    typename Out = arc::FutureTraits<Fut>::Output,
    bool Void = std::is_void_v<Out>,
    typename Callback = std::conditional_t<Void, Function<void()>, Function<void(Out)>>
>
arc::TaskHandle<void> spawn(Fut future, Callback cb) {
    return runtime().spawn([](Fut future, Callback cb) mutable -> arc::Future<> {
        if constexpr (Void) {
            co_await std::move(future);
            geode::queueInMainThread([cb = std::move(cb)]() mutable {
                cb();
            });
        } else {
            auto result = co_await std::move(future);
            geode::queueInMainThread([cb = std::move(cb), result = std::move(result)]() mutable {
                cb(std::move(result));
            });
        }
    }(std::move(future), std::move(cb)));
}

/**
 * Allows an async task to be spawned and then automatically aborted when the holder goes out of scope.
 */
template <typename Ret>
class TaskHolder {
public:
    template <
        typename Fut,
        typename NonVoid = std::conditional_t<std::is_void_v<Ret>, std::monostate, Ret>,
        typename Callback = std::conditional_t<std::is_void_v<Ret>, Function<void()>, Function<void(NonVoid)>>
    >
    void spawn(Fut future, Callback cb) {
        this->cancel();

        m_cancel = std::make_shared<arc::CancellationToken>();
        
        if constexpr (std::is_void_v<Ret>) {
            m_handle = geode::async::spawn(std::move(future), [cancel = m_cancel, cb = std::move(cb)] mutable {
                if (cancel->isCancelled()) return;
                cb();
            });
        } else {
            m_handle = geode::async::spawn(std::move(future), [cancel = m_cancel, cb = std::move(cb)](Ret val) mutable {
                if (cancel->isCancelled()) return;
                cb(std::move(val));
            });
        }
        
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

private:
    std::optional<arc::TaskHandle<void>> m_handle;
    std::shared_ptr<arc::CancellationToken> m_cancel;
};

}