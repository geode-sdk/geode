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

static bool(*s_applicationDidFinishLaunchingOrig)(void*, SEL, void*, void*);

bool applicationDidFinishLaunchingHook(void* self, SEL sel, void* p1, void* p2) {
    // updateGeode();

    int exitCode = geodeEntry(nullptr);
    if (exitCode != 0)
        return false;

    // Don't patch if we are in the wrong version
    if (!LoaderImpl::get()->isForwardCompatMode())
    {
        // Patches the depth format of gd to be GL_DEPTH24_STENCIL8_OES, fixing the CCClippingNode recreation
        if (!LoaderImpl::get()->getInternalMod()->patch(reinterpret_cast<void*>(geode::base::get() + 0x268b38), { 0x03, 0x1e, 0x91, 0x52 }).isOk())
            return false;
    }
    
    return s_applicationDidFinishLaunchingOrig(self, sel, p1, p2);
}


bool loadGeode() {
    auto orig = geode::hook::replaceObjcMethod("AppController", "application:didFinishLaunchingWithOptions:", (void*)applicationDidFinishLaunchingHook);
    if (!orig)
        return false;

    s_applicationDidFinishLaunchingOrig = reinterpret_cast<bool(*)(void*, SEL, void*, void*)>(orig.unwrap());
    return true;
}

__attribute__((constructor)) void _entry() {
    if (!loadGeode())
        return;
}
