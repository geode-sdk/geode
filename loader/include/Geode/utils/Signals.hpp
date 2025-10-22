#pragma once

#include "../DefaultInclude.hpp"
#include <compare>
#include <set>
#include <functional>
#include <optional>

namespace geode {
    template <class T>
    class Signal;

    namespace detail {
        class SignalImpl;
    }

    GEODE_DLL void onSignalDispose(std::function<void()> callback);

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
        GEODE_DLL friend void onSignalDispose(std::function<void()> callback);

    public:
        SignalObserver();
        SignalObserver(
            OnNotify onNotify,
            SignalObserverTime time = SignalObserverTime::EndOfFrame,
            bool executeImmediately = true
        );
        ~SignalObserver();

        SignalObserver(SignalObserver&&);
        SignalObserver& operator=(SignalObserver&&);

        SignalObserver(SignalObserver const&) = delete;
        SignalObserver& operator=(SignalObserver const&) = delete;

        /// Queues execution of the observer, according to the set `SignalObserverTime`.
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

            SignalImpl(SignalImpl&&);
            SignalImpl& operator=(SignalImpl&&);

            void valueObserved() const;
            void valueModified() const;
            void derive(geode::SignalObserver&& observer);

            // This is only needed because of derivation and it's ugly as hell...
            static void valueObserved(Impl* impl);
            static void valueModified(Impl* impl);
            Impl* weak();
        };

        template <class T>
        class ImmutableSignal {
        protected:
            std::unique_ptr<T> m_value;
            detail::SignalImpl m_impl;

            friend class geode::SignalObserver;
            friend class Signal<T>; // needed for Signal::derived

            ImmutableSignal() {} // for DerivedSignal
        public:
            ImmutableSignal(T const& value) requires std::copy_constructible<T>
              : m_value(std::make_unique<T>(value)) {}
            ImmutableSignal(T&& value) requires std::move_constructible<T>
              : m_value(std::make_unique<T>(std::move(value))) {}

            ImmutableSignal(ImmutableSignal&&) = default;
            ImmutableSignal& operator=(ImmutableSignal&&) = default;

            ImmutableSignal(ImmutableSignal const&) = delete;
            ImmutableSignal& operator=(ImmutableSignal const&) = delete;

            T const& get() const {
                m_impl.valueObserved();
                return *m_value;
            }

            bool operator==(T const& other) const requires std::equality_comparable<T> {
                return this->get() == other;
            }
            bool operator==(ImmutableSignal<T> const& other) const requires std::equality_comparable<T> {
                return this->get() == other.get();
            }
            bool operator<=>(T const& other) const requires std::three_way_comparable<T> {
                return this->get() <=> other;
            }
            bool operator<=>(ImmutableSignal<T> const& other) const requires std::three_way_comparable<T> {
                return this->get() <=> other.get();
            }
            bool operator!() const requires std::convertible_to<T, bool> {
                return !this->get();
            }
            T const& operator*() const {
                return this->get();
            }
            T const& operator->() const {
                return this->get();
            }
        };
    }

    template <class T>
    class DerivedSignal final : public detail::ImmutableSignal<T> {
        using Base = detail::ImmutableSignal<T>;
    public:
        DerivedSignal(std::function<T()> derivedEffect) : Base() {
            SignalObserver observer(
                [
                    this,
                    value = static_cast<T*>(nullptr),
                    impl = Base::m_impl.weak(),
                    effect = std::move(derivedEffect)
                ]() mutable {
                    if (!value) [[unlikely]] {
                        // should be fine to access m_value here,
                        // this will only run once
                        Base::m_value = std::make_unique<T>(effect());
                        value = Base::m_value.get();
                    } else {
                        *value = effect();
                        detail::SignalImpl::valueModified(impl);
                    }
                },
                SignalObserverTime::Immediate,
                true
            );
            Base::m_impl.derive(std::move(observer));
        }
    };

    template <class T>
    class Signal final : public detail::ImmutableSignal<T> {
        using Base = detail::ImmutableSignal<T>;
    public:
        Signal(T const& value) requires std::copy_constructible<T>
          : Base(value) {}
        Signal(T&& value) requires std::move_constructible<T>
          : Base(std::move(value)) {}

        Signal(Signal&&) = default;
        Signal& operator=(Signal&&) = default;

        Signal(Signal const&) = delete;
        Signal& operator=(Signal const&) = delete;

        void set(T const& value) requires std::copy_constructible<T> {
            *Base::m_value = value;
            Base::m_impl.valueModified();
        }
        void set(T&& value) requires std::move_constructible<T> {
            *Base::m_value = std::move(value);
            Base::m_impl.valueModified();
        }
        void update(auto&& updater) {
            this->set(updater(this->get()));
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
            Base::m_impl.valueObserved();
            *Base::m_value += value;
            Base::m_impl.valueModified();
            return *this;
        }
        Signal& operator-=(T const& value) requires requires(T& a, T const& b) { a -= b; } {
            Base::m_impl.valueObserved();
            *Base::m_value -= value;
            Base::m_impl.valueModified();
            return *this;
        }
        Signal& operator*=(T const& value) requires requires(T& a, T const& b) { a *= b; } {
            Base::m_impl.valueObserved();
            *Base::m_value *= value;
            Base::m_impl.valueModified();
            return *this;
        }
        Signal& operator/=(T const& value) requires requires(T& a, T const& b) { a /= b; } {
            Base::m_impl.valueObserved();
            *Base::m_value /= value;
            Base::m_impl.valueModified();
            return *this;
        }
    };
}
