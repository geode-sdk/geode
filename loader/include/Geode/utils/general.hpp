#pragma once

#include "Result.hpp"

#include "../DefaultInclude.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <matjson.hpp>
#include <charconv>
#include <clocale>
#include <type_traits>

namespace geode {
    using ByteVector = std::vector<uint8_t>;

    // todo in v4: remove this
    template <typename T>
    [[deprecated("Use geode::toBytes instead")]]
    ByteVector toByteArray(T const& a) {
        ByteVector out;
        out.resize(sizeof(T));
        std::memcpy(out.data(), &a, sizeof(T));
        return out;
    }
    template <typename T>
    ByteVector toBytes(T const& a) {
        ByteVector out;
        out.resize(sizeof(T));
        std::memcpy(out.data(), &a, sizeof(T));
        return out;
    }

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
        constexpr unsigned int hash(std::string_view str, int h = 0) {
            return h >= str.size() ? 5381 : (hash(str, h + 1) * 33) ^ str[h];
        }
        constexpr unsigned int hash(wchar_t const* str, int h = 0) {
            return !str[h] ? 5381 : (hash(str, h + 1) * 33) ^ str[h];
        }
        constexpr unsigned int hash(std::wstring_view str, int h = 0) {
            return h >= str.size() ? 5381 : (hash(str, h + 1) * 33) ^ str[h];
        }

        constexpr size_t operator"" _h(char const* txt, size_t) {
            return geode::utils::hash(txt);
        }
        constexpr size_t operator"" _h(wchar_t const* txt, size_t) {
            return geode::utils::hash(txt);
        }

        template <typename T>
        constexpr const T& clamp(const T& value, const std::type_identity_t<T>& minValue, const std::type_identity_t<T>& maxValue) {
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
         * Turn a number into an abbreviated string, like `1253` to `1.25K`
         */
        template <std::integral Num>
        std::string numToAbbreviatedString(Num num) {
            if (num >= 1'000'000'000) return fmt::format("{:0.3}B", num / 1'000'000'000.f);
            if (num >= 1'000'000) return fmt::format("{:0.3}M", num / 1'000'000.f);
            if (num >= 1'000) return fmt::format("{:0.3}K", num / 1'000.f);
            return numToString(num);
        }

        /**
         * Parse a number from a string
         * @param str The string to parse
         * @param base The base to use
         * @returns String as number, or Err if the string couldn't be converted
         */
        template <class Num>
        Result<Num> numFromString(std::string_view const str, int base = 10) {
            if constexpr (std::is_floating_point_v<Num> 
                #if defined(__cpp_lib_to_chars)
                    && false
                #endif
            ) {
                Num val;
                char* strEnd;
                errno = 0;
                if (std::setlocale(LC_NUMERIC, "C")) {
                    if constexpr (std::is_same_v<Num, float>) val = std::strtof(str.data(), &strEnd);
                    else if constexpr (std::is_same_v<Num, double>) val = std::strtod(str.data(), &strEnd);
                    else if constexpr (std::is_same_v<Num, long double>) val = std::strtold(str.data(), &strEnd);
                    if (errno == ERANGE) return Err("Number is too large to fit");
                    else if (strEnd == str.data()) return Err("String is not a number");
                    else return Ok(val);
                }
                else return Err("Failed to set locale");
            }
            else {
                Num result;
                std::from_chars_result res;
                if constexpr (std::is_floating_point_v<Num>) res = std::from_chars(str.data(), str.data() + str.size(), result);
                else res = std::from_chars(str.data(), str.data() + str.size(), result, base);

                auto [ptr, ec] = res;
                if (ec == std::errc()) return Ok(result);
                else if (ptr != str.data() + str.size()) return Err("String contains trailing extra data");
                else if (ec == std::errc::invalid_argument) return Err("String is not a number");
                else if (ec == std::errc::result_out_of_range) return Err("Number is too large to fit");
                else return Err("Unknown error");
            }
        }

        GEODE_DLL std::string timePointAsString(std::chrono::system_clock::time_point const& tp);

        /**
         * Gets the display pixel factor for the current screen,
         * i.e. the ratio between physical pixels and logical pixels on one axis.
         * On most platforms this is 1.0, but on retina displays for example this returns 2.0.
        */
        GEODE_DLL float getDisplayFactor();
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
