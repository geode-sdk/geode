#pragma once

#include "general.hpp"
#include "MiniFunction.hpp"
#include "../loader/Event.hpp"
#include "../loader/Loader.hpp"
#include <mutex>
#include <string_view>

namespace geode {
    /**
     * Tasks represent an asynchronous operation that will be finished at some 
     * unknown point in the future. Tasks can report their progress, and will 
     * end either through finishing into a value, or due to being cancelled. 
     * Tasks are designed to provide a thread-safe general purpose abstraction 
     * for dealing with any asynchronous operations.
     * The `Task` class satisfies `EventFilter` and as such is listened 
     * to using the Geode events system; tasks may have multiple listeners, and 
     * even if a listener is attached after the Task has finished it will
     * receive the finished value. 
     * Tasks are a very cheap and tiny struct that just have a reference to 
     * a task Handle; as such, Tasks may (and should) be copied around without
     * worry. It should be noted that a Task never owns itself - the listener(s) 
     * of a Task are expected to hold an instance of the Task for as long as 
     * they intend to listen to it. Usually this is done via just setting 
     * the Task as the filter to an `EventListener`, as the `EventListener` 
     * manages the lifetime of its filter
     * Task itself does not carry a notion of fallibility aside from 
     * cancellation; it is customary to use the `Result` type in Tasks that 
     * might finish to a failure value.
     * Once a Task has finished or has been cancelled, it can no longer be 
     * revived
     * @tparam T The type the Task will eventually finish to. This type must be 
     * move-constructible; though as there is no way to move the value out 
     * of the Task (because of potentially multiple listeners), one 
     * should ensure they can reasonably copy the value out in some form if they 
     * wish to gain ownership of it after the Task is finished
     * @tparam P The type of the progress values the Task (may) post
     */
    template <std::move_constructible T, std::move_constructible P = std::monostate>
    class [[nodiscard]] Task final {
    public:
        /**
         * A struct used for cancelling Tasks; Tasks may return an instance of
         * this struct to cancel themselves, or to mark they have handled 
         * outside cancellation
         */
        struct [[nodiscard]] Cancel final {};

        /**
         * A simple holder for the result of this task; holds either the finished 
         * value or a mark that the Task was cancelled. The Task body must return 
         * this type (though it is implicitly convertible from T and Cancel 
         * so the programmer rarely needs to explicitly name it)
         */
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

            // Allow constructing Results using anything that can be used to construct T
            template <class V>
            Result(V&& value) requires std::is_constructible_v<T, V&&>
              : m_value(std::in_place_index<0>, std::forward<V>(value))
            {}
        };
    
        /// The current status of a Task
        enum class Status {
            /// The task is still running or waiting to start
            Pending,
            /// The task has succesfully finished
            Finished,
            /// The task has been cancelled
            Cancelled,
        };

        /**
         * A handle to a running Task. This is what actually keeps track of 
         * the state of the current task; the `Task` class is simply an owning 
         * reference & interface to one of these
         */
        class Handle final {
        private:
            // Handles may contain extra data, for example for holding ownership 
            // of other Tasks for `Task::map` and `Task::all`. This struct 
            // provides type erasure for that extra data
            struct ExtraData final {
                // Pointer to the owned extra data
                void* ptr;
                // Pointer to a function that deletes that extra data
                // The function MUST have a static lifetime
                void(*onDestroy)(void*);
                // Pointer to a function that handles cancelling any tasks within 
                // that extra data when this task is cancelled. Note that the 
                // task may not free up the memory associated with itself here 
                // and this function may not be called if the user uses 
                // `Task::shallowCancel`. However, this pointer *must* always be 
                // valid
                // The function MUST have a static lifetime
                void(*onCancelled)(void*);

                ExtraData(void* ptr, void(*onDestroy)(void*), void(*onCancelled)(void*))
                  : ptr(ptr), onDestroy(onDestroy), onCancelled(onCancelled)
                {}
                ExtraData(ExtraData const&) = delete;
                ExtraData(ExtraData&&) = delete;

                ~ExtraData() {
                    onDestroy(ptr);
                }
                void cancel() {
                    onCancelled(ptr);
                }
            };

            std::recursive_mutex m_mutex;
            Status m_status = Status::Pending;
            std::optional<T> m_resultValue;
            bool m_finalEventPosted = false;
            std::string m_name;
            std::unique_ptr<ExtraData> m_extraData = nullptr;

            class PrivateMarker final {};

            static std::shared_ptr<Handle> create(std::string_view const name) {
                return std::make_shared<Handle>(PrivateMarker(), name);
            }

            bool is(Status status) {
                std::unique_lock<std::recursive_mutex> lock(m_mutex);
                return m_status == status;
            }

            template <std::move_constructible T2, std::move_constructible P2>
            friend class Task;

        public:
            Handle(PrivateMarker, std::string_view const name) : m_name(name) {}
            ~Handle() {
                // If this Task was still pending when the Handle was destroyed, 
                // it can no longer be listened to so just cancel and cleanup
                std::unique_lock<std::recursive_mutex> lock(m_mutex);
                if (m_status == Status::Pending) {
                    m_status = Status::Cancelled;
                    // If this task carries extra data, call the extra data's 
                    // handling method
                    if (m_extraData) {
                        m_extraData->cancel();
                    }
                    // No need to actually post an event because this Task is 
                    // unlisteanable
                    m_finalEventPosted = true;
                }
            }
        };

        /**
         * When the Task progresses,  finishes, or is cancelled, one of these 
         * is posted; the `Task` class itself is used as an event filter to 
         * catch the task events for that specific task
         */
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
            /**
             * Get a reference to the contained finish value, or null if this 
             * event holds a progress value or represents cancellation instead
             */
            T* getValue() {
                return m_value.index() == 0 ? std::get<0>(m_value) : nullptr;
            }
            /**
             * Get a reference to the contained finish value, or null if this 
             * event holds a progress value or represents cancellation instead
             */  
            T const* getValue() const {
                return m_value.index() == 0 ? std::get<0>(m_value) : nullptr;
            }
            /**
             * Get a reference to the contained progress value, or null if 
             * this event holds a finish value or represents cancellation instead
             */   
            P* getProgress() {
                return m_value.index() == 1 ? std::get<1>(m_value) : nullptr;
            }
            /**
             * Get a reference to the contained progress value, or null if 
             * this event holds a finish value or represents cancellation instead
             */   
            P const* getProgress() const {
                return m_value.index() == 1 ? std::get<1>(m_value) : nullptr;
            }
            /** 
             * Check if the Task was cancelled
             */
            bool isCancelled() const {
                return m_value.index() == 2;
            }
            /**
             * Cancel the Task that posted this event. If the task has 
             * already finished or been cancelled, nothing happens
             */
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
                queueInMainThread([handle, value = &*handle->m_resultValue]() mutable {
                    // SAFETY: Task::all() depends on the lifetime of the value pointer
                    // being as long as the lifetime of the task itself
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
                queueInMainThread([handle, value = std::move(value)]() mutable {
                    Event::createProgressed(handle, &value).post();
                });
            }
        }
        static void cancel(std::shared_ptr<Handle> handle, bool shallow = false) {
            if (!handle) return;
            std::unique_lock<std::recursive_mutex> lock(handle->m_mutex);
            if (handle->m_status == Status::Pending) {
                handle->m_status = Status::Cancelled;
                // If this task carries extra data, call the extra data's handling method
                // (unless shallow cancelling was specifically requested)
                if (!shallow && handle->m_extraData) {
                    handle->m_extraData->cancel();
                }
                queueInMainThread([handle]() mutable {
                    Event::createCancelled(handle).post();
                    std::unique_lock<std::recursive_mutex> lock(handle->m_mutex);
                    handle->m_finalEventPosted = true;
                });
            }
        }

        template <std::move_constructible T2, std::move_constructible P2>
        friend class Task;

    public:
        // Allow default-construction
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

        /**
         * Get the value this Task finished to, if the Task had finished, 
         * or null otherwise. Note that this is simply a mutable reference to 
         * the value - *you may not move out of it!*
         */
        T* getFinishedValue() {
            if (m_handle && m_handle->m_resultValue) {
                return &*m_handle->m_resultValue;
            }
            return nullptr;
        }
        /**
         * Cancel this Task. If this is a Task that owns other Task(s) (for example 
         * one created through `Task::map`) then that Task is cancelled 
         * as well. If this is undesirable, use `shallowCancel()`
         * instead
         */
        void cancel() {
            Task::cancel(m_handle);
        }
        /**
         * If this is a Task that owns other Task(s) (for example created 
         * through `Task::map` or `Task::all`), then this method cancels *only* 
         * this Task and *not* any of the Task(s) it is built on top of. 
         * Ownership of the other Task(s) will be released, so if this is the 
         * only Task listening to them, they will still be destroyed due to a 
         * lack of listeners
         */
        void shallowCancel() {
            Task::cancel(m_handle, true);
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
        /** 
         * Check if this Task doesn't actually do anything (for instance it 
         * was default-constructed)
         */
        bool isNull() const {
            return m_handle == nullptr;
        }
        
        /**
         * Create a new Task that immediately finishes with the given 
         * value
         * @param value The value the Task shall be finished with
         * @param name The name of the Task; used for debugging
         */
        static Task immediate(T value, std::string_view const name = "<Immediate Task>") {
            auto task = Task(Handle::create(name));
            Task::finish(task.m_handle, std::move(value));
            return task;
        }
        /**
         * Create a new Task with a function that returns the finished value. 
         * See the class description for details about Tasks
         * @param body The body aka actual code of the Task. Note that this 
         * function MUST be synchronous - Task creates the thread for you!
         * @param name The name of the Task; used for debugging
         */
        static Task run(Run&& body, std::string_view const name = "<Task>") {
            auto task = Task(Handle::create(name));
            std::thread([handle = std::weak_ptr(task.m_handle), name = std::string(name), body = std::move(body)] {
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
        /**
         * Create a Task using a body that may need to create additional 
         * threads within itself; for example due to using an external 
         * library that creates its own thread
         * @param body The body aka actual code of the Task. The body may 
         * call its provided finish callback *exactly once* - subsequent 
         * calls will always be ignored
         * @param name The name of the Task; used for debugging
         */
        static Task runWithCallback(RunWithCallback&& body, std::string_view const name = "<Callback Task>") {
            auto task = Task(Handle::create(name));
            std::thread([handle = std::weak_ptr(task.m_handle), name = std::string(name), body = std::move(body)] {
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
                        return !lock || lock->is(Status::Cancelled);
                    }
                );
            }).detach();
            return task;
        }
        /**
         * Create a Task that waits for a list of other Tasks to finish, and then 
         * finishes with a list of their finish values
         * @param tasks The tasks to wait for
         * @param name The name of the Task; used for debugging
         * @warning The result vector may contain nulls if any of the tasks 
         * were cancelled!
         */
        template <std::move_constructible NP>
        static Task<std::vector<T*>, std::monostate> all(std::vector<Task<T, NP>>&& tasks, std::string_view const name = "<Multiple Tasks>") {
            using AllTask = Task<std::vector<T*>, std::monostate>;

            // If there are no tasks, return an immediate task that does nothing
            if (tasks.empty()) {
                return AllTask::immediate({}, name);
            }

            // Create a new supervising task for all of the provided tasks
            auto task = AllTask(AllTask::Handle::create(name));

            // Storage for storing the results received so far & keeping 
            // ownership of the running tasks
            struct Waiting final {
                std::vector<T*> taskResults;
                std::vector<Task<std::monostate>> taskListeners;
                size_t taskCount;
            };
            task.m_handle->m_extraData = std::make_unique<typename AllTask::Handle::ExtraData>(
                // Create the data
                static_cast<void*>(new Waiting()),
                // When the task is destroyed
                +[](void* ptr) {
                    delete static_cast<Waiting*>(ptr);
                },
                // If the task is cancelled
                +[](void* ptr) {
                    // The move clears the `taskListeners` vector (important!)
                    for (auto task : std::move(static_cast<Waiting*>(ptr)->taskListeners)) {
                        task.cancel();
                    }
                }
            );

            // Store the task count in case some tasks finish immediately during the loop 
            static_cast<Waiting*>(task.m_handle->m_extraData->ptr)->taskCount = tasks.size();

            // Make sure to only give a weak pointer to avoid circular references!
            // (Tasks should NEVER own themselves!!)
            auto markAsDone = [handle = std::weak_ptr(task.m_handle)](T* result) {
                auto lock = handle.lock();

                // If this task handle has expired, consider the task cancelled
                // (We don't have to do anything because the lack of a handle 
                // means all the memory has been freed or is managed by 
                // something else)
                if (!lock) return;

                // Get the waiting handle from the task handle
                auto waiting = static_cast<Waiting*>(lock->m_extraData->ptr);

                // SAFETY: The lifetime of result pointer is the same as the task that 
                // produced that pointer, so as long as we have an owning reference to 
                // the tasks through `taskListeners` we can be sure `result` is valid
                waiting->taskResults.push_back(result);

                // If all tasks are done, finish
                if (waiting->taskResults.size() >= waiting->taskCount) {
                    // SAFETY: The task results' lifetimes are tied to the tasks 
                    // which could have their only owner be `waiting->taskListeners`, 
                    // but since Waiting is owned by the returned AllTask it should 
                    // be safe to access as long as it's accessible
                    AllTask::finish(lock, std::move(waiting->taskResults));
                }
            };

            // Iterate the tasks & start listening to them using
            for (auto& taskToWait : tasks) {
                static_cast<Waiting*>(task.m_handle->m_extraData->ptr)->taskListeners.emplace_back(taskToWait.map(
                    [markAsDone](auto* result) {
                        markAsDone(result);
                        return std::monostate();
                    },
                    [](auto*) { return std::monostate(); },
                    [markAsDone]() { markAsDone(nullptr); }
                ));
            }
            return task;
        }

        /**
         * Create a new Task that listens to this Task and maps the values using 
         * the provided functions. 
         * The new Task takes (shared) ownership of this Task, so the new Task 
         * may very well be its only listener
         * @param resultMapper Function that converts the finished values of 
         * the mapped Task to a desired type. Note that the function is only 
         * given a pointer to the finish value, as `T` is not guaranteed to be 
         * copyable - the mapper may NOT move out of the value!
         * @param progressMapper Function that converts the progress values of 
         * the mapped Task to a desired type
         * @param onCancelled Function that is called if the mapped Task is 
         * cancelled
         * @param name The name of the Task; used for debugging. The name of 
         * the mapped task is appended to the end
         */
        template <class ResultMapper, class ProgressMapper, class OnCancelled>
        auto map(ResultMapper&& resultMapper, ProgressMapper&& progressMapper, OnCancelled&& onCancelled, std::string_view const name = "<Mapping Task>") const {
            using T2 = decltype(resultMapper(std::declval<T*>()));
            using P2 = decltype(progressMapper(std::declval<P*>()));

            static_assert(std::is_move_constructible_v<T2>, "The type being mapped to must be move-constructible!");
            static_assert(std::is_move_constructible_v<P2>, "The type being mapped to must be move-constructible!");

            Task<T2, P2> task = Task<T2, P2>::Handle::create(fmt::format("{} <= {}", name, m_handle->m_name));

            // Lock the current task until we have managed to create our new one
            std::unique_lock<std::recursive_mutex> lock(m_handle->m_mutex);

            // If the current task is cancelled, cancel the new one immediately
            if (m_handle->m_status == Status::Cancelled) {
                onCancelled();
                Task<T2, P2>::cancel(task.m_handle);
            }
            // If the current task is finished, immediately map the value and post that
            else if (m_handle->m_status == Status::Finished) {
                Task<T2, P2>::finish(task.m_handle, std::move(resultMapper(&*m_handle->m_resultValue)));
            }
            // Otherwise start listening and waiting for the current task to finish
            else {
                task.m_handle->m_extraData = std::make_unique<typename Task<T2, P2>::Handle::ExtraData>(
                    static_cast<void*>(new EventListener<Task>(
                        [
                            handle = std::weak_ptr(task.m_handle),
                            resultMapper = std::move(resultMapper),
                            progressMapper = std::move(progressMapper),
                            onCancelled = std::move(onCancelled)
                        ](Event* event) mutable {
                            if (auto v = event->getValue()) {
                                Task<T2, P2>::finish(handle.lock(), std::move(resultMapper(v)));
                            }
                            else if (auto p = event->getProgress()) {
                                Task<T2, P2>::progress(handle.lock(), std::move(progressMapper(p)));
                            }
                            else if (event->isCancelled()) {
                                onCancelled();
                                Task<T2, P2>::cancel(handle.lock());
                            }
                        },
                        *this
                    )),
                    +[](void* ptr) {
                        delete static_cast<EventListener<Task>*>(ptr);
                    },
                    +[](void* ptr) {
                        // Cancel the mapped task too
                        static_cast<EventListener<Task>*>(ptr)->getFilter().cancel();
                    }
                );
            }
            return task;
        }

        /**
         * Create a new Task that listens to this Task and maps the values using 
         * the provided functions. 
         * The new Task takes (shared) ownership of this Task, so the new Task 
         * may very well be its only listener
         * @param resultMapper Function that converts the finished values of 
         * the mapped Task to a desired type. Note that the function is only 
         * given a pointer to the finish value, as `T` is not guaranteed to be 
         * copyable - the mapper may NOT move out of the value!
         * @param progressMapper Function that converts the progress values of 
         * the mapped Task to a desired type
         * @param name The name of the Task; used for debugging. The name of 
         * the mapped task is appended to the end
         */        template <class ResultMapper, class ProgressMapper>
        auto map(ResultMapper&& resultMapper, ProgressMapper&& progressMapper, std::string_view const name = "<Mapping Task>") const {
            return this->map(std::move(resultMapper), std::move(progressMapper), +[]() {}, name);
        }

        /**
         * Create a new Task that listens to this Task and maps the finish value 
         * using the provided function. Progress is mapped by copy-constructing 
         * the value as-is.
         * The new Task takes (shared) ownership of this Task, so the new Task 
         * may very well be its only listener
         * @param resultMapper Function that converts the finished values of 
         * the mapped Task to a desired type. Note that the function is only 
         * given a pointer to the finish value, as `T` is not guaranteed to be 
         * copyable - the mapper may NOT move out of the value!
         * @param name The name of the Task; used for debugging. The name of 
         * the mapped task is appended to the end
         */        template <class ResultMapper>
            requires std::copy_constructible<P>
        auto map(ResultMapper&& resultMapper, std::string_view const name = "<Mapping Task>") const {
            return this->map(std::move(resultMapper), +[](P* p) -> P { return *p; }, name);
        }

        /**
         * Creates an implicit event listener for this Task that will call the
         * provided functions when the Task finishes, progresses, or is cancelled.
         * The listener will automatically be destroyed after the Task has finished.
         * @param onResult Function to call when the Task finishes. The function
         * is given a pointer to the finished value, `T*`.
         * @param onProgress Function to call when the Task progresses. The function
         * is given a pointer to the progress value, `P*`.
         * @param onCancelled Function to call when the Task is cancelled
         * 
         * @warning This method should only be used in a global context. If you rely
         * on some node still existing when the task completes, use an event listener instead.
         */
        template <class OnResult, class OnProgress, class OnCancelled>
        void listen(OnResult&& onResult, OnProgress&& onProgress, OnCancelled&& onCancelled) const {
            // use a raw pointer to avoid cyclic references,
            // we destroy it manually later on
            auto* listener = new EventListener<Task>(*this);
            listener->bind([
                onResult = std::move(onResult),
                onProgress = std::move(onProgress),
                onCancelled = std::move(onCancelled),
                listener
            ](Event* event) mutable {
                bool finished = false;
                if (auto v = event->getValue()) {
                    finished = true;
                    onResult(v);
                }
                else if (auto p = event->getProgress()) {
                    onProgress(p);
                }
                else if (event->isCancelled()) {
                    finished = true;
                    onCancelled();
                }
                if (finished) {
                    // delay destroying the listener for a frame
                    // to prevent any potential use-after-free
                    queueInMainThread([listener] {
                        delete listener;
                    });
                }
            });
        }

        /**
         * Creates an implicit event listener for this Task that will call the
         * provided functions when the Task finishes or progresses.
         * The listener will automatically be destroyed after the Task has finished.
         * @param onResult Function to call when the Task finishes. The function
         * is given a pointer to the finished value, `T*`.
         * @param onProgress Function to call when the Task progresses. The function
         * is given a pointer to the progress value, `P*`.
         * 
         * @warning This method should only be used in a global context. If you rely
         * on some node still existing when the task completes, use an event listener instead.
         */
        template <class OnResult, class OnProgress>
        void listen(OnResult&& onResult, OnProgress&& onProgress) const {
            this->listen(std::move(onResult), std::move(onProgress), [] {});
        }

        /**
         * Creates an implicit event listener for this Task that will call the
         * provided function when the Task finishes.
         * The listener will automatically be destroyed after the Task has finished.
         * @param onResult Function to call when the Task finishes. The function
         * is given a pointer to the finished value, `T*`.
         * 
         * @warning This method should only be used in a global context. If you rely
         * on some node still existing when the task completes, use an event listener instead.
         */
        template <class OnResult, class OnProgress>
        void listen(OnResult&& onResult) const {
            this->listen(std::move(onResult), [](auto const&) {}, [] {});
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
                    queueInMainThread([handle = m_handle, listener = m_listener, value = &*m_handle->m_resultValue]() {
                        auto ev = Event::createFinished(handle, value);
                        ev.m_for = listener;
                        ev.post();
                    });
                }
                else {
                    queueInMainThread([handle = m_handle, listener = m_listener]() {
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
