#include <Geode/utils/string.hpp>
#include <algorithm>

using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS

#include <Windows.h>
#include <cwctype>
#include <stringapiset.h>

std::string utils::string::wideToUtf8(std::wstring const& wstr) {
    int count = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.length(), NULL, 0, NULL, NULL);
    std::string str(count, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
    return str;
}

std::wstring utils::string::utf8ToWide(ZStringView str) {
    int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
    std::wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);
    return wstr;
}

#endif

#include <simdutf.h>

Result<std::string> utils::string::utf16ToUtf8(std::u16string_view str) {
    std::string result;
    result.resize(simdutf::utf8_length_from_utf16(str));
    if (simdutf::convert_utf16_to_utf8(str.data(), str.size(), result.data()) == 0) {
        return Err("Invalid UTF-16 input");
    }
    return Ok(std::move(result));
}

Result<std::u16string> utils::string::utf8ToUtf16(std::string_view str) {
    std::u16string result;
    result.resize(simdutf::utf16_length_from_utf8(str));
    if (simdutf::convert_utf8_to_utf16(str.data(), str.size(), result.data()) == 0) {
        return Err("Invalid UTF-8 input");
    }
    return Ok(std::move(result));
}

Result<std::string> utils::string::utf32ToUtf8(std::u32string_view str) {
    std::string result;
    result.resize(simdutf::utf8_length_from_utf32(str));
    if (simdutf::convert_utf32_to_utf8(str.data(), str.size(), result.data()) == 0) {
        return Err("Invalid UTF-32 input");
    }
    return Ok(std::move(result));
}

Result<std::u32string> utils::string::utf8ToUtf32(std::string_view str) {
    std::u32string result;
    result.resize(simdutf::utf32_length_from_utf8(str));
    if (simdutf::convert_utf8_to_utf32(str.data(), str.size(), result.data()) == 0) {
        return Err("Invalid UTF-8 input");
    }
    return Ok(std::move(result));
}

std::string utils::string::pathToString(std::filesystem::path const& path) {
#ifdef GEODE_IS_WINDOWS
    return utils::string::wideToUtf8(path.native());
#else
    return path.string();
#endif
}


bool utils::string::startsWith(std::string_view str, std::string_view prefix) {
    return str.rfind(prefix, 0) == 0;
}

bool utils::string::endsWith(std::string_view str, std::string_view suffix) {
    if (suffix.size() > str.size()) return false;
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

void utils::string::toLowerIP(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](auto c) {
        return std::tolower(c);
    });
}

std::string utils::string::toLower(std::string str) {
    utils::string::toLowerIP(str);
    return str;
}

void utils::string::toUpperIP(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](auto c) {
        return std::toupper(c);
    });
}

std::string utils::string::toUpper(std::string str) {
    utils::string::toUpperIP(str);
    return str;
}

void utils::string::replaceIP(std::string& str, std::string_view orig, std::string_view repl) {
    if (orig.empty()) return;

    std::string::size_type n = 0;
    while ((n = str.find(orig, n)) != std::string::npos) {
        str.replace(n, orig.size(), repl);
        n += repl.size();
    }
}

std::string utils::string::replace(
    std::string str, std::string_view orig, std::string_view repl
) {
    utils::string::replaceIP(str, orig, repl);
    return str;
}

template <typename T>
auto doSplit(std::string_view str, std::string_view split) {
    std::vector<T> res;
    if (str.empty()) return res;

    size_t pos = 0;
    while ((pos = str.find(split)) != std::string::npos) {
        res.push_back(T{ str.substr(0, pos) });
        str.remove_prefix(pos + split.length());
    }
    res.push_back(T{ str });
    return res;
}

std::vector<std::string> utils::string::split(std::string_view str, std::string_view split) {
    return doSplit<std::string>(str, split);
}

std::vector<std::string_view> utils::string::splitView(std::string_view str, std::string_view split) {
    return doSplit<std::string_view>(str, split);
}

template <typename T>
std::string doJoin(std::span<T const> strs, std::string_view separator) {
    if (strs.empty()) return "";
    if (strs.size() == 1) return std::string(strs[0]);

    std::string res;

    // idk if less allocations but an extra loop is faster but
    size_t size = 0;
    for (auto const& str : strs)
        size += str.size() + separator.size();
    res.reserve(size);

    res.append(strs[0]);
    for (size_t i = 1; i < strs.size(); i++) {
        res.append(separator);
        res.append(strs[i]);
    }

    return res;
}

std::string utils::string::join(std::span<std::string const> strs, std::string_view separator) {
    return doJoin<std::string>(strs, separator);
}

std::string utils::string::join(std::span<std::string_view const> strs, std::string_view separator) {
    return doJoin<std::string_view>(strs, separator);
}

std::vector<char> utils::string::split(std::string_view str) {
    return std::vector<char>(str.begin(), str.end());
}

bool utils::string::contains(std::string_view str, std::string_view subs) {
    return str.find(subs) != std::string::npos;
}

bool utils::string::contains(std::string_view str, std::string::value_type c) {
    return str.find(c) != std::string::npos;
}

bool utils::string::containsAny(std::string_view str, std::span<std::string> subs) {
    for (auto const& sub : subs) {
        if (utils::string::contains(str, sub)) return true;
    }
    return false;
}

bool utils::string::containsAll(std::string_view str, std::span<std::string> subs) {
    bool found = true;
    for (auto const& sub : subs) {
        if (!utils::string::contains(str, sub)) found = false;
    }
    return found;
}

size_t utils::string::count(std::string_view str, char countC) {
    size_t res = 0;
    for (auto c : str)
        if (c == countC) res++;
    return res;
}

constexpr char WHITESPACE[] = " \f\n\r\t\v";
void utils::string::trimLeftIP(std::string& str, std::string_view chars) {
    str.erase(0, str.find_first_not_of(chars));
}

void utils::string::trimLeftIP(std::string& str) {
    utils::string::trimLeftIP(str, WHITESPACE);
}

void utils::string::trimRightIP(std::string& str, std::string_view chars) {
    // rely on npos + 1 = 0, clearing string if all chars are in the filter
    str.erase(str.find_last_not_of(chars) + 1);
}

void utils::string::trimRightIP(std::string& str) {
    utils::string::trimRightIP(str, WHITESPACE);
}

void utils::string::trimIP(std::string& str, std::string_view chars) {
    utils::string::trimRightIP(str, chars);
    utils::string::trimLeftIP(str, chars);
}

void utils::string::trimIP(std::string& str) {
    utils::string::trimRightIP(str);
    utils::string::trimLeftIP(str);
}

std::string utils::string::trimLeft(std::string str, std::string_view chars) {
    size_t start = str.find_first_not_of(chars);
    if (start != std::string::npos) {
        str.erase(0, start);
    }
    return str;
}

std::string utils::string::trimLeft(std::string str) {
    return utils::string::trimLeft(std::move(str), WHITESPACE);
}

std::string utils::string::trimRight(std::string str, std::string_view chars) {
    size_t end = str.find_last_not_of(chars);
    if (end != std::string::npos) {
        str.erase(end + 1);
    } else {
        str.clear();
    }
    return str;
}

std::string utils::string::trimRight(std::string str) {
    return utils::string::trimRight(std::move(str), WHITESPACE);
}

std::string utils::string::trim(std::string str, std::string_view chars) {
    str = utils::string::trimRight(std::move(str), chars);
    return utils::string::trimLeft(std::move(str), chars);
}

std::string utils::string::trim(std::string str) {
    return trim(std::move(str), WHITESPACE);
}

void utils::string::normalizeIP(std::string& str) {
    while (utils::string::contains(str, "  "))
        utils::string::replaceIP(str, "  ", " ");
}

std::string utils::string::normalize(std::string str) {
    utils::string::normalizeIP(str);
    return str;
}

std::strong_ordering utils::string::caseInsensitiveCompare(std::string_view str1, std::string_view str2) {
    for (size_t i = 0; i < str1.size() && i < str2.size(); i++) {
        auto const a = std::tolower(str1[i]);
        auto const b = std::tolower(str2[i]);
        if (a < b) {
            return std::strong_ordering::less;
        } else if (a > b) {
            return std::strong_ordering::greater;
        }
    }
    if (str1.size() < str2.size())
        return std::strong_ordering::less;
    else if (str1.size() > str2.size())
        return std::strong_ordering::greater;
    return std::strong_ordering::equal;
}
