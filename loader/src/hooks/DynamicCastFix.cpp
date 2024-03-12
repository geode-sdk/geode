#include <Geode/DefaultInclude.hpp>
#include <Geode/loader/Mod.hpp>
#include <iostream>

using namespace geode::prelude;

$execute {
    auto hookDynamicCast = []() {
        void* dynamicCastAddr = nullptr;

        #if defined(GEODE_IS_MACOS)
            dynamicCastAddr = reinterpret_cast<void*>(base::get() + 0x7a7c9f);
        #elif defined(GEODE_IS_ANDROID)
            void* handle = dlopen("libcocos2dcpp.so", RTLD_LAZY | RTLD_NOLOAD);
            if (handle == nullptr) {
                std::cerr << "Failed to open libcocos2dcpp.so: " << dlerror() << std::endl;
                return;
            }
            dynamicCastAddr = dlsym(handle, "__dynamic_cast");
            dlclose(handle);
        #endif

        if (dynamicCastAddr != nullptr) {
            Mod::get()->hook(dynamicCastAddr, &cast::typeinfoCastInternal, "__dynamic_cast");
        } else {
            std::cerr << "Failed to locate dynamic_cast address" << std::endl;
        }
    };

    hookDynamicCast();
}
