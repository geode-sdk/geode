#include <Geode/c++stl/gdstdlib.hpp>

#ifdef GEODE_IS_MACOS

namespace gd {
    namespace {
        static inline auto emptyInternalString() {
            return reinterpret_cast<_internal_string*>(
                *reinterpret_cast<uintptr_t*>(geode::base::get() + 0x6030d0) + sizeof(_internal_string)
            );
        }
    }

    string::string() : m_data(nullptr) {
        m_data = emptyInternalString();
    }

    string::string(char const* ok) : m_data(nullptr) {
        reinterpret_cast<void (*)(string*, char const*)>(geode::base::get() + 0x489fc0)(this, ok);
    }

    string::string(string const& ok) : m_data(nullptr) {
        if (*(string**)(&ok) == nullptr) return;
        reinterpret_cast<void (*)(string*, string const&)>(geode::base::get() + 0x489fcc)(this, ok);
    }

    string& string::operator=(char const* ok) {
        auto ret = reinterpret_cast<string* (*)(string*, char const*, size_t)>(
            geode::base::get() + 0x489f96
        )(this, ok, strlen(ok));
        return *ret;
    }

    string& string::operator=(string const& ok) {
        auto ret = reinterpret_cast<string* (*)(string*, string const&)>(
            geode::base::get() + 0x489f9c
        )(this, ok);
        return *ret;
    }

    string::~string() {
        if (m_data == nullptr) return;
        if (m_data == emptyInternalString()) return;
        if (m_data[-1].m_refcount-- <= 0) {
            void* al;
            reinterpret_cast<void (*)(_internal_string*, void*)>(geode::base::get() + 0x489f78)(
                &m_data[-1], al
            );
        }
    }

    bool string::operator<(string const& other) const {
        return std::string(*this) < std::string(other);
    }

    bool string::operator==(string const& other) const {
        return std::string(*this) == std::string(other);
    }

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

    template <class K, class V>
    std::map<K, V> map<K, V>::std() {
        return (std::map<K, V>)(*this);
    }

    template <class K, class V>
    map<K, V>::operator std::map<K, V>() {
        auto iter_node = static_cast<_tree_node>(m_header.m_left);
        auto end_node = static_cast<_tree_node>(&m_header);
        std::map<K, V> out;
        for (; iter_node != end_node; iter_node = static_cast<_tree_node>(_rb_increment(iter_node))) {
            out[iter_node->m_value.first] = iter_node->m_value.second;
        }

        return out;
    }

    template <class K, class V>
    map<K, V>::operator std::map<K, V>() const {
        auto iter_node = static_cast<_tree_node>(m_header.m_left);
        auto end_node = (_tree_node)(&m_header);
        std::map<K, V> out;
        for (; iter_node != end_node; iter_node = static_cast<_tree_node>(_rb_increment(iter_node))) {
            out[iter_node->m_value.first] = iter_node->m_value.second;
        }

        return out;
    }

    template <class K, class V>
    void map<K, V>::insert(_tree_node x, _tree_node p, std::pair<K, V> const& val) {
        bool insert_left =
            (x != 0 || p == static_cast<_tree_node>(&m_header) || val.first < p->m_value.first);

        _tree_node z = new _rb_tree_node<std::pair<K, V>>();
        z->m_value = val;

        _rb_insert_rebalance(insert_left, z, p, m_header);
        ++m_nodecount;
    }

    template <class K, class V>
    void map<K, V>::insert_pair(std::pair<K, V> const& val) {
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

    template <class K, class V>
    map<K, V>::map(std::map<K, V> input) {
        m_header.m_isblack = false;
        m_header.m_parent = 0;
        m_header.m_left = &m_header;
        m_header.m_right = &m_header;

        for (auto i : input) {
            insert_pair(i);
        }
    }

    template <class K, class V>
    void map<K, V>::erase(_tree_node x) {
        while (x != 0) {
            erase(static_cast<_tree_node>(x->m_right));
            auto y = static_cast<_tree_node>(x->m_left);
            delete y;
            x = y;
        }
    }

    template <class K, class V>
    map<K, V>::map(map const& lol) : map(std::map<K, V>(lol)) {}

    template <class K, class V>
    map<K, V>::map() : map(std::map<K, V>()) {}

    template <class K, class V>
    map<K, V>::~map() {
        erase(static_cast<_tree_node>(m_header.m_parent));
    }

    template class map<int, int>;
    template class map<gd::string, gd::string>;
    template class map<gd::string, bool>;
    template class map<short, bool>;
}

#endif