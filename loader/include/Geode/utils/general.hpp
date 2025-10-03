#pragma once

#include <Geode/Result.hpp>

#include "../DefaultInclude.hpp"
#include <chrono>
#include <iomanip>
#include <string>
#include <vector>
#include <filesystem>
#include <matjson.hpp>
#include <charconv>
#include <clocale>
#include <type_traits>
#include <fmt/format.h>
#include <cocos2d.h>

namespace geode {
    using ByteVector = std::vector<uint8_t>;

    template <typename T>
    ByteVector toBytes(T const& a) {
        ByteVector out;
        out.resize(sizeof(T));
        std::memcpy(out.data(), &a, sizeof(T));
        return out;
    }

    namespace utils {
        /**
         * A helper struct for std::visit.
         *
         * @example
         * return std::visit(utils::makeVisitor {
         *     [](float value) {
         *         return "float";
         *     },
         *     [](int value) {
         *         return "int";
         *     },
         *     [](std::string const& value) {
         *         return "string";
         *     },
         * }, stored);
         */
        template<class... Ts> struct makeVisitor : Ts... { using Ts::operator()...; };
        template<class... Ts> makeVisitor(Ts...) -> makeVisitor<Ts...>;

        /**
         * A helper function to get a default value if a variant fails.
         *
         * @param variant The variant
         * @param defValue The fallback value
         */
        template<class T, class ... Args>
        constexpr T getOr(std::variant<Args...> const& variant, T const& defValue) {
            return std::holds_alternative<T>(variant) ?
                std::get<T>(variant) : defValue;
        }

        /**
         * A simple constexpr hash of `char const*` for switch cases and other
         * constexpr uses.
         *
         * @param str The string
         * @returns The computed hash
         */
        constexpr unsigned int hash(char const* str, int h = 0) {
            return !str[h] ? 5381 : (hash(str, h + 1) * 33) ^ str[h];
        }

        /**
         * A simple constexpr hash of `std::string_view` for switch cases and other
         * constexpr uses.
         *
         * @param str The string
         * @returns The computed hash
         */
        constexpr unsigned int hash(std::string_view str, int h = 0) {
            return h >= str.size() ? 5381 : (hash(str, h + 1) * 33) ^ str[h];
        }
        /**
         * A simple constexpr hash of `wchar_t const*` for switch cases and other
         * constexpr uses.
         *
         * @param str The string
         * @returns The computed hash
         */
        constexpr unsigned int hash(wchar_t const* str, int h = 0) {
            return !str[h] ? 5381 : (hash(str, h + 1) * 33) ^ str[h];
        }

        /**
         * A simple constexpr hash of `std::wstring_view` for switch cases and other
         * constexpr uses.
         *
         * @param str The string
         * @returns The computed hash
         */
        constexpr unsigned int hash(std::wstring_view str, int h = 0) {
            return h >= str.size() ? 5381 : (hash(str, h + 1) * 33) ^ str[h];
        }

        constexpr size_t operator""_h(char const* txt, size_t) {
            return geode::utils::hash(txt);
        }
        constexpr size_t operator""_h(wchar_t const* txt, size_t) {
            return geode::utils::hash(txt);
        }

        /**
         * A simple function that clamps a value between two others.
         * @deprecated Use std::clamp instead
         *
         * @param value Value
         * @param minValue The minimum value
         * @param maxValue The maximum value
         * @returns The clamped value
         */
        template <typename T>
        [[deprecated]] constexpr const T& clamp(const T& value, const std::type_identity_t<T>& minValue, const std::type_identity_t<T>& maxValue) {
            return value < minValue ? minValue : maxValue < value ? maxValue : value;
        }

        /**
         * A simple function that converts an integer into a hexadecimal string
         * @deprecated Use `fmt::format("{:#x}", value)` instead
         *
         * @param i The integer
         * @returns The hex string
         */
        template <typename T>
        [[deprecated]] std::string intToHex(T i) {
            return fmt::format("{:#x}", i);
        }

        /**
         * Turn a number into a string, with support for specifying precision
         * (unlike std::to_string).
         * @param num Number to convert to string
         * @param precision Precision of the converted number
         * @returns Number as string
         * @note Precision has no effect on integers
         */
        template <class Num>
        std::string numToString(Num num, size_t precision = 0) {
            if constexpr (std::is_floating_point_v<Num>) {
                if (precision) {
                    return fmt::format("{:.{}f}", num, precision);
                }
            }
            return fmt::to_string(num);
        }

        /**
         * Turn a number into an abbreviated string, like `1253` to `1.25K`
         * @param num Number to convert to string
         * @returns Number as string
         */
        template <std::integral Num>
        std::string numToAbbreviatedString(Num num) {
            // it's a mess... i'm sorry...
            constexpr auto numToFixedTrunc = [](float num) {

                // calculate the number of digits we keep from the decimal
                auto remaining = std::max(3 - static_cast<int>(std::log10(num)) - 1, 0);

                auto factor = std::pow(10, remaining);
                auto trunc = std::trunc(num * factor) / factor;

                // doing this dynamic format thing lets the .0 show when needed
                return fmt::format("{:0.{}f}", trunc, static_cast<int>(remaining));
            };

            if (num >= 1'000'000'000) return fmt::format("{}B", numToFixedTrunc(num / 1'000'000'000.f));
            if (num >= 1'000'000) return fmt::format("{}M", numToFixedTrunc(num / 1'000'000.f));
            if (num >= 1'000) return fmt::format("{}K", numToFixedTrunc(num / 1'000.f));
            return numToString(num);
        }

        /**
         * Parse a number from a string
         * @param str The string to parse
         * @param base The base to use
         * @returns String as number, or Err if the string couldn't be converted
         */
        template <class Num>
        Result<Num> numFromString(std::string_view str, int base = 10) {
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

        /**
         * Converts the given time point into a string using `ctime`.
         *
         * @param tp The timepoint
         * @returns The converted string
         */
        GEODE_DLL std::string timePointAsString(std::chrono::system_clock::time_point const& tp);

        /**
         * Gets the display pixel factor for the current screen,
         * i.e. the ratio between physical pixels and logical pixels on one axis.
         * On most platforms this is 1.0, but on retina displays for example this returns 2.0.
        */
        GEODE_DLL float getDisplayFactor();

        /**
         * Gets an environment variable from the device.
         *
         * @param name The key of the variable
         * @returns The value of the variable
         */
        GEODE_DLL std::string getEnvironmentVariable(const char* name);

        /**
         * Formats an error code (from `GetLastError()` or `errno`) to a user readable string,
         * using `FormatMessageA` on Windows and `strerror` on other platforms.
         */
        GEODE_DLL std::string formatSystemError(int code);

        /**
         * Returns the safe area rectangle, or the area that is not covered by device elements (such as a display cutout or home bar)
         * This is relative to the winSize.
         */
        GEODE_DLL cocos2d::CCRect getSafeAreaRect();
    }

    /**
     * A convenience function that creates a formatted `Err<std::string>`.
     *
     * @param fmt The format string
     * @param args The format args
     * @returns An error `Result`
     */
    template <class... Args>
    requires (sizeof...(Args) > 0)
    constexpr auto Err(fmt::format_string<Args...> fmt, Args&&... args) {
        return Err(fmt::format(fmt, std::forward<Args>(args)...));
    }
}

template<>
struct matjson::Serialize<geode::ByteVector> {
    static Value toJson(geode::ByteVector const& bytes) {
        return std::vector<matjson::Value>(bytes.begin(), bytes.end());
    }

    static geode::Result<geode::ByteVector, std::string> fromJson(Value const& value) {
        if (auto arr = value.as<std::vector<matjson::Value>>()) {
            geode::ByteVector bytes;
            for (auto const& item : *arr) {
                if (auto byte = item.as<uint8_t>()) {
                    bytes.push_back(*byte);
                }
            }
            return geode::Ok(bytes);
        }
        return geode::Err("Expected a JSON array of numbers");
    }
};

namespace geode::utils::clipboard {
    /**
     * Writes the given data into the clipboard as a string.
     *
     * @param data The data to write
     * @returns True if the operation was successful
     */
    GEODE_DLL bool write(std::string const& data);

    /**
     * Reads the clipboards onto a string.
     *
     * @returns The clipboard data if exists, an empty string on error.
     */
    GEODE_DLL std::string read();
}

namespace geode::utils::game {
    /**
     * Exits the game, saving the game data.
     *
     * @deprecated Use `game::exit(true)` instead
     */
    [[deprecated]] GEODE_DLL void exit(); // TODO: left for abi compat

    /**
     * Exits the game, optionally saving the game data.
     *
     * @param saveData Whether to save the game data
     */
    GEODE_DLL void exit(bool saveData /* = true */);

    /**
     * Restarts the game, saving the game data.
     *
     * @deprecated Use `game::restart(true)` instead
     */
    [[deprecated]] GEODE_DLL void restart(); // TODO: left for abi compat

    /**
     * Restarts the game, optionally saving the game data.
     *
     * @param saveData Whether to save the game data
     */
    GEODE_DLL void restart(bool saveData /* = true */);

    /**
     * Lauched the loader uninstaller, optionally deleting saved data.
     *
     * @param deleteSaveData  Whether to delete the saved game data
     */
    GEODE_DLL void launchLoaderUninstaller(bool deleteSaveData);
}

namespace geode::utils::thread {
    /**
     * Gets the assigned name to a thread.
     *
     * @returns The thread name if exists, an empty string if not.
     */
    GEODE_DLL std::string getName();

    /**
     * Gets the default name to a thread.
     *
     * @returns The default thread name.
     */
    GEODE_DLL std::string getDefaultName();

    /**
     * Sets an assigned name to a thread.
     *
     * @param name The thread name to assign
     */
    GEODE_DLL void setName(std::string const& name);
}
