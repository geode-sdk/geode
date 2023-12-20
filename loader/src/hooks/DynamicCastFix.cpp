#include <Geode/DefaultInclude.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

$execute {
    // this replaces the call to __dynamic_cast with a call to our own
    // this is needed because the transitions in cocos uses dynamic cast to check
    // layers, which fail on user layers due to typeinfo not matching

    #if defined(GEODE_IS_MACOS)
        (void)Mod::get()->patch(
            reinterpret_cast<void*>(base::get() + 0x603948), toByteArray(&cast::typeinfoCastInternal)
        );
    #elif defined(GEODE_IS_ANDROID)
        (void)Mod::get()->addHook(reinterpret_cast<void*>(base::get() + 0x519a8c), &cast::typeinfoCastInternal, "__dynamic_cast");
    #endif


}