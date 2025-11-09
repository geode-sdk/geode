#include <Geode/utils/coro.hpp>
#include <chrono>
#include <thread>

namespace geode::utils::coro {
	Task<void> nextFrame() {
		auto [task, post, prog, cancel] = Task<void>::spawn("<Next Frame>");

		queueInMainThread([post]() {
			post(true);
		});

		return task;
	}

	Task<void> skipFrames(int frames) {
		for (int i = 0; i < frames; i++) {
			co_await nextFrame();
		}
	}

	Task<void> sleep(double seconds) {
		return Task<void>::run([seconds](auto, auto) {
			std::this_thread::sleep_for(std::chrono::duration<double>(seconds));
			return true;
		}, "<Sleep>");
	}
}