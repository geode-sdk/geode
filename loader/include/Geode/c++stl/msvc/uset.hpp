// unordered_set standard header

// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// Per Apache License, Version 2.0, Section 4, Point b: I (kynex7510) changed this file.

#ifndef _GEODE_UNORDERED_SET_
#define _GEODE_UNORDERED_SET_
#include <yvals_core.h>
#if _STL_COMPILER_PREPROCESSOR
#include "xhash.hpp"

#if _HAS_CXX17
#include <xpolymorphic_allocator.h>
#endif // _HAS_CXX17

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

namespace geode::stl {

template <class _Kty, // key type (same as value type)
    class _Tr, // comparator predicate type
    class _Alloc, // actual allocator type (should be value allocator)
    bool _Mfl> // true if multiple equivalent keys are permitted
class _Uset_traits : public _Tr { // traits required to make _Hash behave like a set
public:
    using key_type            = _Kty;
    using value_type          = _Kty;
    using _Mutable_value_type = _Kty;
    using key_compare         = _Tr;
    using allocator_type      = _Alloc;
#if _HAS_CXX17
    using node_type = _STD _Node_handle<_STD _List_node<value_type, typename _STD allocator_traits<_Alloc>::void_pointer>, _Alloc,
        _STD _Node_handle_set_base, _Kty>;
#endif // _HAS_CXX17

    static constexpr bool _Multi    = _Mfl;
    static constexpr bool _Standard = true;

    template <class... _Args>
    using _In_place_key_extractor = _STD _In_place_key_extract_set<_Kty, _Args...>;

    _Uset_traits() = default;

    explicit _Uset_traits(const _Tr& _Traits) noexcept(_STD is_nothrow_copy_constructible_v<_Tr>) : _Tr(_Traits) {}

    using value_compare = void; // TRANSITION, remove when _Standard becomes unconditionally true

    static const _Kty& _Kfn(const value_type& _Val) noexcept {
        return _Val;
    }

    static int _Nonkfn(const value_type&) noexcept { // extract "non-key" from element value (for container equality)
        return 0;
    }
};

/* _EXPORT_STD */ template <class _Kty, class _Hasher = _STD hash<_Kty>, class _Keyeq = _STD equal_to<_Kty>,
    class _Alloc = allocator<_Kty>>
class unordered_set : public _Hash<_Uset_traits<_Kty, _STD _Uhash_compare<_Kty, _Hasher, _Keyeq>, _Alloc, false>> {
    // hash table of key-values, unique keys
public:
    static_assert(!_ENFORCE_MATCHING_ALLOCATORS || _STD is_same_v<_Kty, typename _Alloc::value_type>,
        _MISMATCHED_ALLOCATOR_MESSAGE("unordered_set<T, Hasher, Eq, Allocator>", "T"));
    static_assert(_STD is_object_v<_Kty>, "The C++ Standard forbids containers of non-object types "
                                     "because of [container.requirements].");

private:
    using _Mytraits      = _STD _Uhash_compare<_Kty, _Hasher, _Keyeq>;
    using _Mybase        = _Hash<_Uset_traits<_Kty, _Mytraits, _Alloc, false>>;
    using _Alnode        = typename _Mybase::_Alnode;
    using _Alnode_traits = typename _Mybase::_Alnode_traits;
    using _Key_compare   = typename _Mybase::_Key_compare;

public:
    using hasher    = _Hasher;
    using key_type  = _Kty;
    using key_equal = _Keyeq;

    using value_type      = typename _Mybase::value_type;
    using allocator_type  = typename _Mybase::allocator_type;
    using size_type       = typename _Mybase::size_type;
    using difference_type = typename _Mybase::difference_type;
    using pointer         = typename _Mybase::pointer;
    using const_pointer   = typename _Mybase::const_pointer;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using iterator        = typename _Mybase::iterator;
    using const_iterator  = typename _Mybase::const_iterator;

    using local_iterator       = typename _Mybase::iterator;
    using const_local_iterator = typename _Mybase::const_iterator;

#if _HAS_CXX17
    using insert_return_type = _STD _Insert_return_type<iterator, typename _Mybase::node_type>;
#endif // _HAS_CXX17

    unordered_set() : _Mybase(_Key_compare(), allocator_type()) {}

    explicit unordered_set(const allocator_type& _Al) : _Mybase(_Key_compare(), _Al) {}

    unordered_set(const unordered_set& _Right)
        : _Mybase(_Right, _Alnode_traits::select_on_container_copy_construction(_Right._Getal())) {}

    unordered_set(const unordered_set& _Right, const allocator_type& _Al) : _Mybase(_Right, _Al) {}

    explicit unordered_set(size_type _Buckets) : _Mybase(_Key_compare(), allocator_type()) {
        this->rehash(_Buckets);
    }

    unordered_set(size_type _Buckets, const allocator_type& _Al) : _Mybase(_Key_compare(), _Al) {
        this->rehash(_Buckets);
    }

    unordered_set(size_type _Buckets, const hasher& _Hasharg) : _Mybase(_Key_compare(_Hasharg), allocator_type()) {
        this->rehash(_Buckets);
    }

    unordered_set(size_type _Buckets, const hasher& _Hasharg, const allocator_type& _Al)
        : _Mybase(_Key_compare(_Hasharg), _Al) {
        this->rehash(_Buckets);
    }

    unordered_set(size_type _Buckets, const hasher& _Hasharg, const _Keyeq& _Keyeqarg)
        : _Mybase(_Key_compare(_Hasharg, _Keyeqarg), allocator_type()) {
        this->rehash(_Buckets);
    }

    unordered_set(size_type _Buckets, const hasher& _Hasharg, const _Keyeq& _Keyeqarg, const allocator_type& _Al)
        : _Mybase(_Key_compare(_Hasharg, _Keyeqarg), _Al) {
        this->rehash(_Buckets);
    }

    template <class _Iter>
    unordered_set(_Iter _First, _Iter _Last) : _Mybase(_Key_compare(), allocator_type()) {
        this->insert(_First, _Last);
    }

    template <class _Iter>
    unordered_set(_Iter _First, _Iter _Last, const allocator_type& _Al) : _Mybase(_Key_compare(), _Al) {
        this->insert(_First, _Last);
    }

    template <class _Iter>
    unordered_set(_Iter _First, _Iter _Last, size_type _Buckets) : _Mybase(_Key_compare(), allocator_type()) {
        this->rehash(_Buckets);
        this->insert(_First, _Last);
    }

    template <class _Iter>
    unordered_set(_Iter _First, _Iter _Last, size_type _Buckets, const allocator_type& _Al)
        : _Mybase(_Key_compare(), _Al) {
        this->rehash(_Buckets);
        this->insert(_First, _Last);
    }

    template <class _Iter>
    unordered_set(_Iter _First, _Iter _Last, size_type _Buckets, const hasher& _Hasharg)
        : _Mybase(_Key_compare(_Hasharg), allocator_type()) {
        this->rehash(_Buckets);
        this->insert(_First, _Last);
    }

    template <class _Iter>
    unordered_set(_Iter _First, _Iter _Last, size_type _Buckets, const hasher& _Hasharg, const allocator_type& _Al)
        : _Mybase(_Key_compare(_Hasharg), _Al) {
        this->rehash(_Buckets);
        this->insert(_First, _Last);
    }

    template <class _Iter>
    unordered_set(_Iter _First, _Iter _Last, size_type _Buckets, const hasher& _Hasharg, const _Keyeq& _Keyeqarg)
        : _Mybase(_Key_compare(_Hasharg, _Keyeqarg), allocator_type()) {
        this->rehash(_Buckets);
        this->insert(_First, _Last);
    }

    template <class _Iter>
    unordered_set(_Iter _First, _Iter _Last, size_type _Buckets, const hasher& _Hasharg, const _Keyeq& _Keyeqarg,
        const allocator_type& _Al)
        : _Mybase(_Key_compare(_Hasharg, _Keyeqarg), _Al) {
        this->rehash(_Buckets);
        this->insert(_First, _Last);
    }

#if _HAS_CXX23 && defined(__cpp_lib_concepts) // TRANSITION, GH-395
    template <_STD _Container_compatible_range<value_type> _Rng>
    unordered_set(_STD from_range_t, _Rng&& _Range) : _Mybase(_Key_compare(), allocator_type()) {
        this->_Insert_range_unchecked(_RANGES _Ubegin(_Range), _RANGES _Uend(_Range));
    }

    template <_STD _Container_compatible_range<value_type> _Rng>
    unordered_set(_STD from_range_t, _Rng&& _Range, const allocator_type& _Al) : _Mybase(_Key_compare(), _Al) {
        this->_Insert_range_unchecked(_RANGES _Ubegin(_Range), _RANGES _Uend(_Range));
    }

    template <_STD _Container_compatible_range<value_type> _Rng>
    unordered_set(_STD from_range_t, _Rng&& _Range, size_type _Buckets) : _Mybase(_Key_compare(), allocator_type()) {
        _Mybase::rehash(_Buckets);
        this->_Insert_range_unchecked(_RANGES _Ubegin(_Range), _RANGES _Uend(_Range));
    }

    template <_STD _Container_compatible_range<value_type> _Rng>
    unordered_set(_STD from_range_t, _Rng&& _Range, size_type _Buckets, const allocator_type& _Al)
        : _Mybase(_Key_compare(), _Al) {
        _Mybase::rehash(_Buckets);
        this->_Insert_range_unchecked(_RANGES _Ubegin(_Range), _RANGES _Uend(_Range));
    }

    template <_STD _Container_compatible_range<value_type> _Rng>
    unordered_set(_STD from_range_t, _Rng&& _Range, size_type _Buckets, const hasher& _Hasharg)
        : _Mybase(_Key_compare(_Hasharg), allocator_type()) {
        _Mybase::rehash(_Buckets);
        this->_Insert_range_unchecked(_RANGES _Ubegin(_Range), _RANGES _Uend(_Range));
    }

    template <_STD _Container_compatible_range<value_type> _Rng>
    unordered_set(_STD from_range_t, _Rng&& _Range, size_type _Buckets, const hasher& _Hasharg, const allocator_type& _Al)
        : _Mybase(_Key_compare(_Hasharg), _Al) {
        _Mybase::rehash(_Buckets);
        this->_Insert_range_unchecked(_RANGES _Ubegin(_Range), _RANGES _Uend(_Range));
    }

    template <_STD _Container_compatible_range<value_type> _Rng>
    unordered_set(_STD from_range_t, _Rng&& _Range, size_type _Buckets, const hasher& _Hasharg, const _Keyeq& _Keyeqarg)
        : _Mybase(_Key_compare(_Hasharg, _Keyeqarg), allocator_type()) {
        _Mybase::rehash(_Buckets);
        this->_Insert_range_unchecked(_RANGES _Ubegin(_Range), _RANGES _Uend(_Range));
    }

    template <_STD _Container_compatible_range<value_type> _Rng>
    unordered_set(_STD from_range_t, _Rng&& _Range, size_type _Buckets, const hasher& _Hasharg, const _Keyeq& _Keyeqarg,
        const allocator_type& _Al)
        : _Mybase(_Key_compare(_Hasharg, _Keyeqarg), _Al) {
        _Mybase::rehash(_Buckets);
        this->_Insert_range_unchecked(_RANGES _Ubegin(_Range), _RANGES _Uend(_Range));
    }
#endif // _HAS_CXX23 && defined(__cpp_lib_concepts)

    unordered_set& operator=(const unordered_set& _Right) {
        _Mybase::operator=(_Right);
        return *this;
    }

    unordered_set(unordered_set&& _Right) : _Mybase(_STD move(_Right)) {}

    unordered_set(unordered_set&& _Right, const allocator_type& _Al) : _Mybase(_STD move(_Right), _Al) {}

    unordered_set& operator=(unordered_set&& _Right) noexcept(_Alnode_traits::is_always_equal::value&&
            _STD is_nothrow_move_assignable_v<_Hasher>&& _STD is_nothrow_move_assignable_v<_Keyeq>) {
        _Mybase::operator=(_STD move(_Right));
        return *this;
    }

    void swap(unordered_set& _Right) noexcept(noexcept(_Mybase::swap(_Right))) {
        _Mybase::swap(_Right);
    }

    unordered_set(_STD initializer_list<value_type> _Ilist) : _Mybase(_Key_compare(), allocator_type()) {
        this->insert(_Ilist);
    }

    unordered_set(_STD initializer_list<value_type> _Ilist, const allocator_type& _Al) : _Mybase(_Key_compare(), _Al) {
        this->insert(_Ilist);
    }

    unordered_set(_STD initializer_list<value_type> _Ilist, size_type _Buckets) : _Mybase(_Key_compare(), allocator_type()) {
        this->rehash(_Buckets);
        this->insert(_Ilist);
    }

    unordered_set(_STD initializer_list<value_type> _Ilist, size_type _Buckets, const allocator_type& _Al)
        : _Mybase(_Key_compare(), _Al) {
        this->rehash(_Buckets);
        this->insert(_Ilist);
    }

    unordered_set(_STD initializer_list<value_type> _Ilist, size_type _Buckets, const hasher& _Hasharg)
        : _Mybase(_Key_compare(_Hasharg), allocator_type()) {
        this->rehash(_Buckets);
        this->insert(_Ilist);
    }

    unordered_set(
        _STD initializer_list<value_type> _Ilist, size_type _Buckets, const hasher& _Hasharg, const allocator_type& _Al)
        : _Mybase(_Key_compare(_Hasharg), _Al) {
        this->rehash(_Buckets);
        this->insert(_Ilist);
    }

    unordered_set(
        _STD initializer_list<value_type> _Ilist, size_type _Buckets, const hasher& _Hasharg, const _Keyeq& _Keyeqarg)
        : _Mybase(_Key_compare(_Hasharg, _Keyeqarg), allocator_type()) {
        this->rehash(_Buckets);
        this->insert(_Ilist);
    }

    unordered_set(_STD initializer_list<value_type> _Ilist, size_type _Buckets, const hasher& _Hasharg,
        const _Keyeq& _Keyeqarg, const allocator_type& _Al)
        : _Mybase(_Key_compare(_Hasharg, _Keyeqarg), _Al) {
        this->rehash(_Buckets);
        this->insert(_Ilist);
    }

    unordered_set& operator=(_STD initializer_list<value_type> _Ilist) {
        this->clear();
        this->insert(_Ilist);
        return *this;
    }

    _NODISCARD hasher hash_function() const {
        return this->_Traitsobj._Mypair._Get_first();
    }

    _NODISCARD key_equal key_eq() const {
        return this->_Traitsobj._Mypair._Myval2._Get_first();
    }

    using _Mybase::_Unchecked_begin;
    using _Mybase::_Unchecked_end;
};

#if _HAS_CXX17
template <class _Iter, class _Hasher = _STD hash<_STD _Iter_value_t<_Iter>>, class _Keyeq = _STD equal_to<_STD _Iter_value_t<_Iter>>,
    class _Alloc = allocator<_STD _Iter_value_t<_Iter>>,
    _STD enable_if_t<
        _STD conjunction_v<_STD _Is_iterator<_Iter>, _Is_hasher<_Hasher>, _STD negation<_STD _Is_allocator<_Keyeq>>, _STD _Is_allocator<_Alloc>>,
        int> = 0>
unordered_set(_Iter, _Iter, _STD _Guide_size_type_t<_Alloc> = 0, _Hasher = _Hasher(), _Keyeq = _Keyeq(), _Alloc = _Alloc())
    -> unordered_set<_STD _Iter_value_t<_Iter>, _Hasher, _Keyeq, _Alloc>;

template <class _Kty, class _Hasher = _STD hash<_Kty>, class _Keyeq = _STD equal_to<_Kty>, class _Alloc = allocator<_Kty>,
    _STD enable_if_t<_STD conjunction_v<_Is_hasher<_Hasher>, _STD negation<_STD _Is_allocator<_Keyeq>>, _STD _Is_allocator<_Alloc>>, int> = 0>
unordered_set(_STD initializer_list<_Kty>, _STD _Guide_size_type_t<_Alloc> = 0, _Hasher = _Hasher(), _Keyeq = _Keyeq(),
    _Alloc = _Alloc()) -> unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>;

template <class _Iter, class _Alloc, _STD enable_if_t<_STD conjunction_v<_STD _Is_iterator<_Iter>, _STD _Is_allocator<_Alloc>>, int> = 0>
unordered_set(_Iter, _Iter, _STD _Guide_size_type_t<_Alloc>, _Alloc)
    -> unordered_set<_STD _Iter_value_t<_Iter>, _STD hash<_STD _Iter_value_t<_Iter>>, _STD equal_to<_STD _Iter_value_t<_Iter>>, _Alloc>;

template <class _Iter, class _Hasher, class _Alloc,
    _STD enable_if_t<_STD conjunction_v<_STD _Is_iterator<_Iter>, _Is_hasher<_Hasher>, _STD _Is_allocator<_Alloc>>, int> = 0>
unordered_set(_Iter, _Iter, _STD _Guide_size_type_t<_Alloc>, _Hasher, _Alloc)
    -> unordered_set<_STD _Iter_value_t<_Iter>, _Hasher, _STD equal_to<_STD _Iter_value_t<_Iter>>, _Alloc>;

template <class _Kty, class _Alloc, _STD enable_if_t<_STD _Is_allocator<_Alloc>::value, int> = 0>
unordered_set(_STD initializer_list<_Kty>, _STD _Guide_size_type_t<_Alloc>, _Alloc)
    -> unordered_set<_Kty, _STD hash<_Kty>, _STD equal_to<_Kty>, _Alloc>;

template <class _Kty, class _Hasher, class _Alloc,
    _STD enable_if_t<_STD conjunction_v<_Is_hasher<_Hasher>, _STD _Is_allocator<_Alloc>>, int> = 0>
unordered_set(_STD initializer_list<_Kty>, _STD _Guide_size_type_t<_Alloc>, _Hasher, _Alloc)
    -> unordered_set<_Kty, _Hasher, _STD equal_to<_Kty>, _Alloc>;

#if _HAS_CXX23 && defined(__cpp_lib_concepts) // TRANSITION, GH-395
template <_RANGES input_range _Rng, class _Hasher = _STD hash<_RANGES range_value_t<_Rng>>,
    class _Keyeq = _STD equal_to<_RANGES range_value_t<_Rng>>, class _Alloc = allocator<_RANGES range_value_t<_Rng>>,
    _STD enable_if_t<_STD conjunction_v<_Is_hasher<_Hasher>, _STD negation<_STD _Is_allocator<_Keyeq>>, _STD _Is_allocator<_Alloc>>, int> = 0>
unordered_set(_STD from_range_t, _Rng&&, _STD _Guide_size_type_t<_Alloc> = 0, _Hasher = _Hasher(), _Keyeq = _Keyeq(),
    _Alloc = _Alloc()) -> unordered_set<_RANGES range_value_t<_Rng>, _Hasher, _Keyeq, _Alloc>;

template <_RANGES input_range _Rng, class _Alloc, _STD enable_if_t<_STD _Is_allocator<_Alloc>::value, int> = 0>
unordered_set(_STD from_range_t, _Rng&&, _STD _Guide_size_type_t<_Alloc>, _Alloc) -> unordered_set<_RANGES range_value_t<_Rng>,
    _STD hash<_RANGES range_value_t<_Rng>>, _STD equal_to<_RANGES range_value_t<_Rng>>, _Alloc>;

template <_RANGES input_range _Rng, class _Alloc, _STD enable_if_t<_STD _Is_allocator<_Alloc>::value, int> = 0>
unordered_set(_STD from_range_t, _Rng&&, _Alloc) -> unordered_set<_RANGES range_value_t<_Rng>,
    _STD hash<_RANGES range_value_t<_Rng>>, _STD equal_to<_RANGES range_value_t<_Rng>>, _Alloc>;

template <_RANGES input_range _Rng, class _Hasher, class _Alloc,
    _STD enable_if_t<_STD conjunction_v<_Is_hasher<_Hasher>, _STD _Is_allocator<_Alloc>>, int> = 0>
unordered_set(_STD from_range_t, _Rng&&, _STD _Guide_size_type_t<_Alloc>, _Hasher, _Alloc)
    -> unordered_set<_RANGES range_value_t<_Rng>, _Hasher, _STD equal_to<_RANGES range_value_t<_Rng>>, _Alloc>;
#endif // _HAS_CXX23 && defined(__cpp_lib_concepts)
#endif // _HAS_CXX17

/* _EXPORT_STD */ template <class _Kty, class _Hasher, class _Keyeq, class _Alloc>
void swap(unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>& _Left,
    unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>& _Right) noexcept(noexcept(_Left.swap(_Right))) {
    _Left.swap(_Right);
}

#if _HAS_CXX20
/* _EXPORT_STD */ template <class _Kty, class _Hasher, class _Keyeq, class _Alloc, class _Pr>
unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>::size_type erase_if(
    unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>& _Cont, _Pr _Pred) {
    return _STD _Erase_nodes_if(_Cont, _STD _Pass_fn(_Pred));
}
#endif // _HAS_CXX20

/* _EXPORT_STD */ template <class _Kty, class _Hasher, class _Keyeq, class _Alloc>
_NODISCARD bool operator==(const unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>& _Left,
    const unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>& _Right) {
    return geode::stl:: _Hash_equal(_Left, _Right);
}

#if !_HAS_CXX20
template <class _Kty, class _Hasher, class _Keyeq, class _Alloc>
_NODISCARD bool operator!=(const unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>& _Left,
    const unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>& _Right) {
    return !(_Left == _Right);
}
#endif // !_HAS_CXX20

_STD_END
#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)
#endif // _STL_COMPILER_PREPROCESSOR
#endif // _GEODE_UNORDERED_SET_
