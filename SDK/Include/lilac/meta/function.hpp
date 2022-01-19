#ifndef LILAC_CORE_META_FUNCTION_HPP
#define LILAC_CORE_META_FUNCTION_HPP

#include "tuple.hpp"
#include "common.hpp"
#include "callconv.hpp"

#include <type_traits>

namespace lilac::core::meta {
    /* The Lilac Function class wraps functions with unconventional
    *  calling conventions (how ironic).
    */
    template<
        class Func,
        template<class, class...> class Conv
    >
    class Function {
        static_assert(always_false<Func>, "Not a valid function pointer!");
    };

    template<
        class Ret, class... Args,
        template<class, class...> class Conv
    >
    class Function<Ret(Args...), Conv> {
    private:
        using MyConv = Conv<Ret, Args...>;
        using MyTuple = Tuple<Args...>;

    private:
        void* addr;

    public:
        template<class T>
        Function(const T& addr)
            : addr(reinterpret_cast<void*>(addr)) {}

        decltype(auto) operator()(Args... all) const {
            return MyConv::invoke(
                addr,
                { all... }
            );
        }
    };
}

#endif /* LILAC_CORE_META_FUNCTION_HPP */