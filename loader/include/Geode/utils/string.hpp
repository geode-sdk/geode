#pragma once

#include <string_view>
#include <string>
#include <vector>
#include <compare>

namespace geode::utils::string {
    /**
     * Convert std::wstring to std::string (UTF-8)
     * @param str String to convert
     * @returns std::string
     */
    GEODE_DLL std::string wideToUtf8(std::wstring const& str);
    /**
     * Convert std::string (UTF-8) to std::wstring
     * @param str String to convert
     * @returns std::wstring
     */
    GEODE_DLL std::wstring utf8ToWide(std::string const& str);

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
    GEODE_DLL std::string& trimRightIP(std::string& str);
    GEODE_DLL std::string& trimIP(std::string& str);

    GEODE_DLL std::string trimLeft(std::string const& str);
    GEODE_DLL std::string trimRight(std::string const& str);
    GEODE_DLL std::string trim(std::string const& str);

    GEODE_DLL std::string& normalizeIP(std::string& str);
    GEODE_DLL std::string normalize(std::string const& str);

    GEODE_DLL bool startsWith(std::string const& str, std::string const& prefix);
    GEODE_DLL bool endsWith(std::string const& str, std::string const& suffix);

    /**
     * Similar to strcmp, but case insensitive.
     * Uses std::tolower, but could change in the future for better locale support
     */
    GEODE_DLL std::strong_ordering caseInsensitiveCompare(std::string_view a, std::string_view b);
}
