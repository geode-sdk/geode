#pragma once

#include "../DefaultInclude.hpp"
#include <compare>
#include <set>
#include <functional>
#include <optional>

namespace geode {
    template <class T>
    class Signal;

    class Mod;
    Mod* getMod();

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

        void notify(Mod* mod);
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

            void valueObserved(Mod* mod) const;
            void valueModified(Mod* mod) const;
            void derive(geode::SignalObserver&& observer);
        };
    }

    template <class T>
    class ImmutableSignal {
    protected:
        std::unique_ptr<T> m_value;
        detail::SignalImpl m_impl;

        friend class SignalObserver;
        friend class Signal<T>; // needed for Signal::derived

    public:
        ImmutableSignal(T const& value) requires std::copy_constructible<T>
          : m_value(std::make_unique<T>(value)) {}
        ImmutableSignal(T&& value) requires std::move_constructible<T>
          : m_value(std::make_unique<T>(std::move(value))) {}

        ImmutableSignal(ImmutableSignal&&) = default;
        ImmutableSignal& operator=(ImmutableSignal&&) = default;

        ImmutableSignal(ImmutableSignal const&) = delete;
        ImmutableSignal& operator=(ImmutableSignal const&) = delete;

        T const& get(Mod* in = getMod()) const {
            m_impl.valueObserved(in);
            return *m_value;
        }

        bool operator==(T const& other) const requires std::equality_comparable<T> {
            return this->get() == other;
        }
        bool operator==(Signal<T> const& other) const requires std::equality_comparable<T> {
            return this->get() == other.get();
        }
        bool operator<=>(T const& other) const requires std::three_way_comparable<T> {
            return this->get() <=> other;
        }
        bool operator<=>(Signal<T> const& other) const requires std::three_way_comparable<T> {
            return this->get() <=> other.get();
        }
        T const& operator*() const {
            return this->get();
        }
        T const& operator->() const {
            return this->get();
        }
    };

    template <class T>
    class Signal final : public ImmutableSignal<T> {
    public:
        Signal(T const& value) requires std::copy_constructible<T>
          : ImmutableSignal<T>(value) {}
        Signal(T&& value) requires std::move_constructible<T>
          : ImmutableSignal<T>(std::move(value)) {}
        
        static ImmutableSignal<T> derived(std::function<T()> effect) {
            ImmutableSignal<T> signal = ImmutableSignal<T>(effect());
            signal.m_impl.derive(SignalObserver(
                [value = signal.m_value.get(), effect = std::move(effect)]() {
                    *value = effect();
                    // Do NOT fire valueModified as signals are not normally 
                    // allowed to be mutated in observed contexts and would 
                    // terminate! 
                },
                SignalObserverTime::Immediate
            ));
            return signal;
        }

        Signal(Signal&&) = default;
        Signal& operator=(Signal&&) = default;

        Signal(Signal const&) = delete;
        Signal& operator=(Signal const&) = delete;

        void set(T const& value, Mod* in = getMod()) requires std::copy_constructible<T> {
            *ImmutableSignal<T>::m_value = value;
            ImmutableSignal<T>::m_impl.valueModified(in);
        }
        void set(T&& value, Mod* in = getMod()) requires std::move_constructible<T> {
            *ImmutableSignal<T>::m_value = std::move(value);
            ImmutableSignal<T>::m_impl.valueModified(in);
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
            ImmutableSignal<T>::m_impl.valueObserved(getMod());
            *ImmutableSignal<T>::m_value += value;
            ImmutableSignal<T>::m_impl.valueModified(getMod());
            return *this;
        }
        Signal& operator-=(T const& value) requires requires(T& a, T const& b) { a -= b; } {
            ImmutableSignal<T>::m_impl.valueObserved(getMod());
            *ImmutableSignal<T>::m_value -= value;
            ImmutableSignal<T>::m_impl.valueModified(getMod());
            return *this;
        }
        Signal& operator*=(T const& value) requires requires(T& a, T const& b) { a *= b; } {
            ImmutableSignal<T>::m_impl.valueObserved(getMod());
            *ImmutableSignal<T>::m_value *= value;
            ImmutableSignal<T>::m_impl.valueModified(getMod());
            return *this;
        }
        Signal& operator/=(T const& value) requires requires(T& a, T const& b) { a /= b; } {
            ImmutableSignal<T>::m_impl.valueObserved(getMod());
            *ImmutableSignal<T>::m_value /= value;
            ImmutableSignal<T>::m_impl.valueModified(getMod());
            return *this;
        }
    };
}
