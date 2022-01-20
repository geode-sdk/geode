#ifndef GEODE_CORE_META_MEMBERCALL_HPP
#define GEODE_CORE_META_MEMBERCALL_HPP

#include "tuple.hpp"
#include "x86.hpp"

namespace geode::core::meta::x86 {
    template <class Ret, class... Args>
    class Membercall : public CallConv<Ret, Args...> {
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
                (!gpr_passable<Current>::value || i != 0) &&
                (!sse_passable<Current>::value || i > 3);

            static constexpr size_t index = i;
            static constexpr size_t counter = c;
        };

        template <size_t i, class Current, size_t stack_offset>
        class filter_from {
        private:
            static constexpr bool sse = sse_passable<Current>::value && i <= 3 && i != 0;
            static constexpr bool gpr = gpr_passable<Current>::value && i == 0;

        public:
            // We're not even really filtering, just reordering.
            static constexpr bool result = true;

            static constexpr size_t index = 
                // If in GPR or SSE, retain index
                (gpr || sse) ? i
                // If on stack, offset by 4 (3 SSE + 1 GPR register(s) available)
                : stack_offset + 4;

            // If our output index is greater than 4, it has to be on stack. Increment.
            static constexpr size_t counter = stack_offset + static_cast<size_t>(index >= 4);
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
        public:
            static Ret invoke(void* address, const Tuple<Args...>& all) {
                Ret(__vectorcall* raw)(
                    float,
                    typename MyConv::template type_if<1, sse_passable, float>,
                    typename MyConv::template type_if<2, sse_passable, float>,
                    typename MyConv::template type_if<3, sse_passable, float>,
                    float,
                    float,
                    typename MyConv::template type_if<0, gpr_passable, int>,
                    int,
                    typename MyTuple::template type_at<to>...
                ) = reinterpret_cast<decltype(raw)>(address);

                return raw(
                    1907.0f,
                    MyConv::template value_if<1, sse_passable>(all, 1907.0f),
                    MyConv::template value_if<2, sse_passable>(all, 1907.0f),
                    MyConv::template value_if<3, sse_passable>(all, 1907.0f),
                    1907.0f,
                    1907.0f,
                    MyConv::template value_if<0, gpr_passable>(all, 1907),
                    1907,
                    all.template at<to>()...
                );
            }

            template <Ret(* detour)(Args...)>
            static Ret __vectorcall wrapper(
                float,
                typename MyConv::template type_if<1, sse_passable, float> f1,
                typename MyConv::template type_if<2, sse_passable, float> f2,
                typename MyConv::template type_if<3, sse_passable, float> f3,
                float,
                float,
                typename MyConv::template type_if<0, gpr_passable, int> i0,
                int,
                typename MyTuple::template type_at<to>... rest
            ) {
                auto all = Tuple<>::make(i0, f1, f2, f3, rest...);
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
        static decltype(auto) get_wrapper() {
            return &MyImpl::template wrapper<detour>;
        }
    };
}

#endif /* GEODE_CORE_META_MEMBERCALL_HPP */