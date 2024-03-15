#pragma once

#include "Result.hpp"
#include "MiniFunction.hpp"
#include "../loader/Event.hpp"

namespace geode {
    namespace impl {
        struct DefaultProgress {
            std::string message;
            std::optional<uint8_t> percentage;

            DefaultProgress() = default;
            DefaultProgress(std::string const& msg) : message(msg) {}
            DefaultProgress(auto msg, uint8_t percentage) : message(msg), percentage(percentage) {}
        };
    }

    struct CancelledState final {};
    
    template <class T = impl::DefaultValue, class E = impl::DefaultError, class P = impl::DefaultProgress>
    class PromiseEventFilter;

    template <class T = impl::DefaultValue, class E = impl::DefaultError, class P = impl::DefaultProgress>
    class Promise final {
    public:
        using Value    = T;
        using Error    = E;
        using Progress = P;

        using OnResolved = utils::MiniFunction<void(Value)>;
        using OnRejected = utils::MiniFunction<void(Error)>;
        using OnProgress = utils::MiniFunction<void(Progress)>;
        using OnFinished = utils::MiniFunction<void()>;
        using OnCancelled = utils::MiniFunction<void()>;

        using State = std::variant<Value, Error, Progress, CancelledState>;
        using OnStateChange = utils::MiniFunction<void(State)>;

        // These are needed if for example Value and Error are the same type
        static constexpr size_t STATE_VALUE_INDEX     = 0;
        static constexpr size_t STATE_ERROR_INDEX     = 1;
        static constexpr size_t STATE_PROGRESS_INDEX  = 2;
        static constexpr size_t STATE_CANCELLED_INDEX = 3;

        Promise() : m_data(std::make_shared<Data>()) {}

        Promise(utils::MiniFunction<void(OnResolved, OnRejected)> source, bool threaded = true) 
          : Promise([source](auto resolve, auto reject, auto, auto const&) {
                source(resolve, reject);
            }, threaded) {}
        
        Promise(utils::MiniFunction<void(OnResolved, OnRejected, OnProgress, std::atomic_bool const&)> source, bool threaded = true)
          : Promise([source](auto onStateChanged, auto const& cancelled) {
                source(
                    [onStateChanged](auto&& value) {
                        onStateChanged(State(std::in_place_index<STATE_VALUE_INDEX>, value));
                    },
                    [onStateChanged](auto&& error) {
                        onStateChanged(State(std::in_place_index<STATE_ERROR_INDEX>, error));
                    },
                    [onStateChanged](auto&& progress) {
                        onStateChanged(State(std::in_place_index<STATE_PROGRESS_INDEX>, progress));
                    },
                    cancelled
                );
            }, threaded, std::monostate()) {}
        
        Promise(utils::MiniFunction<void(OnStateChange, std::atomic_bool const&)> source, bool threaded, std::monostate tag) : m_data(std::make_shared<Data>()) {
            m_data->shouldStartThreaded = threaded;
            if (threaded) {
                std::thread([source = std::move(source), data = m_data]() mutable {
                    Promise::invoke_source(std::move(source), data);
                }).detach();
            }
            else {
                Promise::invoke_source(std::move(source), m_data);
            }
        }

        Promise then(utils::MiniFunction<void(Value)>&& callback) {
            return this->template then<Value>([callback](auto value) {
                callback(value);
                return std::move(value);
            });
        }
        template <class T2>
            requires (!std::is_void_v<T2>)
        Promise<T2, E, P> then(utils::MiniFunction<T2(T)>&& callback) {
            if (m_data->cancelled) {
                return make_cancelled<T2, E, P>();
            }
            return make_fwd<STATE_VALUE_INDEX, T2, E, P>(callback, m_data);
        }

        template <class T2, class E2>
            requires (!std::is_void_v<T2>)
        Promise<T2, E2, P> then(utils::MiniFunction<Result<T2, E2>(Result<T, E>)>&& callback) {
            if (m_data->cancelled) {
                return make_cancelled<T2, E2, P>();
            }
            return make_fwd_with_result<T2, E2, P>(callback, m_data);
        }

        Promise expect(utils::MiniFunction<void(Error)>&& callback) {
            return this->template expect<Error>([callback](auto error) {
                callback(error);
                return std::move(error);
            });
        }
        template <class E2>
            requires (!std::is_void_v<E2>)
        Promise<T, E2, P> expect(utils::MiniFunction<E2(E)>&& callback) {
            if (m_data->cancelled) {
                return make_cancelled<T, E2, P>();
            }
            return make_fwd<STATE_ERROR_INDEX, T, E2, P>(callback, m_data);
        }

        Promise progress(utils::MiniFunction<void(Progress)>&& callback) {
            return this->template progress<Progress>([callback](auto prog) {
                callback(prog);
                return std::move(prog);
            });
        }
        template <class P2>
            requires (!std::is_void_v<P2>)
        Promise<T, E, P2> progress(utils::MiniFunction<P2(P)>&& callback) {
            if (m_data->cancelled) {
                return make_cancelled<T, E, P2>();
            }
            return make_fwd<STATE_PROGRESS_INDEX, T, E, P2>(callback, m_data);
        }

        Promise finally(utils::MiniFunction<void()>&& callback) {
            if (m_data->cancelled) {
                return make_cancelled();
            }

            return Promise([data = m_data, callback](auto fwdStateToNextPromise) {
                std::unique_lock lock(data->mutex);

                // Check if this Promise has already been resolved, and if so 
                // immediately queue the callback with the value
                if (data->result.has_value()) {
                    Loader::get()->queueInMainThread([callback = std::move(callback)] {
                        callback();
                    });
                }
                else {
                    data->callback = [fwdStateToNextPromise, callback](auto&& state) {
                        if (state.index() == STATE_VALUE_INDEX || state.index() == STATE_ERROR_INDEX) {
                            callback();
                        }
                        fwdStateToNextPromise(state);
                    };
                }
            });
        }

        Promise cancelled(utils::MiniFunction<void()>&& callback) {
            if (m_data->cancelled) {
                Loader::get()->queueInMainThread([callback = std::move(callback)] {
                    callback();
                });
                return make_cancelled();
            }

            return Promise([data = m_data, callback](auto fwdStateToNextPromise) {
                std::unique_lock<std::mutex> lock(data->mutex);
                if (!data->result.has_value()) {
                    data->callback = [fwdStateToNextPromise, callback](auto&& state) {
                        if (state.index() == STATE_CANCELLED_INDEX) {
                            callback();
                        }
                        fwdStateToNextPromise(state);
                    };
                }
            });
        }

        void resolve(Value&& value) {
            invoke_callback(State(std::in_place_index<STATE_VALUE_INDEX>, std::move(value)), m_data);
        }
        void reject(Error&& error) {
            invoke_callback(State(std::in_place_index<STATE_ERROR_INDEX>, std::move(error)), m_data);
        }
        void cancel() {
            m_data->cancelled = true;
            invoke_callback(State(std::in_place_index<STATE_CANCELLED_INDEX>, CancelledState()), m_data);
        }

        /**
         * Returns a filter for listening to this `Promise` through the Geode 
         * Events system. Useful for example for using `Promise`s on layers, 
         * which may be removed from the node tree before the `Promise` 
         * finishes and as such calling a `then` callback that captures the 
         * layer would then read undefined memory
         */
        PromiseEventFilter<T, E, P> listen();

    private:
        struct Data final {
            std::mutex mutex;
            OnStateChange callback;
            std::optional<std::variant<Value, Error>> result;
            std::atomic_bool cancelled;
            bool shouldStartThreaded;
        };
        std::shared_ptr<Data> m_data;

        template <class T2 = Value, class E2 = Error, class P2 = Progress>
        static Promise<T2, E2, P2> make_cancelled() {
            auto ret = Promise<T2, E2, P2>();
            ret.cancel();
            return std::move(ret);
        }

        template <class T2, class E2, class P2>
        static Promise<T2, E2, P2> make_fwd_with_result(auto mapper, std::shared_ptr<Data> data) {
            return Promise<T2, E2, P2>([data, mapper](auto fwdStateToNextPromise, auto const& cancelled) {
                std::unique_lock lock(data->mutex);

                // Check if this Promise has already been resolved, and if so 
                // immediately queue the callback with the value
                if (data->result.has_value()) {
                    auto v = data->result.value();
                    if (v.index() == 0) {
                        Loader::get()->queueInMainThread([mapper = std::move(mapper), ok = std::move(std::get<0>(v))] {
                            (void)mapper(Ok(std::move(ok)));
                        });
                    }
                    else {
                        Loader::get()->queueInMainThread([mapper = std::move(mapper), err = std::move(std::get<1>(v))] {
                            (void)mapper(Err(std::move(err)));
                        });
                    }
                    return;
                }

                data->callback = [fwdStateToNextPromise, &cancelled, mapper](auto&& state) {
                    // Can't use std::visit if Value and Error are the same >:(
                    switch (state.index()) {
                        case STATE_VALUE_INDEX: {
                            auto mapped = mapper(Ok(std::move(std::get<STATE_VALUE_INDEX>(state))));
                            if (mapped) {
                                fwdStateToNextPromise(Promise<T2, E2, P2>::State(
                                    std::in_place_index<STATE_VALUE_INDEX>,
                                    std::move(mapped).unwrap()
                                ));
                            }
                            else {
                                fwdStateToNextPromise(Promise<T2, E2, P2>::State(
                                    std::in_place_index<STATE_ERROR_INDEX>,
                                    std::move(mapped).unwrapErr()
                                ));
                            }
                        } break;

                        case STATE_ERROR_INDEX: {
                            auto mapped = mapper(Err(std::move(std::get<STATE_ERROR_INDEX>(state))));
                            if (mapped) {
                                fwdStateToNextPromise(Promise<T2, E2, P2>::State(
                                    std::in_place_index<STATE_VALUE_INDEX>,
                                    std::move(mapped).unwrap()
                                ));
                            }
                            else {
                                fwdStateToNextPromise(Promise<T2, E2, P2>::State(
                                    std::in_place_index<STATE_ERROR_INDEX>,
                                    std::move(mapped).unwrapErr()
                                ));
                            }
                        } break;

                        case STATE_PROGRESS_INDEX: {
                            fwdStateToNextPromise(Promise<T2, E2, P2>::State(
                                std::in_place_index<STATE_PROGRESS_INDEX>,
                                std::move(std::get<STATE_PROGRESS_INDEX>(state))
                            ));
                        } break;

                        case STATE_CANCELLED_INDEX: {
                            fwdStateToNextPromise(Promise<T2, E2, P2>::State(
                                std::in_place_index<STATE_CANCELLED_INDEX>,
                                std::move(std::get<STATE_CANCELLED_INDEX>(state))
                            ));
                        } break;
                    }
                };
            }, data->shouldStartThreaded, std::monostate());
        }

        template <size_t Ix, class T2, class E2, class P2>
        static Promise<T2, E2, P2> make_fwd(auto mapper, std::shared_ptr<Data> data) {
            return Promise<T2, E2, P2>([data, mapper](auto fwdStateToNextPromise, auto const&) {
                std::unique_lock lock(data->mutex);

                // Check if the Promise has already been resolved
                if (data->result.has_value()) {
                    if constexpr (Ix == 0 || Ix == 1) {
                        auto v = data->result.value();
                        if (v.index() == Ix) {
                            Loader::get()->queueInMainThread([mapper = std::move(mapper), val = std::move(std::get<Ix>(v))] {
                                (void)mapper(val);
                            });
                        }
                    }
                    return;
                }
                
                data->callback = [fwdStateToNextPromise, mapper](auto&& state) {
                    if (state.index() == Ix) {
                        auto mapped = mapper(std::get<Ix>(state));
                        fwdStateToNextPromise(Promise<T2, E2, P2>::State(
                            std::in_place_index<Ix>,
                            mapped
                        ));
                    }
                    // Can't use std::visit if Value and Error are the same >:(
                    else switch (state.index()) {
                        case STATE_VALUE_INDEX: if constexpr (Ix != STATE_VALUE_INDEX) {
                            fwdStateToNextPromise(Promise<T2, E2, P2>::State(
                                std::in_place_index<STATE_VALUE_INDEX>,
                                std::move(std::get<STATE_VALUE_INDEX>(state))
                            ));
                        } break;

                        case STATE_ERROR_INDEX: if constexpr (Ix != STATE_ERROR_INDEX) {
                            fwdStateToNextPromise(Promise<T2, E2, P2>::State(
                                std::in_place_index<STATE_ERROR_INDEX>,
                                std::move(std::get<STATE_ERROR_INDEX>(state))
                            ));
                        } break;

                        case STATE_PROGRESS_INDEX: if constexpr (Ix != STATE_PROGRESS_INDEX) {
                            fwdStateToNextPromise(Promise<T2, E2, P2>::State(
                                std::in_place_index<STATE_PROGRESS_INDEX>,
                                std::move(std::get<STATE_PROGRESS_INDEX>(state))
                            ));
                        } break;

                        case STATE_CANCELLED_INDEX: if constexpr (Ix != STATE_CANCELLED_INDEX) {
                            fwdStateToNextPromise(Promise<T2, E2, P2>::State(
                                std::in_place_index<STATE_CANCELLED_INDEX>,
                                std::move(std::get<STATE_CANCELLED_INDEX>(state))
                            ));
                        } break;
                    }
                };
            }, data->shouldStartThreaded, std::monostate());
        }

        static void invoke_callback(State&& state, std::shared_ptr<Data> data) {
            if (data->cancelled) {
                return;
            }

            std::unique_lock lock(data->mutex);

            if (data->callback) {
                Loader::get()->queueInMainThread([callback = data->callback, state = State(state)]() {
                    callback(state);
                });
            }

            // Store the state to let future installed callbacks be immediately resolved
            if (state.index() == STATE_VALUE_INDEX) {
                data->result = std::variant<Value, Error>(std::in_place_index<0>, std::get<0>(std::move(state)));
            }
            else if (state.index() == STATE_ERROR_INDEX) {
                data->result = std::variant<Value, Error>(std::in_place_index<1>, std::get<1>(std::move(state)));
            }
            else if (state.index() == STATE_CANCELLED_INDEX) {
                data->cancelled = true;
            }
        }

        static void invoke_source(utils::MiniFunction<void(OnStateChange, std::atomic_bool const&)>&& source, std::shared_ptr<Data> data) {
            source(
                [data](auto&& state) {
                    invoke_callback(std::move(state), data);
                },
                data->cancelled
            );
        }
    };

    /**
     * Wraps a `Promise` in the Geode Event system for easier consumption. 
     * Useful for example for layers, where just regularly waiting for the 
     * `Promise` could run into issues if the layer is freed from memory; 
     * whereas with event listeners being RAII, they are automatically 
     * removed from layers, avoiding use-after-free errors
     */
    template <class T = impl::DefaultValue, class E = impl::DefaultError, class P = impl::DefaultProgress>
    class PromiseEvent : public Event {
    protected:
        size_t m_id;
        std::variant<T, E, P> m_value;

        PromiseEvent(size_t id, std::variant<T, E, P>&& value) : m_id(id), m_value(value) {}

        friend class Promise<T, E, P>;
        friend class PromiseEventFilter<T, E, P>;

    public:
        T const* getResolve()  const { return std::get_if<0>(&m_value); }
        E const* getReject()   const { return std::get_if<1>(&m_value); }
        P const* getProgress() const { return std::get_if<2>(&m_value); }
        bool isFinally() const { return m_value.index() != 2; }
    };

    template <class T, class E, class P>
    class PromiseEventFilter : public EventFilter<PromiseEvent<T, E, P>> {
    public:
        using Callback = void(PromiseEvent<T, E, P>*);

    protected:
        size_t m_id;

        friend class Promise<T, E, P>;

        PromiseEventFilter(size_t id) : m_id(id) {}

    public:
        PromiseEventFilter() : m_id(0) {}

        ListenerResult handle(utils::MiniFunction<Callback> fn, PromiseEvent<T, E, P>* event) {
            // log::debug("Event mod filter: {}, {}, {}, {}", m_mod, static_cast<int>(m_type), event->getMod(), static_cast<int>(event->getType()));
            if (m_id == event->m_id) {
                fn(event);
            }
            return ListenerResult::Propagate;
        }
    };

    template <class T, class E, class P>
    PromiseEventFilter<T, E, P> Promise<T, E, P>::listen() {
        // After 4 billion promises this will overflow and start producing 
        // the same IDs again, so technically if some promise takes 
        // literally forever then this could cause issues later on
        static size_t ID_COUNTER = 0;
        ID_COUNTER += 1;
        // Reserve 0 for PromiseEventFilter not listening to anything
        if (ID_COUNTER == 0) {
            ID_COUNTER += 1;
        }
        size_t id = ID_COUNTER;
        this
            ->then([id](auto&& value) { 
                PromiseEvent<T, E, P>(id, std::variant<T, E, P> { std::in_place_index<0>, std::forward<T>(value) }).post();
            })
            .expect([id](auto&& error) {
                PromiseEvent<T, E, P>(id, std::variant<T, E, P> { std::in_place_index<1>, std::forward<E>(error) }).post();
            })
            .progress([id](auto&& prog) {
                PromiseEvent<T, E, P>(id, std::variant<T, E, P> { std::in_place_index<2>, std::forward<P>(prog) }).post();
            });
        return PromiseEventFilter<T, E, P>(id);
    }
}
