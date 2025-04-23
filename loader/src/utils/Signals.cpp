#include <Geode/utils/Signals.hpp>
#include <Geode/loader/Loader.hpp>

using namespace geode;

class SignalObserver::Impl {
public:
    SignalObserverTime time = SignalObserverTime::EndOfFrame;
    bool scheduledEndOfFrame = false;
    SignalObserver::OnNotify onNotify;
    std::vector<std::function<void()>> onDispose;
    std::vector<SignalObserver::OnUnsubscribe> unsubscribe;

    static inline thread_local std::vector<SignalObserver::Ref> CURRENT_OBSERVER_STACK = {};

    Impl(SignalObserver::OnNotify&& onNotify, SignalObserverTime time)
      : onNotify(std::move(onNotify)), time(time)
    {}
};

SignalObserver::Ref::Ref(Impl* impl) : m_impl(impl) {}

void geode::onSignalDispose(std::function<void()> callback) {
    if (SignalObserver::Impl::CURRENT_OBSERVER_STACK.size()) {
        SignalObserver::Impl::CURRENT_OBSERVER_STACK.back().m_impl->onDispose.push_back(callback);
    }
}

void SignalObserver::Ref::execute() {
    this->dispose();
    Impl::CURRENT_OBSERVER_STACK.push_back(*this);
    m_impl->onNotify();
    Impl::CURRENT_OBSERVER_STACK.pop_back();
}
void SignalObserver::Ref::dispose() {
    for (auto unsubscribe : m_impl->unsubscribe) {
        unsubscribe();
    }
    m_impl->unsubscribe.clear();
    for (auto dispose : m_impl->onDispose) {
        dispose();
    }
    m_impl->onDispose.clear();
}

void SignalObserver::Ref::onSubscribed(SignalObserver::OnUnsubscribe unsubscribe) {
    m_impl->unsubscribe.push_back(unsubscribe);
}
void SignalObserver::Ref::notify() {
    if (m_impl->time == SignalObserverTime::Immediate) {
        this->execute();
    }
    else if (!m_impl->scheduledEndOfFrame) {
        m_impl->scheduledEndOfFrame = true;
        Loader::get()->queueInMainThreadAtEndOfFrame([ref = Ref(m_impl)]() mutable {
            ref.execute();
            ref.m_impl->scheduledEndOfFrame = false;
        });
    }
}

bool SignalObserver::Ref::operator<(Ref const& other) const {
    return m_impl < other.m_impl;
}

std::optional<SignalObserver::Ref> SignalObserver::getCurrentObserver() {
    return !Impl::CURRENT_OBSERVER_STACK.empty() ?
        std::optional(Impl::CURRENT_OBSERVER_STACK.back()) :
        std::nullopt;
}

SignalObserver::SignalObserver() = default;
SignalObserver::SignalObserver(SignalObserver::OnNotify onNotify, SignalObserverTime time)
  : m_impl(std::make_unique<Impl>(std::move(onNotify), time))
{
    Ref(m_impl.get()).execute();
}
SignalObserver::~SignalObserver() {
    // The observer may have been moved out of
    if (m_impl) {
        Ref(m_impl.get()).dispose();
    }
}

SignalObserver::SignalObserver(SignalObserver&&) = default;
SignalObserver& SignalObserver::operator=(SignalObserver&&) = default;

void SignalObserver::notify() {
    Ref(m_impl.get()).notify();
}
