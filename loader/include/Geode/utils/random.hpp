#pragma once

#include <Geode/platform/platform.hpp>
#include <cstdint>
#include <type_traits>
#include <cstring>
#include <array>
#include <random>

namespace geode::utils::random {
    /// Generates a secure random 64-bit integer from the OS
    inline uint64_t secureU64() {
        std::random_device rd;
        uint32_t a = rd();
        uint32_t b = rd();
        return (static_cast<uint64_t>(a) << 32) | static_cast<uint64_t>(b);
    }

    /// A PRNG engine using the RomuTrio algorithm.
    /// This engine is fully inlined, requires small amount of storage and is very fast and reseedable.
    /// https://www.romu-random.org/
    class Generator {
    public:
        using result_type = uint64_t;

        /// Creates the random generator with a seed generated from the OS
        Generator() : Generator(secureU64()) {}

        /// Creates the random generator with the given seed
        Generator(uint64_t seed) {
            this->seed(seed);
        }

        /// Re-seeds the generator with the given seed
        void seed(uint64_t seed) {
            m_x = m_y = m_z = seed;
        }

        /// Generates the next random 64-bit integer
        result_type next() {
            auto rotl = [](uint64_t x, int k) {
                return (x << k) | (x >> (64 - k));
            };

            uint64_t xp = m_x, yp = m_y, zp = m_z;
            m_x = 15241094284759029579u * zp;
            m_y = yp - xp; m_y = rotl(m_y, 12);
            m_z = zp - yp; m_z = rotl(m_z, 44);
            return xp;
        }

        /// Generates a random value of type T
        /// For bools, this is either true or false
        /// For integral types, this is any value that fits in the type
        /// For floating point types, this is a value in the range [0.0, 1.0)
        template <typename T>
        T generate() {
            if constexpr (std::is_same_v<T, bool>) {
                return (this->next() & 1) != 0;
            } else if constexpr (std::is_integral_v<T>) {
                return this->generate<T>(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
            } else if constexpr (std::is_floating_point_v<T>) {
                return this->generate<T>(0.0, 1.0);
            } else {
                static_assert(!std::is_same_v<T, T>, "unsupported type for generate");
            }
        }

        /// Generates a random value of type R (if specified), in the range [min, max),
        /// meaning the max value can never be returned
        template <
            typename R = void,
            typename T,
            typename Y,
            typename Out = std::conditional_t<std::is_void_v<R>, std::common_type_t<T, Y>, R>
        >
        Out generate(T min_, Y max_) {
            if (max_ <= min_) return static_cast<Out>(min_);

            if constexpr (std::is_integral_v<Out>) {
                // uniform int distribution does not support chars :)
                if constexpr (sizeof(Out) == 1) {
                    return static_cast<Out>(this->generate<int>(static_cast<int>(min_), static_cast<int>(max_)));
                } else {
                    std::uniform_int_distribution<Out> dist(min_, max_ - 1);
                    return dist(*this);
                }
            } else if constexpr (std::is_floating_point_v<Out>) {
                std::uniform_real_distribution<Out> dist(min_, max_);
                return dist(*this);
            } else {
                static_assert(!std::is_same_v<T, T>, "unsupported type for generate");
            }
        }

        result_type operator()() {
            return this->next();
        }

        static constexpr result_type min() {
            return 0;
        }

        static constexpr result_type max() {
            return std::numeric_limits<result_type>::max();
        }

    private:
        uint64_t m_x;
        uint64_t m_y;
        uint64_t m_z;
    };

    /// Returns the global, thread-local random generator
    GEODE_DLL Generator& _getGenerator();

    /// Generates a random 64-bit integer from the thread-local generator, prefer to use generate<T>() instead
    inline uint64_t nextU64() {
        return _getGenerator().next();
    }

    /// Generates a random value of type T
    /// For bools, this is either true or false
    /// For integral types, this is any value that fits in the type
    /// For floating point types, this is a value in the range [0.0, 1.0)
    template <typename T>
    T generate() {
        return _getGenerator().generate<T>();
    }

    /// Generates a random value of type T, in the range [min, max),
    /// meaning the max value can never be returned
    template <typename R = void, typename T, typename Y>
    auto generate(T min, Y max) {
        return _getGenerator().generate<R, T, Y>(min, max);
    }

    /// Shuffles an array of elements in place.
    /// Use the shuffle(T&) overload for easier usage if your container supports iterators
    template <typename Iter>
    void shuffle(Iter begin, Iter end) {
        auto& gen = _getGenerator();

        using diff_t = std::iterator_traits<Iter>::difference_type;
        for (diff_t i = end - begin - 1; i > 0; i--) {
            using std::swap;
            auto j = gen.generate<diff_t>(0, i + 1);
            swap(*(begin + i), *(begin + j));
        }
    }

    template <typename Cont>
    void shuffle(Cont& cont) {
        using std::begin, std::end;
        shuffle(begin(cont), end(cont));
    }

    GEODE_DLL std::string generateString(size_t length, std::string_view alphabet);
    GEODE_DLL std::string generateHexString(size_t length);
    GEODE_DLL std::string generateAlphanumericString(size_t length);

    /// Generates a random UUID v4 string
    GEODE_DLL std::string generateUUID();

    GEODE_DLL void fillBytes(void* buffer, size_t size);
}