#pragma once

#include <Geode/DefaultInclude.hpp>
#include <string>
#include <algorithm>

namespace geode::utils::container {
    /**
     * Check if a container contains an element by value.
     * @param vec The vector to check.
     * @param elem The element to find.
     * @returns True if element is in `vec`, false if not.
     */
    template<class C, class T>
    [[deprecated("Use geode::utils::ranges::contains instead")]]
    bool contains(C const& cont, T const& elem) {
        return std::find(cont.begin(), cont.end(), elem) != cont.end();
    }

    /** 
     * Check if a vector contains an element via a function.
     * @param vec The vector to check.
     * @param containFunc A function that returns bool if the
     * element parameter is what is looked for.
     * @returns True if an element matching `containFunc` is
     * in `vec`, false if not.
     */
    template<class C, class T>
    [[deprecated("Use geode::utils::ranges::contains instead")]]
    bool contains(C const& cont, std::function<bool(T)> containFunc) {
        for (auto const& item : cont) {
            if (containFunc(item))
                return true;
        }
        return false;
    }

    /** 
     * Turn a vector into a string. T must be either a string,
     * or convertible via `std::to_string`.
     * @param vec The vector to add to.
     * @param sep Separator string.
     * @returns Joined string.
     */
    template<class C>
    [[deprecated("Use geode::utils::ranges::join instead")]]
    std::string join(C const& cont, std::string const& sep) {
        std::string res = "";
        bool first = true;
        for (auto p : cont) {
            if (!first) {
                res += sep;
            } else {
                first = false;
            }
            if constexpr (std::is_same_v<decltype(p), std::string>) {
                res += p;
            } else {
                res += std::to_string(p);
            }
        }
        return res;
    }

    /** 
     * Map a container of items type T to a new container of items
     * type T2.
     * @param vec The container to map.
     * @param mapFunc Function that converts an element from T to T2.
     * @returns Mapped container.
     */
    template<class C, class C2, class T, class T2>
    [[deprecated("Use geode::utils::ranges::map instead")]]
    C2 map(C const& cont, std::function<T2(T)> mapFunc) {
        C2 res;
        std::transform(cont.begin(), cont.end(), res.end(), mapFunc);
        return res;
    }
}
