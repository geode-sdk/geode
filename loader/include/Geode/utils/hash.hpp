#pragma once
#include <Geode/platform/platform.hpp>
#include <typeinfo>
#include <string_view>
#include <string>
#include <array>

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

template <size_t N>
struct Hash {
    std::array<uint8_t, N> data;

    bool operator==(Hash const& other) const noexcept = default;
    bool operator!=(Hash const& other) const noexcept = default;
    bool operator<(Hash const& other) const noexcept = default;

    /// Returns the hex-encoded string representation of the hash, e.g. "a3b4c5d6e7f8..."
    std::string toString() const {
        std::string str;
        str.resize(N * 2);
        this->hexEncode(str.data());
        return str;
    }

    /// Writes hex-encoded hash into the given buffer, the buffer must be at least 2 * N bytes long
    void hexEncode(void* buf) const {
        uint8_t* dest = reinterpret_cast<uint8_t*>(buf);
        constexpr char hexChars[] = "0123456789abcdef";

        for (size_t i = 0; i < N; ++i) {
            dest[2 * i] = hexChars[(data[i] >> 4) & 0x0F];
            dest[2 * i + 1] = hexChars[data[i] & 0x0F];
        }
    }
};

}
