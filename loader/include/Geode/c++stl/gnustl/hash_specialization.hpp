#pragma once
#include "functional_hash.h"

namespace geode::stl {
    template <>
    struct hash<gd::string> : public __hash_base<size_t, gd::string> {
        size_t operator()(const gd::string& s) const noexcept {
            return _Hash_impl::hash(s.data(), s.size());
        }
    };
}
