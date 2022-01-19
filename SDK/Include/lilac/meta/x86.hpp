#ifndef LILAC_CORE_META_X86_HPP
#define LILAC_CORE_META_X86_HPP

namespace lilac::core::meta::x86 {
    // Logic needed by x86 calling conventions for stack fixing / filtering.
    template<class Class>
    struct gpr_passable {
        static constexpr bool value = 
            any_of<
                std::remove_cv_t<Class>,
                bool, signed char, signed short, signed int, signed long,
                unsigned char, unsigned short, unsigned int, unsigned long,
                char, short, int, long
            > || 
            std::is_pointer_v<Class> || 
            std::is_reference_v<Class>;
    };

    template<class Class>
    struct sse_passable {
        static constexpr bool value = 
            any_of<
                std::remove_cv_t<Class>,
                float, double
            >;
    };

    
    template<class... Stack>
    static constexpr size_t stack_fix = 
        (((sizeof(Stack) % sizeof(void*) == 0) ?
            sizeof(Stack) :
            sizeof(Stack) - (sizeof(Stack) % sizeof(void*)) + sizeof(void*)) + ...);

    template<>
    static constexpr size_t stack_fix<> = 0;

    template<>
    static constexpr size_t stack_fix<void> = 0;
}

#endif /* LILAC_CORE_META_X86_HPP */