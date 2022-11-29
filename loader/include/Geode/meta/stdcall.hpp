#ifndef GEODE_CORE_META_STDCALL_HPP
#define GEODE_CORE_META_STDCALL_HPP

namespace geode::core::meta::x86 {
    template <class Ret, class... Args>
    class Stdcall {
    private:
        template <Ret (*detour)(Args...)>
        static Ret __stdcall wrapper(Args... all) {
            return detour(all...);
        }

    public:
        static Ret invoke(void* address, Args... all) {
            Ret(__stdcall * raw)(Args...) = reinterpret_cast<decltype(raw)>(address);
            return raw(all...);
        }

        template <Ret (*detour)(Args...)>
        static auto get_wrapper() {
            return reinterpret_cast<void*>(&wrapper<detour>);
        }
    };
}

#endif /* GEODE_CORE_META_STDCALL_HPP */
