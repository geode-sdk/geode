#pragma once
#include <Geode/platform/platform.hpp>
#include <Geode/Result.hpp>
#include <utility>
#include <typeinfo>
#include <string_view>
#include <filesystem>
#include <string>
#include <span>
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

using Sha256 = Hash<32>;

/// A class for incrementally calculating the SHA256 hash of some data.
/// Call `update` to advance the state of the hasher with new data, then call `finish` to get the final digest.
class GEODE_DLL Sha256Hasher {
public:
    Sha256Hasher();
    ~Sha256Hasher();

    Sha256Hasher(Sha256Hasher const&) = delete;
    Sha256Hasher& operator=(Sha256Hasher const&) = delete;
    Sha256Hasher(Sha256Hasher&&) noexcept;
    Sha256Hasher& operator=(Sha256Hasher&&) noexcept;

    void update(std::span<uint8_t const> data);
    void update(void const* data, size_t bytes);
    void update(std::string_view data);

    Sha256 finish() const;

    /// Resets the internal state of the hash. This allows you to treat the hasher like a new instance,
    /// without having to actually create a new object and allocate memory.
    void reset();

private:
    void* m_state = nullptr;
};

inline auto format_as(Sha256 const& hash) -> std::string {
    return hash.toString();
}

/// Reads the binary file at the given path and calculates the SHA256 digest of the contents.
/// This reads the file lazily and uses C++ iostreams. If the performance of that is unacceptable, use `Sha256Hasher` directly.
Result<Sha256> GEODE_DLL sha256File(std::filesystem::path const& path);

/// Calculates the SHA256 digest of the given data.
Sha256 GEODE_DLL sha256(std::span<uint8_t const> data);
/// Calculates the SHA256 digest of the given data.
Sha256 GEODE_DLL sha256(std::string_view data);

}
