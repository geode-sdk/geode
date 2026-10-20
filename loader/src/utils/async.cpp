#include <Geode/loader/ModEvent.hpp>
#include <Geode/utils/async.hpp>
#include <Geode/loader/GameEvent.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/utils/terminate.hpp>
#include <loader/LogImpl.hpp>

using namespace geode::prelude;

namespace geode::async {

asp::SharedPtr<arc::Runtime>& runtimePtr() {
    static auto runtime = []{
        auto rt = arc::Runtime::create(4);
        rt->setTerminateHandler([](const std::exception& e) {
            utils::terminate(fmt::format(
                "arc runtime terminated due to unhandled exception: {}",
                e.what()
            ));
        });

        return rt;
    }();

    return runtime;
}

arc::Runtime& runtime() {
    auto& ptr = runtimePtr();

    if (!ptr) {
        utils::terminate("arc runtime has already been destroyed");
    }

    return *ptr;
}

}

$on_mod(Loaded) {
    GameEvent(GameEventType::Exiting).listen([] {
        log::info("Shutting down logger and async runtime..");
        log::Logger::get()->shutdownThread();
        async::runtime().safeShutdown();
        async::runtimePtr().reset();
        log::debug("Shutdown complete.");
    }, 100).leak();
}
