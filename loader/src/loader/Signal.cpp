#include <Geode/loader/Signal.hpp>

namespace geode::comm {
	thread_local std::vector<ObserverContext> ObserverContext::stack;

	ObserverContext* ObserverContext::top() noexcept {
		if (ObserverContext::stack.empty())
			return nullptr;
		return &ObserverContext::stack.back();
	}

	void ObserverContext::operator()() const noexcept {
		impl->clearSignals();

		ObserverContext::stack.push_back(*this);
		std::invoke(impl->effect);
		ObserverContext::stack.pop_back();
	}

	void ObserverContext::registerSignal(std::shared_ptr<SignalInternal> sig) noexcept {
		for (auto& [s, _] : impl->registered) {
			if (!s.owner_before(sig) && !sig.owner_before(s))
				return;
		}
		impl->registered.emplace_back(sig, sig->addPortReceiver(*this));
	}

	void ObserverContext::Impl::clearSignals() const noexcept {
		for (auto& [weak, handle] : registered) {
			if (auto sig = weak.lock())
				sig->removePortReceiver(handle);
		}
		registered.clear();
	}

	ObserverContext::Impl::~Impl() noexcept { clearSignals(); }

	ObserverContext::ObserverContext(geode::Function<void()> eff, std::monostate) noexcept
		: impl(std::make_shared<ObserverContext::Impl>(std::move(eff))) {}
}
