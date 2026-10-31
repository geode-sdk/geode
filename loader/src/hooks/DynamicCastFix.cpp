#include <Geode/DefaultInclude.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

$on_mod(Loaded) {
    // this replaces the call to __dynamic_cast with a call to our own
    // this is needed because the transitions in cocos uses dynamic cast to check
    // layers, which fail on user layers due to typeinfo not matching

    #if defined(GEODE_IS_MACOS) && GEODE_COMP_GD_VERSION != 22081 && GEODE_COMP_GD_VERSION != 22091
        #error "Unsupported version for macOS dynamic cast fix, please update the addresses"
    #endif
    #if defined(GEODE_IS_IOS) && GEODE_COMP_GD_VERSION != 22081 && GEODE_COMP_GD_VERSION != 22091
        #error "Unsupported version for iOS dynamic cast fix, please update the addresses"
    #endif

    #if defined(GEODE_IS_INTEL_MAC)
        #if GEODE_COMP_GD_VERSION == 22091
        void* dynamicCastAddr = reinterpret_cast<void*>(base::get() + 0x9aa24c);
        #else
        void* dynamicCastAddr = reinterpret_cast<void*>(base::get() + 0x7cf4ce);
        #endif
        (void) Mod::get()->hook(dynamicCastAddr, &cast::typeinfoCastInternal, "__dynamic_cast");
    #elif defined(GEODE_IS_ARM_MAC)
        #if GEODE_COMP_GD_VERSION == 22091
        void* dynamicCastAddr = reinterpret_cast<void*>(base::get() + 0x882710);
        #else
        void* dynamicCastAddr = reinterpret_cast<void*>(base::get() + 0x6d1dfc);
        #endif
        (void)Mod::get()->hook(dynamicCastAddr, &cast::typeinfoCastInternal, "__dynamic_cast");
    #elif defined(GEODE_IS_ANDROID)
        void* handle = dlopen("libcocos2dcpp.so", RTLD_LAZY | RTLD_NOLOAD);
        void* dynamicCastAddr = dlsym(handle, "__dynamic_cast");

        (void)Mod::get()->hook(dynamicCastAddr, &cast::typeinfoCastInternal, "__dynamic_cast");

        dlclose(handle);
    #elif defined(GEODE_IS_IOS)
        #if GEODE_COMP_GD_VERSION == 22091
        void* addr = reinterpret_cast<void*>(base::get() + 0x8ceeb8);
        #else
        void* addr = reinterpret_cast<void*>(base::get() + 0x7812d8);
        #endif
        (void) Mod::get()->patch(addr, geode::toBytes(&cast::typeinfoCastInternal));
    #endif
}
