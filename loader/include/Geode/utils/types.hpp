#pragma once

#include <vector>

namespace geode {
    using byte_array = std::vector<uint8_t>;

    template <typename T>
    byte_array to_byte_array(T const& a) {
        byte_array out;
        out.resize(sizeof(T));
        std::memcpy(out.data(), &a, sizeof(T));
        return out;
    }

    template <class T>
    struct TypeIdentity {
        using type = T;
    };

    template <class T>
    using TypeIdentityType = typename TypeIdentity<T>::type;
}
