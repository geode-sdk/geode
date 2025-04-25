#pragma once

#include "../DefaultInclude.hpp"
#include <set>
#include <functional>
#include <optional>

namespace geode {
    template <class T>
    class Signal;
    
    namespace detail {
        class SignalImpl;
    }

    void onSignalDispose(std::function<void()> callback);

    enum class SignalObserverTime : uint8_t {
        EndOfFrame,
        Immediate,
    };

    class GEODE_DLL SignalObserver final {
    public:
        using OnNotify = std::function<void()>;

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

        template <class T>
        friend class Signal;
        friend class detail::SignalImpl;
        friend void onSignalDispose(std::function<void()> callback);
    
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

    namespace detail {
        class GEODE_DLL SignalImpl final {
        private:
            class Impl;
            std::unique_ptr<Impl> m_impl;

            friend class geode::SignalObserver;
        
        public:
            SignalImpl();
            ~SignalImpl();

            void valueObserved() const;
            void valueModified() const;
        };
    }

    template <class T>
    class Signal final {
    private:
        T m_value;
        detail::SignalImpl m_impl;

        friend class SignalObserver;

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
            m_impl.valueObserved();
            return m_value;
        }
        void set(T const& value) requires std::copy_constructible<T> {
            m_value = value;
            m_impl.valueModified();
        }
        void set(T&& value) requires std::move_constructible<T> {
            m_value = std::move(value);
            m_impl.valueModified();
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
            m_impl.valueModified();
            return *this;
        }
        Signal& operator-=(T const& value) requires requires(T& a, T const& b) { a -= b; } {
            m_value -= value;
            m_impl.valueModified();
            return *this;
        }
        Signal& operator*=(T const& value) requires requires(T& a, T const& b) { a *= b; } {
            m_value *= value;
            m_impl.valueModified();
            return *this;
        }
        Signal& operator/=(T const& value) requires requires(T& a, T const& b) { a /= b; } {
            m_value /= value;
            m_impl.valueModified();
            return *this;
        }
    };
}
