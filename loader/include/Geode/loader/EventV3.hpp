#pragma once

#include <functional>
#include <concepts>
#include <mutex>
#include <ranges>
#include <unordered_map>
#include <atomic>
#include <algorithm>
#include <tuple>

namespace geode::event::v3 {
    template <typename T>
    concept IsHashable = requires(T a) {
        { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
    };

    template <typename T>
    concept IsEqualityComparable = requires(T a, T b) {
        { a == b } -> std::convertible_to<bool>;
    };

    template <class PoolType>
    class Pool : protected PoolType {
    public:
        using EventType = typename PoolType::EventType;
        using CallbackType = typename PoolType::CallbackType;

    private:
        std::mutex mutex;
        std::atomic_size_t recurse;
        std::vector<std::pair<EventType*, CallbackType>> toAdd;
        std::vector<std::pair<EventType*, EventType*>> toMove;
        std::vector<EventType*> toRemove;

        void catchup() {
            std::lock_guard lock(mutex);
            for (auto& ev : toRemove) {
                this->PoolType::removeListener(ev);
            }
            for (auto& [ev, cb] : toAdd) {
                this->PoolType::addListener(ev, std::move(cb));
            }
            for (auto& [from, to] : toMove) {
                this->PoolType::moveListener(from, to);
            }
            toRemove.clear();
            toAdd.clear();
            toMove.clear();
        }

    public:
        template <class... Args>
        void callListeners(EventType* event, Args&&... args) {
            recurse++;
            auto listeners = this->getListeners(event);
            for (const auto& listener : listeners) {
                listener(std::forward<Args>(args)...);
            }
            recurse--;

            if (recurse == 0) {
                this->catchup();
            }
        }

        void addListener(EventType* event, CallbackType callback) {
            std::lock_guard lock(mutex);
            if (recurse == 0) {
                this->PoolType::addListener(event, std::move(callback));
            } else {
                toAdd.emplace_back(event, std::move(callback));
            }
        }

        void removeListener(EventType* event) {
            std::lock_guard lock(mutex);
            if (recurse == 0) {
                this->PoolType::removeListener(event);
            } else {
                toRemove.emplace_back(event);
            }
        }

        void moveListener(EventType* from, EventType* to) {
            std::lock_guard lock(mutex);
            if (recurse == 0) {
                this->PoolType::moveListener(from, to);
            } else {
                toMove.emplace_back(from, to);
            }
        }
    };

    template <class E, class C>
    class HashablePool {
    protected:
        using EventType = E;
        using CallbackType = std::function<C>;

    private:
        std::unordered_multimap<std::size_t, CallbackType> listeners;

    protected:
        auto getListeners(EventType* event) const {
            auto const hash = std::hash<EventType>{}(*event);
            auto [begin, end] = listeners.equal_range(hash);
            auto getSecond = [](auto const& pair) {
                return pair.second;
            };
            return std::ranges::subrange(begin, end) | std::views::transform(getSecond);
        }

        void addListener(EventType* event, CallbackType callback) {
            auto const hash = std::hash<EventType>{}(*event);
            listeners.emplace(hash, std::move(callback));
        }

        void removeListener(EventType* event) {
            auto const hash = std::hash<EventType>{}(*event);
            listeners.erase(hash);
        }

        void moveListener(EventType* from, EventType* to) {}
    };

    template <class E, class C>
    class EqualityComparablePool {
    protected:
        using EventType = E;
        using CallbackType = std::function<C>;

    private:
        std::vector<std::pair<EventType*, CallbackType>> listeners;

    protected:
        auto getListeners(EventType* event) const {
            auto isEvent = [event](auto const& pair) {
                return pair.first->operator==(*event);
            };
            auto getSecond = [](auto const& pair) {
                return pair.second;
            };
            return std::ranges::filter_view(listeners, isEvent) | std::views::transform(getSecond);
        }

        void addListener(EventType* event, CallbackType callback) {
            listeners.emplace_back(event, std::move(callback));
        }

        void removeListener(EventType* event) {
            std::erase_if(listeners, [&event](auto const& pair) {
                return pair.first == event;
            });
        }

        void moveListener(EventType* from, EventType* to) {
            std::ranges::for_each(listeners, [&](auto& pair) {
                if (pair.first == from) {
                    pair.first = to;
                }
            });
        }
    };

    template <class PoolType>
    class Event {
    public:
        using CallbackType = typename PoolType::CallbackType;
        using EventType = typename PoolType::EventType;

    protected:
        static inline PoolType pool;

        EventType* self() {
            return static_cast<EventType*>(this);
        }

    private:
        bool moved;

    public:
        template <class... Args>
        void post(Args&&... args) {
            pool.callListeners(self(), std::forward<Args>(args)...);
        }

        EventType listen(CallbackType callback) && {
            moved = false;
            pool.addListener(self(), std::move(callback));
            return std::move(*self());
        }

        EventType& listen(CallbackType callback) & {
            moved = false;
            pool.addListener(self(), std::move(callback));
            return *self();
        }

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

    class TestEvent : public Event<Pool<EqualityComparablePool<TestEvent, void(std::string_view)>>> {
    public:
        std::optional<size_t> key;

        TestEvent() : key() {}
        TestEvent(size_t key) : key(key) {}

        bool operator==(TestEvent const& other) {
            if (!key) {
                return true;
            }
            if (!other.key) {
                return false;
            }
            return *key == *other.key;
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