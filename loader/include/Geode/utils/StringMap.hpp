#pragma once

#include <string_view>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <Geode/utils/ZStringView.hpp>

namespace geode::utils {
    
inline uint64_t fnv1aHash(char const* str) {
    uint64_t hash = 0xcbf29ce484222325;
    while (*str) {
        hash ^= *str++;
        hash *= 0x100000001b3;
    }
    return hash;
}

inline uint64_t fnv1aHash(std::string_view str) {
    uint64_t hash = 0xcbf29ce484222325;
    for (char c : str) {
        hash ^= c;
        hash *= 0x100000001b3;
    }
    return hash;
}

// see https://ibob.bg/blog/2022/09/17/transparent-lookups-for-maps-and-sets/
// for more information
struct StringHash {
    using is_transparent = void;

    size_t operator()(const char* str) const { return fnv1aHash(str); }
    size_t operator()(std::string_view str) const { return fnv1aHash(str); }
    size_t operator()(std::string const& str) const { return fnv1aHash(str); }
    size_t operator()(ZStringView str) const { return fnv1aHash(std::string_view{str}); }
};
    
template <typename T>
using StringMap = std::unordered_map<std::string, T, StringHash, std::equal_to<>>;

using StringSet = std::unordered_set<std::string, StringHash, std::equal_to<>>;

template <typename T>
using StringMultimap = std::unordered_multimap<std::string, T, StringHash, std::equal_to<>>;

using StringMultiset = std::unordered_multiset<std::string, StringHash, std::equal_to<>>;

}