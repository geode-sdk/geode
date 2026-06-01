#include <Geode/Geode.hpp>

using namespace geode::prelude;

// This fix may or may not be irrelevant once robtop updates NDK, but for now we have to deal with it.
//
// On Android, when you first access a thread_local variable (in a mod or Geode), the following operations are performed internally:
// 1. __emutls_get_address is called to allocate a TLS slot
// 2. __cxa_thread_atexit is called to register a C++ destructor for your variable (if non-trivial)
//
// Both symbols exist in the GD binary, and both use pthread_key_create under the hood to ensure destruction.
// This creates an issue - pthread keys do not have a well defined destruction order, and in bionic libc they are destroyed in the addition order.
//
// This means that when a thread exits, first it will free the TLS slot, and afterwards it will try to call
// the destructor of the variable. This is a use-after-free, and potentially can cause a crash on thread exit.
//
// Our fix is to hook __cxa_thread_atexit and redirect it to __cxa_thread_atexit_impl.
// This is what happens on modern systems, but on ancient NDKs with libstdc++, it may not get used (as is in GD).
// Bionic libc had this function since at least 2015, and it does not use pthread keys, so on all semi-recent devices this should fix the use-after-free.

#ifdef GEODE_IS_ANDROID

using AtexitFn = int(*)(void (*)(void*), void*, void*);
static AtexitFn g_atexitFn = nullptr;

extern "C" {
    __attribute__((weak)) int __cxa_thread_atexit_impl(void(*func)(void*), void* arg, void* dso_handle);
}

int atexitHook(void (*func)(void*), void* arg, void* dso) {
    if (__cxa_thread_atexit_impl) {
        return __cxa_thread_atexit_impl(func, arg, dso);
    }
    return g_atexitFn(func, arg, dso);
}

$execute {
    auto handle = dlopen("libcocos2dcpp.so", RTLD_LAZY | RTLD_NOLOAD);
    g_atexitFn = (AtexitFn)dlsym(handle, "__cxa_thread_atexit");

    if (g_atexitFn) {
        (void)Mod::get()->hook((void*)g_atexitFn, &atexitHook, "__cxa_thread_atexit");
    }
}

#endif
