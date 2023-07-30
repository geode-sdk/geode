#include <Geode/DefaultInclude.hpp>

#if defined(GEODE_IS_MACOS)

#import <Cocoa/Cocoa.h>
#include "../load.hpp"
#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <unistd.h>
#include <tulip/TulipHook.hpp>
#include <array>

#include <thread>

using namespace geode::prelude;

std::length_error::~length_error() _NOEXCEPT {} // do not ask...

// camila has an old ass macos and this function turned
// from dynamic to static thats why she needs to define it
// this is what old versions does to a silly girl

extern "C" void fake() {}

void applicationDidFinishLaunchingHook(void* self, SEL sel, NSNotification* notification) {
    // updateGeode();

    std::array<uint8_t, 6> patchBytes = {
        0x55,
        0x48, 0x89, 0xe5,
        0x41, 0x57
    };

    auto res = tulip::hook::writeMemory((void*)(base::get() + 0x69a0), patchBytes.data(), 6);
    if (!res)
        return;
    
    int exitCode = geodeEntry(nullptr);
    if (exitCode != 0)
        return;

    return reinterpret_cast<void(*)(void*, SEL, NSNotification*)>(geode::base::get() + 0x69a0)(self, sel, notification);
}


bool loadGeode() {
    auto detourAddr = reinterpret_cast<uintptr_t>(&applicationDidFinishLaunchingHook) - geode::base::get() - 0x69a5;
    auto detourAddrPtr = reinterpret_cast<uint8_t*>(&detourAddr);

    std::array<uint8_t, 5> patchBytes = {
        0xe9, detourAddrPtr[0], detourAddrPtr[1], detourAddrPtr[2], detourAddrPtr[3]
    };

    auto res = tulip::hook::writeMemory((void*)(base::get() + 0x69a0), patchBytes.data(), 5);
    if (!res)
        return false;
    
    return true;
}

__attribute__((constructor)) void _entry() {
    if (!loadGeode())
        return;
}

#endif