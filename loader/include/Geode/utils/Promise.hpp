#pragma once

#include "Result.hpp"

namespace geode {
    namespace impl {
        struct DefaultProgress {
            std::string message;
            std::optional<uint8_t> percentage;
        };
    }

    /**
     * Represents an asynchronous `Result`. Similar to `Future` in Rust, or 
     * `Promise` in JavaScript. May have only one of each kind of callback. Can 
     * also be used to monitor the progress of the upcoming value. All 
     * callbacks are always run in the main thread, so interacting with UI is 
     * safe
     */
    template <class T = impl::DefaultValue, class E = impl::DefaultError, class P = impl::DefaultProgress>
    class [[nodiscard]] Promise final {
    public:
        using Then = MiniFunction<void(T)>;
        using Expect = MiniFunction<void(E)>;
        using Progress = MiniFunction<void(P)>;
        using Finally = MiniFunction<void()>;
        using Create = MiniFunction<void(Then, Expect, Progress)>;

        /**
         * Create a Promise. Call the provided callbacks to notify the 
         * listener of updates (progress, value resolved, rejected). See the 
         * class description for general information about Promises
         */
        Promise(Create&& create) : m_data(std::make_unique<Data>()) {
            create(
                [this](auto&& value) {
                    std::unique_lock<std::mutex> _(m_data->mutex);
                    bool handled = false;
                    if (m_data->thenHandler) {
                        Loader::get()->queueInMainThread([fun = std::move(m_data->thenHandler), v = std::move(value)] {
                            handler(v);
                        });
                        handled = true;
                    }
                    if (m_data->finallyHandler) {
                        Loader::get()->queueInMainThread([fun = std::move(m_data->finallyHandler)] {
                            handler();
                        });
                        handled = true;
                    }
                    if (!handled) {
                        m_data->result = Ok(std::move(value));
                    }
                },
                [this](auto&& error) {
                    std::unique_lock<std::mutex> _(m_data->mutex);
                    bool handled = false;
                    if (m_data->expectHandler) {
                        Loader::get()->queueInMainThread([fun = std::move(m_data->expectHandler), v = std::move(error)] {
                            handler(v);
                        });
                        handled = true;
                    }
                    if (m_data->finallyHandler) {
                        Loader::get()->queueInMainThread([fun = std::move(m_data->finallyHandler)] {
                            handler();
                        });
                        handled = true;
                    }
                    if (!handled) {
                        m_data->result = Err(std::move(error));
                    }
                },
                [this](auto&& p) {
                    std::unique_lock<std::mutex> _(m_data->mutex);
                    if (auto handler = m_data->progressHandler) {
                        handler(std::move(p));
                    }
                }
            );
        }

        // No copying
        Promise(Promise const& other) = delete;
        // Moving allowed
        Promise(Promise&& other) : m_data(std::move(other.m_data)) {}

        /**
         * Add a listener for when the Promise finishes. There may only be one 
         * listener at a time. If the Promise has already been resolved, the 
         * callback is immediately queued in the main thread
         */
        Promise& then(Then handler) {
            std::unique_lock<std::mutex> _(m_data->mutex);
            if (m_data->result.has_value()) {
                auto v = std::move(m_data->result).value();
                if (v.isOk()) {
                    Loader::get()->queueInMainThread([ok = std::move(v).unwrap()] {
                        handler(ok);
                    });
                }
            }
            else {
                m_data->thenHandlers.push_back(handler);
            }
            return *this;
        }
        /**
         * Add a listener for when the Promise fails. There may only be one 
         * listener at a time. If the Promise has already been resolved, the 
         * callback is immediately queued in the main thread
         */
        Promise& expect(Expect handler) {
            std::unique_lock<std::mutex> _(m_data->mutex);
            if (m_data->result.has_value()) {
                auto v = std::move(m_data->result).value();
                if (v.isErr()) {
                    Loader::get()->queueInMainThread([err = std::move(v).unwrapErr()] {
                        handler(err);
                    });
                }
            }
            else {
                m_data->expectHandler = handler;
            }
            return *this;
        }
        /**
         * Add a listener for when the Promise's progress is updated. There may 
         * only be one listener at a time. If the Promise has already been 
         * resolved, nothing happens
         */
        Promise& progress(Progress handler) {
            std::unique_lock<std::mutex> _(m_data->mutex);
            if (!m_data->result.has_value()) {
                m_data->progressHandler = handler;
            }
            return *this;
        }
        /**
         * Add a listener for when the Promise is finished, regardless of if 
         * it was succesful or not. There may only be one listener at a time. 
         * If the Promise has already been resolved, the callback is 
         * immediately queued in the main thread
         */
        Promise& finally(Finally handler) {
            std::unique_lock<std::mutex> _(m_data->mutex);
            if (m_data->result.has_value()) {
                Loader::get()->queueInMainThread([] {
                    handler();
                });
            }
            else {
                m_data->finallyHandler = handler;
            }
            return *this;
        }
    
    private:
        struct Data {
            std::mutex mutex;
            Then thenHandler;
            Expect expectHandler;
            Progress progressHandler;
            Finally finallyHandler;
            std::optional<Result<T, E>> result;
        };
        std::unique_ptr<Data> m_data;
    };
}
