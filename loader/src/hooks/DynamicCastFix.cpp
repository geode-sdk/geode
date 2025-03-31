#include <Geode/DefaultInclude.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

$execute {
    // this replaces the call to __dynamic_cast with a call to our own
    // this is needed because the transitions in cocos uses dynamic cast to check
    // layers, which fail on user layers due to typeinfo not matching

    #if defined(GEODE_IS_MACOS) && GEODE_COMP_GD_VERSION != 22074
        #error "Unsupported version for macOS dynamic cast fix, please update the addresses"
    #endif
    #if defined(GEODE_IS_IOS) && GEODE_COMP_GD_VERSION != 22074
        #error "Unsupported version for iOS dynamic cast fix, please update the addresses"
    #endif

    #if defined(GEODE_IS_INTEL_MAC)
        void* dynamicCastAddr = reinterpret_cast<void*>(base::get() + 0x7ba1d8);
        (void) Mod::get()->hook(dynamicCastAddr, &cast::typeinfoCastInternal, "__dynamic_cast");
    #elif defined(GEODE_IS_ARM_MAC)
        void* dynamicCastAddr = reinterpret_cast<void*>(base::get() + 0x6c8bcc);
        (void)Mod::get()->hook(dynamicCastAddr, &cast::typeinfoCastInternal, "__dynamic_cast");
    #elif defined(GEODE_IS_ANDROID)
        void* handle = dlopen("libcocos2dcpp.so", RTLD_LAZY | RTLD_NOLOAD);
        void* dynamicCastAddr = dlsym(handle, "__dynamic_cast");

        (void)Mod::get()->hook(dynamicCastAddr, &cast::typeinfoCastInternal, "__dynamic_cast");

        dlclose(handle);
    #elif defined(GEODE_IS_IOS)
        void* addr = reinterpret_cast<void*>(base::get() + 0x769208);
        (void) Mod::get()->patch(addr, geode::toBytes(&cast::typeinfoCastInternal));
    #endif
}
