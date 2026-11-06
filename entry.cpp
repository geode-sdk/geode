#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/utils/async.hpp>

namespace geode {
    /**
     * To bypass the need for cyclic dependencies,
     * this function does the exact same as Mod::get()
     * However, it can be externed, unlike Mod::get()
     * @returns Same thing Mod::get() returns
     */
    Mod* getMod() {
        return Mod::get();
    }

    ZStringView getModID(Mod* mod) {
        return mod->getID();
    }
}

GEODE_API void geodeImplicitEntry() {
    // to make sure the instance is set into the sharedMod<> in load time
    auto mod = geode::getMod();

    // initialize arc runtime
    if (!mod->isInternal()) {
        arc::setGlobalRuntime(&geode::async::runtime());
    }

    arc::setLogFunction([](auto msg, arc::LogLevel level) {
        using enum arc::LogLevel;

        switch (level) {
            case Warn:
                geode::log::warn("[arc] {}", msg); break;
            case Error:
                geode::log::error("[arc] {}", msg); break;
            default:
                // log::debug("[arc] {}", msg);
                break;
        }
    });
}
