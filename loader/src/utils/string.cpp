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

std::wstring utils::string::utf8ToWide(std::string const& str) {
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
    return utils::string::wideToUtf8(path.wstring());
#else
    return path.string();
#endif
}


bool utils::string::startsWith(std::string const& str, std::string const& prefix) {
    return str.rfind(prefix, 0) == 0;
}

bool utils::string::endsWith(std::string const& str, std::string const& suffix) {
    if (suffix.size() > str.size()) return false;
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

std::string& utils::string::toLowerIP(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](auto c) {
        return std::tolower(c);
    });
    return str;
}

std::string utils::string::toLower(std::string const& str) {
    std::string ret = str;
    return utils::string::toLowerIP(ret);
}

std::string& utils::string::toUpperIP(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](auto c) {
        return std::toupper(c);
    });
    return str;
}

std::string utils::string::toUpper(std::string const& str) {
    std::string ret = str;
    return utils::string::toUpperIP(ret);
}

std::string& utils::string::replaceIP(std::string& str, std::string const& orig, std::string const& repl) {
    if (orig.empty()) return str;

    std::string::size_type n = 0;
    while ((n = str.find(orig, n)) != std::string::npos) {
        str.replace(n, orig.size(), repl);
        n += repl.size();
    }
    return str;
}

std::string utils::string::replace(
    std::string const& str, std::string const& orig, std::string const& repl
) {
    auto ret = str;
    return utils::string::replaceIP(ret, orig, repl);
}

std::vector<std::string> utils::string::split(std::string const& str, std::string const& split) {
    std::vector<std::string> res;
    if (str.empty()) return res;
    auto s = str;
    size_t pos;
    while ((pos = s.find(split)) != std::string::npos) {
        res.push_back(s.substr(0, pos));
        s.erase(0, pos + split.length());
    }
    res.push_back(s);
    return res;
}

std::string utils::string::join(std::vector<std::string> const& strs, std::string const& separator) {
    std::string res;
    if (strs.empty())
        return res;
    if (strs.size() == 1)
        return strs[0];
    // idk if less allocations but an extra loop is faster but
    size_t size = 0;
    for (auto const& str : strs)
        size += str.size() + separator.size();
    res.reserve(size);
    for (auto const& str : strs)
        res += str + separator;
    res.erase(res.size() - separator.size());
    return res;
}

std::vector<char> utils::string::split(std::string const& str) {
    std::vector<char> res;
    for (auto const& s : str) {
        res.push_back(s);
    }
    return res;
}

bool utils::string::contains(std::string const& str, std::string const& subs) {
    return str.find(subs) != std::string::npos;
}

bool utils::string::contains(std::string const& str, std::string::value_type c) {
    return str.find(c) != std::string::npos;
}

bool utils::string::containsAny(std::string const& str, std::vector<std::string> const& subs) {
    for (auto const& sub : subs) {
        if (utils::string::contains(str, sub)) return true;
    }
    return false;
}

bool utils::string::containsAll(std::string const& str, std::vector<std::string> const& subs) {
    bool found = true;
    for (auto const& sub : subs) {
        if (!utils::string::contains(str, sub)) found = false;
    }
    return found;
}

size_t utils::string::count(std::string const& str, char countC) {
    size_t res = 0;
    for (auto c : str)
        if (c == countC) res++;
    return res;
}

constexpr char WHITESPACE[] = " \f\n\r\t\v";
std::string& utils::string::trimLeftIP(std::string& str, std::string const& chars) {
    str.erase(0, str.find_first_not_of(chars));
    return str;
}
std::string& utils::string::trimLeftIP(std::string& str) {
    return utils::string::trimLeftIP(str, WHITESPACE);
}

std::string& utils::string::trimRightIP(std::string& str, std::string const& chars) {
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}
std::string& utils::string::trimRightIP(std::string& str) {
    return utils::string::trimRightIP(str, WHITESPACE);
}

std::string& utils::string::trimIP(std::string& str, std::string const& chars) {
    return utils::string::trimLeftIP(utils::string::trimRightIP(str, chars), chars);
}
std::string& utils::string::trimIP(std::string& str) {
    return utils::string::trimLeftIP(utils::string::trimRightIP(str));
}

std::string utils::string::trimLeft(std::string const& str, std::string const& chars) {
    size_t start = str.find_first_not_of(chars);
    return start == -1 ? std::string() : str.substr(start);
}
std::string utils::string::trimLeft(std::string const& str) {
    return utils::string::trimLeft(str, WHITESPACE);
}

std::string utils::string::trimRight(std::string const& str, std::string const& chars) {
    return str.substr(0, str.find_last_not_of(chars) + 1);
}
std::string utils::string::trimRight(std::string const& str) {
    return utils::string::trimRight(str, WHITESPACE);
}

std::string utils::string::trim(std::string const& str, std::string const& chars) {
    size_t start = str.find_first_not_of(chars);
    return start == -1 ? std::string() : str.substr(start, str.find_last_not_of(chars) + 1 - start);
}
std::string utils::string::trim(std::string const& str) {
    return utils::string::trim(str, WHITESPACE);
}

std::string& utils::string::normalizeIP(std::string& str) {
    while (utils::string::contains(str, "  "))
        utils::string::replaceIP(str, "  ", " ");
    return str;
}

std::string utils::string::normalize(std::string const& str) {
    auto ret = str;
    return utils::string::normalizeIP(ret);
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
