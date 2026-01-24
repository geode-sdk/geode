#include <Geode/DefaultInclude.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

$on_mod(Loaded) {
    // this replaces the call to __dynamic_cast with a call to our own
    // this is needed because the transitions in cocos uses dynamic cast to check
    // layers, which fail on user layers due to typeinfo not matching

    #if defined(GEODE_IS_MACOS) && GEODE_COMP_GD_VERSION != 22081
        #error "Unsupported version for macOS dynamic cast fix, please update the addresses"
    #endif
    #if defined(GEODE_IS_IOS) && GEODE_COMP_GD_VERSION != 22074
        #error "Unsupported version for iOS dynamic cast fix, please update the addresses"
    #endif

    #if defined(GEODE_IS_INTEL_MAC)
        void* dynamicCastAddr = reinterpret_cast<void*>(base::get() + 0x7cf4ce);
        (void) Mod::get()->hook(dynamicCastAddr, &cast::typeinfoCastInternal, "__dynamic_cast");
    #elif defined(GEODE_IS_ARM_MAC)
        void* dynamicCastAddr = reinterpret_cast<void*>(base::get() + 0x6d1dfc);
        (void)Mod::get()->hook(dynamicCastAddr, &cast::typeinfoCastInternal, "__dynamic_cast");
    #elif defined(GEODE_IS_ANDROID)
        void* handle = dlopen("libcocos2dcpp.so", RTLD_LAZY | RTLD_NOLOAD);
        void* dynamicCastAddr = dlsym(handle, "__dynamic_cast");

        (void)Mod::get()->hook(dynamicCastAddr, &cast::typeinfoCastInternal, "__dynamic_cast");

        dlclose(handle);
    #elif defined(GEODE_IS_IOS)
        void* addr = reinterpret_cast<void*>(base::get() + 0x775208);
        (void) Mod::get()->patch(addr, geode::toBytes(&cast::typeinfoCastInternal));
    #endif
}
