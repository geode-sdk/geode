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

    void notify(Mod* mod) {
        // You are not allowed to mutate signals in observed contexts
        if (CURRENT_OBSERVER_STACK.size()) {
            utils::terminate(fmt::format(
                "The mod '{}' attempted to mutate a signal in an observed context. "
                "This is disallowed - use `Signal::derived` to create signals whose "
                "values depend on other signals!",
                mod->getID()
            ));
        }
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

detail::SignalImpl::SignalImpl() : m_impl(std::make_unique<Impl>()) {}
detail::SignalImpl::~SignalImpl() {
    for (auto observer : m_impl->observers) {
        observer->observedSignals.erase(m_impl.get());
    }
}
detail::SignalImpl::SignalImpl(SignalImpl&&) = default;
detail::SignalImpl& detail::SignalImpl::operator=(SignalImpl&&) = default;

void detail::SignalImpl::valueObserved(Mod*) const {
    if (!SignalObserver::Impl::CURRENT_OBSERVER_STACK.empty()) {
        auto obs = SignalObserver::Impl::CURRENT_OBSERVER_STACK.back();
        if (!m_impl->observers.contains(obs)) {
            m_impl->observers.insert(obs);
            obs->observedSignals.insert(m_impl.get());
        }
    }
}
void detail::SignalImpl::valueModified(Mod* mod) const {
    // notify() calls unsubscribe methods which will mutate m_observers 
    auto observers = m_impl->observers;
    for (auto observer : observers) {
        observer->notify(mod);
    }
}
void detail::SignalImpl::derive(geode::SignalObserver&& observer) {
    m_impl->derivedObserver = std::move(observer);
}

void geode::onSignalDispose(std::function<void()> callback) {
    if (SignalObserver::Impl::CURRENT_OBSERVER_STACK.size()) {
        SignalObserver::Impl::CURRENT_OBSERVER_STACK.back()->onDispose.push_back(callback);
    }
}

SignalObserver::SignalObserver() = default;
SignalObserver::SignalObserver(SignalObserver::OnNotify onNotify, SignalObserverTime time)
  : m_impl(std::make_unique<Impl>(std::move(onNotify), time))
{
    m_impl->execute();
}
SignalObserver::~SignalObserver() {
    // The observer may have been moved out of
    if (m_impl) {
        m_impl->dispose();
    }
}

SignalObserver::SignalObserver(SignalObserver&&) = default;
SignalObserver& SignalObserver::operator=(SignalObserver&&) = default;

void SignalObserver::notify(Mod* mod) {
    m_impl->notify(mod);
}
