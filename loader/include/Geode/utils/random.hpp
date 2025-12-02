#pragma once

#include <cstdint>
#include <type_traits>
#include <cstring>

namespace geode::utils::random {
    GEODE_DLL std::array<uint64_t, 3>& _getState();
    
    /// Generates a random 64-bit integer, prefer to use generate<T>() instead
    inline uint64_t nextU64() {
        // https://www.romu-random.org/
        // this is the RomuTrio algorithm, inlined for better speed
        auto rotl = [](uint64_t x, int k) {
            return (x << k) | (x >> (64 - k));
        };
        auto& state = _getState();
        uint64_t xp = state[0];
        uint64_t yp = state[1];
        uint64_t zp = state[2];
        state[0] = 15241094284759029579u * zp;
        state[1] = yp - xp;
        state[1] = rotl(state[1], 12);
        state[2] = zp - yp;
        state[2] = rotl(state[2], 44);
        return xp;
    }

    /// Generates a random value of type T
    /// For bools, this is either true or false
    /// For integral types, this is any value that fits in the type
    /// For floating point types, this is a value in the range [0.0, 1.0)
    template <typename T>
    T generate() {
        if constexpr (std::is_same_v<T, bool>) {
            return (nextU64() & 1) != 0;
        } else if constexpr (std::is_integral_v<T>) {
            uint64_t val = nextU64();
            T out;
            std::memcpy(&out, &val, sizeof(T));
            return out;
        } else if constexpr (std::is_floating_point_v<T>) {
            uint64_t val = nextU64();
            double y = (val >> 11) * (1.0 / (1ULL << 53));
            return static_cast<T>(y);
        } else {
            static_assert(!std::is_same_v<T, T>, "unsupported type for generate");
        }
    }

    /// Generates a random value of type T, in the range [min, max),
    /// meaning the max value can never be returned
    template <typename T, typename Y, typename Out = std::common_type_t<T, Y>>
    Out generate(T min, Y max) {
        if constexpr (std::is_integral_v<Out>) {
            uint64_t range = static_cast<uint64_t>(max) - static_cast<uint64_t>(min);
            uint64_t val = nextU64();
            return static_cast<Out>(static_cast<uint64_t>(min) + (val % range));
        } else if constexpr (std::is_floating_point_v<Out>) {
            double range = static_cast<double>(max) - static_cast<double>(min);
            double val = generate<double>();
            return static_cast<Out>(static_cast<double>(min) + val * range);
        } else {
            static_assert(!std::is_same_v<T, T>, "unsupported type for generate");
        }
    }

    /// Shuffles an array of elements in place.
    /// Use shuffle(std::vector<T>&) or shuffle(std::array<T, N>&) for easier usage.
    template <typename T>
    void shuffle(T* data, size_t count) {
        for (ptrdiff_t i = count - 1; i > 0; i--) {
            using std::swap;
            auto j = generate<ptrdiff_t>(0, i + 1);
            swap(data[i], data[j]);
        }
    }

    template <typename T>
    void shuffle(std::vector<T>& vec) {
        shuffle(vec.data(), vec.size());
    }

    template <typename T, size_t N>
    void shuffle(std::array<T, N>& arr) {
        shuffle(arr.data(), arr.size());
    }

    GEODE_DLL std::string generateString(size_t length, std::string_view alphabet);
    GEODE_DLL std::string generateHexString(size_t length);
    GEODE_DLL std::string generateAlphanumericString(size_t length);

    /// Generates a random UUID v4 string
    GEODE_DLL std::string generateUUID();
}