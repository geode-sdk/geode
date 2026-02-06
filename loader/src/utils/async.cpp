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

// this serves as just a small test for all the functions in async.hpp

namespace {
arc::Future<> dummy() { co_return; }
arc::Future<int> dummyInt() { co_return 1; }

void spawn_fut() {
    auto t1 = async::spawn(dummy());
    auto t2 = async::spawn(dummy(), [] {});

    auto t3 = async::spawn(dummyInt());
    auto t4 = async::spawn(dummyInt(), [](int val) {});

    static_assert(std::is_same_v<decltype(t1), arc::TaskHandle<void>>);
    static_assert(std::is_same_v<decltype(t2), arc::TaskHandle<void>>);
    static_assert(std::is_same_v<decltype(t3), arc::TaskHandle<int>>);
    static_assert(std::is_same_v<decltype(t4), arc::TaskHandle<void>>);
}

void spawn_lambda() {
    auto l1 = [] -> arc::Future<> { co_return; };
    auto l2 = [] -> arc::Future<int> { co_return 1; };

    auto t1 = async::spawn(l1);
    auto t2 = async::spawn(l1, [] {});
    auto t3 = async::spawn(std::move(l1));

    auto t4 = async::spawn(l2);
    auto t5 = async::spawn(l2, [](int val) {});
    auto t6 = async::spawn(std::move(l2));

    static_assert(std::is_same_v<decltype(t1), arc::TaskHandle<void>>);
    static_assert(std::is_same_v<decltype(t2), arc::TaskHandle<void>>);
    static_assert(std::is_same_v<decltype(t3), arc::TaskHandle<void>>);
    static_assert(std::is_same_v<decltype(t4), arc::TaskHandle<int>>);
    static_assert(std::is_same_v<decltype(t5), arc::TaskHandle<void>>);
    static_assert(std::is_same_v<decltype(t6), arc::TaskHandle<int>>);
}

void taskholder() {
    async::TaskHolder<> th1;
    async::TaskHolder<int> th2;

    auto l1 = [] -> arc::Future<> { co_return; };
    auto l2 = [] -> arc::Future<int> { co_return 1; };

    th1.spawn(dummy(), [] {});
    th1.spawn(l1, [] {});
    th1.spawn(std::move(l1), [] {});

    th2.spawn(dummyInt(), [](int val) {});
    th2.spawn(l2, [](int val) {});
    th2.spawn(std::move(l2), [](int val) {});

    // this should error
    // th1.spawn(dummyInt(), [] (int val) {});
}

arc::Future<> waitformt() {
    std::optional<int> x = co_await waitForMainThread<int>([] {
        return 42;
    });

    bool y = co_await waitForMainThread<void>([] {});
}
}