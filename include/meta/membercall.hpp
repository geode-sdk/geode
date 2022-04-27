#ifndef GEODE_CORE_META_MEMBERCALL_HPP
#define GEODE_CORE_META_MEMBERCALL_HPP

#include "callconv.hpp"
#include "tuple.hpp"
#include "x86.hpp"

namespace geode::core::meta::x86 {
    template <class Ret, class... Args>
    class Membercall : public CallConv<Ret, Args...> {
    private:
        // Metaprogramming / typedefs we need for the rest of the class.
        using MyConv = CallConv<Ret, Args...>;

    private:
        class Sequences {
        private:
            // These are required for proper reordering.
            static constexpr size_t length = sizeof...(Args);

            static constexpr size_t SSES = 3;
            static constexpr bool is_sse[length] = { sse_passable<Args>... };

            static constexpr bool is_gpr[length] = { gpr_passable<Args>... };

            static constexpr auto reordered_arr = reorder_pack<Args...>();

            // Setup call from our caller, to "foreign" function
            static constexpr auto filter_to() {
                /* The size of our output may be longer than the input.
                 * For the third time, annoyingly, we need to make a lambda for this.
                 */
                constexpr auto arr_size = []() -> size_t {
                    // Magic constant 5: XMM0, XMM4, XMM5, ECX, and EDX.
                    size_t size = length + SSES + 5;

                    // Check our only GPR.
                    if (0 < length && is_gpr[reordered_arr[0]]) {
                        --size;
                    }

                    /* We assume there are no SSES initially.
                     * Any SSES we encounter, we have to remove a "duplicate".
                     */
                    for (size_t i = 1; i < SSES + 1; ++i) {
                        if (i < length && is_sse[reordered_arr[i]]) {
                            --size;
                        }
                    }

                    return size;
                };
                std::array<size_t, arr_size()> to = {};

                /* These are the indices of the placeholder float and int, for clobbering SSEs and
                 * GPRs, respectively.
                 */
                constexpr size_t CLOBBER_SSE = length;
                constexpr size_t CLOBBER_GPR = length + 1;

                // Put the SSEs into the right XMM registers, if they exist.
                for (size_t i = 1; i < SSES + 1; ++i) {
                    if (i < length && is_sse[reordered_arr[i]]) {
                        to[i] = reordered_arr[i];
                    }
                    else {
                        to[i] = CLOBBER_SSE;
                    }
                }

                // Clobber XMM0, XMM4, XMM5, and EDX.
                to[0] = CLOBBER_SSE;
                to[4] = CLOBBER_SSE;
                to[5] = CLOBBER_SSE;
                to[7] = CLOBBER_GPR;

                // Handle our GPR and put it in ECX if we can.
                if (length > 0 && is_gpr[reordered_arr[0]]) {
                    to[6] = reordered_arr[0];
                }
                else {
                    to[6] = CLOBBER_GPR;
                }

                for (size_t in = 1, out = SSES + 5; in < length; ++in) {
                    // Put all non SSEs and non GPRs in their places.
                    size_t current = reordered_arr[in];
                    if (!(is_sse[current] && in < SSES + 1)) {
                        to[out] = current;
                        ++out;
                    }
                }

                return to;
            }

            // Setup call from "foreign" function, to one of ours.
            static constexpr auto filter_from() {
                std::array<size_t, length> from = {};

                constexpr size_t CLOBBER_SSE = length;
                constexpr size_t CLOBBER_GPR = length + 1;

                if (length > 0 && is_gpr[reordered_arr[0]]) {
                    // SSES + 3 = ECX
                    from[reordered_arr[0]] = SSES + 3;
                }

                for (size_t i = 1, offset = 0; i < length; ++i) {
                    size_t current = reordered_arr[i];
                    if (is_sse[current] && i < SSES + 1) {
                        // If in SSE, retain index
                        from[current] = i;
                    }
                    else {
                        // If on stack, offset by 8 (6 SSE + 2 GPR registers available)
                        from[current] = offset + SSES + 5;
                        ++offset;
                    }
                }

                return from;
            }

            // Annoyingly, unless we're using C++20, we can't eliminate these intermediates. (afaik)
            static constexpr auto to_arr = filter_to();
            static constexpr auto from_arr = filter_from();

        public:
            using to = typename MyConv::template arr_to_seq<to_arr>;
            using from = typename MyConv::template arr_to_seq<from_arr>;
        };

    private:
        // Where all the logic is actually implemented.
        template <class Class, class>
        class Impl {
            static_assert(
                always_false<Class>,
                "Please report a bug to the Geode developers! This should never be reached.\n"
                "SFINAE didn't reach the right overload!"
            );
        };

        template <size_t... to, size_t... from>
        class Impl<std::index_sequence<to...>, std::index_sequence<from...>> {
        public:
            static Ret invoke(void* address, const Tuple<Args..., float, int>& all) {
                return reinterpret_cast<Ret(__vectorcall*)(
                    typename Tuple<Args..., float, int>::template type_at<to>...
                )>(address)(all.template at<to>()...);
            }

            template <Ret (*detour)(Args...)>
            static Ret __vectorcall wrapper(
                /* It's wrapped to stop MSVC from giving me error messages with internal compiler
                 * info. WTF.
                 */
                typename Tuple<Args..., float, int>::template type_at_wrap<to>... raw
            ) {
                auto all = Tuple<>::make(raw...);
                return detour(all.template at<from>()...);
            }
        };

    private:
        // Putting it all together: instantiating Impl with our filters.
        using MyImpl = Impl<typename Sequences::to, typename Sequences::from>;

    public:
        // Just wrapping MyImpl.
        static Ret invoke(void* address, Args... all) {
            /* The extra float and int here are so that we can grab placeholders.
             * If we don't have anything in XMM0 - 5 or ECX / EDX, we will use
             * these placeholders instead. The values are 314 to avoid unintentional
             * bugs (since 0 may work coincidentally).
             */
            return MyImpl::invoke(address, { all..., 314.0f, 314 });
        }

        template <Ret (*detour)(Args...)>
        static constexpr decltype(auto) get_wrapper() {
            return &MyImpl::template wrapper<detour>;
        }
    };
}

#endif /* GEODE_CORE_META_MEMBERCALL_HPP */