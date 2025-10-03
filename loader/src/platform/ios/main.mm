#include <Geode/DefaultInclude.hpp>

#include <Geode/loader/Dirs.hpp>
#include <Geode/Utils.hpp>
#include "../load.hpp"
#include "../../loader/LoaderImpl.hpp"
#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <unistd.h>

#include <thread>

using namespace geode::prelude;

// static bool(*s_applicationDidFinishLaunchingOrig)(void*, SEL, void*, void*);

// bool applicationDidFinishLaunchingHook(void* self, SEL sel, void* p1, void* p2) {
//     // updateGeode();

    
    
//     return s_applicationDidFinishLaunchingOrig(self, sel, p1, p2);
// }


// bool loadGeode() {
//     auto orig = geode::hook::replaceObjcMethod("AppController", "application:didFinishLaunchingWithOptions:", (void*)applicationDidFinishLaunchingHook);
//     if (!orig)
//         return false;

//     s_applicationDidFinishLaunchingOrig = reinterpret_cast<bool(*)(void*, SEL, void*, void*)>(orig.unwrap());
//     return true;
// }

__attribute__((constructor)) void _entry() {
    int exitCode = geodeEntry(nullptr);
    if (exitCode != 0) return;

#if defined(GEODE_IS_IOS) && GEODE_COMP_GD_VERSION != 22074
    #error "Unsupported version for iOS clipping node fix, please update the addresses"
#endif

    auto mod = LoaderImpl::get()->getInternalMod();
    // Patches the depth format of gd to be GL_DEPTH24_STENCIL8_OES, fixing the CCClippingNode recreation
    if (Loader::get()->isPatchless()) {
        GEODE_MOD_STATIC_PATCH(0x268b38, "\x03\x1e\x91\x52");
    }
    else {
        auto const address = reinterpret_cast<void*>(geode::base::get() + 0x268b38);
        (void) mod->patch(address, { 0x03, 0x1e, 0x91, 0x52 });
    }
}
