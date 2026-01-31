#include <Geode/loader/Signal.hpp>

namespace geode::comm {
	std::vector<Observer> Observer::stack;

	std::optional<Observer> Observer::top() noexcept {
		if (Observer::stack.empty())
			return std::nullopt;
		return Observer(Observer::stack.back());
	}

	void Observer::operator()() noexcept {
		clearSignals();
		Observer::stack.push_back(*this);
		std::invoke(impl->effect);
		Observer::stack.pop_back();
	}

	void Observer::registerSignal(std::shared_ptr<SignalInternal> sig) noexcept {
		for (auto& [s, _] : impl->registered) {
			if (!s.owner_before(sig) && !sig.owner_before(s))
				return;
		}
		impl->registered.emplace_back(sig, sig->addPortReceiver(*this));
	}

	void Observer::clearSignals() noexcept {
		for (auto& [weak, handle] : impl->registered) {
			if (auto sig = weak.lock())
				sig->removePortReceiver(handle);
		}
	}
}
