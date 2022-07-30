#ifndef LILAC_CORE_META_X86_HPP
#define LILAC_CORE_META_X86_HPP

#include "common.hpp"

#include <array>

namespace lilac::meta::x86 {
    // Logic needed by x86 calling conventions for stack fixing / filtering.
    template <class Class>
    static constexpr bool sse_passable = any_of<std::remove_cv_t<Class>, float, double>;

    template <class Class>
    static constexpr bool gpr_passable =
        ((sizeof(Class) <= sizeof(void*) && !std::is_class_v<Class> && !sse_passable<Class>)
         || std::is_member_function_pointer_v<Class>);

    template <class... Classes>
    static inline constexpr std::array<size_t, sizeof...(Classes)> reorder_pack() {
        constexpr size_t length = sizeof...(Classes);
        constexpr bool should_reorder[] = {
            std::is_class_v<Classes> && sizeof(Classes) > sizeof(void*)...
        };

        std::array<size_t, length> reordered = {};
        size_t out = 0;
        // Non-reordered go first.
        for (size_t in = 0; in < length; ++in) {
            if (!should_reorder[in]) {
                reordered[out] = in;
                ++out;
            }
        }

        // Reordered go last.
        for (size_t in = 0; in < length; ++in) {
            if (should_reorder[in]) {
                reordered[out] = in;
                ++out;
            }
        }

        return reordered;
    }

    template <class... Stack>
    static constexpr size_t stack_fix =
        (((sizeof(Stack) % sizeof(void*) == 0)
              ? sizeof(Stack)
              : sizeof(Stack) - (sizeof(Stack) % sizeof(void*)) + sizeof(void*))
         + ...);

    template <>
    static constexpr size_t stack_fix<> = 0;

    template <>
    static constexpr size_t stack_fix<void> = 0;

    template <class From>
    class Register {
    public:
        From raw;

    public:
        template <class To>
        explicit operator To() {
            static_assert(
                sizeof(From) >= sizeof(To),
                "Please report a bug to the Geode developers! This should never be reached.\n"
                "Size of Register is smaller than the size of the destination type!"
            );

            union {
                From from;
                To to;
            } u;

            u.from = raw;
            return u.to;
        }
    };

// Ignore this for now, it's for discarding calling convention qualifier later.
#if 0
    template <class Func>
    class remove_conv {
    public:
        using result = Func;
    };

    // We all hate macros but I'd say this is a good use case.
    #define REMOVE_FOR(CC)                                \
        template <class Ret, class... Args>               \
        class remove_conv<Ret(CC*)(Args...)> {            \
        public:                                           \
            using result = Ret (*)(Args...);              \
        };                                                \
                                                          \
        template <class Ret, class Parent, class... Args> \
        class remove_conv<Ret (CC Parent::*)(Args...)> {  \
        public:                                           \
            using result = Ret (Parent::*)(Args...);      \
        }

    REMOVE_FOR(__cdecl);
    REMOVE_FOR(__stdcall);
    REMOVE_FOR(__thiscall);
    REMOVE_FOR(__fastcall);
    REMOVE_FOR(__vectorcall);

    #undef REMOVE_FOR
#endif
}

#endif /* LILAC_CORE_META_X86_HPP */