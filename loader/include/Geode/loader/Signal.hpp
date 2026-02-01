#include "Event.hpp"
#include "../platform/platform.hpp"
#include "Log.hpp"

namespace geode::comm {
	class ObserverContext;

	struct SignalInternal {
		virtual void removePortReceiver(ReceiverHandle handle) noexcept = 0;
		virtual ReceiverHandle addPortReceiver(ObserverContext handle) noexcept = 0;
	};

	class ObserverContext {
		template <class Type, bool ThreadSafe>
		friend class Signal;
		template <bool ThreadSafe>
		friend class BasicObserver;

		struct Impl {
			geode::Function<void()> effect;
			mutable std::vector<std::pair<std::weak_ptr<SignalInternal>, ReceiverHandle>> registered;

			void clearSignals() const noexcept;
			~Impl() noexcept;
		};
		std::shared_ptr<Impl> impl;

		static thread_local std::vector<ObserverContext> stack;
		static ObserverContext* top() noexcept;
		void registerSignal(std::shared_ptr<SignalInternal> sig) noexcept;

		// sooo std23::move_only_function is broken in single-arg ctors with operator()
		ObserverContext(geode::Function<void()> effect, std::monostate) noexcept;
	public:
		void operator()() const noexcept;
	};

	template <bool ThreadSafe = false>
	class BasicObserver {
		std::vector<ObserverContext> contexts;
		GEODE_NO_UNIQUE_ADDRESS std::conditional_t<ThreadSafe, std::mutex, std::monostate> mutex;
	public:
		BasicObserver() noexcept = default;
		BasicObserver(BasicObserver&& other) noexcept {
			if constexpr (ThreadSafe) {
				std::lock_guard<std::mutex> guard(other.mutex);
				std::lock_guard<std::mutex> guard2(mutex);

				contexts = std::move(other.contexts);
			}
		}

		void reactToChanges(geode::Function<void()> func) noexcept {
			if constexpr (ThreadSafe)
				mutex.lock();
			contexts.push_back(ObserverContext(std::move(func), {}));
			auto& back = contexts.back();
			if constexpr (ThreadSafe)
				mutex.unlock();
			std::invoke(contexts.back());
		}

		~BasicObserver() {
			if constexpr (ThreadSafe)
				std::lock_guard<std::mutex> guard(mutex);
		}
	};

	using Observer = BasicObserver<false>;
	using ThreadSafeObserver = BasicObserver<true>;

	template <class Type, bool ThreadSafe = false>
	class Signal {
		struct Impl : SignalInternal {
			using PortType = Port<ObserverContext, ThreadSafe>;

			PortType port;
			Type value;
			std::conditional_t<ThreadSafe, std::atomic_flag, bool> inCtx = false;
			GEODE_NO_UNIQUE_ADDRESS mutable std::conditional_t<ThreadSafe, std::mutex, std::monostate> mutex;

			Impl() noexcept {}
			Impl(std::convertible_to<Type> auto&& v) noexcept : value(v) {}
		
			void removePortReceiver(ReceiverHandle handle) noexcept override {
				port.removeReceiver(handle);
			}
			ReceiverHandle addPortReceiver(ObserverContext obs) noexcept override {
				return port.addReceiver(obs);
			}
		};

		using LockType = std::conditional_t<ThreadSafe, std::lock_guard<std::mutex>, std::monostate>;

		std::shared_ptr<Impl> impl;
	public:
		Signal() noexcept : impl(std::make_shared<Impl>()) {}
		Signal(std::convertible_to<Type> auto&& value) noexcept
			: impl(std::make_shared<Impl>(std::forward<decltype(value)>(value))) {}
		Signal(Signal const& sig) noexcept : impl(sig.impl) {}

		void operator=(Signal sig) = delete;
		void rebind(Signal sig) noexcept {
			impl = sig.impl;
		}

		void set(Type val) noexcept {
		    if constexpr (ThreadSafe) {
		        if (!impl->inCtx.test_and_set()) {
		            std::lock_guard lk(impl->mutex);
		            impl->value = std::move(val);
		            impl->port.send();
		            impl->inCtx.clear();
		        } else {
		            geode::log::debug("Attempted to modify signal within its own callback");
		        }
		    } else {
		        if (!impl->inCtx) {
		            impl->inCtx = true;
		            impl->value = std::move(val);
		            impl->port.send();
		            impl->inCtx = false;
		        } else {
		            geode::log::debug("Attempted to modify signal within its own callback");
		        }
		    }
		}

		std::conditional_t<ThreadSafe, Type, Type const&> get_internal() const noexcept {
		    LockType lk(impl->mutex);
		    return impl->value;
		}

		std::conditional_t<ThreadSafe, Type, Type const&> get() noexcept {
		    LockType lk(impl->mutex);

		    if (auto observer = ObserverContext::top()) {
		    	observer->registerSignal(impl);
		    }

		    return impl->value;
		}

		auto operator*() noexcept { return get(); }
		auto operator->() noexcept { return get(); }

		Signal const& operator=(Type value) noexcept {
			set(std::move(value));
			return *this;
		}

		#define OP(op, assign) \
			Signal const& operator assign(Type value) noexcept { \
				LockType lk(impl->mutex); \
				set(impl->value op std::move(value)); \
				return *this; \
			}

		OP(+, +=) OP(-, -=) OP(*, *=) OP(/, /=) OP(% , %=) OP(&, &=) OP(|, |=) OP(^, ^=) OP(<<, <<=) OP(>>, >>=)
		#undef OP
	};

	template <class Type>
	using ThreadSafeSignal = Signal<Type, true>;
};
