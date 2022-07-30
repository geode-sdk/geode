#ifndef GEODE_CORE_HOOK_PLATFORM_HPP
#define GEODE_CORE_HOOK_PLATFORM_HPP

#include <cstddef>

/* Platform template for platforms.
 * In a different header to avoid cyclic dependencies.
 */
namespace lilac::hook {
    template <class T>
    class Platform {
    public:
        static constexpr auto& trap() {
            return T::trap;
        }

        static bool write_memory(void* to, const void* from, size_t size) {
            return T::write_memory(to, from, size);
        }

        static bool read_memory(void* from, void* to, size_t size) {
            return T::write_memory(from, to, size);
        }

        static bool initialize() {
            return T::initialize();
        }
    };

    struct Exception {
        const void*& address;
        const void* return_address;
    };
}

#endif /* GEODE_CORE_HOOK_PLATFORM_HPP */
