#pragma once
#include <utility>
#include <typeinfo>
#include <string_view>

namespace geode {

// Taken from boost::hash_combine, license bsl 1.0
template <class T>
inline void hashCombine(size_t& seed, T const& v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <class T>
inline size_t typenameHash() {
    std::hash<std::string_view> hasher;
    return hasher(typeid(T).name());
}

}
