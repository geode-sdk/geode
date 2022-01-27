#ifndef GEODE_CORE_META_X86_HPP
#define GEODE_CORE_META_X86_HPP

namespace geode::core::meta::x86 {
    // Logic needed by x86 calling conventions for stack fixing / filtering.
    template <class Class>
    struct gpr_passable {
        static constexpr bool value = 
            any_of<
                std::remove_cv_t<Class>,
                bool, signed char, signed short, signed int, signed long,
                unsigned char, unsigned short, unsigned int, unsigned long,
                char, short, int, long
            > || 
            std::is_pointer_v<Class> || 
            std::is_reference_v<Class> ||
            std::is_member_function_pointer_v<Class>;
    };

    template <class Class>
    struct sse_passable {
        static constexpr bool value = 
            any_of<
                std::remove_cv_t<Class>,
                float, double
            >;
    };
    
    template <class... Stack>
    static constexpr size_t stack_fix = 
        (((sizeof(Stack) % sizeof(void*) == 0) ?
            sizeof(Stack) :
            sizeof(Stack) - (sizeof(Stack) % sizeof(void*)) + sizeof(void*)) + ...);

    template <>
    static constexpr size_t stack_fix<> = 0;

    template <>
    static constexpr size_t stack_fix<void> = 0;

    template <class From, class To>
    class Register {
    public:
        From raw;

    public:
        To get() {
            static_assert(sizeof(From) >= sizeof(To), 
                "Please report a bug to the Geode developers! This should never be reached.\n"
                "Size of Register is smaller than the size of the destination type!");
            union {
                From from;
                To to;
            } u;
            u.from = raw;
            return u.to;
        }
    };
}

#endif /* GEODE_CORE_META_X86_HPP */