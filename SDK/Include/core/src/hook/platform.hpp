#ifndef GEODE_CORE_HOOK_PLATFORM_HPP
#define GEODE_CORE_HOOK_PLATFORM_HPP

#include <cstddef>

/* platform template for platforms, lol
* in a different header to avoid cyclic dependencies
*/
namespace geode::core::hook {
    template <typename T>
    class Platform {
    public:
        static constexpr const char* get_trap() {
            return T::trap;
        }

        static constexpr size_t get_trap_size() {
            return sizeof(T::trap);
        }

        static bool write_memory(void* to, const void* from, size_t size) {
            return T::write_memory(to, from, size);
        }

        static bool read_memory(void* addr, void* to, size_t size) {
            return T::write_memory(addr, to, size);
        }

        static bool initialize() {
            return T::initialize();
        }

        static const inline bool init = initialize();
    };

    struct Exception {
        const void* address = nullptr;
        const void* return_address = nullptr;
        const void*& instruction_pointer;
    };
}

#endif /* GEODE_CORE_HOOK_PLATFORM_HPP */
