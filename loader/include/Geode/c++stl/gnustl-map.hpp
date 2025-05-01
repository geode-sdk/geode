#pragma once

namespace geode::stl {
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

    inline void _rb_tree_rotate_left(_rb_tree_base* const x, _rb_tree_base*& root);
    inline void _rb_tree_rotate_right(_rb_tree_base* const x, _rb_tree_base*& root);
    inline void _rb_insert_rebalance(
        bool const insert_left, _rb_tree_base* x, _rb_tree_base* p, _rb_tree_base& header
    );
    inline _rb_tree_base* _rb_increment(_rb_tree_base* __x) noexcept;
    inline _rb_tree_base* _rb_decrement(_rb_tree_base* __x) noexcept;
    inline _rb_tree_base* _rb_rebalance_for_erase(_rb_tree_base* const __z, _rb_tree_base& __header) noexcept;

    template <typename T>
    struct _rb_tree_iterator {
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;

        typedef _rb_tree_iterator<T> _Self;
        typedef _rb_tree_base* _Base_ptr;
        typedef _rb_tree_node<T>* _Link_type;

        _rb_tree_iterator() : m_node() {}

        explicit _rb_tree_iterator(_Base_ptr __x) noexcept : m_node(__x) {}

        reference operator*() const noexcept {
            return static_cast<_Link_type>(m_node)->m_value;
        }

        pointer operator->() const noexcept {
            return &static_cast<_Link_type>(m_node)->m_value;
        }

        _Self& operator++() noexcept {
            m_node = _rb_increment(m_node);
            return *this;
        }

        _Self operator++(int) noexcept {
            _Self __tmp = *this;
            m_node = _rb_increment(m_node);
            return __tmp;
        }

        _Self& operator--() noexcept {
            m_node = _rb_decrement(m_node);
            return *this;
        }

        _Self operator--(int) noexcept {
            _Self __tmp = *this;
            m_node = _rb_decrement(m_node);
            return __tmp;
        }

        bool operator==(_Self const& __x) const noexcept {
            return m_node == __x.m_node;
        }

        bool operator!=(_Self const& __x) const noexcept {
            return m_node != __x.m_node;
        }

        _Base_ptr m_node;
    };

    inline void _rb_tree_rotate_left(_rb_tree_base* const x, _rb_tree_base*& root) {
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

    inline void _rb_tree_rotate_right(_rb_tree_base* const x, _rb_tree_base*& root) {
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

    inline void _rb_insert_rebalance(
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

    inline _rb_tree_base* _rb_increment(_rb_tree_base* __x) noexcept {
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

    inline _rb_tree_base* _rb_decrement(_rb_tree_base* __x) noexcept {
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

    inline _rb_tree_base* _rb_rebalance_for_erase(
        _rb_tree_base* const __z, _rb_tree_base& __header
    ) noexcept {
        _rb_tree_base*& __root = __header.m_parent;
        _rb_tree_base*& __leftmost = __header.m_left;
        _rb_tree_base*& __rightmost = __header.m_right;
        _rb_tree_base* __y = __z;
        _rb_tree_base* __x = 0;
        _rb_tree_base* __x_parent = 0;

        if (__y->m_left == 0) // __z has at most one non-null child. y == z.
            __x = __y->m_right; // __x might be null.
        else if (__y->m_right == 0) // __z has exactly one non-null child. y == z.
            __x = __y->m_left; // __x is not null.
        else {
            // __z has two non-null children.  Set __y to
            __y = __y->m_right; //   __z's successor.  __x might be null.
            while (__y->m_left != 0)
                __y = __y->m_left;
            __x = __y->m_right;
        }
        if (__y != __z) {
            // relink y in place of z.  y is z's successor
            __z->m_left->m_parent = __y;
            __y->m_left = __z->m_left;
            if (__y != __z->m_right) {
                __x_parent = __y->m_parent;
                if (__x) __x->m_parent = __y->m_parent;
                __y->m_parent->m_left = __x; // __y must be a child of m_left
                __y->m_right = __z->m_right;
                __z->m_right->m_parent = __y;
            }
            else __x_parent = __y;
            if (__root == __z) __root = __y;
            else if (__z->m_parent->m_left == __z) __z->m_parent->m_left = __y;
            else __z->m_parent->m_right = __y;
            __y->m_parent = __z->m_parent;
            std::swap(__y->m_isblack, __z->m_isblack);
            __y = __z;
            // __y now points to node to be actually deleted
        }
        else { // __y == __z
            __x_parent = __y->m_parent;
            if (__x) __x->m_parent = __y->m_parent;
            if (__root == __z) __root = __x;
            else if (__z->m_parent->m_left == __z) __z->m_parent->m_left = __x;
            else __z->m_parent->m_right = __x;
            if (__leftmost == __z) {
                if (__z->m_right == 0) // __z->m_left must be null also
                    __leftmost = __z->m_parent;
                // makes __leftmost == _M_header if __z == __root
                else {
                    __leftmost = __x;
                    while (__leftmost->m_left != 0)
                        __leftmost = __leftmost->m_left;
                }
            }
            if (__rightmost == __z) {
                if (__z->m_left == 0) // __z->m_right must be null also
                    __rightmost = __z->m_parent;
                // makes __rightmost == _M_header if __z == __root
                else { // __x == __z->m_left
                    __rightmost = __x;
                    while (__rightmost->m_right != 0)
                        __rightmost = __rightmost->m_right;
                }
            }
        }
        if (__y->m_isblack != false) {
            while (__x != __root && (__x == 0 || __x->m_isblack))
                if (__x == __x_parent->m_left) {
                    _rb_tree_base* __w = __x_parent->m_right;
                    if (__w->m_isblack == false) {
                        __w->m_isblack = true;
                        __x_parent->m_isblack = false;
                        _rb_tree_rotate_left(__x_parent, __root);
                        __w = __x_parent->m_right;
                    }
                    if ((__w->m_left == 0 || __w->m_left->m_isblack == true) &&
                        (__w->m_right == 0 || __w->m_right->m_isblack == true)) {
                        __w->m_isblack = false;
                        __x = __x_parent;
                        __x_parent = __x_parent->m_parent;
                    }
                    else {
                        if (__w->m_right == 0 || __w->m_right->m_isblack == true) {
                            __w->m_left->m_isblack = true;
                            __w->m_isblack = false;
                            _rb_tree_rotate_right(__w, __root);
                            __w = __x_parent->m_right;
                        }
                        __w->m_isblack = __x_parent->m_isblack;
                        __x_parent->m_isblack = true;
                        if (__w->m_right) __w->m_right->m_isblack = true;
                        _rb_tree_rotate_left(__x_parent, __root);
                        break;
                    }
                }
                else {
                    // same as above, with m_right <-> m_left.
                    _rb_tree_base* __w = __x_parent->m_left;
                    if (__w->m_isblack == false) {
                        __w->m_isblack = true;
                        __x_parent->m_isblack = false;
                        _rb_tree_rotate_right(__x_parent, __root);
                        __w = __x_parent->m_left;
                    }
                    if ((__w->m_right == 0 || __w->m_right->m_isblack == true) &&
                        (__w->m_left == 0 || __w->m_left->m_isblack == true)) {
                        __w->m_isblack = false;
                        __x = __x_parent;
                        __x_parent = __x_parent->m_parent;
                    }
                    else {
                        if (__w->m_left == 0 || __w->m_left->m_isblack == true) {
                            __w->m_right->m_isblack = true;
                            __w->m_isblack = false;
                            _rb_tree_rotate_left(__w, __root);
                            __w = __x_parent->m_left;
                        }
                        __w->m_isblack = __x_parent->m_isblack;
                        __x_parent->m_isblack = true;
                        if (__w->m_left) __w->m_left->m_isblack = true;
                        _rb_tree_rotate_right(__x_parent, __root);
                        break;
                    }
                }
            if (__x) __x->m_isblack = true;
        }
        return __y;
    }
}