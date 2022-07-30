#ifndef LILAC_CORE_META_THISCALL_HPP
#define LILAC_CORE_META_THISCALL_HPP

namespace lilac::meta::x86 {
    template <class Ret, class... Args>
    class Thiscall {
    private:
        template <Ret (*detour)(Args...)>
        static Ret __thiscall wrapper(Args... all) {
            return detour(all...);
        }

    public:
        static Ret invoke(void* address, Args... all) {
            Ret(__thiscall * raw)(Args...) = reinterpret_cast<decltype(raw)>(address);
            return raw(all...);
        }

        template <Ret (*detour)(Args...)>
        static constexpr decltype(auto) get_wrapper() {
            return &wrapper<detour>;
        }
    };
}

#endif /* LILAC_CORE_META_THISCALL_HPP */