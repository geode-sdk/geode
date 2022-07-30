#include <geode/core/meta/function.hpp>
#include <geode/core/meta/hook.hpp>
#include <geode/core/meta/membercall.hpp>
#include <geode/core/meta/optcall.hpp>
#include <geode/core/meta/thiscall.hpp>
#include <iostream>
#include <string>

using namespace lilac;
using namespace lilac::meta;

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

struct Small {
    char value;
};

std::string optcall_test(
    std::string s1, float f1, std::string s2, int i1, Small sm1, float f2, int i2, float f3
) {
    std::cout << "xmm0 " << f1 << '\n';
    std::cout << "ecx " << i1 << '\n';
    std::cout << "stack 0 " << sm1.value << '\n';
    std::cout << "xmm3 " << f2 << '\n';
    std::cout << "edx " << i2 << '\n';
    std::cout << "stack 1 " << f3 << '\n';
    std::cout << "stack 2 " << s1 << '\n';
    std::cout << "stack 3 " << s2 << '\n';
    return std::string("hello ") + std::to_string(f3);
}

std::string membercall_test(int a, std::string s1, int b, int c, float d, std::string s2, float e) {
    std::cout << "ecx " << a << '\n';
    std::cout << "xmm3 " << d << '\n';
    std::cout << "stack 0 " << b << '\n';
    std::cout << "stack 1 " << c << '\n';
    std::cout << "stack 2 " << e << '\n';
    std::cout << "stack 3 " << s1 << '\n';
    std::cout << "stack 4 " << s2 << '\n';
    return std::string("hello member");
}

std::string thiscall_test(int ecx, float stack) {
    std::cout << "ecx " << ecx << '\n';
    std::cout << "stack " << stack << '\n';
    return std::string("lol");
}

template <auto, template <class, class...> class>
struct get_wrapper;

template <class Ret, class... Args, Ret (*func)(Args...), template <class, class...> class Conv>
struct get_wrapper<func, Conv> {
    static constexpr auto result = Conv<Ret, Args...>::template get_wrapper<func>();
};

int main() {
    // Hook<&to_hook, &hook1, x86::Optcall> hook;
    Function<int(int, std::string, int, float, int, float, bool), x86::Optcall> f1 = test1;

    // Hi 4
    int val = f1(2, "lol", 2, 3.2f, 4, 5.6f, false);

    meta::Function<void(float, float, float, float, int, int, int), meta::x86::Optcall> f2 = test1;
    // Hi 234
    f2(6.0f, 2.0f, 3.0f, 5.0f, 2234, 2, 234);

    meta::Function<void(void*, float, float, int), meta::x86::Membercall> f3 = test1;
    // Hi 455
    f3(nullptr, 23.0f, 52.0f, 455);

    to_hook(6);
    to_hook(24);

    auto optcall = get_wrapper<&optcall_test, meta::x86::Optcall>::result;
    // expected:
    // xmm0 0.123
    // ecx 123
    // stack 0 x
    // xmm3 3.123
    // edx 420
    // stack 1 1337.3
    // stack 2 Hell
    // stack 3 Lo
    float fl0 = 0.123f;
    float fl3 = 3.123f;
    __asm {
        movss xmm0, fl0
        mov ecx, 123
        mov edx, 420
        movss xmm3, fl3
    }
    auto optcall_ret = optcall({ 'x' }, 1337.3f, "Hell", "Lo");
    // "hello 1337"
    std::cout << "optcall wrapper returned \"" << optcall_ret << "\"\n\n";

    auto thiscall = get_wrapper<&thiscall_test, meta::x86::Thiscall>::result;
    auto thiscall_ret = thiscall(222, 341.0f);
    // "lol"
    std::cout << "thiscall wrapper returned \"" << thiscall_ret << "\"\n\n";

    // ecx 3
    // xmm3 1908.6
    // stack 0 555
    // stack 1 666
    // stack 2 777.2
    auto membercall = get_wrapper<&membercall_test, meta::x86::Membercall>::result;
    // xmm0,  xmm1,    xmm2,    xmm3,    xmm4,   xmm5, ecx, edx, stack...
    auto membercall_ret = membercall(
        69.0f, 2333.0f, 1333.0f, 1908.6f, 222.0f, 223.0f, 3, 45, 555, 666, 777.2f, "Goo", "Dbye"
    );
    std::cout << "membercall wrapper returned \"" << membercall_ret << "\"\n\n";
}