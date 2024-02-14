#pragma once

#include "Result.hpp"

#include "../DefaultInclude.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <ghc/fs_fwd.hpp>
#include <matjson.hpp>
#include <charconv>

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
        constexpr const T& clamp(const T& value, const TypeIdentityType<T>& minValue, const TypeIdentityType<T>& maxValue) {
            return value < minValue ? minValue : maxValue < value ? maxValue : value;
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

        /**
         * Parse a number from a string
         * @param str The string to parse
         * @param base The base to use
         * @returns String as number, or Err if the string couldn't be converted
         */
        template <class Num>
        Result<Num> numFromString(std::string_view const str, int base = 10) {
            Num result;
            auto [_, ec] = std::from_chars(str.data(), str.data() + str.size(), result, base);
            switch (ec) {
                case std::errc(): return Ok(result);
                case std::errc::invalid_argument: return Err("String is not a number");
                case std::errc::result_out_of_range: return Err("Number is too large to fit");
                default: return Err("Unknown error");
            }
        }

        GEODE_DLL std::string timePointAsString(std::chrono::system_clock::time_point const& tp);
    }
}

template<>
struct matjson::Serialize<geode::ByteVector> {
    static matjson::Value to_json(geode::ByteVector const& bytes) {
        return matjson::Array(bytes.begin(), bytes.end());
    }
};

namespace geode::utils::clipboard {
    GEODE_DLL bool write(std::string const& data);
    GEODE_DLL std::string read();
}

namespace geode::utils::game {
    GEODE_DLL void exit();
    GEODE_DLL void restart();
    GEODE_DLL void launchLoaderUninstaller(bool deleteSaveData);
}

namespace geode::utils::thread {
    GEODE_DLL std::string getName();
    GEODE_DLL std::string getDefaultName();
    GEODE_DLL void setName(std::string const& name);
}
