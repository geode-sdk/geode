#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <vector>

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

    // template <class Type>
    // using vector = std::vector<Type>;

    template <typename T>
    class GEODE_DLL vector {
    public:
        using value_type = T;

        auto allocator() const {
            return std::allocator<T>();
        }

        operator std::vector<T>() const {
            return std::vector<T>(m_start, m_finish);
        }

        vector() {
            m_start = nullptr;
            m_finish = nullptr;
            m_reserveEnd = nullptr;
        }

        vector(std::vector<T> const& input) : vector() {
            if (input.size()) {
                m_start = this->allocator().allocate(input.size());
                m_finish = m_start + input.size();
                m_reserveEnd = m_start + input.size();

                std::copy(input.begin(), input.end(), m_start);
            }
        }

        vector(gd::vector<T> const& input) : vector() {
            if (input.size()) {
                m_start = this->allocator().allocate(input.size());
                m_finish = m_start + input.size();
                m_reserveEnd = m_start + input.size();

                std::copy(input.begin(), input.end(), m_start);
            }
        }

        vector(gd::vector<T>&& input) : vector() {
            m_start = input.m_start;
            m_finish = input.m_finish;
            m_reserveEnd = input.m_reserveEnd;

            input.m_start = nullptr;
            input.m_finish = nullptr;
            input.m_reserveEnd = nullptr;
        }

        vector& operator=(gd::vector<T> const& input) {
            this->clear();

            if (input.size()) {
                m_start = this->allocator().allocate(input.size());
                m_finish = m_start + input.size();
                m_reserveEnd = m_start + input.size();

                std::copy(input.begin(), input.end(), m_start);
            }

            return *this;
        }

        vector& operator=(gd::vector<T>&& input) {
            m_start = input.m_start;
            m_finish = input.m_finish;
            m_reserveEnd = input.m_reserveEnd;

            input.m_start = nullptr;
            input.m_finish = nullptr;
            input.m_reserveEnd = nullptr;

            return *this;
        }

        vector(std::initializer_list<T> const& input) : vector() {
            if (input.size()) {
                m_start = this->allocator().allocate(input.size());
                m_finish = m_start + input.size();
                m_reserveEnd = m_start + input.size();

                std::copy(input.begin(), input.end(), m_start);
            }
        }

        void clear() {
            if (m_start) {
                std::destroy(m_start, m_finish);

                this->allocator().deallocate(m_start, this->size());
            }

            m_start = nullptr;
            m_finish = nullptr;
            m_reserveEnd = nullptr;
        }

        T& operator[](size_t index) {
            return m_start[index];
        }

        T const& operator[](size_t index) const {
            return m_start[index];
        }

        T& at(size_t index) {
            if (index >= this->size()) {
                throw std::out_of_range("gd::vector::at");
            }
            return m_start[index];
        }

        T const& at(size_t index) const {
            if (index >= this->size()) {
                throw std::out_of_range("gd::vector::at");
            }
            return m_start[index];
        }

        T& front() {
            return *m_start;
        }

        T* begin() {
            return m_start;
        }

        T* end() {
            return m_finish;
        }

        T const* begin() const {
            return m_start;
        }

        T const* end() const {
            return m_finish;
        }

        ~vector() {
            for (auto i = m_start; i != m_finish; ++i) {
                delete i;
            }
        }

        size_t size() const {
            return m_finish - m_start;
        }

        size_t capacity() const {
            return m_reserveEnd - m_start;
        }

    protected:
        T* m_start;
        T* m_finish;
        T* m_reserveEnd;
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
        auto allocator() const {
            return std::allocator<uintptr_t>();
        }

        vector() : m_start(nullptr), m_end(nullptr), m_capacity_end(nullptr) {}

        // vector(std::vector<bool> input) : vector() {
        //     auto realsize = input.size() / int(sizeof(uintptr_t));
        //     auto start = this->allocator().allocate(realsize);

        //     m_start = _bit_iterator(start);
        //     m_end = _bit_iterator(start + realsize, input.size() % sizeof(uintptr_t));
        //     m_capacity_end = start + realsize;

        //     auto itmp = m_start;
        //     for (auto i : input) {
        //         *itmp = i;
        //         ++itmp;
        //     }
        // }

        // vector(vector<bool> const& input) : vector() {

        // }

        // vector() : vector(std::vector<bool>()) {}

        ~vector() {
            delete[] m_start.m_bitptr;
        }

        operator std::vector<bool>() const {
            std::vector<bool> out;
            for (auto i = m_start; i != m_end; ++i) {
                out.push_back(*i);
            }
            return out;
        }

        _bit_reference operator[](size_t index) {
            auto const real_index = index / sizeof(uintptr_t);
            auto const offset = index % sizeof(uintptr_t);
            return _bit_reference(&m_start.m_bitptr[real_index], 1UL << offset);
        }

        bool operator[](size_t index) const {
            return const_cast<vector&>(*this)[index];
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
