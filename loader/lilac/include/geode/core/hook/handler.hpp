#define GEODE_CORE_HANDLER_HPP

#include "../meta/meta.hpp"

#include <vector>

namespace lilac::handler {
    template <typename Ret, typename... Args>
    struct Handler {
        std::vector<Ret (*)(Args...)> m_detours;

        static Ret pure(Args... args) {
            static thread_local size_t counter = 0;
        }
    }

}
