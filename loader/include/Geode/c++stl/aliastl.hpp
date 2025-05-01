#pragma once

#include <map>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>

namespace gd {
    template <class T>
    using vector = std::vector<T>;

    template <class K, class V>
    using map = std::map<K, V>;

    template <class K, class V>
    using unordered_map = std::unordered_map<K, V>;

    template <class K>
    using set = std::set<K>;

    template <class K>
    using unordered_set = std::unordered_set<K>;

    template <class T1, class T2>
    using pair = std::pair<T1, T2>;
}