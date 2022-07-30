#include <Geode/utils/ext.hpp>
#include <algorithm>

USE_GEODE_NAMESPACE();

#ifdef GEODE_IS_WINDOWS

#include <cwctype>
#include <Windows.h>
#include <stringapiset.h>

std::string string_utils::wideToUtf8(std::wstring const& wstr) {
    int count = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.length(), NULL, 0, NULL, NULL);
    std::string str(count, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
    return str;
}

std::wstring string_utils::utf8ToWide(std::string const& str) {
    int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
    std::wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);
    return wstr;
}

#endif

bool string_utils::startsWith(std::string const& str, std::string const& prefix) {
	return str.rfind(prefix, 0) == 0;
}
bool string_utils::startsWith(std::wstring const& str, std::wstring const& prefix) {
	return str.rfind(prefix, 0) == 0;
}
bool string_utils::endsWith(std::string const& str, std::string const& suffix) {
    if (suffix.size() > str.size()) return false;
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}
bool string_utils::endsWith(std::wstring const& str, std::wstring const& suffix) {
    if (suffix.size() > str.size()) return false;
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

std::string& string_utils::toLowerIP(std::string& str) {
    std::transform(
        str.begin(), str.end(),
        str.begin(),
        [](auto c) {
            return std::tolower(c);
        }
    );
    return str;
}

std::wstring& string_utils::toLowerIP(std::wstring& str) {
    std::transform(
        str.begin(), str.end(),
        str.begin(),
        [](auto c) {
            return std::towlower(c);
        }
    );
    return str;
}

std::string string_utils::toLower(std::string const& str) {
    std::string ret = str;
    return string_utils::toLowerIP(ret);
}

std::wstring string_utils::toLower(std::wstring const& str) {
    std::wstring ret = str;
    return string_utils::toLowerIP(ret);
}

std::string& string_utils::toUpperIP(std::string& str) {
    std::transform(
        str.begin(), str.end(),
        str.begin(),
        [](auto c) {
            return std::toupper(c);
        }
    );
    return str;
}

std::wstring& string_utils::toUpperIP(std::wstring& str) {
    std::transform(
        str.begin(), str.end(),
        str.begin(),
        [](auto c) {
            return std::towupper(c);
        }
    );
    return str;
}

std::string string_utils::toUpper(std::string const& str) {
    std::string ret = str;
    return string_utils::toUpperIP(ret);
}

std::wstring string_utils::toUpper(std::wstring const& str) {
    std::wstring ret = str;
    return string_utils::toUpperIP(ret);
}

std::string& string_utils::replaceIP(
    std::string & str,
    std::string const& orig,
    std::string const& repl
) {
    std::string::size_type n = 0;
    while ( ( n = str.find( orig, n ) ) != std::string::npos ) {
        str.replace( n, orig.size(), repl );
        n += repl.size();
    }
    return str;
}

std::wstring& string_utils::replaceIP(
    std::wstring & str,
    std::wstring const& orig,
    std::wstring const& repl
) {
    std::wstring::size_type n = 0;
    while ( ( n = str.find( orig, n ) ) != std::wstring::npos ) {
        str.replace( n, orig.size(), repl );
        n += repl.size();
    }
    return str;
}

std::string string_utils::replace(
    std::string const& str,
    std::string const& orig,
    std::string const& repl
) {
    auto ret = str;
    return string_utils::replaceIP(ret, orig, repl);
}

std::wstring string_utils::replace(
    std::wstring const& str,
    std::wstring const& orig,
    std::wstring const& repl
) {
    auto ret = str;
    return string_utils::replaceIP(ret, orig, repl);
}

std::vector<std::string> string_utils::split(
    std::string const& str,
    std::string const& split
) {
    std::vector<std::string> res;
    if (str.size()) {
        auto s = str;
        size_t pos = 0;
        while ((pos = s.find(split)) != std::string::npos) {
            res.push_back(s.substr(0, pos));
            s.erase(0, pos + split.length());
        }
        res.push_back(s);
    }
    return res;
}

std::vector<std::wstring> string_utils::split(
    std::wstring const& str,
    std::wstring const& split
) {
    std::vector<std::wstring> res;
    if (str.size()) {
        auto s = str;
        size_t pos = 0;
        while ((pos = s.find(split)) != std::wstring::npos) {
            res.push_back(s.substr(0, pos));
            s.erase(0, pos + split.length());
        }
        res.push_back(s);
    }
    return res;
}

std::vector<char> string_utils::split(std::string const& str) {
    std::vector<char> res;
    for (auto const& s : str) {
        res.push_back(s);
    }
    return res;
}

std::vector<wchar_t> string_utils::split(std::wstring const& str) {
    std::vector<wchar_t> res;
    for (auto const& s : str) {
        res.push_back(s);
    }
    return res;
}

bool string_utils::contains(std::string const& str, std::string const& subs) {
    return str.find(subs) != std::string::npos;
}

bool string_utils::contains(std::wstring const& str, std::wstring const& subs) {
    return str.find(subs) != std::wstring::npos;
}

bool string_utils::contains(std::string const& str, std::string::value_type c) {
    return str.find(c) != std::string::npos;
}

bool string_utils::contains(std::wstring const& str, std::wstring::value_type c) {
    return str.find(c) != std::wstring::npos;
}

bool string_utils::containsAny(
    std::string const& str,
    std::vector<std::string> const& subs
) {
    for (auto const& sub : subs) {
        if (string_utils::contains(str, sub))
            return true;
    }
    return false;
}

bool string_utils::containsAny(
    std::wstring const& str,
    std::vector<std::wstring> const& subs
) {
    for (auto const& sub : subs) {
        if (string_utils::contains(str, sub))
            return true;
    }
    return false;
}

bool string_utils::containsAll(
    std::string const& str,
    std::vector<std::string> const& subs
) {
    bool found = true;
    for (auto const& sub : subs) {
        if (!string_utils::contains(str, sub))
            found = false;
    }
    return found;
}

bool string_utils::containsAll(
    std::wstring const& str,
    std::vector<std::wstring> const& subs
) {
    bool found = true;
    for (auto const& sub : subs) {
        if (!string_utils::contains(str, sub))
            found = false;
    }
    return found;
}

size_t string_utils::count(std::string const& str, char countC) {
    size_t res = 0;
    for (auto c : str)
        if (c == countC) res++;
    return res;
}

size_t string_utils::count(std::wstring const& str, wchar_t countC) {
    size_t res = 0;
    for (auto c : str)
        if (c == countC) res++;
    return res;
}

std::string& string_utils::trimLeftIP(std::string & str) {
    str.erase(str.begin(),
        std::find_if(str.begin(), str.end(), [](auto ch) {
            return !std::isspace(ch);
        }
    ));
    return str;
}

std::wstring& string_utils::trimLeftIP(std::wstring & str) {
    str.erase(str.begin(),
        std::find_if(str.begin(), str.end(), [](auto ch) {
            return !std::isspace(ch);
        }
    ));
    return str;
}

std::string& string_utils::trimRightIP(std::string & str) {
    str.erase(std::find_if(str.rbegin(), str.rend(), [](auto ch) {
        return !std::isspace(ch);
    }).base(), str.end());
    return str;
}

std::wstring& string_utils::trimRightIP(std::wstring & str) {
    str.erase(std::find_if(str.rbegin(), str.rend(), [](auto ch) {
        return !std::isspace(ch);
    }).base(), str.end());
    return str;
}

std::string& string_utils::trimIP(std::string & str) {
    return 
        string_utils::trimLeftIP(
        string_utils::trimRightIP(
            str
        ));
}

std::wstring& string_utils::trimIP(std::wstring & str) {
    return 
        string_utils::trimLeftIP(
        string_utils::trimRightIP(
            str
        ));
}

std::string string_utils::trimLeft(std::string const& str) {
    auto s2 = str;
    return string_utils::trimLeftIP(s2);
}

std::wstring string_utils::trimLeft(std::wstring const& str) {
    auto s2 = str;
    return string_utils::trimLeftIP(s2);
}

std::string string_utils::trimRight(std::string const& str) {
    auto ret = str;
    return string_utils::trimRightIP(ret);
}

std::wstring string_utils::trimRight(std::wstring const& str) {
    auto ret = str;
    return string_utils::trimRightIP(ret);
}

std::string string_utils::trim(std::string const& str) {
    auto ret = str;
    return string_utils::trimIP(ret);
}

std::wstring string_utils::trim(std::wstring const& str) {
    auto ret = str;
    return string_utils::trimIP(ret);
}

std::string& string_utils::normalizeIP(std::string & str) {
    while (string_utils::contains(str, "  "))
        string_utils::replaceIP(str, "  ", " ");
    return str;
}

std::wstring& string_utils::normalizeIP(std::wstring & str) {
    while (string_utils::contains(str, L"  "))
        string_utils::replaceIP(str, L"  ", L" ");
    return str;
}

std::string string_utils::normalize(std::string const& str) {
    auto ret = str;
    return string_utils::normalizeIP(ret);
}

std::wstring string_utils::normalize(std::wstring const& str) {
    auto ret = str;
    return string_utils::normalizeIP(ret);
}
