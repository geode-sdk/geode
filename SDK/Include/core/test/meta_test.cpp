#include <geode/core/meta/function.hpp>
#include <geode/core/meta/hook.hpp>
#include <geode/core/meta/optcall.hpp>
#include <geode/core/meta/thiscall.hpp>
#include <geode/core/meta/membercall.hpp>
#include <iostream>
#include <string>

using namespace geode::core;

#if defined(GEODE_IS_WINDOWS)
    #define NOINLINE __declspec(noinline)
#elif defined(GEODE_IS_MACOS)
    #define NOINLINE __attribute__((noinline))
#endif

int test1(int x) {
    std::cout << "Hi " << x << '\n';
    return 2;
}

int NOINLINE __fastcall to_hook(int x) {
    std::cout << x << ": to_hook\n";
    return x - 3;
}

int hook1(int x) {
    std::cout << "HACKED!!! u left urself logged in at the apple store\n";
    return to_hook(x + 6);
}

std::string optcall_test(float a, int b, int c, float d, float e) {
    std::cout << "xmm0 " << a << '\n';
    std::cout << "edx " << b << '\n';
    std::cout << "stack 0 " << c << '\n';
    std::cout << "xmm3 " << d << '\n';
    std::cout << "stack 1 " << e << '\n';
    return std::string("hello ") + std::to_string(e);
}

std::string membercall_test(int a, int b, int c, float d, float e) {
    std::cout << "ecx " << a << '\n';
    std::cout << "stack 0" << b << '\n';
    std::cout << "stack 1 " << c << '\n';
    std::cout << "xmm3 " << d << '\n';
    std::cout << "stack 2 " << e << '\n';
    return std::string("hello member");
}

std::string thiscall_test(int ecx, float stack) {
    std::cout << "ecx " << ecx << '\n';
    std::cout << "stack " << stack << '\n';
    return std::string("lol");
}

int main() {
    meta::Hook<&to_hook, &hook1, meta::x86::Optcall> hook;
    meta::Function<int(int, int, int), meta::x86::Optcall> f1 = test1;
    int val = f1(2, 3, 4);

    meta::Function<void(float, float, float, float, int, int, int), meta::x86::Optcall> f2 = test1;
    f2(6.0f, 2.0f, 3.0f, 5.0f, 2234, 2, 234);

    to_hook(6);
    to_hook(24);

    auto optcall = meta::Hook<&to_hook, &optcall_test, meta::x86::Optcall>::get_wrapper();
    // expected:
    // xmm0 0.123
    // edx 123
    // stack 0 420
    // xmm3 3.123
    // stack 1 1337
    float f0 = 0.123f;
    float f3 = 3.123f;
    __asm {
        movss xmm0, f0
        mov edx, 123
        movss xmm3, f3
    }
    auto optcall_ret = optcall(420, 1337.f);
    std::cout << "optcall wrapper returned \"" << optcall_ret << "\"\n\n";

    auto thiscall = meta::Hook<&to_hook, &thiscall_test, meta::x86::Thiscall>::get_wrapper();
    auto thiscall_ret = thiscall(222, 341.0f);
    std::cout << "thiscall wrapper returned \"" << thiscall_ret << "\"\n\n";

    auto membercall = meta::Hook<&to_hook, membercall_test, meta::x86::Membercall>::get_wrapper();
    auto membercall_ret = membercall(69.0f, 2333.0f, 1333.0f, 1908.0f, 222.0f, 223.0f, 3, 45, 555, 666, 777.0f);
    std::cout << "membercall wrapper returned \"" << membercall_ret << "\"\n\n";
    
    //result = wrapper(0.123f, 1907.f, 1908.f, 3.123f, 1909.f, 1910.f, 2337, 123, 420, 1234.5f);
    //std::cout << "another result \"" << result << '"' << std::endl;
}