#include "Event.hpp"
#include "../platform/platform.hpp"

#include "Log.hpp"

namespace geode::comm {
	class ObserverContext;

	class SignalInternal {
		friend class ObserverContext;

		virtual ~SignalInternal() noexcept;
		virtual void removePortReceiver(ReceiverHandle handle) noexcept;
		virtual ReceiverHandle addPortReceiver(ObserverContext handle) noexcept;
	};

	class ObserverContext {
		template <class Type, bool ThreadSafe>
		friend class Signal;
		friend class Observer;

		struct Impl {
			geode::Function<void()> effect;
			std::vector<std::pair<std::weak_ptr<SignalInternal>, ReceiverHandle>> registered;
		};
		std::shared_ptr<Impl> impl;

		static std::vector<ObserverContext> stack;
		static ObserverContext* top() noexcept;
		void registerSignal(std::shared_ptr<SignalInternal> sig) noexcept;
		void clearSignals() noexcept;

		// sooo std23::move_only_function is broken in single-arg ctors
		ObserverContext(geode::Function<void()> effect, std::monostate) noexcept;
	public:
		~ObserverContext() noexcept;
		void operator()() noexcept;
	};

	class Observer {
		std::vector<ObserverContext> contexts;
	public:
		Observer() noexcept = default;
		Observer(Observer&&) noexcept = default;

		void reactToChanges(geode::Function<void()> func) noexcept;
	};

	template <class Type, bool ThreadSafe>
	class Signal {
		class Impl : SignalInternal {
			using PortType = Port<ObserverContext, ThreadSafe>;

			PortType port;
			Type value;
			std::conditional_t<ThreadSafe, std::atomic_flag, bool> inCtx = false;
			GEODE_NO_UNIQUE_ADDRESS mutable std::conditional_t<ThreadSafe, std::mutex, std::monostate> mutex;

			Impl() noexcept {}
			Impl(std::convertible_to<PortType> auto&& p, std::convertible_to<Type> auto&& v) noexcept : port(p), value(v) {}
		
			void removePortReceiver(ReceiverHandle handle) noexcept override {
				port.removeReceiver(handle);
			}
			ReceiverHandle addPortReceiver(ObserverContext obs) noexcept override {
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

		    if (auto observer = ObserverContext::top())
		    	observer->registerSignal(impl);

		    return impl->value;
		}
	};
};
