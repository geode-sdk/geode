#ifndef GEODE_CORE_META_OPTCALL_HPP
#define GEODE_CORE_META_OPTCALL_HPP

#include "callconv.hpp"
#include "tuple.hpp"
#include "x86.hpp"

namespace geode::core::meta::x86 {
    template <class Ret, class... Args>
    class Optcall : public CallConv<Ret, Args...> {
    private:
        // Metaprogramming / typedefs we need for the rest of the class.
        using MyConv = CallConv<Ret, Args...>;

    private:
        // These go in a class to not pollute the namespace.
        class Sequences {
        private:
            // These are required for proper reordering.
            static constexpr size_t length = sizeof...(Args);

            static constexpr size_t SSES = 4;
            static constexpr bool is_sse[length] = { sse_passable<Args>... };

            static constexpr size_t GPRS = 2;
            static constexpr bool is_gpr[length] = { gpr_passable<Args>... };

            static constexpr auto reordered_arr = reorder_pack<Args...>();

            // Setup call from our caller, to "foreign" function
            static constexpr auto filter_to() {
                /* The size of our output may be longer than the input.
                 * Also, annoyingly, we must make a lambda to calculate this.
                 */
                constexpr auto arr_size = []() -> size_t {
                    // Magic constant 2 is to pad XMM4 and XMM5: can't be used.
                    size_t size = length + SSES + 2;

                    /* We assume there are no SSES initially.
                     * Any SSES we encounter, we have to remove a "duplicate".
                     */
                    for (size_t i = 0; i < SSES; ++i) {
                        if (i < length && is_sse[reordered_arr[i]]) {
                            --size;
                        }
                    }

                    return size;
                };
                std::array<size_t, arr_size()> to = {};

                // This is the index of the placeholder float, for clobbering SSEs.
                constexpr size_t CLOBBER_SSE = length;

                // Put the SSEs into the right XMM registers, if they exist.
                for (size_t i = 0; i < SSES; ++i) {
                    if (i < length && is_sse[reordered_arr[i]]) {
                        to[i] = reordered_arr[i];
                    }
                    else {
                        to[i] = CLOBBER_SSE;
                    }
                }

                // Clobber XMM4 and XMM5.
                to[4] = CLOBBER_SSE;
                to[5] = CLOBBER_SSE;

                for (size_t in = 0, out = SSES + 2; in < length; ++in) {
                    // Put all the non SSEs in their correct places. Skip SSEs.
                    size_t current = reordered_arr[in];
                    if (!(is_sse[current] && in < SSES)) {
                        to[out] = current;
                        ++out;
                    }
                }

                return to;
            }

            // Setup call from "foreign" function, to one of ours.
            static constexpr auto filter_from() {
                std::array<size_t, length> from = {};

                for (size_t i = 0, gprs = 0, offset = 0; i < length; ++i) {
                    size_t current = reordered_arr[i];
                    if (is_sse[current] && i < SSES) {
                        // If in SSE, retain index
                        from[current] = i;
                    }
                    else if (is_gpr[current] && gprs < GPRS) {
                        // If in GPR, offset by 4 (4 SSE registers available)
                        from[current] = gprs + SSES;
                        ++gprs;
                    }
                    else {
                        // If on stack, offset by 6 (4 SSE + 2 GPR registers available)
                        from[current] = offset + SSES + GPRS;
                        ++offset;
                    }
                }

                return from;
            }

            // Grab only the stack values. For determining stack fixup.
            static constexpr auto filter_stack() {
                /* The size of our output may be shorter than the input.
                 * Again, annoyingly, we must make a lambda to calculate this.
                 */
                constexpr auto arr_size = []() -> size_t {
                    size_t size = length;

                    for (size_t i = 0, gprs = 0; i < length; ++i) {
                        size_t current = reordered_arr[i];
                        if (is_sse[current] && i < SSES) {
                            --size;
                        }
                        else if (is_gpr[current] && gprs < GPRS) {
                            --size;
                            ++gprs;
                        }
                    }

                    return size;
                };
                std::array<size_t, arr_size()> stack = {};

                for (size_t in = 0, out = 0, gprs = 0; in < length; ++in) {
                    size_t current = reordered_arr[in];
                    if ((!is_sse[current] || in >= SSES) && (!is_gpr[current] || gprs >= GPRS)) {
                        stack[out] = current;
                        ++out;
                    }

                    if (is_gpr[current]) {
                        ++gprs;
                    }
                }

                return stack;
            }

            // Annoyingly, unless we're using C++20, we can't eliminate these intermediates. (afaik)
            static constexpr auto to_arr = filter_to();
            static constexpr auto from_arr = filter_from();
            static constexpr auto stack_arr = filter_stack();

        public:
            using to = typename MyConv::template arr_to_seq<to_arr>;
            using from = typename MyConv::template arr_to_seq<from_arr>;
            using stack = typename MyConv::template arr_to_seq<stack_arr>;
        };

    private:
        // Where all the logic is actually implemented.
        template <class Class, class, class>
        class Impl {
            static_assert(
                always_false<Class>,
                "Please report a bug to the Geode developers! This should never be reached.\n"
                "SFINAE didn't reach the right overload!"
            );
        };

        template <size_t... to, size_t... from, size_t... stack>
        class Impl<
            std::index_sequence<to...>, std::index_sequence<from...>,
            std::index_sequence<stack...>> {
        private:
            static constexpr size_t fix = (std::is_class_v<Ret> ? stack_fix<Ret> : 0)
                + stack_fix<typename Tuple<Args...>::template type_at<stack>...>;

        public:
            static Ret invoke(void* address, const Tuple<Args..., float>& all) {
                if constexpr (!std::is_same_v<Ret, void>) {
                    Ret ret =
                        reinterpret_cast<Ret(__vectorcall*)(decltype(all.template at<to>())...)>(
                            address
                        )(all.template at<to>()...);

                    if constexpr (fix != 0) {
                        __asm add esp, [fix]
                    }

                    return ret;
                }
                else {
                    reinterpret_cast<Ret(__vectorcall*)(decltype(all.template at<to>())...)>(address
                    )(all.template at<to>()...);

                    if constexpr (fix != 0) {
                        __asm add esp, [fix]
                    }
                }
            }

            template <Ret (*detour)(Args...)>
            static Ret __cdecl wrapper(
                /* It's wrapped to stop MSVC from giving me error messages with internal compiler
                 * info. WTF.
                 */
                typename Tuple<Args...>::template type_at_wrap<stack>... rest
            ) {
                Register<double> f0, f1, f2, f3;
                Register<void*> i0, i1;

                __asm {
                    movlpd f0, xmm0
                    movlpd f1, xmm1
                    movlpd f2, xmm2
                    movlpd f3, xmm3
                    
                    mov i0, ecx
                    mov i1, edx
                }

                auto all = Tuple<>::make(f0, f1, f2, f3, i0, i1, rest...);

                // Register<Type> has a explicit conversion operator we can take advantage of.
                return detour(static_cast<Args>(all.template at<from>())...);
            }
        };

    private:
        // Putting it all together: instantiating Impl with our filters.
        using MyImpl =
            Impl<typename Sequences::to, typename Sequences::from, typename Sequences::stack>;

    public:
        // Just wrapping MyImpl.
        static Ret invoke(void* address, Args... all) {
            /* The extra float here is so that we can grab placeholders.
             * If we don't have anything in XMM0 - 5, we will use
             * this placeholder instead. The value is 314 to avoid unintentional
             * bugs (since 0 may work coincidentally).
             */
            return MyImpl::invoke(address, { all..., 314.0f });
        }

        template <Ret (*detour)(Args...)>
        static constexpr decltype(auto) get_wrapper() {
            return &MyImpl::template wrapper<detour>;
        }
    };

    template <class Ret>
    class Optcall<Ret> : public Cdecl<Ret> {
    public:
        using Cdecl<Ret>::invoke;
        using Cdecl<Ret>::get_wrapper;
    };
}

#endif /* GEODE_CORE_META_OPTCALL_HPP */