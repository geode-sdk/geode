#include <Geode/utils/async.hpp>
#include <Geode/loader/Log.hpp>

using namespace geode::prelude;

namespace geode::async {

arc::Runtime& runtime() {
    static auto instance = arc::Runtime::create(4);
    return *instance;
}

}

$execute {
    // TODO: v5 more or less temporary? might keep
    arc::setLogFunction([](auto msg, arc::LogLevel level) {
        using enum arc::LogLevel;

        switch (level) {
            case Warn:
                log::warn("[arc] {}", msg); break;
            case Error:
                log::error("[arc] {}", msg); break;
            default:
                log::debug("[arc] {}", msg); break;
        }
    });
}