#include <dlfcn.h>
#include <unistd.h>
#include <thread>

void inject() __attribute__((constructor)) {
	std::thread t1([]() {
		sleep(1);
		dlopen("/Library/MobileSubstrate/DynamicLibraries/Geode.dylib", RTLD_NOW);
	});
	t1.detach();
}