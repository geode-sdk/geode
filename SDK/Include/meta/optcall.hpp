#ifndef GEODE_CORE_META_OPTCALL_HPP
#define GEODE_CORE_META_OPTCALL_HPP

#include "tuple.hpp"
#include "x86.hpp"

namespace geode::core::meta::x86 {
    template <class Ret, class... Args>
    class Optcall : public CallConv<Ret, Args...> {
    private:
        // Metaprogramming / typedefs we need for the rest of the class.
        using MyConv = CallConv<Ret, Args...>;
        // for some reason using definitions dont get inherited properly
        using MyTuple = typename MyConv::MyTuple;

    private:
        // Filters that will be passed to Tuple::filter.
        template <size_t i, class Current, size_t c>
        class filter_to {
        public:
            static constexpr bool result = 
                (!gpr_passable<Current>::value || i > 1) &&
                (!sse_passable<Current>::value || i > 3);

            static constexpr size_t index = i;
            static constexpr size_t counter = c;
        };

        template <size_t i, class Current, size_t stack_offset>
        class filter_from {
        private:
            static constexpr bool sse = sse_passable<Current>::value && i <= 3;
            static constexpr bool gpr = gpr_passable<Current>::value && i <= 1;

        public:
            // We're not even really filtering, just reordering.
            static constexpr bool result = true;

            static constexpr size_t index = 
                // If in SSE, retain index
                sse ? i
                // If in GPR, offset by 4 (4 SSE registers available)
                : (gpr ? i + 4
                // If on stack, offset by 6 (4 SSE + 2 GPR registers available)
                : stack_offset + 6);

            // If our output index is greater than 6, it has to be on stack. Increment.
            static constexpr size_t counter = stack_offset + static_cast<size_t>(index >= 6);
        };

    private:
        // Where all the logic is actually implemented. Needs to be instantiated by Optcall, though.
        template <class Class, class>
        class Impl {
            static_assert(always_false<Class>, 
                "Please report a bug to the Geode developers! This should never be reached.\n"
                "SFINAE didn't reach the right overload!");
        };

        template <size_t... to, size_t... from>
        class Impl<std::index_sequence<to...>, std::index_sequence<from...>> {
        private:
            static constexpr size_t fix =
                (std::is_class_v<Ret> ? stack_fix<Ret> : 0)
                + stack_fix<typename MyTuple::template type_at<to>...>;
                
        public:
            static Ret invoke(void* address, const Tuple<Args...>& all) {
                Ret(__vectorcall* raw)(
                    typename MyConv::template type_if<0, sse_passable, float>,
                    typename MyConv::template type_if<1, sse_passable, float>,
                    typename MyConv::template type_if<2, sse_passable, float>,
                    typename MyConv::template type_if<3, sse_passable, float>,
                    float,
                    float,
                    typename MyConv::template type_if<0, gpr_passable, int>,
                    typename MyConv::template type_if<1, gpr_passable, int>,
                    typename MyTuple::template type_at<to>...
                ) = reinterpret_cast<decltype(raw)>(address);

                if constexpr (!std::is_same_v<Ret, void>) {
                    Ret ret = raw(
                        MyConv::template value_if<0, sse_passable>(all, 1907.0f),
                        MyConv::template value_if<1, sse_passable>(all, 1907.0f),
                        MyConv::template value_if<2, sse_passable>(all, 1907.0f),
                        MyConv::template value_if<3, sse_passable>(all, 1907.0f),
                        1907.0f,
                        1907.0f,
                        MyConv::template value_if<0, gpr_passable>(all, 1907),
                        MyConv::template value_if<1, gpr_passable>(all, 1907),
                        all.template at<to>()...
                    );

                    if constexpr (fix != 0) {
                        __asm add esp, [fix]
                    }

                    return ret;
                }
                else {
                    raw(
                        MyConv::template value_if<0, sse_passable>(all, 1907.0f),
                        MyConv::template value_if<1, sse_passable>(all, 1907.0f),
                        MyConv::template value_if<2, sse_passable>(all, 1907.0f),
                        MyConv::template value_if<3, sse_passable>(all, 1907.0f),
                        1907.0f,
                        1907.0f,
                        MyConv::template value_if<0, gpr_passable>(all, 1907),
                        MyConv::template value_if<1, gpr_passable>(all, 1907),
                        all.template at<to>()...
                    );

                    if constexpr (fix != 0) {
                        __asm add esp, [fix]
                    }
                }
            }

            template <Ret(* detour)(Args...)>
            static Ret __cdecl wrapper(
                // Not sure why this doesn't work otherwise, but oh well...
                typename MyConv::template type_at_wrap<to>... rest
            ) {
                Register<double, typename MyConv::template type_if<0, sse_passable, float>> f0;
                Register<double, typename MyConv::template type_if<1, sse_passable, float>> f1;
                Register<double, typename MyConv::template type_if<2, sse_passable, float>> f2;
                Register<double, typename MyConv::template type_if<3, sse_passable, float>> f3;
                
                Register<void*, typename MyConv::template type_if<0, gpr_passable, int>> i0;
                Register<void*, typename MyConv::template type_if<1, gpr_passable, int>> i1;

                __asm {
                    movlpd f0, xmm0
                    movlpd f1, xmm1
                    movlpd f2, xmm2
                    movlpd f3, xmm3
                    
                    mov i0, ecx
                    mov i1, edx
                }

                auto all = Tuple<>::make(
                    f0.get(), f1.get(), f2.get(), f3.get(), 
                    i0.get(), i1.get(), rest...
                );

                return detour(all.template at<from>()...);
            }
        };

    private:
        // Putting it all together: instantiating Impl with our filters.
        using MyImpl = 
            Impl<
                typename MyTuple::template filter<filter_to>,
                typename MyTuple::template filter<filter_from>
            >;

    public:
        // Just wrapping MyImpl.
        static Ret invoke(void* address, Args... all) {
            return MyImpl::invoke(address, { all... });
        }

        template <Ret(* detour)(Args...)>
        static constexpr decltype(auto) get_wrapper() {
            return &MyImpl::template wrapper<detour>;
        }
    };
}

#endif /* GEODE_CORE_META_OPTCALL_HPP */