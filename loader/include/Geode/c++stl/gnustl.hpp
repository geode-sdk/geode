#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>

namespace geode::base {
    uintptr_t get();
}

#if defined(GEODE_IS_ANDROID)

    #include "gnustl/functexcept.h"
    #include "gnustl/stl_vector.h"
    #include "gnustl/stl_bvector.h"
    #include "gnustl/vector.tcc"
    #include "gnustl/stl_map.h"
    #include "gnustl/stl_set.h"
    #include "gnustl/unordered_map.hpp"
    #include "gnustl/unordered_set.hpp"
    #include "gnustl/hash_specialization.hpp"
    #undef _GLIBCXX_RELEASE
    #undef __GLIBCXX__
    #undef _GLIBCXX_USE_DUAL_ABI

namespace gd {
    using namespace geode::stl;

    void* operatorNew(size_t size);
    void operatorDelete(void* ptr);

    struct _bit_reference {
        uintptr_t* m_bitptr;
        uintptr_t m_mask;

        _bit_reference(uintptr_t* x, uintptr_t y) : m_bitptr(x), m_mask(y) {}

        _bit_reference() : m_bitptr(0), m_mask(0) {}

        operator bool() const {
            return !!(*m_bitptr & m_mask);
        }

        _bit_reference& operator=(bool x) {
            if (x) *m_bitptr |= m_mask;
            else *m_bitptr &= ~m_mask;
            return *this;
        }

        _bit_reference& operator=(_bit_reference const& x) {
            return *this = bool(x);
        }

        bool operator==(_bit_reference const& x) const {
            return bool(*this) == bool(x);
        }

        bool operator<(_bit_reference const& x) const {
            return !bool(*this) && bool(x);
        }

        void flip() {
            *m_bitptr ^= m_mask;
        }
    };

    struct _bit_iterator {
        uintptr_t* m_bitptr;
        unsigned int m_offset;

        _bit_iterator(uintptr_t* x) : m_bitptr(x), m_offset(0) {}

        _bit_iterator(uintptr_t* x, unsigned o) : m_bitptr(x), m_offset(o) {}

        _bit_reference operator*() const {
            return _bit_reference(m_bitptr, 1UL << m_offset);
        }

        _bit_iterator& operator++() {
            if (m_offset++ == sizeof(uintptr_t) - 1) {
                m_offset = 0;
                m_bitptr++;
            }
            return *this;
        }

        bool operator!=(_bit_iterator const& b) {
            return !(m_bitptr == b.m_bitptr && m_offset == b.m_offset);
        }
    };

    template<typename Tp, typename Alloc = allocator<Tp>>
    using vector = geode::stl::vector<Tp, Alloc>;

    template<typename Key, typename Tp, typename Compare = geode::stl::less<Key>, typename Alloc = allocator<Key>>
    using map = geode::stl::map<Key, Tp, Compare, Alloc>;

    template<typename Key, typename Compare = geode::stl::less<Key>, typename Alloc = allocator<Key>>
    using set = geode::stl::set<Key, Compare, Alloc>;

    template <class Key, class Tp, class Hash = geode::stl::hash<Key>, class Pred = geode::stl::equal_to<Key>, class Alloc = allocator<std::pair<const Key, Tp>>>
    using unordered_map = geode::stl::unordered_map<Key, Tp, Hash, Pred, Alloc>;

    template <class Value, class Hash = geode::stl::hash<Value>, class Pred = geode::stl::equal_to<Value>, class Alloc = allocator<Value>>
    using unordered_set = geode::stl::unordered_set<Value, Hash, Pred, Alloc>;
};

template <>
struct std::__ndk1::iterator_traits<typename gd::vector<char>::iterator> : std::__ndk1::iterator_traits<char*> {};

#elif defined(GEODE_IS_IOS)
namespace gd {
    class GEODE_DLL string {
    public:
        string() {}

        string(char const* ok) : m_internal(ok) {}

        string(std::string ok) : m_internal(ok) {}

        operator std::string() {
            return m_internal;
        }

        operator std::string() const {
            return m_internal;
        }

        string(string const& ok) : m_internal(ok) {}

        string& operator=(char const* ok) {
            m_internal = ok;
            return *this;
        }

        string& operator=(string const& ok) {
            m_internal = ok;
            return *this;
        }

        ~string() {}

        char const* c_str() const {
            return m_internal.c_str();
        }

    protected:
        std::string m_internal;
    };

    template <typename T>
    class GEODE_DLL vector {
    public:
        using value_type = T;

        operator std::vector<T>() {
            return m_internal;
        }

        void clear() {
            m_internal.clear();
        }

        operator std::vector<T>() const {
            return m_internal;
        }

        vector(std::vector<T> input) : m_internal(input) {}

        T& front() {
            return m_internal.front();
        }

        vector(vector const& lol) : m_internal(lol) {}

        vector() : m_internal() {}

        ~vector() {}

    protected:
        std::vector<T> m_internal;
    };

    template <typename K, typename V>
    class GEODE_DLL map {
    protected:
        std::map<K, V> m_internal;

    public:
        operator std::map<K, V>() {
            return m_internal;
        }

        operator std::map<K, V>() const {
            return m_internal;
        }

        map(std::map<K, V> input) : m_internal(input) {}

        map(map const& lol) : m_internal(lol) {}

        map() {}

        ~map() {}
    };
}
#endif
