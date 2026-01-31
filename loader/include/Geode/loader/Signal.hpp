#include "EventV2.hpp"

namespace geode::comm {
	class Observer;

	class SignalInternal {
		friend class Observer;

		virtual ~SignalInternal() noexcept;
		virtual void removePortReceiver(ReceiverHandle handle) noexcept;
		virtual ReceiverHandle addPortReceiver(Observer handle) noexcept;
	};

	class Observer {
		template <class Type, bool ThreadSafe>
		friend class Signal;

		class Impl {
		public:
			geode::Function<void()> effect;
			std::vector<std::pair<std::weak_ptr<SignalInternal>, ReceiverHandle>> registered;
		};
		std::shared_ptr<Impl> impl;

		static std::vector<Observer> stack;
		static std::optional<Observer> top() noexcept;
		void registerSignal(std::shared_ptr<SignalInternal> sig) noexcept;
		void clearSignals() noexcept;
	public:
		void operator()() noexcept;
	};

	template <class Type, bool ThreadSafe>
	class Signal {
		class Impl : SignalInternal {
			using PortType = Port<Observer, ThreadSafe>;

			PortType port;
			Type value;
			std::conditional_t<ThreadSafe, std::atomic_flag, bool> inCtx = false;
			[[no_unique_address]] mutable std::conditional_t<ThreadSafe, std::mutex, std::monostate> mutex;

			Impl() noexcept {}
			Impl(std::convertible_to<PortType> auto&& p, std::convertible_to<Type> auto&& v) noexcept : port(p), value(v) {}
		
			void removePortReceiver(ReceiverHandle handle) noexcept override {
				port.removeReceiver(handle);
			}
			ReceiverHandle addPortReceiver(Observer obs) noexcept override {
				return port.addReceiver(obs);
			}
		};

		std::shared_ptr<Impl> impl;
	public:
		void set(Type val) noexcept {
		    if constexpr (ThreadSafe) {
		        if (!impl->inCtx.test_and_set()) {
		            std::lock_guard lk(impl->mutex);
		            impl->value = std::move(val);
		            impl->port.send(impl->value);
		            impl->inCtx.clear();
		        } else {
		            geode::log::debug("Attempted to modify signal within its own callback");
		        }
		    } else {
		        if (!impl->inCtx) {
		            impl->inCtx = true;
		            impl->value = std::move(val);
		            impl->port.send(impl->value);
		            impl->inCtx = false;
		        } else {
		            geode::log::debug("Attempted to modify signal within its own callback");
		        }
		    }
		}

		std::conditional_t<ThreadSafe, Type, Type const&> get_internal() const noexcept {
		    std::lock_guard lk(impl->mutex);
		    return impl->value;
		}

		std::conditional_t<ThreadSafe, Type, Type const&> get() noexcept {
		    std::lock_guard lk(impl->mutex);

		    if (auto observer = Observer::top())
		    	observer->registerSignal(impl);

		    return impl->value;
		}
	};
};

