#pragma once

#include <Macros.hpp>
#include "Result.hpp"
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

namespace geode::map_utils { 
    /**
     * Returns true if the map contains
     * a value matching `containFunc`.
     * @param map Map to check
     * @param containFunc Predicate run against each value in map;
     *                    return true if the item matches what is
     *                    beeing looked for, and false if not.
     * @returns True if value matching `containFunc` was found,
     * false if not.
     * @author HJfod
     */
    template<typename T, typename R>
    bool contains(
        std::unordered_map<T, R> const& map,
        std::function<bool(R)> containFunc
    ) {
        for (auto const& [_, r] : map) {
            if (containFunc(r))
                return true;
        }
        return false;
    }

    /**
     * Get the first item in a map that
     * matches `selectFunc`.
     * @param map Map to check
     * @param selectFunc Predicate run against each value in map;
     *                   return true if the item matches what is
     *                   beeing looked for, and false if not.
     * @returns The value matching `selectFunc` if one was found,
     * otherwise the default value for type R or `nullptr` if R is
     * a pointer.
     * @author HJfod
     */
    template<class T, class R>
    R select(
        std::unordered_map<T, R> const& map,
        std::function<bool(R)> selectFunc
    ) {
        for (auto const& [_, r] : map) {
            if (selectFunc(r))
                return r;
        }
        if (std::is_pointer<R>::value) {
            return nullptr;
        }
        return R();
    }

    /**
     * Get all items in a map that match `selectFunc`.
     * @param map Map to check
     * @param selectFunc Predicate run against each value in map;
     *                   return true if the item matches what is
     *                   beeing looked for, and false if not.
     * @returns Vector of all values that matched.
     * @author HJfod
     */
    template<class T, class R>
    std::vector<R> selectAll(
        std::unordered_map<T, R> const& map,
        std::function<bool(R)> selectFunc
    ) {
        std::vector<R> res;
        for (auto const& [_, r] : map) {
            if (selectFunc(r)) {
                res.push_back(r);
            }
        }
        return res;
    }

    /**
     * Get all values in a map.
     * @param map Map to get values from
     * @returns Vector of all values.
     * @author HJfod
     */
    template<class T, class R>
    std::vector<R> getValues(std::unordered_map<T, R> const& map) {
        std::vector<R> res;
        for (auto const& [_, r] : map) {
            res.push_back(r);
        }
        return res;
    }

    /**
     * Get all keys in a map.
     * @param map Map to get keys from
     * @returns Vector of all keys.
     * @author HJfod
     */
    template<class T, class R>
    std::vector<T> getKeys(std::unordered_map<T, R> const& map) {
        std::vector<T> res;
        for (auto const& [t, _] : map) {
            res.push_back(t);
        }
        return res;
    }

    /**
     * Transform an unordered_map into
     * another unordered_map of a different
     * type.
     * @param map Map to convert
     * @param remapFunc Function that converts 
     * key-value pairs from the first map to 
     * the second
     * @returns New map
     */
    template<class T1, class V1, class T2, class V2>
    std::unordered_map<T2, V2> remap(
        std::unordered_map<T1, V1> const& map,
        std::function<std::pair<T2, V2>(std::pair<T1, V1>)> remapFunc
    ) {
        std::unordered_map<T2, V2> res;
        for (auto const& [t, v] : map) {
            res.insert(remapFunc({ t, v }));
        }
        return res;
    }
}
