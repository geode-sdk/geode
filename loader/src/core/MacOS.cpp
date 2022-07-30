#include "MacOS.hpp"
#include "Core.hpp"

#ifdef GEODE_IS_MACOS

#include <mach/mach.h>
#include <mach/task.h>
#include <mach/mach_port.h>
#include <mach/mach_vm.h>       /* mach_vm_*            */
#include <mach/mach_init.h>     /* mach_task_self()     */

#include <signal.h>             /* sigaction            */
#include <sys/ucontext.h>       /* ucontext_t           */

using namespace geode::core::hook;
using namespace geode::core::impl;

namespace {
    void signalHandler(int signal, siginfo_t* signal_info, void* vcontext) {
        auto context = reinterpret_cast<ucontext_t*>(vcontext);

        auto current = reinterpret_cast<void*>(context->uc_mcontext->__ss.__rip);

        handleContext(vcontext, current);
    }
}

bool MacOSX::enableSingleStep(void* vcontext) {
	auto context = reinterpret_cast<ucontext_t*>(vcontext);
	context->uc_mcontext->__ss.__rflags |= ((unsigned long)0x100);
	return true;
}

bool MacOSX::disableSingleStep(void* vcontext) {
	auto context = reinterpret_cast<ucontext_t*>(vcontext);
	context->uc_mcontext->__ss.__rflags &= ~((unsigned long)0x100);
	return true;
}

void* MacOSX::allocateVM(size_t size) {
	mach_vm_address_t ret;

	kern_return_t status; //return status

	status = mach_vm_allocate(mach_task_self(), &ret, (mach_vm_size_t)size, VM_FLAGS_ANYWHERE);

	return (void*)ret;
}

std::vector<std::byte> MacOSX::jump(void* from, void* to) {
	constexpr size_t size = sizeof(int) + 1;
	std::vector<std::byte> ret(size);
	ret[0] = {0xe9};

	int offset = (int)((size_t)to - (size_t)from - size);
	// im too lazy
	((int*)((size_t)ret.data() + 1))[0] = offset;

	return ret;
}


bool MacOSX::writeMemory(void* to, void* from, size_t size) {

	kern_return_t status; //return status

	mach_vm_size_t vmsize;
    mach_vm_address_t address = (mach_vm_address_t)to;
    vm_region_basic_info_data_t info;
    mach_msg_type_number_t info_count = VM_REGION_BASIC_INFO_COUNT;
    mach_port_t object;

    // std::cout << "get memory protection" << std::endl;
    // // get memory protection
    // status = mach_vm_region(mach_task_self(), &address, &vmsize, VM_REGION_BASIC_INFO, (vm_region_info_t)&info, &info_count, &object);
    // std::cout << status << std::endl;
    // if (status != KERN_SUCCESS) return false;

    // std::cout << "set to write protection" << std::endl;
    // set to write protection
    status = mach_vm_protect(mach_task_self(), (mach_vm_address_t)to, size, FALSE, VM_PROT_COPY | VM_PROT_EXECUTE | VM_PROT_WRITE | VM_PROT_READ);
    if (status != KERN_SUCCESS) return false;

    // std::cout << "write to memory" << std::endl;
    // write to memory
    status = mach_vm_write(mach_task_self(), (mach_vm_address_t)to, (vm_offset_t)from, (mach_msg_type_number_t)size);
    if (status != KERN_SUCCESS) return false;

    // std::cout << "revert to old protection" << std::endl;
    // // revert to old protection
    // status = mach_vm_protect(mach_task_self(), (mach_vm_address_t)to, size, FALSE, info.protection);
    // if (status != KERN_SUCCESS) return false;

    return status == KERN_SUCCESS;
}

bool MacOSX::initialize() {
	task_set_exception_ports(
		mach_task_self(),
		EXC_MASK_BAD_INSTRUCTION,
		MACH_PORT_NULL,//m_exception_port,
		EXCEPTION_DEFAULT,
		0); 
	// first reached here
    struct sigaction action = {};
    action.sa_sigaction = &signalHandler;
    action.sa_flags = SA_SIGINFO;

    return sigaction(SIGILL, &action, NULL) == 0 && sigaction(SIGTRAP, &action, NULL) == 0;
}

#endif
