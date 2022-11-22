#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <Geode/binding/GDString.hpp>

namespace geode::base {
    uintptr_t get();
}

#if defined(GEODE_IS_MACOS) || defined(GEODE_IS_ANDROID)
namespace gd {
    struct _internal_string {
        size_t m_len;
        size_t m_capacity;
        int m_refcount;
    };

    class GEODE_DLL string {
    public:
        string();
        string(char const* ok);

        string(std::string ok) : string(ok.c_str()) {}

        operator std::string() const {
            return std::string((char*)m_data, m_data[-1].m_len);
        }

        bool operator==(string const& other) const;
        string(string const& ok);
        string& operator=(char const* ok);
        string& operator=(string const& ok);
        __attribute__((noinline)) ~string();

        char const* c_str() const {
            return (char const*)m_data;
        }

        size_t size() const {
            return m_data[-1].m_len;
        }

    protected:
        _internal_string* m_data;
    };

    struct _rb_tree_base {
        bool m_isblack;
        _rb_tree_base* m_parent;
        _rb_tree_base* m_left;
        _rb_tree_base* m_right;
    };

    template <typename T>
    struct _rb_tree_node : public _rb_tree_base {
        T m_value;
    };

    static void _rb_tree_rotate_left(_rb_tree_base* const x, _rb_tree_base*& root) {
        _rb_tree_base* const y = x->m_right;

        x->m_right = y->m_left;
        if (y->m_left != 0) y->m_left->m_parent = x;
        y->m_parent = x->m_parent;

        if (x == root) root = y;
        else if (x == x->m_parent->m_left) x->m_parent->m_left = y;
        else x->m_parent->m_right = y;
        y->m_left = x;
        x->m_parent = y;
    }

    static void _rb_tree_rotate_right(_rb_tree_base* const x, _rb_tree_base*& root) {
        _rb_tree_base* const y = x->m_left;

        x->m_left = y->m_right;
        if (y->m_right != 0) y->m_right->m_parent = x;
        y->m_parent = x->m_parent;

        if (x == root) root = y;
        else if (x == x->m_parent->m_right) x->m_parent->m_right = y;
        else x->m_parent->m_left = y;
        y->m_right = x;
        x->m_parent = y;
    }

    static void _rb_insert_rebalance(
        bool const insert_left, _rb_tree_base* x, _rb_tree_base* p, _rb_tree_base& header
    ) {
        _rb_tree_base*& root = header.m_parent;

        x->m_parent = p;
        x->m_left = 0;
        x->m_right = 0;
        x->m_isblack = false;

        if (insert_left) {
            p->m_left = x;

            if (p == &header) {
                header.m_parent = x;
                header.m_right = x;
            }
            else if (p == header.m_left) {
                header.m_left = x;
            }
        }
        else {
            p->m_right = x;

            if (p == header.m_right) {
                header.m_right = x;
            }
        }

        while (x != root && x->m_parent->m_isblack == false) {
            _rb_tree_base* const xpp = x->m_parent->m_parent;

            if (x->m_parent == xpp->m_left) {
                _rb_tree_base* const y = xpp->m_right;
                if (y && y->m_isblack == false) {
                    x->m_parent->m_isblack = true;
                    y->m_isblack = true;
                    xpp->m_isblack = false;
                    x = xpp;
                }
                else {
                    if (x == x->m_parent->m_right) {
                        x = x->m_parent;
                        _rb_tree_rotate_left(x, root);
                    }
                    x->m_parent->m_isblack = true;
                    xpp->m_isblack = false;
                    _rb_tree_rotate_right(xpp, root);
                }
            }
            else {
                _rb_tree_base* const y = xpp->m_left;
                if (y && y->m_isblack == false) {
                    x->m_parent->m_isblack = true;
                    y->m_isblack = true;
                    xpp->m_isblack = false;
                    x = xpp;
                }
                else {
                    if (x == x->m_parent->m_left) {
                        x = x->m_parent;
                        _rb_tree_rotate_right(x, root);
                    }
                    x->m_parent->m_isblack = true;
                    xpp->m_isblack = false;
                    _rb_tree_rotate_left(xpp, root);
                }
            }
        }
        root->m_isblack = true;
    }

    static _rb_tree_base* _rb_increment(_rb_tree_base* __x) throw() {
        if (__x->m_right != 0) {
            __x = __x->m_right;
            while (__x->m_left != 0)
                __x = __x->m_left;
        }
        else {
            _rb_tree_base* __y = __x->m_parent;
            while (__x == __y->m_right) {
                __x = __y;
                __y = __y->m_parent;
            }
            if (__x->m_right != __y) __x = __y;
        }
        return __x;
    }

    static _rb_tree_base* _rb_decrement(_rb_tree_base* __x) throw() {
        if (!__x->m_isblack && __x->m_parent->m_parent == __x) __x = __x->m_right;
        else if (__x->m_left != 0) {
            _rb_tree_base* __y = __x->m_left;
            while (__y->m_right != 0)
                __y = __y->m_right;
            __x = __y;
        }
        else {
            _rb_tree_base* __y = __x->m_parent;
            while (__x == __y->m_left) {
                __x = __y;
                __y = __y->m_parent;
            }
            __x = __y;
        }
        return __x;
    }

    template <typename K, typename V>
    class GEODE_DLL map {
    protected:
        std::less<K> compare;
        _rb_tree_base m_header;
        size_t m_nodecount;

    public:
        typedef _rb_tree_node<std::pair<K, V>>* _tree_node;

        std::map<K, V> std() {
            return (std::map<K, V>)(*this);
        }

        operator std::map<K, V>() {
            auto iter_node = static_cast<_tree_node>(m_header.m_left);
            auto end_node = static_cast<_tree_node>(&m_header);
            std::map<K, V> out;
            for (; iter_node != end_node;
                 iter_node = static_cast<_tree_node>(_rb_increment(iter_node))) {
                out[iter_node->m_value.first] = iter_node->m_value.second;
            }

            return out;
        }

        operator std::map<K, V>() const {
            auto iter_node = static_cast<_tree_node>(m_header.m_left);
            auto end_node = (_tree_node)(&m_header);
            std::map<K, V> out;
            for (; iter_node != end_node;
                 iter_node = static_cast<_tree_node>(_rb_increment(iter_node))) {
                out[iter_node->m_value.first] = iter_node->m_value.second;
            }

            return out;
        }

        void insert(_tree_node x, _tree_node p, std::pair<K, V> const& val) {
            bool insert_left =
                (x != 0 || p == static_cast<_tree_node>(&m_header) || val.first < p->m_value.first);

            _tree_node z = new _rb_tree_node<std::pair<K, V>>();
            z->m_value = val;

            _rb_insert_rebalance(insert_left, z, p, m_header);
            ++m_nodecount;
        }

        void insert_pair(std::pair<K, V> const& val) {
            _tree_node x = static_cast<_tree_node>(m_header.m_parent);
            _tree_node y = static_cast<_tree_node>(&m_header);
            bool comp = true;
            while (x != 0) {
                y = x;
                comp = val.first < x->m_value.first;
                x = comp ? static_cast<_tree_node>(x->m_left) : static_cast<_tree_node>(x->m_right);
            }
            auto iter = y;

            if (comp) {
                if (iter == static_cast<_tree_node>(m_header.m_left)) {
                    insert(x, y, val);
                }
                else {
                    iter = static_cast<_tree_node>(_rb_decrement(iter));
                }
            }
            if (iter->m_value.first < val.first) {
                insert(x, y, val);
            }
        }

        map(std::map<K, V> input) {
            m_header.m_isblack = false;
            m_header.m_parent = 0;
            m_header.m_left = &m_header;
            m_header.m_right = &m_header;

            for (auto i : input) {
                insert_pair(i);
            }
        }

        void erase(_tree_node x) {
            while (x != 0) {
                erase(static_cast<_tree_node>(x->m_right));
                auto y = static_cast<_tree_node>(x->m_left);
                delete y;
                x = y;
            }
        }

        map(map const& lol) : map(std::map<K, V>(lol)) {}

        map() : map(std::map<K, V>()) {}

        ~map() {
            erase(static_cast<_tree_node>(m_header.m_parent));
        }
    };

    template <typename T>
    class GEODE_DLL vector {
    public:
        using value_type = T;

        operator std::vector<T>() const {
            std::vector<T> out;

            for (auto i = m_start; i != m_finish; ++i) {
                out.push_back(*i);
            }
            return out;
        }

        vector(std::vector<T> input) {
            auto tmp = new T[input.size()];

            m_start = tmp;
            m_finish = m_start + input.size();
            m_capacity_end = m_start + input.size();
            for (auto i : input) {
                *tmp = i;
                tmp++;
            }
        }

        vector(std::initializer_list<T> const& input) {
            auto tmp = new T[input.size()];
            m_start = tmp;
            m_finish = m_start + input.size();
            m_capacity_end = m_start + input.size();
            std::copy(input.begin(), input.end(), tmp);
        }

        T& front() {
            return *m_start;
        }

        auto begin() {
            return m_start;
        }

        auto end() {
            return m_finish;
        }

        auto begin() const {
            return static_cast<const T*>(m_start);
        }

        auto end() const {
            return static_cast<const T*>(m_finish);
        }

        vector(vector const& lol) : vector(std::vector<T>(lol)) {}

        vector() : vector(std::vector<T>()) {}

        ~vector() {
            delete[] m_start;
        }

    protected:
        T* m_start;
        T* m_finish;
        T* m_capacity_end;
    };

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

    template <>
    class vector<bool> {
    protected:
        _bit_iterator m_start;
        _bit_iterator m_end;
        uintptr_t* m_capacity_end;

    public:
        vector(std::vector<bool> input) : m_start(0), m_end(0) {
            auto realsize = input.size() / int(sizeof(uintptr_t));
            auto tmp = new uintptr_t[realsize];

            m_start = _bit_iterator(tmp);
            m_end = _bit_iterator(tmp + realsize, input.size() % sizeof(uintptr_t));
            m_capacity_end = tmp + realsize;

            auto itmp = m_start;
            for (auto i : input) {
                *itmp = i;
                ++itmp;
            }
        }

        operator std::vector<bool>() {
            std::vector<bool> out;
            for (auto i = m_start; i != m_end; ++i) {
                out.push_back(*i);
            }
            return out;
        }

        operator std::vector<bool>() const {
            std::vector<bool> out;
            for (auto i = m_start; i != m_end; ++i) {
                out.push_back(*i);
            }
            return out;
        }

        vector(vector<bool> const& lol) : vector(std::vector<bool>(lol)) {}

        vector() : vector(std::vector<bool>()) {}

        ~vector() {
            delete[] m_start.m_bitptr;
        }
    };
};

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
