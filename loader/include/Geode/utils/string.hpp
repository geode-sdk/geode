#pragma once

#include <string_view>
#include <string>
#include <array>
#include <vector>
#include <compare>
#include <filesystem>
#include "../DefaultInclude.hpp"
#include <Geode/Result.hpp>

namespace geode::utils::string {
#ifdef GEODE_IS_WINDOWS
    /**
     * Convert std::wstring to std::string (UTF-8)
     * @param str String to convert
     * @returns std::string
     * @note The purpose of this method is only for wide winapi methods,
     * as the conversion may be lossy.
     */
    GEODE_DLL std::string wideToUtf8(std::wstring const& str);
    /**
     * Convert std::string (UTF-8) to std::wstring
     * @param str String to convert
     * @returns std::wstring
     * @note The purpose of this method is only for wide winapi methods,
     * as the conversion may be lossy.
     */
    GEODE_DLL std::wstring utf8ToWide(std::string const& str);
#endif

    /// Convert UTF-16 string to UTF-8
    /// @param str UTF-16 string view
    /// @return UTF-8 encoded std::string, or an error
    GEODE_DLL Result<std::string> utf16ToUtf8(std::u16string_view str);

    /// Convert UTF-8 string to UTF-16
    /// @param str UTF-8 string view
    /// @return UTF-16 encoded std::u16string, or an error
    GEODE_DLL Result<std::u16string> utf8ToUtf16(std::string_view str);

    /// Convert UTF-32 string to UTF-8
    /// @param str UTF-32 string view
    /// @return UTF-8 encoded std::string, or an error
    GEODE_DLL Result<std::string> utf32ToUtf8(std::u32string_view str);

    /// Convert UTF-8 string to UTF-32
    /// @param str UTF-8 string view
    /// @return UTF-32 encoded std::u32string, or an error
    GEODE_DLL Result<std::u32string> utf8ToUtf32(std::string_view str);

    /// Convert path to UTF-8 string
    /// @param path filesystem path
    /// @return UTF-8 encoded std::string
    GEODE_DLL std::string pathToString(std::filesystem::path const& path);

    GEODE_DLL std::string& toLowerIP(std::string& str);

    GEODE_DLL std::string toLower(std::string const& str);

    GEODE_DLL std::string& toUpperIP(std::string& str);

    GEODE_DLL std::string toUpper(std::string const& str);

    GEODE_DLL std::string& replaceIP(
        std::string& str, std::string const& orig, std::string const& repl
    );

    GEODE_DLL std::string replace(
        std::string const& str, std::string const& orig, std::string const& repl
    );

    GEODE_DLL std::vector<std::string> split(std::string const& str, std::string const& split);

    GEODE_DLL std::string join(std::vector<std::string> const& strs, std::string const& separator);

    GEODE_DLL std::vector<char> split(std::string const& str);

    GEODE_DLL bool contains(std::string const& str, std::string const& subs);

    GEODE_DLL bool contains(std::string const& str, char c);

    GEODE_DLL bool containsAny(std::string const& str, std::vector<std::string> const& subs);

    GEODE_DLL bool containsAll(std::string const& str, std::vector<std::string> const& subs);

    GEODE_DLL size_t count(std::string const& str, char c);

    GEODE_DLL std::string& trimLeftIP(std::string& str);
    GEODE_DLL std::string& trimLeftIP(std::string& str, std::string const& chars);
    GEODE_DLL std::string& trimRightIP(std::string& str);
    GEODE_DLL std::string& trimRightIP(std::string& str, std::string const& chars);
    GEODE_DLL std::string& trimIP(std::string& str);
    GEODE_DLL std::string& trimIP(std::string& str, std::string const& chars);

    GEODE_DLL std::string trimLeft(std::string const& str);
    GEODE_DLL std::string trimLeft(std::string const& str, std::string const& chars);
    GEODE_DLL std::string trimRight(std::string const& str);
    GEODE_DLL std::string trimRight(std::string const& str, std::string const& chars);
    GEODE_DLL std::string trim(std::string const& str);
    GEODE_DLL std::string trim(std::string const& str, std::string const& chars);

    GEODE_DLL std::string& normalizeIP(std::string& str);
    GEODE_DLL std::string normalize(std::string const& str);

    GEODE_DLL bool startsWith(std::string const& str, std::string const& prefix);
    GEODE_DLL bool endsWith(std::string const& str, std::string const& suffix);

    /**
     * Similar to strcmp, but case insensitive.
     * Uses std::tolower, but could change in the future for better locale support
     */
    GEODE_DLL std::strong_ordering caseInsensitiveCompare(std::string_view a, std::string_view b);

    struct ConstexprString {
        std::array<char, 1024> m_buffer{};
        std::size_t m_size = 0;
        constexpr ConstexprString() {
            m_buffer[0] = 0;
        }

        constexpr char* begin() {
            return m_buffer.data();
        }
        constexpr char const* begin() const {
            return m_buffer.data();
        }
        constexpr char* end() {
            return m_buffer.data() + m_size;
        }
        constexpr char const* end() const {
            return m_buffer.data() + m_size;
        }
        constexpr char* data() {
            return m_buffer.data();
        }
        constexpr char const* data() const {
            return m_buffer.data();
        }
        constexpr std::size_t size() const {
            return m_size;
        }

        constexpr void push(char x) {
            m_buffer[m_size++] = x;
            m_buffer[m_size] = 0;
        }

        constexpr void push(std::string_view x) {
            std::copy(x.begin(), x.end(), end());
            m_size += x.size();
            m_buffer[m_size] = 0;
        }
        constexpr void push(ConstexprString const& x) {
            std::copy(x.begin(), x.end(), end());
            m_size += x.size();
            m_buffer[m_size] = 0;
        }
        template <std::integral Int, size_t N>
        constexpr void push(std::array<Int, N> const& arr) {
            std::copy(arr.begin(), arr.end(), end());
            m_size += N;
            m_buffer[m_size] = 0;
        }
        template <std::integral Int>
        constexpr void push(std::initializer_list<Int> const& arr) {
            std::copy(arr.begin(), arr.end(), end());
            m_size += arr.size();
            m_buffer[m_size] = 0;
        }
        template <std::integral Int>
        constexpr void push(Int value, size_t base) {
            if (value < 0) {
                push('-');
                value = -value;
            }
            constexpr auto digits = "0123456789abcdef";

            std::array<char, 64> buffer;
            auto index = buffer.size();
            do {
                const auto digit = value % base;
                value /= base;
                buffer[--index] = digits[digit];
            } while (value != 0);

            std::copy(buffer.begin() + index, buffer.end(), end());
            m_size += buffer.size() - index;
            m_buffer[m_size] = 0;
        }

        static consteval auto toWideArray(std::invocable auto callable) {
            auto str = callable();
            struct BigArray {
                std::array<char, 1024> data{};
                std::size_t size = 0;
            };
            BigArray arr;
            std::copy(str.m_buffer.begin(), str.m_buffer.end(), arr.data.begin());
            arr.size = str.m_size;
            return arr;
        }

        static consteval auto toFitArray(std::invocable auto callable) {
            constexpr auto arr = ConstexprString::toWideArray(callable);
            std::array<char, arr.size + 1> fit;
            std::copy(arr.data.begin(), arr.data.begin() + arr.size, fit.begin());
            fit[arr.size] = 0;
            return fit;
        }

        template <auto data>
        static consteval const auto& makeStatic() {
            return data;
        }

        static consteval const char* toLiteral(std::invocable auto callable) {
        constexpr auto& staticData = makeStatic<toFitArray(callable)>();
            return staticData.data();
        }
    };
}
