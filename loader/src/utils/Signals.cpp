#include <Geode/utils/Signals.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/terminate.hpp>
#include <unordered_set>

using namespace geode;

class detail::SignalImpl::Impl {
public:
    std::multiset<SignalObserver::Impl*> observers;
    std::optional<SignalObserver> derivedObserver;
};

class SignalObserver::Impl {
public:
    SignalObserverTime time = SignalObserverTime::EndOfFrame;
    bool scheduledEndOfFrame = false;
    SignalObserver::OnNotify onNotify;
    std::vector<std::function<void()>> onDispose;
    std::unordered_set<detail::SignalImpl::Impl*> observedSignals;

    static inline thread_local std::vector<Impl*> CURRENT_OBSERVER_STACK = {};
    static Impl* getTopObserver() {
        return CURRENT_OBSERVER_STACK.empty() ? nullptr : CURRENT_OBSERVER_STACK.back();
    }

    Impl(SignalObserver::OnNotify&& onNotify, SignalObserverTime time)
      : onNotify(std::move(onNotify)), time(time)
    {}

    void execute() {
        this->dispose();
        CURRENT_OBSERVER_STACK.push_back(this);
        this->onNotify();
        CURRENT_OBSERVER_STACK.pop_back();
    }
    void dispose() {
        for (auto observedSignal : this->observedSignals) {
            observedSignal->observers.erase(this);
        }
        this->observedSignals.clear();
        for (auto dispose : this->onDispose) {
            dispose();
        }
        this->onDispose.clear();
    }

    void notify() {
        if (this->time == SignalObserverTime::Immediate) {
            this->execute();
        }
        else if (!this->scheduledEndOfFrame) {
            this->scheduledEndOfFrame = true;
            Loader::get()->queueInMainThreadAtEndOfFrame([this]() {
                this->execute();
                this->scheduledEndOfFrame = false;
            });
        }
    }
};

void geode::onSignalDispose(std::function<void()> callback) {
    auto* observer = SignalObserver::Impl::getTopObserver();
    if (observer) {
        observer->onDispose.emplace_back(std::move(callback));
    }
}

SignalObserver::SignalObserver() = default;
SignalObserver::SignalObserver(
    SignalObserver::OnNotify onNotify,
    SignalObserverTime time,
    bool executeImmediately
)
  : m_impl(std::make_unique<Impl>(std::move(onNotify), time))
{
    // todo: should this notify() instead of execute(), so UI signals would get deferred?
    if (executeImmediately) {
        m_impl->execute();
    }
}
SignalObserver::~SignalObserver() {
    // The observer may have been moved out of
    if (m_impl) {
        m_impl->dispose();
    }
}

SignalObserver::SignalObserver(SignalObserver&&) = default;
SignalObserver& SignalObserver::operator=(SignalObserver&&) = default;

void SignalObserver::notify() {
    m_impl->notify();
}


detail::SignalImpl::SignalImpl() : m_impl(std::make_unique<Impl>()) {}
detail::SignalImpl::~SignalImpl() {
    // SignalImpl may be moved
    if (m_impl) {
        for (auto observer : m_impl->observers) {
            observer->observedSignals.erase(m_impl.get());
        }
    }
}
detail::SignalImpl::SignalImpl(SignalImpl&&) = default;
detail::SignalImpl& detail::SignalImpl::operator=(SignalImpl&&) = default;

detail::SignalImpl::Impl* detail::SignalImpl::weak() {
    return m_impl.get();
}

void detail::SignalImpl::valueObserved(Impl* impl) {
    auto* observer = SignalObserver::Impl::getTopObserver();
    if (observer) {
        if (!impl->observers.contains(observer)) {
            impl->observers.insert(observer);
            observer->observedSignals.insert(impl);
        }
    }
}
void detail::SignalImpl::valueModified(Impl* impl) {
    auto* activeObserver = SignalObserver::Impl::getTopObserver();

    // notify() calls unsubscribe methods which will mutate m_observers
    auto observersCopy = impl->observers;
    for (auto observer : observersCopy) {
        // Do not cause infinite loops!
        if (observer == activeObserver) {
            continue;
        }
        if (impl->derivedObserver && impl->derivedObserver->m_impl.get() == observer) {
            continue;
        }
        observer->notify();
    }
}

void detail::SignalImpl::valueObserved() const {
    SignalImpl::valueObserved(m_impl.get());
}
void detail::SignalImpl::valueModified() const {
    SignalImpl::valueModified(m_impl.get());
}
void detail::SignalImpl::derive(geode::SignalObserver&& observer) {
    m_impl->derivedObserver = std::move(observer);
    // todo: if we decide that SignalObserver constructor should notify() instead of execute(),
    // make that change here too
    // m_impl->derivedObserver->m_impl->execute();
}
