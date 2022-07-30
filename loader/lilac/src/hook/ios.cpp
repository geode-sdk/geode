#include "ios.hpp"

#include "impl.hpp"

#include <mach/mach_init.h> /* mach_task_self()     */
#include <mach/vm_map.h>    /* mach_vm_*            */
#include <signal.h>         /* sigaction            */
#include <sys/ucontext.h>   /* ucontext_t           */

#include <mach-o/dyld.h>

using namespace lilac::hook;

#if __DARWIN_OPAQUE_ARM_THREAD_STATE64
    #define _ios_get_reg(f, r) f.__opaque_##r
#else
    #define _ios_get_reg(f, r) f.__##r
#endif

extern "C" void MSHookMemory(void* dst, const void* src, uintptr_t size);

namespace {
    void handler(int signal, siginfo_t* signal_info, void* vcontext) {
        ucontext_t* context = reinterpret_cast<ucontext_t*>(vcontext);

        const void* ret = reinterpret_cast<void*>(context->uc_mcontext->__ss.__lr);
        const void* current = reinterpret_cast<void*>(context->uc_mcontext->__ss.__pc);

        Exception exception = {
            current,
            ret
        };

        HookManager::handler(exception);
    }
}

bool iOS::write_memory(void* to, const void* from, size_t size) {
    MSHookMemory(to, from, size);
    return true;
}

bool iOS::initialize() {
    struct sigaction action;
    memset(&action, '\0', sizeof(action));
    action.sa_sigaction = &handler;
    action.sa_flags = SA_SIGINFO;

    int signal = SIGTRAP;

    return sigaction(signal, &action, NULL) == 0;
}

const void* iOS::align_address(const void* address) {
    return address;
}
