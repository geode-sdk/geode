#include <geode/core/hook/hook.hpp>

#if defined(GEODE_IS_WINDOWS)
    #include <Windows.h>
    #define NOINLINE __declspec(noinline)
#elif defined(GEODE_IS_MACOS)
    #define NOINLINE __attribute__((noinline))
#endif

#include <stdio.h>

using namespace lilac;

// you see, on release builds this function got inlined
// meaning none of the hooks would work at all
void NOINLINE to_hook(const char* str) {
    printf("[STATUS]: %s\n", str);
}

void hook3(const char* str) {
    static int times = 0;
    switch (times) {
    case 0:
        to_hook("0");
        break;
    case 1:
        to_hook("1");
        break;
    case 2:
        to_hook("2");
        break;
    default:
        to_hook("max");
        break;
    }
    to_hook(str);
    ++times;
}

void hook2(const char* str) {
    to_hook("hook2");
}

void hook1(const char* str) {
    for (unsigned int i = 0; i < 5; ++i) {
        to_hook(str);
    }
}

template <class A, class B>
decltype(auto) add_hook(A func, B hook) {
    return hook::add((void*)func, (void*)hook);
}

int main() {
    hook::initialize();

    auto h1 = add_hook(to_hook, hook1);
    auto h2 = add_hook(to_hook, hook2);
    auto h3 = add_hook(to_hook, hook3);

    to_hook("main");

    printf("\n--removing hook1--\n\n");
    hook::remove(h1);

    to_hook("main");

    printf("\n--removing all--\n\n");
    hook::remove(h2);
    hook::remove(h3);

    printf("removing invalid hook returned: %s\n\n", (hook::remove(h1) ? "true" : "false"));

    to_hook("main");
}
