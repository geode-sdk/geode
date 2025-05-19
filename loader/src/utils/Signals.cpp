#include <Geode/utils/Signals.hpp>
#include <Geode/loader/Loader.hpp>
#include <unordered_set>

using namespace geode;

class detail::SignalImpl::Impl {
    public:
        std::multiset<SignalObserver::Impl*> observers;
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

    void notify() {
        // Do not notify if this observer is actively observing to prevent an 
        // infinite loop
        for (auto& observer : CURRENT_OBSERVER_STACK) {
            if (observer == this) return;
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

void detail::SignalImpl::valueObserved() const {
    if (!SignalObserver::Impl::CURRENT_OBSERVER_STACK.empty()) {
        auto obs = SignalObserver::Impl::CURRENT_OBSERVER_STACK.back();
        if (!m_impl->observers.contains(obs)) {
            m_impl->observers.insert(obs);
            obs->observedSignals.insert(m_impl.get());
        }
    }
}
void detail::SignalImpl::valueModified() const {
    // notify() calls unsubscribe methods which will mutate m_observers 
    auto observers = m_impl->observers;
    for (auto observer : observers) {
        observer->notify();
    }
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

void SignalObserver::notify() {
    m_impl->notify();
}
