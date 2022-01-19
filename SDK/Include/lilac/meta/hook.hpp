#ifndef LILAC_CORE_META_HOOK_HPP
#define LILAC_CORE_META_HOOK_HPP

#include "tuple.hpp"
#include "common.hpp"
#include "callconv.hpp"

#include "../hook/hook.hpp"

#include <type_traits>

namespace lilac::core::meta {
    template<
        auto address,
        auto detour,
        template<class, class...> class Conv
    >
    class Hook {
        static_assert(always_false<decltype(address)>, 
            "Not a valid function pointer, or hook and detour aren't compatible!");
    };
    
    template<
        class Ret,
        class... Args,
        Ret(* address)(Args...),
        Ret(* detour)(Args...),
        template<class, class...> class Conv
    >
    class Hook<address, detour, Conv> {
    private:
        using MyConv = Conv<Ret, Args...>;
        using MyTuple = Tuple<Args...>;

    private:
        static inline lilac::core::hook::Handle handle;

    public:
        Hook() {
            auto wrapper = MyConv::get_wrapper<detour>();
            this->handle = lilac::core::hook::add(address, wrapper);
        }
    };

    // member functions.
    template<
        class Ret, class Parent, class... Args,
        Ret(Parent::* address)(Args...),
        Ret(Parent::* detour)(Args...),
        template<class, class...> class Conv
    >
    class Hook<address, detour, Conv> {
        // deal with this later lol
    };
}


#endif /* LILAC_CORE_META_HOOK_HPP */