#pragma once

#include "Result.hpp"

#include "../DefaultInclude.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <ghc/filesystem.hpp>

// for some reason std::filesystem::path doesn't have std::hash defined in C++17 
// and ghc seems to have inherited this limitation
template<>
struct std::hash<ghc::filesystem::path> {
    std::size_t operator()(ghc::filesystem::path const& path) const noexcept {
        return ghc::filesystem::hash_value(path);
    }
};

namespace geode {
    using ByteVector = std::vector<uint8_t>;

    template <typename T>
    ByteVector toByteArray(T const& a) {
        ByteVector out;
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
        
    namespace utils {
        // helper for std::visit
        template<class... Ts> struct makeVisitor : Ts... { using Ts::operator()...; };
        template<class... Ts> makeVisitor(Ts...) -> makeVisitor<Ts...>;

        template<class T, class ... Args>
        constexpr T getOr(std::variant<Args...> const& variant, T const& defValue) {
            return std::holds_alternative<T>(variant) ? 
                std::get<T>(variant) : defValue;
        }

        constexpr unsigned int hash(char const* str, int h = 0) {
            return !str[h] ? 5381 : (hash(str, h + 1) * 33) ^ str[h];
        }

        constexpr unsigned int hash(wchar_t const* str, int h = 0) {
            return !str[h] ? 5381 : (hash(str, h + 1) * 33) ^ str[h];
        }

        constexpr size_t operator"" _h(char const* txt, size_t) {
            return geode::utils::hash(txt);
        }

        constexpr size_t operator"" _h(wchar_t const* txt, size_t) {
            return geode::utils::hash(txt);
        }

        template <typename T>
        constexpr const T& clamp(const T& value, const T& minValue, const T& maxValue) {
            return value < minValue ? minValue : maxValue < value ? maxValue : value;
        }

        // from https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
        template <typename... Args>
        std::string strfmt(std::string const& format, Args... args) {
            int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
            if (size_s <= 0) {
                throw std::runtime_error("Error during formatting.");
            }
            auto size = static_cast<size_t>(size_s);
            auto buf = std::make_unique<char[]>(size);
            std::snprintf(buf.get(), size, format.c_str(), args...);
            return std::string(buf.get(), buf.get() + size - 1);
        }

        /**
         * Format a string
         * @returns Pointer to char array. MAKE SURE TO CALL DELETE[]!
         */
        template <typename... Args>
        char const* cstrfmt(char const* fmt, Args... args) {
            auto str = strfmt(fmt, args...);
            char* ptr = new char[str.size() + 1];
            strcpy_s(ptr, str.c_str());
            return ptr;
        }

        template <typename T>
        std::string intToHex(T i) {
            std::stringstream stream;
            stream << std::showbase << std::setbase(16) << (uint64_t)i;
            return stream.str();
        }

        /**
         * Turn a number into a string, with support for specifying precision
         * (unlike std::to_string).
         * @param num Number to convert to string
         * @param precision Precision of the converted number
         * @returns Number as string
         */
        template <class Num>
        std::string numToString(Num num, size_t precision = 0) {
            std::stringstream ss;
            if (precision) {
                ss << std::fixed << std::setprecision(precision);
            }
            ss << num;
            return ss.str();
        }

        GEODE_DLL std::string timePointAsString(std::chrono::system_clock::time_point const& tp);
    }
}

namespace geode::utils::clipboard {
    GEODE_DLL bool write(std::string const& data);
    GEODE_DLL std::string read();
}
