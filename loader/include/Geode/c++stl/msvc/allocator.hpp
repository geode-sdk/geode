#pragma once

#include <cstddef>
#include <type_traits>

namespace geode::stl {

    void* operatorNew(std::size_t size);
    void operatorDelete(void* ptr);

    template <class T>
    struct allocator {
        using value_type = T;
        using size_type	= std::size_t;
        using difference_type = std::ptrdiff_t;
        using propagate_on_container_move_assignment = std::true_type;

        constexpr allocator() noexcept {}
        constexpr allocator(const allocator& other) noexcept {}

        template<class U>
        constexpr allocator(const allocator<U>& other) noexcept {}

        constexpr ~allocator() {}

        [[nodiscard]] T* allocate(std::size_t n) {
            return reinterpret_cast<T*>(operatorNew(n * sizeof(T)));
        }

        void deallocate(T* p, std::size_t n) {
            operatorDelete(reinterpret_cast<void*>(p));
        }
    };
}