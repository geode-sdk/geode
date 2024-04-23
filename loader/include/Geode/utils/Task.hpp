#pragma once

#include "general.hpp"
#include "MiniFunction.hpp"
#include "../loader/Event.hpp"
#include "../loader/Loader.hpp"

namespace geode {
    template <std::move_constructible T, std::move_constructible P = std::monostate>
    class [[nodiscard]] Task final {
    public:
        struct [[nodiscard]] Cancel final {};

        class Result final {
        private:
            std::variant<T, Cancel> m_value;

            std::optional<T> getValue() && {
                if (m_value.index() == 0) {
                    return std::optional(std::move(std::get<0>(std::move(m_value))));
                }
                return std::nullopt;
            }
            bool isCancelled() const {
                return m_value.index() == 1;
            }

            template <std::move_constructible T2, std::move_constructible P2>
            friend class Task;

        public:
            Result(Result&&) = default;
            Result(Result const&) = delete;
            Result(T&& value) : m_value(std::in_place_index<0>, std::forward<T>(value)) {}
            Result(Cancel const&) : m_value(std::in_place_index<1>, Cancel()) {}

            template <class V>
            Result(V&& value) requires std::is_constructible_v<T, V&&>
              : m_value(std::in_place_index<0>, std::forward<V>(value))
            {}
        };
    
    public:
        enum class Status {
            Pending,
            Finished,
            Cancelled,
        };

        class Handle final {
        private:
            std::recursive_mutex m_mutex;
            Status m_status = Status::Pending;
            std::optional<T> m_resultValue;
            bool m_finalEventPosted = false;
            std::unique_ptr<void, void(*)(void*)> m_mapListener = { nullptr, +[](void*) {} };
            std::string m_name;

            class PrivateMarker final {};

            static std::shared_ptr<Handle> create(std::string const& name) {
                return std::make_shared<Handle>(PrivateMarker(), name);
            }

            bool is(Status status) {
                std::unique_lock<std::recursive_mutex> lock(m_mutex);
                return m_status == status;
            }

            template <std::move_constructible T2, std::move_constructible P2>
            friend class Task;

        public:
            Handle(PrivateMarker, std::string const& name) : m_name(name) {}
        };

        class Event final : public geode::Event {
        private:
            std::shared_ptr<Handle> m_handle;
            std::variant<T*, P*, Cancel> m_value;
            EventListenerProtocol* m_for = nullptr;

            Event(std::shared_ptr<Handle> handle, std::variant<T*, P*, Cancel>&& value)
              : m_handle(handle), m_value(std::move(value)) {}
            
            static Event createFinished(std::shared_ptr<Handle> handle, T* value) {
                return Event(handle, std::variant<T*, P*, Cancel>(std::in_place_index<0>, value));
            }
            static Event createProgressed(std::shared_ptr<Handle> handle, P* value) {
                return Event(handle, std::variant<T*, P*, Cancel>(std::in_place_index<1>, value));
            }
            static Event createCancelled(std::shared_ptr<Handle> handle) {
                return Event(handle, std::variant<T*, P*, Cancel>(std::in_place_index<2>, Cancel()));
            }

            template <std::move_constructible T2, std::move_constructible P2>
            friend class Task;

        public:
            T* getValue() {
                return m_value.index() == 0 ? std::get<0>(m_value) : nullptr;
            }
            T const* getValue() const {
                return m_value.index() == 0 ? std::get<0>(m_value) : nullptr;
            }
            P* getProgress() {
                return m_value.index() == 1 ? std::get<1>(m_value) : nullptr;
            }
            P const* getProgress() const {
                return m_value.index() == 1 ? std::get<1>(m_value) : nullptr;
            }
            bool isCancelled() const {
                return m_value.index() == 2;
            }
            void cancel() {
                Task::cancel(m_handle);
            }
        };

        using Value            = T;
        using Progress         = P;
        using PostResult       = utils::MiniFunction<void(Result)>;
        using PostProgress     = utils::MiniFunction<void(P)>;
        using HasBeenCancelled = utils::MiniFunction<bool()>;
        using Run              = utils::MiniFunction<Result(PostProgress, HasBeenCancelled)>;
        using RunWithCallback  = utils::MiniFunction<void(PostResult, PostProgress, HasBeenCancelled)>;

        using Callback = void(Event*);

    private:
        EventListenerProtocol* m_listener = nullptr;
        std::shared_ptr<Handle> m_handle;

        Task(std::shared_ptr<Handle> handle) : m_handle(handle) {}

        static void finish(std::shared_ptr<Handle> handle, T&& value) {
            if (!handle) return;
            std::unique_lock<std::recursive_mutex> lock(handle->m_mutex);
            if (handle->m_status == Status::Pending) {
                handle->m_status = Status::Finished;
                handle->m_resultValue.emplace(std::move(value));
                Loader::get()->queueInMainThread([handle, value = &*handle->m_resultValue]() mutable {
                    Event::createFinished(handle, value).post();
                    std::unique_lock<std::recursive_mutex> lock(handle->m_mutex);
                    handle->m_finalEventPosted = true;
                });
            }
        }
        static void progress(std::shared_ptr<Handle> handle, P&& value) {
            if (!handle) return;
            std::unique_lock<std::recursive_mutex> lock(handle->m_mutex);
            if (handle->m_status == Status::Pending) {
                Loader::get()->queueInMainThread([handle, value = std::move(value)]() mutable {
                    Event::createProgressed(handle, &value).post();
                });
            }
        }
        static void cancel(std::shared_ptr<Handle> handle) {
            if (!handle) return;
            std::unique_lock<std::recursive_mutex> lock(handle->m_mutex);
            if (handle->m_status == Status::Pending) {
                handle->m_status = Status::Cancelled;
                Loader::get()->queueInMainThread([handle]() mutable {
                    Event::createCancelled(handle).post();
                    std::unique_lock<std::recursive_mutex> lock(handle->m_mutex);
                    handle->m_finalEventPosted = true;
                });
            }
        }

        template <std::move_constructible T2, std::move_constructible P2>
        friend class Task;

    public:
        Task() : m_handle(nullptr) {}

        Task(Task const& other) : m_handle(other.m_handle) {}
        Task(Task&& other) : m_handle(std::move(other.m_handle)) {}
        Task& operator=(Task const& other) {
            m_handle = other.m_handle;
            return *this;
        }
        Task& operator=(Task&& other) {
            m_handle = std::move(other.m_handle);
            return *this;
        }

        bool operator==(Task const& other) const {
            return m_handle == other.m_handle;
        }
        bool operator!=(Task const& other) const {
            return m_handle != other.m_handle;
        }
        bool operator<(Task const& other) const {
            return m_handle < other.m_handle;
        }
        bool operator<=(Task const& other) const {
            return m_handle <= other.m_handle;
        }
        bool operator>(Task const& other) const {
            return m_handle > other.m_handle;
        }
        bool operator>=(Task const& other) const {
            return m_handle >= other.m_handle;
        }

        T* getFinishedValue() {
            if (m_handle && m_handle->m_resultValue) {
                return &*m_handle->m_resultValue;
            }
            return nullptr;
        }
        void cancel() {
            Task::cancel(m_handle);
        }
        bool isPending() const {
            return m_handle && m_handle->is(Status::Pending);
        }
        bool isFinished() const {
            return m_handle && m_handle->is(Status::Finished);
        }
        bool isCancelled() const {
            return m_handle && m_handle->is(Status::Cancelled);
        }
        bool isNull() const {
            return m_handle == nullptr;
        }
        
        static Task immediate(T value, std::string const& name = "<Immediate Task>") {
            auto task = Task(Handle::create(name));
            Task::finish(task.m_handle, std::move(value));
            return task;
        }
        static Task run(Run&& body, std::string const& name = "<Task>") {
            auto task = Task(Handle::create(name));
            std::thread([handle = std::weak_ptr(task.m_handle), name, body = std::move(body)] {
                utils::thread::setName(fmt::format("Task '{}'", name));
                auto result = body(
                    [handle](P progress) {
                        Task::progress(handle.lock(), std::move(progress));
                    },
                    [handle]() -> bool {
                        // The task has been cancelled if the user has explicitly cancelled it, 
                        // or if there is no one listening anymore
                        auto lock = handle.lock();
                        return !(lock && lock->is(Status::Pending));
                    }
                );
                if (result.isCancelled()) {
                    Task::cancel(handle.lock());
                }
                else {
                    Task::finish(handle.lock(), std::move(*std::move(result).getValue()));
                }
            }).detach();
            return task;
        }
        static Task runWithCallback(RunWithCallback&& body, std::string const& name = "<Callback Task>") {
            auto task = Task(Handle::create(name));
            std::thread([handle = std::weak_ptr(task.m_handle), name, body = std::move(body)] {
                utils::thread::setName(fmt::format("Task '{}'", name));
                body(
                    [handle](Result result) {
                        if (result.isCancelled()) {
                            Task::cancel(handle.lock());
                        }
                        else {
                            Task::finish(handle.lock(), std::move(*std::move(result).getValue()));
                        }
                    },
                    [handle](P progress) {
                        Task::progress(handle.lock(), std::move(progress));
                    },
                    [handle]() -> bool {
                        // The task has been cancelled if the user has explicitly cancelled it, 
                        // or if there is no one listening anymore
                        auto lock = handle.lock();
                        return !(lock && lock->is(Status::Pending));
                    }
                );
            }).detach();
            return task;
        }

        template <class ResultMapper, class ProgressMapper>
        auto map(ResultMapper&& resultMapper, ProgressMapper&& progressMapper, std::string const& name = "<Mapping Task>") {
            using T2 = decltype(resultMapper(std::declval<T*>()));
            using P2 = decltype(progressMapper(std::declval<P*>()));

            auto task = Task<T2, P2>(Task<T2, P2>::Handle::create(fmt::format("{} <= {}", name, m_handle->m_name))); 

            // Lock the current task until we have managed to create our new one
            std::unique_lock<std::recursive_mutex> lock(m_handle->m_mutex);

            // If the current task is cancelled, cancel the new one immediately
            if (m_handle->m_status == Status::Cancelled) {
                Task<T2, P2>::cancel(task.m_handle);
            }
            // If the current task is finished, immediately map the value and post that
            else if (m_handle->m_status == Status::Finished) {
                Task<T2, P2>::finish(task.m_handle, std::move(resultMapper(&*m_handle->m_resultValue)));
            }
            // Otherwise start listening and waiting for the current task to finish
            else {
                task.m_handle->m_mapListener = std::unique_ptr<void, void(*)(void*)>(
                    static_cast<void*>(new EventListener<Task>(
                        [
                            handle = std::weak_ptr(task.m_handle),
                            resultMapper = std::move(resultMapper),
                            progressMapper = std::move(progressMapper)
                        ](Event* event) {
                            if (auto v = event->getValue()) {
                                Task<T2, P2>::finish(handle.lock(), std::move(resultMapper(v)));
                            }
                            else if (auto p = event->getProgress()) {
                                Task<T2, P2>::progress(handle.lock(), std::move(progressMapper(p)));
                            }
                            else if (event->isCancelled()) {
                                Task<T2, P2>::cancel(handle.lock());
                            }
                        },
                        *this
                    )),
                    +[](void* ptr) {
                        delete static_cast<EventListener<Task>*>(ptr);
                    }
                );
            }
            return task;
        }

        template <class ResultMapper>
            requires std::copy_constructible<P>
        auto map(ResultMapper&& resultMapper, std::string const& name = "<Mapping Task>") {
            return this->map(std::move(resultMapper), +[](P* p) -> P { return *p; }, name);
        }

        ListenerResult handle(utils::MiniFunction<Callback> fn, Event* e) {
            if (e->m_handle == m_handle && (!e->m_for || e->m_for == m_listener)) {
                fn(e);
            }
            return ListenerResult::Propagate;
        }

        // todo: i believe alk wanted tasks to be in their own pool
        EventListenerPool* getPool() const {
            return DefaultEventListenerPool::get();
        }

        void setListener(EventListenerProtocol* listener) {
            m_listener = listener;

            if (!m_handle) return;

            // If this task has already been finished and the finish event 
            // isn't pending in the event queue, immediately queue up a 
            // finish event for this listener
            std::unique_lock<std::recursive_mutex> lock(m_handle->m_mutex);
            if (m_handle->m_finalEventPosted) {
                if (m_handle->m_status == Status::Finished) {
                    Loader::get()->queueInMainThread([handle = m_handle, listener = m_listener, value = &*m_handle->m_resultValue]() {
                        auto ev = Event::createFinished(handle, value);
                        ev.m_for = listener;
                        ev.post();
                    });
                }
                else {
                    Loader::get()->queueInMainThread([handle = m_handle, listener = m_listener]() {
                        auto ev = Event::createCancelled(handle);
                        ev.m_for = listener;
                        ev.post();
                    });
                }
            }
        }
        EventListenerProtocol* getListener() const {
            return m_listener;
        }
    };

    static_assert(is_filter<Task<int>>, "The Task class must be a valid event filter!");
}
