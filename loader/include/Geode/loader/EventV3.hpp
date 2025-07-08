#pragma once

#include <functional>
#include <concepts>
#include <mutex>
#include <ranges>
#include <unordered_map>
#include <atomic>
#include <algorithm>
#include <tuple>
#include <optional>

namespace geode::event::v3 {
    template <typename T>
    concept IsHashable = requires(T a) {
        { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
    };

    template <typename T>
    concept IsEqualityComparable = requires(T a, T b) {
        { a == b } -> std::convertible_to<bool>;
    };

    class Pool {
    private:
        std::mutex mutex;
        std::atomic_size_t recurse;
        std::vector<std::pair<void*, void*>> toAdd;
        std::vector<std::pair<void*, void*>> toMove;
        std::vector<void*> toRemove;

        void catchup() {
            std::lock_guard lock(mutex);
            for (auto& ev : toRemove) {
                this->removeListener(ev);
            }
            for (auto& [ev, cb] : toAdd) {
                this->addListener(ev, std::move(cb));
            }
            for (auto& [from, to] : toMove) {
                this->moveListener(from, to);
            }
            toRemove.clear();
            toAdd.clear();
            toMove.clear();
        }

    protected:
        virtual ~Pool() = default;
        virtual void callListener(void* event, void* params) = 0;
        virtual void addListener(void* event, void* callable) = 0;
        virtual void removeListener(void* event) = 0;
        virtual void moveListener(void* from, void* to) = 0;

    public:
        template <class... Args>
        void callListeners(void* event, void* params) {
            recurse++;
            this->callListener(event, params);
            recurse--;

            if (recurse == 0) {
                this->catchup();
            }
        }

        void addListenerSafe(void* event, void* callable) {
            std::lock_guard lock(mutex);
            if (recurse >= 0) {
                toAdd.emplace_back(event, callable);
            }
            else {
                this->addListener(event, callable);
            }
        }

        void removeListenerSafe(void* event) {
            std::lock_guard lock(mutex);
            if (recurse >= 0) {
                toRemove.emplace_back(event);
            }
            else {
                this->removeListener(event);
            }
        }

        void moveListenerSafe(void* from, void* to) {
            std::lock_guard lock(mutex);
            if (recurse >= 0) {
                toMove.emplace_back(from, to);
            }
            else {
                this->moveListener(from, to);
            }
        }
    };

    template <class... Args>
    class ForwardHelper {
    protected:
        using CallbackType = std::function<void(Args...)>;
        using ForwardTupleType = std::tuple<Args&&...>;

        void call(void* c, void* t) {
            auto callback = static_cast<CallbackType*>(c);
            auto tuple = static_cast<ForwardTupleType*>(t);
            std::apply(*callback, *tuple);
        }

    public:
        auto forward(Args&&... args) {
            return ForwardTupleType(std::forward<Args>(args)...);
        }

        auto callback(std::invocable auto callable) {
            return CallbackType(callable);
        }
    };

    template <class E, class... Args>
    class HashablePool : public Pool, public ForwardHelper<Args...> {
    protected:
        using EventType = E;

    private:
        std::unordered_multimap<std::size_t, CallbackType> listeners;

        auto getListeners(EventType* event) const {
            auto const hash = std::hash<EventType>{}(*event);
            auto [begin, end] = listeners.equal_range(hash);
            auto getSecond = [](auto const& pair) {
                return pair.second;
            };
            return std::ranges::subrange(begin, end) | std::views::transform(getSecond);
        }

    protected:
        void callListener(void* e, void* p) override {
            auto event = static_cast<EventType*>(e);

            auto listeners = this->getListeners(event);
            for (auto const& listener : listeners) {
                this->call(&listener, p);
            }
        }

        void addListener(void* e, void* c) override {
            auto event = static_cast<EventType*>(e);
            auto callback = static_cast<CallbackType*>(c);

            auto const hash = std::hash<EventType>{}(*event);
            listeners.emplace(hash, std::move(*callback));
        }

        void removeListener(void* e) override {
            auto event = static_cast<EventType*>(e);

            auto const hash = std::hash<EventType>{}(*event);
            listeners.erase(hash);
        }

        void moveListener(void* from, void* to) override {}
    };

    template <class E, class... Args>
    class EqualityComparablePool : public Pool, public ForwardHelper<Args...> {
    protected:
        using EventType = E;

    private:
        std::vector<std::pair<EventType*, CallbackType>> listeners;

        auto getListeners(EventType* event) const {
            auto isEvent = [event](auto const& pair) {
                return pair.first->operator==(*event);
            };
            auto getSecond = [](auto const& pair) {
                return pair.second;
            };
            return std::ranges::filter_view(listeners, isEvent) | std::views::transform(getSecond);
        }

    protected:
        void callListener(void* e, void* p) override {
            auto event = static_cast<EventType*>(e);

            auto listeners = this->getListeners(event);
            for (auto const& listener : listeners) {
                this->call(&listener, p);
            }
        }

        void addListener(void* e, void* c) override {
            auto event = static_cast<EventType*>(e);
            auto callback = static_cast<CallbackType*>(c);

            listeners.emplace_back(event, std::move(*callback));
        }

        void removeListener(void* e) override {
            auto event = static_cast<EventType*>(e);

            std::erase_if(listeners, [&event](auto const& pair) {
                return pair.first == event;
            });
        }

        void moveListener(void* f, void* t) override {
            auto from = static_cast<EventType*>(f);
            auto to = static_cast<EventType*>(t);

            std::ranges::for_each(listeners, [&](auto& pair) {
                if (pair.first == from) {
                    pair.first = to;
                }
            });
        }
    };

    class Event {
    public:
        virtual Pool* getPool() = 0;

    private:
        bool moved;

    public:

        Event() : moved(true) {}
        Event(Event const&) = delete;
        Event(Event&& other) : moved(false) {
            pool.moveListener(other.self(), self());
            other.moved = true;
        }
        Event& operator=(Event const&) = delete;
        Event& operator=(Event&& other) {
            pool.moveListener(other.self(), self());
            other.moved = true;
            return *this;
        }
        ~Event() {
            if (moved) return;
            pool.removeListener(self());
        }
    };

    namespace event {
        using namespace geode::event::v3;
    }

    class TestEvent : public Event {
    private:
        static inline EqualityComparablePool<TestEvent, std::string_view> s_pool;

    public:
        std::optional<size_t> key;

        TestEvent() : key() {}
        TestEvent(size_t key) : key(key) {}

        template <class... Args>
        void post(Args&&... args) {
            pool.callListeners(self(), std::forward<Args>(args)...);
        }

        TestEvent listen(std::invocable auto callback) && {
            moved = false;
            pool.addListener(self(), std::move(callback));
            return std::move(*this);
        }

        TestEvent& listen(CallbackType callback) & {
            moved = false;
            pool.addListener(self(), std::move(callback));
            return *this;
        }

        bool operator==(TestEvent const& other) {
            if (!key) {
                return true;
            }
            return *key == *other.key;
        }

        Pool* getPool() override {
            return &s_pool;
        }
    };
}

namespace std {
    template <class EventType>
    class hash<geode::event::v3::Event<EventType>> {
    public:
        std::size_t operator()(geode::event::v3::Event<EventType> const& event) const {
            return 0;
        }
    };
}