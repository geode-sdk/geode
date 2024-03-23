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

        class State final {
        private:
            std::variant<Value, Error, Progress, CancelledState> m_value;
        
            template <size_t Ix, class V>
            State(std::in_place_index_t<Ix> index, V&& value) : m_value(index, std::forward<V>(value)) {}

        public:
            static State make_value(Value&& value) {
                return State(std::in_place_index<0>, std::move(value));
            }
            static State make_error(Error&& error) {
                return State(std::in_place_index<1>, std::move(error));
            }
            static State make_progress(Progress&& progress) {
                return State(std::in_place_index<2>, std::move(progress));
            }
            static State make_cancelled() {
                return State(std::in_place_index<3>, CancelledState());
            }

            template <class T2, class E2, class P2>
            Promise<T2, E2, P2>::State convert() && {
                if (this->has_value()) {
                    if constexpr (std::is_same_v<T, T2>) {
                        return Promise<T2, E2, P2>::State::make_value(std::move(std::move(*this).take_value()));
                    }
                    log::error("THIS CODE PATH SHOULD BE UNREACHABLE!!!!");
                }
                if (this->has_error()) {
                    if constexpr (std::is_same_v<E, E2>) {
                        return Promise<T2, E2, P2>::State::make_error(std::move(std::move(*this).take_error()));
                    }
                    log::error("THIS CODE PATH SHOULD BE UNREACHABLE!!!!");
                }
                if (this->has_progress()) {
                    if constexpr (std::is_same_v<P, P2>) {
                        return Promise<T2, E2, P2>::State::make_progress(std::move(std::move(*this).take_progress()));
                    }
                    log::error("THIS CODE PATH SHOULD BE UNREACHABLE!!!!");
                }
                return Promise<T2, E2, P2>::State::make_cancelled();
            }

            bool has_value() { return m_value.index() == 0; }
            Value get_value() const { return std::get<0>(m_value); }
            Value take_value() && { return std::get<0>(std::move(m_value)); }

            bool has_error() { return m_value.index() == 1; }
            Error get_error() const { return std::get<1>(m_value); }
            Error take_error() && { return std::get<1>(std::move(m_value)); }

            bool has_progress() { return m_value.index() == 2; }
            Progress get_progress() const { return std::get<2>(m_value); }
            Progress take_progress() && { return std::get<2>(std::move(m_value)); }

            bool is_cancelled() { return m_value.index() == 3; }
        };

        using OnStateChange = utils::MiniFunction<void(State)>;

        Promise() : m_data(std::make_shared<Data>()) {}

        Promise(utils::MiniFunction<void(OnResolved, OnRejected)> source, bool threaded = true) 
          : Promise([source](auto resolve, auto reject, auto, auto const&) {
                source(resolve, reject);
            }, threaded) {}
        
        Promise(utils::MiniFunction<void(OnResolved, OnRejected, OnProgress, std::atomic_bool const&)> source, bool threaded = true)
          : Promise([source](auto onStateChanged, auto const& cancelled) {
                source(
                    [onStateChanged](auto&& value) {
                        onStateChanged(State::make_value(std::move(value)));
                    },
                    [onStateChanged](auto&& error) {
                        onStateChanged(State::make_error(std::move(error)));
                    },
                    [onStateChanged](auto&& progress) {
                        onStateChanged(State::make_progress(std::move(progress)));
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
            return make_fwd<T, E, P>(
                [callback](typename Promise::State&& state) -> typename Promise::State {
                    if (state.has_value()) {
                        callback(state.get_value());
                    }
                    return std::move(state);
                },
                m_data
            );
        }
        template <class T2>
            requires (!std::is_void_v<T2>)
        Promise<T2, E, P> then(utils::MiniFunction<T2(T)>&& callback) {
            return make_fwd<T2, E, P>(
                [callback](typename Promise::State&& state) -> typename Promise<T2, E, P>::State {
                    if (state.has_value()) {
                        return Promise<T2, E, P>::State::make_value(callback(std::move(state).take_value()));
                    }
                    return std::move(state).template convert<T2, E, P>();
                },
                m_data
            );
        }

        template <class T2, class E2>
            requires (!std::is_void_v<T2>)
        Promise<T2, E2, P> then(utils::MiniFunction<Result<T2, E2>(Result<T, E>)>&& callback) {
            return make_fwd<T2, E2, P>(
                [callback](typename Promise::State&& state) -> typename Promise<T2, E2, P>::State {
                    if (state.has_value() || state.has_error()) {
                        auto current = state.has_value() ?
                            Result<T, E>(Ok(std::move(state).take_value())) :
                            Result<T, E>(Err(std::move(state).take_error()));
                        
                        auto result = callback(std::move(current));
                        if (result.isOk()) {
                            return Promise<T2, E2, P>::State::make_value(std::move(std::move(result).unwrap()));
                        }
                        else {
                            return Promise<T2, E2, P>::State::make_error(std::move(std::move(result).unwrapErr()));
                        }
                    }
                    return std::move(state).template convert<T2, E2, P>();
                },
                m_data
            );
        }

        Promise expect(utils::MiniFunction<void(Error)>&& callback) {
            return make_fwd<T, E, P>(
                [callback](typename Promise::State&& state) -> typename Promise::State {
                    if (state.has_error()) {
                        callback(state.get_error());
                    }
                    return std::move(state);
                },
                m_data
            );
        }
        template <class E2>
            requires (!std::is_void_v<E2>)
        Promise<T, E2, P> expect(utils::MiniFunction<E2(E)>&& callback) {
            return make_fwd<T, E2, P>(
                [callback](typename Promise::State&& state) -> typename Promise<T, E2, P>::State {
                    if (state.has_error()) {
                        return Promise<T, E2, P>::State::make_error(callback(std::move(state).take_error()));
                    }
                    return std::move(state).template convert<T, E2, P>();
                },
                m_data
            );
        }

        Promise progress(utils::MiniFunction<void(Progress)>&& callback) {
            return make_fwd<T, E, P>(
                [callback](typename Promise::State&& state) -> typename Promise::State {
                    if (state.has_progress()) {
                        callback(state.get_progress());
                    }
                    return std::move(state);
                },
                m_data
            );
        }

        template <class P2>
            requires (!std::is_void_v<P2>)
        Promise<T, E, P2> progress(utils::MiniFunction<P2(P)>&& callback) {
            return make_fwd<T, E, P2>(
                [callback](typename Promise::State&& state) -> typename Promise<T, E, P2>::State {
                    if (state.has_progress()) {
                        return Promise<T, E, P2>::State::make_progress(callback(std::move(state).take_progress()));
                    }
                    return std::move(state).template convert<T, E, P2>();
                },
                m_data
            );
        }

        Promise finally(utils::MiniFunction<void()>&& callback) {
            return make_fwd<T, E, P>(
                [callback](typename Promise::State&& state) -> typename Promise::State {
                    if (state.has_value() || state.has_error()) {
                        callback();
                    }
                    return std::move(state);
                },
                m_data
            );
        }

        Promise cancelled(utils::MiniFunction<void()>&& callback) {
            return make_fwd<T, E, P>(
                [callback](typename Promise::State&& state) -> typename Promise::State {
                    if (state.is_cancelled()) {
                        callback();
                    }
                    return std::move(state);
                },
                m_data
            );
        }

        void resolve(Value&& value) {
            invoke_callback(State::make_value(std::move(value)), m_data);
        }
        void reject(Error&& error) {
            invoke_callback(State::make_error(std::move(error)), m_data);
        }
        void cancel() {
            m_data->cancelled = true;
            invoke_callback(State::make_cancelled(), m_data);
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
        // I'm not sure just how un-performant this is, although then again you 
        // should not be using Promises in performance-sensitive code since the 
        // whole point of them is to wait for stuff that happens in the 
        // possibly distant future
        struct Data final {
            std::mutex mutex;
            std::vector<OnStateChange> callbacks;
            std::optional<std::variant<Value, Error>> result;
            std::atomic_bool cancelled;
            std::atomic_bool shouldStartThreaded;
        };
        std::shared_ptr<Data> m_data;

        template <class T2, class E2, class P2>
        static Promise<T2, E2, P2> make_fwd(
            auto&& transformState,
            std::shared_ptr<Data> data
        ) {
            return Promise<T2, E2, P2>([data, transformState](auto fwdStateToNextPromise, auto const&) {
                Promise::set_callback(
                    [fwdStateToNextPromise, transformState](auto&& state) { 
                        // Map the state
                        auto mapped = transformState(std::move(state));
                        // Forward the value to the next Promise
                        fwdStateToNextPromise(std::move(mapped));
                    },
                    data
                );
            }, data->shouldStartThreaded, std::monostate());
        }

        static void set_callback(OnStateChange&& callback, std::shared_ptr<Data> data) {
            std::unique_lock lock(data->mutex);
            data->callbacks.emplace_back(std::move(callback));

            // Check if the callback should be immediately fired because 
            // the Promise is already resolved or cancelled
            if (data->cancelled) {
                invoke_callback_no_lock(State::make_cancelled(), data);
            }
            if (data->result) {
                if (data->result->index() == 0) {
                    invoke_callback_no_lock(State::make_value(Value(std::get<0>(*data->result))), data);
                }
                else {
                    invoke_callback_no_lock(State::make_error(Error(std::get<1>(*data->result))), data);
                }
            }
        }

        static void invoke_callback(State&& state, std::shared_ptr<Data> data) {
            std::unique_lock lock(data->mutex);
            invoke_callback_no_lock(std::move(state), data);
        }

        static void invoke_callback_no_lock(State&& state, std::shared_ptr<Data> data) {
            // Run callbacks in the main thread
            Loader::get()->queueInMainThread([callbacks = data->callbacks, state = State(state)]() {
                for (auto&& callback : std::move(callbacks)) {
                    callback(state);
                }
            });

            // Store the state to let future installed callbacks be immediately resolved
            if (state.has_value()) {
                data->result = std::variant<Value, Error>(std::in_place_index<0>, std::move(state).take_value());
            }
            else if (state.has_error()) {
                data->result = std::variant<Value, Error>(std::in_place_index<1>, std::move(state).take_error());
            }
            else if (state.is_cancelled()) {
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
