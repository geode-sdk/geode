#pragma once

#include "../DefaultInclude.hpp"
#include <set>
#include <functional>
#include <optional>

namespace geode {
    template <class T>
    class Signal;

    void onSignalDispose(std::function<void()> callback);

    enum class SignalObserverTime : uint8_t {
        EndOfFrame,
        Immediate,
    };

    class GEODE_DLL SignalObserver final {
    public:
        using OnUnsubscribe = std::function<void()>;
        using OnNotify = std::function<void()>;

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

        template <class T>
        friend class Signal;

        friend void onSignalDispose(std::function<void()> callback);
    
        class Ref final {
        private:
            Impl* m_impl;

            friend class SignalObserver;
            friend void onSignalDispose(std::function<void()> callback);

            void execute();
            void dispose();

        public:
            Ref(Impl*);

            void onSubscribed(OnUnsubscribe unsubscribe);
            void notify();

            bool operator<(Ref const& other) const;
        };

        static std::optional<SignalObserver::Ref> getCurrentObserver();

    public:
        SignalObserver();
        SignalObserver(OnNotify onNotify, SignalObserverTime time = SignalObserverTime::EndOfFrame);
        ~SignalObserver();

        SignalObserver(SignalObserver&&);
        SignalObserver& operator=(SignalObserver&&);

        SignalObserver(SignalObserver const&) = delete;
        SignalObserver& operator=(SignalObserver const&) = delete;

        void notify();
    };

    template <class T>
    class Signal final {
    private:
        T m_value;
        mutable std::multiset<SignalObserver::Ref> m_observers;

        // currently this crashes because i think ~Signal is called before ~SignalObserver which results in UB
        // so SignalObserver should probably own its Signals in some way
    
    public:
        Signal(T const& value) requires std::copy_constructible<T>
          : m_value(value) {}
        Signal(T&& value) requires std::move_constructible<T>
          : m_value(std::move(value)) {}

        Signal(Signal&&) = default;
        Signal& operator=(Signal&&) = default;

        Signal(Signal const&) = delete;
        Signal& operator=(Signal const&) = delete;

        T const& get() const {
            auto obs = SignalObserver::getCurrentObserver();
            if (obs && !m_observers.contains(*obs)) {
                m_observers.insert(*obs);
                obs->onSubscribed([this, obs = *obs]() {
                    m_observers.erase(obs);
                });
            }
            return m_value;
        }
        void modified() {
            // notify() calls unsubscribe methods which will mutate m_observers 
            auto observers = m_observers;
            for (auto observer : observers) {
                observer.notify();
            }
        }
        void set(T const& value) requires std::copy_constructible<T> {
            m_value = value;
            this->modified();
        }
        void set(T&& value) requires std::move_constructible<T> {
            m_value = std::move(value);
            this->modified();
        }
        void update(auto&& updater) {
            this->set(updater(m_value));
        }

        bool operator==(T const& other) const requires std::equality_comparable<T> {
            return this->get() == other;
        }
        bool operator==(Signal<T> const& other) const requires std::equality_comparable<T> {
            return this->get() == other.get();
        }
        T const& operator*() const {
            return this->get();
        }
        T const& operator->() const {
            return this->get();
        }
        Signal& operator=(T const& value) requires std::copy_constructible<T> {
            this->set(value);
            return *this;
        }
        Signal& operator=(T&& value) requires std::move_constructible<T> {
            this->set(std::move(value));
            return *this;
        }
        Signal& operator+=(T const& value) requires requires(T& a, T const& b) { a += b; } {
            m_value += value;
            this->modified();
            return *this;
        }
        Signal& operator-=(T const& value) requires requires(T& a, T const& b) { a -= b; } {
            m_value -= value;
            this->modified();
            return *this;
        }
        Signal& operator*=(T const& value) requires requires(T& a, T const& b) { a *= b; } {
            m_value *= value;
            this->modified();
            return *this;
        }
        Signal& operator/=(T const& value) requires requires(T& a, T const& b) { a /= b; } {
            m_value /= value;
            this->modified();
            return *this;
        }
    };
}
