#include <Geode/utils/async.hpp>
#include <Geode/loader/GameEvent.hpp>
#include <Geode/loader/Log.hpp>
#include <loader/LogImpl.hpp>
#include <thread>

using namespace geode::prelude;

namespace geode::async {

asp::SharedPtr<arc::Runtime>& runtimePtr() {
    static auto runtime = arc::Runtime::create(4);
    return runtime;
}

arc::Runtime& runtime() {
    return *runtimePtr();
}

}

$on_mod(Loaded) {
    arc::setLogFunction([](auto msg, arc::LogLevel level) {
        using enum arc::LogLevel;

        switch (level) {
            case Warn:
                log::warn("[arc] {}", msg); break;
            case Error:
                log::error("[arc] {}", msg); break;
            default:
                // log::debug("[arc] {}", msg); break;
        }
    });

    GameEvent(GameEventType::Exiting).listen([] {
        log::info("Shutting down logger and async runtime..");
        log::Logger::get()->shutdownThread();
        async::runtime().safeShutdown();
        async::runtimePtr().reset();
        log::debug("Shutdown complete.");
    }, 100).leak();
}