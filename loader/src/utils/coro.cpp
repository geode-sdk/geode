#include <Geode/utils/coro.hpp>
#include <arc/sync/oneshot.hpp>

namespace geode::utils::coro {
	arc::Future<void> nextFrame() {
		auto [send, recv] = arc::oneshot::channel<std::monostate>();

		queueInMainThread([send = std::move(send)] mutable {
			(void)send.send({});
		});

		(void)co_await recv.recv();
	}

	arc::Future<void> skipFrames(int frames) {
		for (int i = 0; i < frames; i++) {
			co_await nextFrame();
		}
	}
}
