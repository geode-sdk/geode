#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_MACOS

using namespace geode::prelude;

    #include <Geode/loader/Mod.hpp>
    #include <Geode/modify/Modify.hpp>

$execute {
    // this replaces the call to __dynamic_cast with a call to our own
    // this is needed because the transitions in cocos uses dynamic cast to check
    // layers, which fail on user layers due to typeinfo not matching
    (void)Mod::get()->patch(
        reinterpret_cast<void*>(base::get() + 0x603948), toByteArray(&cast::typeinfoCastInternal)
    );
}

#endif