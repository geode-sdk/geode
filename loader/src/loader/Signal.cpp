#include <Geode/loader/Signal.hpp>

namespace geode::comm {
	std::vector<ObserverContext> ObserverContext::stack;

	ObserverContext* ObserverContext::top() noexcept {
		if (ObserverContext::stack.empty())
			return nullptr;
		return &ObserverContext::stack.back();
	}

	void ObserverContext::operator()() noexcept {
		clearSignals();

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

	void ObserverContext::clearSignals() noexcept {
		for (auto& [weak, handle] : impl->registered) {
			if (auto sig = weak.lock())
				sig->removePortReceiver(handle);
		}
	}

	ObserverContext::~ObserverContext() noexcept {
		clearSignals();
	}

	ObserverContext::ObserverContext(geode::Function<void()> eff, std::monostate) noexcept
		: impl(std::make_shared<ObserverContext::Impl>(std::move(eff))) {}
}
