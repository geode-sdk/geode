#pragma once

#include "Result.hpp"
#include "MiniFunction.hpp"
#include "../loader/Event.hpp"
#include "ranges.hpp"

namespace geode {
    struct DefaultProgress {
        std::string message;
        std::optional<uint8_t> percentage;

        DefaultProgress() = default;
        DefaultProgress(std::string const& msg) : message(msg) {}
        DefaultProgress(auto msg, uint8_t percentage) : message(msg), percentage(percentage) {}
    };
    
    namespace impl {
        template <size_t Ty>
        struct LogBug {
            static inline size_t COUNT = 0;
            static const char* ty() {
                return Ty ? "Promise" : "Data";
            }

            LogBug() {
                // log::info("created {} that holds {}, {}", ty(), fmt::ptr(this), ++COUNT);
            }
            LogBug& operator=(LogBug&&) {
                // log::info("moved {} that holds {}, {}", ty(), fmt::ptr(this), ++COUNT);
                return *this;
            }
            LogBug& operator=(LogBug const&) {
                // log::info("copied {} that holds {}, {}", ty(), fmt::ptr(this), ++COUNT);
                return *this;
            }
            LogBug(LogBug&&) {
                // log::info("moved {} that holds {}, {}", ty(), fmt::ptr(this), ++COUNT);
            }
            LogBug(LogBug const&) {
                // log::info("copied {} that holds {}, {}", ty(), fmt::ptr(this), ++COUNT);
            }
            ~LogBug() {
                // log::info("destroyed {} that holds {}, {}", ty(), fmt::ptr(this), --COUNT);
            }
        };
    }

    struct CancelledState final {};
    
    template <class T = impl::DefaultValue, class E = impl::DefaultError, class P = DefaultProgress>
    class PromiseEventFilter;

    template <class T = impl::DefaultValue, class E = impl::DefaultError, class P = DefaultProgress>
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
            typename Promise<T2, E2, P2>::State convert() && {
                if (this->has_value()) {
                    if constexpr (std::is_same_v<T, T2>) {
                        return Promise<T2, E2, P2>::State::make_value(std::move(std::move(*this).take_value()));
                    }
                }
                if (this->has_error()) {
                    if constexpr (std::is_same_v<E, E2>) {
                        return Promise<T2, E2, P2>::State::make_error(std::move(std::move(*this).take_error()));
                    }
                }
                if (this->has_progress()) {
                    if constexpr (std::is_same_v<P, P2>) {
                        return Promise<T2, E2, P2>::State::make_progress(std::move(std::move(*this).take_progress()));
                    }
                }
                if (this->is_cancelled()) {
                    return Promise<T2, E2, P2>::State::make_cancelled();
                }
                geode::utils::unreachable(
                    "Promise::State::convert called on a State that isn't in a convertible state! "
                    "All code should verify before calling convert() that the State holds a value "
                    "which is trivially convertible (holds the same type)"
                );
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

        Promise() : m_data(nullptr) {}

        Promise(utils::MiniFunction<void(OnResolved, OnRejected)> source) 
          : Promise([source](auto resolve, auto reject, auto, auto const&) {
                source(resolve, reject);
            }) {}
        
        Promise(utils::MiniFunction<void(OnResolved, OnRejected, OnProgress, std::atomic_bool const&)> source)
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
            }, std::monostate()) {}
        
        Promise(utils::MiniFunction<void(OnStateChange, std::atomic_bool const&)> source, std::monostate tag)
          : m_data(std::make_shared<Data>())
        {
            std::thread([source = std::move(source), data = m_data]() mutable {
                log::info("start invoke_source");
                source(
                    [data = std::weak_ptr(data)](auto&& state) {
                        if (auto d = data.lock()) {
                            log::info("callback from invoke_source");
                            invoke_callback(std::move(state), d.get());
                        }
                        else {
                            log::info("tried to callback from invoke_source but deleted");
                        }
                    },
                    data->cancelled
                );
                log::info("end invoke_source");
            }).detach();
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

        Promise forward() {
            return make_fwd<T, E, P>([](auto state) { return std::move(state); }, m_data);
        }

        void resolve(Value&& value) {
            if (!m_data) return;
            invoke_callback(State::make_value(std::move(value)), m_data.get());
        }
        void reject(Error&& error) {
            if (!m_data) return;
            invoke_callback(State::make_error(std::move(error)), m_data.get());
        }
        void cancel() {
            if (!m_data) return;
            m_data->cancelled = true;
            invoke_callback(State::make_cancelled(), m_data.get());
        }

        /**
         * Returns a filter for listening to this `Promise` through the Geode 
         * Events system. Useful for example for using `Promise`s on layers, 
         * which may be removed from the node tree before the `Promise` 
         * finishes and as such calling a `then` callback that captures the 
         * layer would then read undefined memory
         */
        PromiseEventFilter<T, E, P> listen();

        // UNFINISHED!!
        // I'm pretty sure this has a memory leak somewhere in it too
        // static Promise<std::vector<T>, E, P> all(std::vector<Promise>&& promises, bool own = true, bool threaded = true) {
        //     return Promise<std::vector<T>, E, P>([own, promises = std::move(promises)](auto resolve, auto reject, auto progress, auto const& cancelled) {
        //         struct All final {
        //             std::vector<T> results;
        //             std::vector<Promise> promises;
        //         };
        //         auto all = std::make_shared<All>(All {
        //             .results = {},
        //             .promises = std::move(promises),
        //         });
        //         for (auto& promise : all->promises) {
        //             // SAFETY: all of the accesses to `all` are safe since the Promise 
        //             // callbacks are guaranteed to run in the same thread
        //             promise
        //                 // Wait for all of them to finish
        //                 .then([all, resolve](auto result) {
        //                     all->results.push_back(result);
        //                     if (all->results.size() >= all->promises.size()) {
        //                         resolve(all->results);
        //                         all->promises.clear();
        //                         all->results.clear();
        //                     }
        //                 })
        //                 // If some Promise fails, the whole `all` fails
        //                 .expect([own, all, reject](auto error) {
        //                     // Only cancel contained Promises if the `all` is considered to be 
        //                     // owning them, since cancelling shared Promises could have bad 
        //                     // consequences
        //                     if (own) {
        //                         for (auto& promise : all->promises) {
        //                             promise.cancel();
        //                         }
        //                     }
        //                     all->promises.clear();
        //                     all->results.clear();
        //                     reject(error);
        //                 })
        //                 // Check if the `Promise::all` has been cancelled
        //                 .progress([&cancelled, own, all, progress](auto prog) {
        //                     if (cancelled) {
        //                         // Only cancel contained Promises if the `all` is considered to be 
        //                         // owning them, since cancelling shared Promises could have bad 
        //                         // consequences
        //                         if (own) {
        //                             for (auto& promise : all->promises) {
        //                                 promise.cancel();
        //                             }
        //                         }
        //                         all->promises.clear();
        //                         all->results.clear();
        //                     }
        //                     else {
        //                         progress(prog);
        //                     }
        //                 })
        //                 // Remove cancelled promises from the list
        //                 .cancelled([promise, all] {
        //                     utils::ranges::remove(all->promises, [promise](auto other) {
        //                         return other.m_data == promise.m_data;
        //                     });
        //                 });
        //         }
        //     }, threaded);
        // }

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
            impl::LogBug<0> log;
        };
        std::shared_ptr<Data> m_data;
        impl::LogBug<1> log;

        template <class T2, class E2, class P2>
        static Promise<T2, E2, P2> make_fwd(auto&& transformState, std::shared_ptr<Data> data) {
            return Promise<T2, E2, P2>([data, transformState](auto fwdStateToNextPromise, auto const&) {
                if (!data) return;
                Promise::set_callback(
                    [fwdStateToNextPromise, transformState](auto&& state) { 
                        // Map the state
                        auto mapped = transformState(std::move(state));
                        // Forward the value to the next Promise
                        fwdStateToNextPromise(std::move(mapped));
                    },
                    data.get()
                );
            }, std::monostate());
        }

        static void set_callback(OnStateChange&& callback, Data* data) {
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

        static void invoke_callback(State&& state, Data* data) {
            std::unique_lock lock(data->mutex);
            invoke_callback_no_lock(std::move(state), data);
        }

        static void invoke_callback_no_lock(State&& state, Data* data) {
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
    };

    /**
     * Wraps a `Promise` in the Geode Event system for easier consumption. 
     * Useful for example for layers, where just regularly waiting for the 
     * `Promise` could run into issues if the layer is freed from memory; 
     * whereas with event listeners being RAII, they are automatically 
     * removed from layers, avoiding use-after-free errors
     */
    template <class T = impl::DefaultValue, class E = impl::DefaultError, class P = DefaultProgress>
    class PromiseEvent : public Event {
    protected:
        std::shared_ptr<void> m_handle;
        std::variant<T, E, P> m_value;

        PromiseEvent(std::shared_ptr<void> handle, std::variant<T, E, P>&& value)
          : m_handle(handle), m_value(value) {}

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
        std::shared_ptr<void> m_handle;

        friend class Promise<T, E, P>;

        PromiseEventFilter(std::shared_ptr<void> handle) : m_handle(handle) {}

    public:
        PromiseEventFilter() : m_handle(nullptr) {}

        ListenerResult handle(utils::MiniFunction<Callback> fn, PromiseEvent<T, E, P>* event) {
            // log::debug("Event mod filter: {}, {}, {}, {}", m_mod, static_cast<int>(m_type), event->getMod(), static_cast<int>(event->getType()));
            if (m_handle == event->m_handle) {
                fn(event);
            }
            return ListenerResult::Propagate;
        }
    };

    template <class T, class E, class P>
    PromiseEventFilter<T, E, P> Promise<T, E, P>::listen() {
        this
            ->then([data = std::weak_ptr(m_data)](auto&& value) {
                if (auto d = std::static_pointer_cast<void>(data.lock())) {
                    PromiseEvent<T, E, P>(d, std::variant<T, E, P> { std::in_place_index<0>, std::forward<T>(value) }).post();
                }
            })
            .expect([data = std::weak_ptr(m_data)](auto&& error) {
                if (auto d = std::static_pointer_cast<void>(data.lock())) {
                    PromiseEvent<T, E, P>(d, std::variant<T, E, P> { std::in_place_index<1>, std::forward<E>(error) }).post();
                }
            })
            .progress([data = std::weak_ptr(m_data)](auto&& prog) {
                if (auto d = std::static_pointer_cast<void>(data.lock())) {
                    PromiseEvent<T, E, P>(d, std::variant<T, E, P> { std::in_place_index<2>, std::forward<P>(prog) }).post();
                }
            });
        return PromiseEventFilter<T, E, P>(m_data);
    }
}
