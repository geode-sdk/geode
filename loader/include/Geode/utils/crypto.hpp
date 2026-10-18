#pragma once
#include "hash.hpp"
#include <Geode/Result.hpp>
#include <asp/fs/Path.hpp>
#include <span>
#include <utility>

namespace geode {
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
    Result<Sha256> GEODE_DLL sha256File(asp::fs::PathView path);

    /// Calculates the SHA256 digest of the given data.
    Sha256 GEODE_DLL sha256(std::span<uint8_t const> data);
    /// Calculates the SHA256 digest of the given data.
    Sha256 GEODE_DLL sha256(std::string_view data);
}
