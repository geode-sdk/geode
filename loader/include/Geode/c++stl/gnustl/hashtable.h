// hashtable.h header -*- C++ -*-

// Copyright (C) 2007-2014 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file bits/hashtable.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{unordered_map, unordered_set}
 */

#ifndef _HASHTABLE_H
#define _HASHTABLE_H 1

#pragma GCC system_header

#include "hashtable_policy.h"
#include "functional_hash.h"
#include "type_traits.h"

namespace geode::stl {
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  template<typename _Tp, typename _Hash>
    using __cache_default
      =  __not_<std::conjunction<// Do not cache for fast hasher.
		       __is_fast_hash<_Hash>,
		       // Mandatory to have erase not throwing.
		       __detail::__is_noexcept_hash<_Tp, _Hash>>>;

  /**
   *  Primary class template _Hashtable.
   *
   *  @ingroup hashtable-detail
   *
   *  @tparam _Value  CopyConstructible type.
   *
   *  @tparam _Key    CopyConstructible type.
   *
   *  @tparam _Alloc  An allocator type
   *  ([lib.allocator.requirements]) whose _Alloc::value_type is
   *  _Value.  As a conforming extension, we allow for
   *  _Alloc::value_type != _Value.
   *
   *  @tparam _ExtractKey  Function object that takes an object of type
   *  _Value and returns a value of type _Key.
   *
   *  @tparam _Equal  Function object that takes two objects of type k
   *  and returns a bool-like value that is true if the two objects
   *  are considered equal.
   *
   *  @tparam _H1  The hash function. A unary function object with
   *  argument type _Key and result type size_t. Return values should
   *  be distributed over the entire range [0, numeric_limits<size_t>:::max()].
   *
   *  @tparam _H2  The range-hashing function (in the terminology of
   *  Tavori and Dreizin).  A binary function object whose argument
   *  types and result type are all size_t.  Given arguments r and N,
   *  the return value is in the range [0, N).
   *
   *  @tparam _Hash  The ranged hash function (Tavori and Dreizin). A
   *  binary function whose argument types are _Key and size_t and
   *  whose result type is size_t.  Given arguments k and N, the
   *  return value is in the range [0, N).  Default: hash(k, N) =
   *  h2(h1(k), N).  If _Hash is anything other than the default, _H1
   *  and _H2 are ignored.
   *
   *  @tparam _RehashPolicy  Policy class with three members, all of
   *  which govern the bucket count. _M_next_bkt(n) returns a bucket
   *  count no smaller than n.  _M_bkt_for_elements(n) returns a
   *  bucket count appropriate for an element count of n.
   *  _M_need_rehash(n_bkt, n_elt, n_ins) determines whether, if the
   *  current bucket count is n_bkt and the current element count is
   *  n_elt, we need to increase the bucket count.  If so, returns
   *  make_pair(true, n), where n is the new bucket count.  If not,
   *  returns make_pair(false, <anything>)
   *
   *  @tparam _Traits  Compile-time class with three boolean
   *  integral_constant members:  __cache_hash_code, __constant_iterators,
   *   __unique_keys.
   *
   *  Each _Hashtable data structure has:
   *
   *  - _Bucket[]       _M_buckets
   *  - _Hash_node_base _M_before_begin
   *  - size_type       _M_bucket_count
   *  - size_type       _M_element_count
   *
   *  with _Bucket being _Hash_node* and _Hash_node containing:
   *
   *  - _Hash_node*   _M_next
   *  - Tp            _M_value
   *  - size_t        _M_hash_code if cache_hash_code is true
   *
   *  In terms of Standard containers the hashtable is like the aggregation of:
   *
   *  - std::forward_list<_Node> containing the elements
   *  - std::vector<std::forward_list<_Node>::iterator> representing the buckets
   *
   *  The non-empty buckets contain the node before the first node in the
   *  bucket. This design makes it possible to implement something like a
   *  std::forward_list::insert_after on container insertion and
   *  std::forward_list::erase_after on container erase
   *  calls. _M_before_begin is equivalent to
   *  std::forward_list::before_begin. Empty buckets contain
   *  nullptr.  Note that one of the non-empty buckets contains
   *  &_M_before_begin which is not a dereferenceable node so the
   *  node pointer in a bucket shall never be dereferenced, only its
   *  next node can be.
   *
   *  Walking through a bucket's nodes requires a check on the hash code to
   *  see if each node is still in the bucket. Such a design assumes a
   *  quite efficient hash functor and is one of the reasons it is
   *  highly advisable to set __cache_hash_code to true.
   *
   *  The container iterators are simply built from nodes. This way
   *  incrementing the iterator is perfectly efficient independent of
   *  how many empty buckets there are in the container.
   *
   *  On insert we compute the element's hash code and use it to find the
   *  bucket index. If the element must be inserted in an empty bucket
   *  we add it at the beginning of the singly linked list and make the
   *  bucket point to _M_before_begin. The bucket that used to point to
   *  _M_before_begin, if any, is updated to point to its new before
   *  begin node.
   *
   *  On erase, the simple iterator design requires using the hash
   *  functor to get the index of the bucket to update. For this
   *  reason, when __cache_hash_code is set to false the hash functor must
   *  not throw and this is enforced by a static assertion.
   *
   *  Functionality is implemented by decomposition into base classes,
   *  where the derived _Hashtable class is used in _Map_base,
   *  _Insert, _Rehash_base, and _Equality base classes to access the
   *  "this" pointer. _Hashtable_base is used in the base classes as a
   *  non-recursive, fully-completed-type so that detailed nested type
   *  information, such as iterator type and node type, can be
   *  used. This is similar to the "Curiously Recurring Template
   *  Pattern" (CRTP) technique, but uses a reconstructed, not
   *  explicitly passed, template pattern.
   *
   *  Base class templates are:
   *    - __detail::_Hashtable_base
   *    - __detail::_Map_base
   *    - __detail::_Insert
   *    - __detail::_Rehash_base
   *    - __detail::_Equality
   */
  template<typename _Key, typename _Value, typename _Alloc,
	   typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash,
	   typename _RehashPolicy, typename _Traits>
    class _Hashtable
    : public __detail::_Hashtable_base<_Key, _Value, _ExtractKey, _Equal,
				       _H1, _H2, _Hash, _Traits>,
      public __detail::_Map_base<_Key, _Value, _Alloc, _ExtractKey, _Equal,
				 _H1, _H2, _Hash, _RehashPolicy, _Traits>,
      public __detail::_Insert<_Key, _Value, _Alloc, _ExtractKey, _Equal,
			       _H1, _H2, _Hash, _RehashPolicy, _Traits>,
      public __detail::_Rehash_base<_Key, _Value, _Alloc, _ExtractKey, _Equal,
				    _H1, _H2, _Hash, _RehashPolicy, _Traits>,
      public __detail::_Equality<_Key, _Value, _Alloc, _ExtractKey, _Equal,
				 _H1, _H2, _Hash, _RehashPolicy, _Traits>,
      private __detail::_Hashtable_alloc<
	typename __alloctr_rebind<_Alloc,
	  __detail::_Hash_node<_Value,
			       _Traits::__hash_cached::value> >::__type>
    {
      using __traits_type = _Traits;
      using __hash_cached = typename __traits_type::__hash_cached;
      using __node_type = __detail::_Hash_node<_Value, __hash_cached::value>;
      using __node_alloc_type =
	typename __alloctr_rebind<_Alloc, __node_type>::__type;

      using __hashtable_alloc = __detail::_Hashtable_alloc<__node_alloc_type>;

      using __value_alloc_traits =
	typename __hashtable_alloc::__value_alloc_traits;
      using __node_alloc_traits =
	typename __hashtable_alloc::__node_alloc_traits;
      using __node_base = typename __hashtable_alloc::__node_base;
      using __bucket_type = typename __hashtable_alloc::__bucket_type;

    public:
      typedef _Key						key_type;
      typedef _Value						value_type;
      typedef _Alloc						allocator_type;
      typedef _Equal						key_equal;

      // mapped_type, if present, comes from _Map_base.
      // hasher, if present, comes from _Hash_code_base/_Hashtable_base.
      typedef typename __value_alloc_traits::pointer		pointer;
      typedef typename __value_alloc_traits::const_pointer	const_pointer;
      typedef value_type&					reference;
      typedef const value_type&					const_reference;

    private:
      using __rehash_type = _RehashPolicy;
      using __rehash_state = typename __rehash_type::_State;

      using __constant_iterators = typename __traits_type::__constant_iterators;
      using __unique_keys = typename __traits_type::__unique_keys;

      using __key_extract = typename conditional<
					     __constant_iterators::value,
				       	     __detail::_Identity,
					     __detail::_Select1st>::type;

      using __hashtable_base = __detail::
			       _Hashtable_base<_Key, _Value, _ExtractKey,
					      _Equal, _H1, _H2, _Hash, _Traits>;

      using __hash_code_base =  typename __hashtable_base::__hash_code_base;
      using __hash_code =  typename __hashtable_base::__hash_code;
      using __ireturn_type = typename __hashtable_base::__ireturn_type;

      using __map_base = __detail::_Map_base<_Key, _Value, _Alloc, _ExtractKey,
					     _Equal, _H1, _H2, _Hash,
					     _RehashPolicy, _Traits>;

      using __rehash_base = __detail::_Rehash_base<_Key, _Value, _Alloc,
						   _ExtractKey, _Equal,
						   _H1, _H2, _Hash,
						   _RehashPolicy, _Traits>;

      using __eq_base = __detail::_Equality<_Key, _Value, _Alloc, _ExtractKey,
					    _Equal, _H1, _H2, _Hash,
					    _RehashPolicy, _Traits>;

      using __reuse_or_alloc_node_type =
	__detail::_ReuseOrAllocNode<__node_alloc_type>;

      // Metaprogramming for picking apart hash caching.
      template<typename _Cond>
	using __if_hash_cached = std::disjunction<__not_<__hash_cached>, _Cond>;

      template<typename _Cond>
	using __if_hash_not_cached = std::disjunction<__hash_cached, _Cond>;

      // Compile-time diagnostics.

      // _Hash_code_base has everything protected, so use this derived type to
      // access it.
      struct __hash_code_base_access : __hash_code_base
      { using __hash_code_base::_M_bucket_index; };

      // Getting a bucket index from a node shall not throw because it is used
      // in methods (erase, swap...) that shall not throw.
      static_assert(noexcept(std::declval<const __hash_code_base_access&>()
			     ._M_bucket_index((const __node_type*)nullptr,
					      (std::size_t)0)),
		    "Cache the hash code or qualify your functors involved"
		    " in hash code and bucket index computation with noexcept");

      // Following two static assertions are necessary to guarantee
      // that local_iterator will be default constructible.

      // When hash codes are cached local iterator inherits from H2 functor
      // which must then be default constructible.
      static_assert(__if_hash_cached<std::is_default_constructible<_H2>>::value,
		    "Functor used to map hash code to bucket index"
		    " must be default constructible");

      template<typename _Keya, typename _Valuea, typename _Alloca,
	       typename _ExtractKeya, typename _Equala,
	       typename _H1a, typename _H2a, typename _Hasha,
	       typename _RehashPolicya, typename _Traitsa,
	       bool _Unique_keysa>
	friend struct __detail::_Map_base;

      template<typename _Keya, typename _Valuea, typename _Alloca,
	       typename _ExtractKeya, typename _Equala,
	       typename _H1a, typename _H2a, typename _Hasha,
	       typename _RehashPolicya, typename _Traitsa>
	friend struct __detail::_Insert_base;

      template<typename _Keya, typename _Valuea, typename _Alloca,
	       typename _ExtractKeya, typename _Equala,
	       typename _H1a, typename _H2a, typename _Hasha,
	       typename _RehashPolicya, typename _Traitsa,
	       bool _Constant_iteratorsa, bool _Unique_keysa>
	friend struct __detail::_Insert;

    public:
      using size_type = typename __hashtable_base::size_type;
      using difference_type = typename __hashtable_base::difference_type;

      using iterator = typename __hashtable_base::iterator;
      using const_iterator = typename __hashtable_base::const_iterator;

      using local_iterator = typename __hashtable_base::local_iterator;
      using const_local_iterator = typename __hashtable_base::
				   const_local_iterator;

    private:
      __bucket_type*		_M_buckets;
      size_type			_M_bucket_count;
      __node_base		_M_before_begin;
      size_type			_M_element_count;
      _RehashPolicy		_M_rehash_policy;

      // A single bucket used when only need for 1 bucket. Especially
      // interesting in move semantic to leave hashtable with only 1 buckets
      // which is not allocated so that we can have those operations noexcept
      // qualified.
      // Note that we can't leave hashtable with 0 bucket without adding
      // numerous checks in the code to avoid 0 modulus.
      __bucket_type		_M_single_bucket;

      bool
      _M_uses_single_bucket(__bucket_type* __bkts) const
      { return __builtin_expect(__bkts == &_M_single_bucket, false); }

      bool
      _M_uses_single_bucket() const
      { return _M_uses_single_bucket(_M_buckets); }

      __hashtable_alloc&
      _M_base_alloc() { return *this; }

      __bucket_type*
      _M_allocate_buckets(size_type __n)
      {
	if (__builtin_expect(__n == 1, false))
	  {
	    _M_single_bucket = nullptr;
	    return &_M_single_bucket;
	  }

	return __hashtable_alloc::_M_allocate_buckets(__n);
      }

      void
      _M_deallocate_buckets(__bucket_type* __bkts, size_type __n)
      {
	if (_M_uses_single_bucket(__bkts))
	  return;

	__hashtable_alloc::_M_deallocate_buckets(__bkts, __n);
      }

      void
      _M_deallocate_buckets()
      { _M_deallocate_buckets(_M_buckets, _M_bucket_count); }

      // Gets bucket begin, deals with the fact that non-empty buckets contain
      // their before begin node.
      __node_type*
      _M_bucket_begin(size_type __bkt) const;

      __node_type*
      _M_begin() const
      { return static_cast<__node_type*>(_M_before_begin._M_nxt); }

      template<typename _NodeGenerator>
	void
	_M_assign(const _Hashtable&, const _NodeGenerator&);

      void
      _M_move_assign(_Hashtable&&, geode::stl::true_type);

      void
      _M_move_assign(_Hashtable&&, geode::stl::false_type);

      void
      _M_reset() noexcept;

    public:
      // Constructor, destructor, assignment, swap
      _Hashtable(size_type __bucket_hint,
		 const _H1&, const _H2&, const _Hash&,
		 const _Equal&, const _ExtractKey&,
		 const allocator_type&);

      template<typename _InputIterator>
	_Hashtable(_InputIterator __first, _InputIterator __last,
		   size_type __bucket_hint,
		   const _H1&, const _H2&, const _Hash&,
		   const _Equal&, const _ExtractKey&,
		   const allocator_type&);

      _Hashtable(const _Hashtable&);

      _Hashtable(_Hashtable&&) noexcept;

      _Hashtable(const _Hashtable&, const allocator_type&);

      _Hashtable(_Hashtable&&, const allocator_type&);

      // Use delegating constructors.
      explicit
      _Hashtable(const allocator_type& __a)
      : _Hashtable(10, _H1(), _H2(), _Hash(), key_equal(),
		   __key_extract(), __a)
      { }

      explicit
      _Hashtable(size_type __n = 10,
		 const _H1& __hf = _H1(),
		 const key_equal& __eql = key_equal(),
		 const allocator_type& __a = allocator_type())
      : _Hashtable(__n, __hf, _H2(), _Hash(), __eql,
		   __key_extract(), __a)
      { }

      template<typename _InputIterator>
	_Hashtable(_InputIterator __f, _InputIterator __l,
		   size_type __n = 0,
		   const _H1& __hf = _H1(),
		   const key_equal& __eql = key_equal(),
		   const allocator_type& __a = allocator_type())
	: _Hashtable(__f, __l, __n, __hf, _H2(), _Hash(), __eql,
		     __key_extract(), __a)
	{ }

      _Hashtable(initializer_list<value_type> __l,
		 size_type __n = 0,
		 const _H1& __hf = _H1(),
		 const key_equal& __eql = key_equal(),
		 const allocator_type& __a = allocator_type())
      : _Hashtable(__l.begin(), __l.end(), __n, __hf, _H2(), _Hash(), __eql,
		   __key_extract(), __a)
      { }

      _Hashtable&
      operator=(const _Hashtable& __ht);

      _Hashtable&
      operator=(_Hashtable&& __ht)
      noexcept(__node_alloc_traits::_S_nothrow_move())
      {
        constexpr bool __move_storage =
          __node_alloc_traits::_S_propagate_on_move_assign()
          || __node_alloc_traits::_S_always_equal();
        _M_move_assign(std::move(__ht),
                       integral_constant<bool, __move_storage>());
	return *this;
      }

      _Hashtable&
      operator=(initializer_list<value_type> __l)
      {
	__reuse_or_alloc_node_type __roan(_M_begin(), *this);
	_M_before_begin._M_nxt = nullptr;
	clear();
	this->_M_insert_range(__l.begin(), __l.end(), __roan);
	return *this;
      }

      ~_Hashtable() noexcept;

      void
      swap(_Hashtable&)
      noexcept(__node_alloc_traits::_S_nothrow_swap());

      // Basic container operations
      iterator
      begin() noexcept
      { return iterator(_M_begin()); }

      const_iterator
      begin() const noexcept
      { return const_iterator(_M_begin()); }

      iterator
      end() noexcept
      { return iterator(nullptr); }

      const_iterator
      end() const noexcept
      { return const_iterator(nullptr); }

      const_iterator
      cbegin() const noexcept
      { return const_iterator(_M_begin()); }

      const_iterator
      cend() const noexcept
      { return const_iterator(nullptr); }

      size_type
      size() const noexcept
      { return _M_element_count; }

      bool
      empty() const noexcept
      { return size() == 0; }

      allocator_type
      get_allocator() const noexcept
      { return allocator_type(this->_M_node_allocator()); }

      size_type
      max_size() const noexcept
      { return __node_alloc_traits::max_size(this->_M_node_allocator()); }

      // Observers
      key_equal
      key_eq() const
      { return this->_M_eq(); }

      // hash_function, if present, comes from _Hash_code_base.

      // Bucket operations
      size_type
      bucket_count() const noexcept
      { return _M_bucket_count; }

      size_type
      max_bucket_count() const noexcept
      { return max_size(); }

      size_type
      bucket_size(size_type __n) const
      { return std::distance(begin(__n), end(__n)); }

      size_type
      bucket(const key_type& __k) const
      { return _M_bucket_index(__k, this->_M_hash_code(__k)); }

      local_iterator
      begin(size_type __n)
      {
	return local_iterator(*this, _M_bucket_begin(__n),
			      __n, _M_bucket_count);
      }

      local_iterator
      end(size_type __n)
      { return local_iterator(*this, nullptr, __n, _M_bucket_count); }

      const_local_iterator
      begin(size_type __n) const
      {
	return const_local_iterator(*this, _M_bucket_begin(__n),
				    __n, _M_bucket_count);
      }

      const_local_iterator
      end(size_type __n) const
      { return const_local_iterator(*this, nullptr, __n, _M_bucket_count); }

      // DR 691.
      const_local_iterator
      cbegin(size_type __n) const
      {
	return const_local_iterator(*this, _M_bucket_begin(__n),
				    __n, _M_bucket_count);
      }

      const_local_iterator
      cend(size_type __n) const
      { return const_local_iterator(*this, nullptr, __n, _M_bucket_count); }

      float
      load_factor() const noexcept
      {
	return static_cast<float>(size()) / static_cast<float>(bucket_count());
      }

      // max_load_factor, if present, comes from _Rehash_base.

      // Generalization of max_load_factor.  Extension, not found in
      // TR1.  Only useful if _RehashPolicy is something other than
      // the default.
      const _RehashPolicy&
      __rehash_policy() const
      { return _M_rehash_policy; }

      void
      __rehash_policy(const _RehashPolicy&);

      // Lookup.
      iterator
      find(const key_type& __k);

      const_iterator
      find(const key_type& __k) const;

      size_type
      count(const key_type& __k) const;

      pair<iterator, iterator>
      equal_range(const key_type& __k);

      pair<const_iterator, const_iterator>
      equal_range(const key_type& __k) const;

    protected:
      // Bucket index computation helpers.
      size_type
      _M_bucket_index(__node_type* __n) const noexcept
      { return __hash_code_base::_M_bucket_index(__n, _M_bucket_count); }

      size_type
      _M_bucket_index(const key_type& __k, __hash_code __c) const
      { return __hash_code_base::_M_bucket_index(__k, __c, _M_bucket_count); }

      // Find and insert helper functions and types
      // Find the node before the one matching the criteria.
      __node_base*
      _M_find_before_node(size_type, const key_type&, __hash_code) const;

      __node_type*
      _M_find_node(size_type __bkt, const key_type& __key,
		   __hash_code __c) const
      {
	__node_base* __before_n = _M_find_before_node(__bkt, __key, __c);
	if (__before_n)
	  return static_cast<__node_type*>(__before_n->_M_nxt);
	return nullptr;
      }

      // Insert a node at the beginning of a bucket.
      void
      _M_insert_bucket_begin(size_type, __node_type*);

      // Remove the bucket first node
      void
      _M_remove_bucket_begin(size_type __bkt, __node_type* __next_n,
			     size_type __next_bkt);

      // Get the node before __n in the bucket __bkt
      __node_base*
      _M_get_previous_node(size_type __bkt, __node_base* __n);

      // Insert node with hash code __code, in bucket bkt if no rehash (assumes
      // no element with its key already present). Take ownership of the node,
      // deallocate it on exception.
      iterator
      _M_insert_unique_node(size_type __bkt, __hash_code __code,
			    __node_type* __n);

      // Insert node with hash code __code. Take ownership of the node,
      // deallocate it on exception.
      iterator
      _M_insert_multi_node(__node_type* __hint,
			   __hash_code __code, __node_type* __n);

      template<typename... _Args>
	pair<iterator, bool>
	_M_emplace(geode::stl::true_type, _Args&&... __args);

      template<typename... _Args>
	iterator
	_M_emplace(geode::stl::false_type __uk, _Args&&... __args)
	{ return _M_emplace(cend(), __uk, std::forward<_Args>(__args)...); }

      // Emplace with hint, useless when keys are unique.
      template<typename... _Args>
	iterator
	_M_emplace(const_iterator, geode::stl::true_type __uk, _Args&&... __args)
	{ return _M_emplace(__uk, std::forward<_Args>(__args)...).first; }

      template<typename... _Args>
	iterator
	_M_emplace(const_iterator, geode::stl::false_type, _Args&&... __args);

      template<typename _Arg, typename _NodeGenerator>
	pair<iterator, bool>
	_M_insert(_Arg&&, const _NodeGenerator&, geode::stl::true_type);

      template<typename _Arg, typename _NodeGenerator>
	iterator
	_M_insert(_Arg&& __arg, const _NodeGenerator& __node_gen,
		  geode::stl::false_type __uk)
	{
	  return _M_insert(cend(), std::forward<_Arg>(__arg), __node_gen,
			   __uk);
	}

      // Insert with hint, not used when keys are unique.
      template<typename _Arg, typename _NodeGenerator>
	iterator
	_M_insert(const_iterator, _Arg&& __arg, const _NodeGenerator& __node_gen,
		  geode::stl::true_type __uk)
	{
	  return
	    _M_insert(std::forward<_Arg>(__arg), __node_gen, __uk).first;
	}

      // Insert with hint when keys are not unique.
      template<typename _Arg, typename _NodeGenerator>
	iterator
	_M_insert(const_iterator, _Arg&&, const _NodeGenerator&, geode::stl::false_type);

      size_type
      _M_erase(geode::stl::true_type, const key_type&);

      size_type
      _M_erase(geode::stl::false_type, const key_type&);

      iterator
      _M_erase(size_type __bkt, __node_base* __prev_n, __node_type* __n);

    public:
      // Emplace
      template<typename... _Args>
	__ireturn_type
	emplace(_Args&&... __args)
	{ return _M_emplace(__unique_keys(), std::forward<_Args>(__args)...); }

      template<typename... _Args>
	iterator
	emplace_hint(const_iterator __hint, _Args&&... __args)
	{
	  return _M_emplace(__hint, __unique_keys(),
			    std::forward<_Args>(__args)...);
	}

      // Insert member functions via inheritance.

      // Erase
      iterator
      erase(const_iterator);

      // LWG 2059.
      iterator
      erase(iterator __it)
      { return erase(const_iterator(__it)); }

      size_type
      erase(const key_type& __k)
      { return _M_erase(__unique_keys(), __k); }

      iterator
      erase(const_iterator, const_iterator);

      void
      clear() noexcept;

      // Set number of buckets to be appropriate for container of n element.
      void rehash(size_type __n);

      // DR 1189.
      // reserve, if present, comes from _Rehash_base.

    private:
      // Helper rehash method used when keys are unique.
      void _M_rehash_aux(size_type __n, geode::stl::true_type);

      // Helper rehash method used when keys can be non-unique.
      void _M_rehash_aux(size_type __n, geode::stl::false_type);

      // Unconditionally change size of bucket array to n, restore
      // hash policy state to __state on exception.
      void _M_rehash(size_type __n, const __rehash_state& __state);
    };


  // Definitions of class template _Hashtable's out-of-line member functions.
  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey,
			_Equal, _H1, _H2, _Hash, _RehashPolicy,
			_Traits>::__node_type*
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    _M_bucket_begin(size_type __bkt) const
    {
      __node_base* __n = _M_buckets[__bkt];
      return __n ? static_cast<__node_type*>(__n->_M_nxt) : nullptr;
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    _Hashtable(size_type __bucket_hint,
	       const _H1& __h1, const _H2& __h2, const _Hash& __h,
	       const _Equal& __eq, const _ExtractKey& __exk,
	       const allocator_type& __a)
    : __hashtable_base(__exk, __h1, __h2, __h, __eq),
      __map_base(),
      __rehash_base(),
      __hashtable_alloc(__node_alloc_type(__a)),
      _M_element_count(0),
      _M_rehash_policy()
    {
      _M_bucket_count = _M_rehash_policy._M_next_bkt(__bucket_hint);
      _M_buckets = _M_allocate_buckets(_M_bucket_count);
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    template<typename _InputIterator>
      _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
		 _H1, _H2, _Hash, _RehashPolicy, _Traits>::
      _Hashtable(_InputIterator __f, _InputIterator __l,
		 size_type __bucket_hint,
		 const _H1& __h1, const _H2& __h2, const _Hash& __h,
		 const _Equal& __eq, const _ExtractKey& __exk,
		 const allocator_type& __a)
      : __hashtable_base(__exk, __h1, __h2, __h, __eq),
	__map_base(),
	__rehash_base(),
	__hashtable_alloc(__node_alloc_type(__a)),
	_M_element_count(0),
	_M_rehash_policy()
      {
	auto __nb_elems = __detail::__distance_fw(__f, __l);
	_M_bucket_count =
	  _M_rehash_policy._M_next_bkt(
	    std::max(_M_rehash_policy._M_bkt_for_elements(__nb_elems),
		     __bucket_hint));

	_M_buckets = _M_allocate_buckets(_M_bucket_count);
	__try
	  {
	    for (; __f != __l; ++__f)
	      this->insert(*__f);
	  }
	__catch(...)
	  {
	    clear();
	    _M_deallocate_buckets();
	    __throw_exception_again;
	  }
      }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>&
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::operator=(
		const _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
				 _H1, _H2, _Hash, _RehashPolicy, _Traits>& __ht)
      {
	if (&__ht == this)
	  return *this;

	if (__node_alloc_traits::_S_propagate_on_copy_assign())
	  {
	    auto& __this_alloc = this->_M_node_allocator();
	    auto& __that_alloc = __ht._M_node_allocator();
	    if (!__node_alloc_traits::_S_always_equal()
		&& __this_alloc != __that_alloc)
	      {
		// Replacement allocator cannot free existing storage.
		this->_M_deallocate_nodes(_M_begin());
		_M_before_begin._M_nxt = nullptr;
		_M_deallocate_buckets();
		_M_buckets = nullptr;
		__alloc_on_copy(__this_alloc, __that_alloc);
		__hashtable_base::operator=(__ht);
		_M_bucket_count = __ht._M_bucket_count;
		_M_element_count = __ht._M_element_count;
		_M_rehash_policy = __ht._M_rehash_policy;
		__try
		  {
		    _M_assign(__ht,
			      [this](const __node_type* __n)
			      { return this->_M_allocate_node(__n->_M_v()); });
		  }
		__catch(...)
		  {
		    // _M_assign took care of deallocating all memory. Now we
		    // must make sure this instance remains in a usable state.
		    _M_reset();
		    __throw_exception_again;
		  }
		return *this;
	      }
	    __alloc_on_copy(__this_alloc, __that_alloc);
	  }

	// Reuse allocated buckets and nodes.
	__bucket_type* __former_buckets = nullptr;
	std::size_t __former_bucket_count = _M_bucket_count;
	const __rehash_state& __former_state = _M_rehash_policy._M_state();

	if (_M_bucket_count != __ht._M_bucket_count)
	  {
	    __former_buckets = _M_buckets;
	    _M_buckets = _M_allocate_buckets(__ht._M_bucket_count);
	    _M_bucket_count = __ht._M_bucket_count;
	  }
	else
	  __builtin_memset(_M_buckets, 0,
			   _M_bucket_count * sizeof(__bucket_type));

	__try
	  {
	    __hashtable_base::operator=(__ht);
	    _M_element_count = __ht._M_element_count;
	    _M_rehash_policy = __ht._M_rehash_policy;
	    __reuse_or_alloc_node_type __roan(_M_begin(), *this);
	    _M_before_begin._M_nxt = nullptr;
	    _M_assign(__ht,
		      [&__roan](const __node_type* __n)
		      { return __roan(__n->_M_v()); });
	    if (__former_buckets)
	      _M_deallocate_buckets(__former_buckets, __former_bucket_count);
	  }
	__catch(...)
	  {
	    if (__former_buckets)
	      {
		// Restore previous buckets.
		_M_deallocate_buckets();
		_M_rehash_policy._M_reset(__former_state);
		_M_buckets = __former_buckets;
		_M_bucket_count = __former_bucket_count;
	      }
	    __builtin_memset(_M_buckets, 0,
			     _M_bucket_count * sizeof(__bucket_type));
	    __throw_exception_again;
	  }
	return *this;
      }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    template<typename _NodeGenerator>
      void
      _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
		 _H1, _H2, _Hash, _RehashPolicy, _Traits>::
      _M_assign(const _Hashtable& __ht, const _NodeGenerator& __node_gen)
      {
	__bucket_type* __buckets = nullptr;
	if (!_M_buckets)
	  _M_buckets = __buckets = _M_allocate_buckets(_M_bucket_count);

	__try
	  {
	    if (!__ht._M_before_begin._M_nxt)
	      return;

	    // First deal with the special first node pointed to by
	    // _M_before_begin.
	    __node_type* __ht_n = __ht._M_begin();
	    __node_type* __this_n = __node_gen(__ht_n);
	    this->_M_copy_code(__this_n, __ht_n);
	    _M_before_begin._M_nxt = __this_n;
	    _M_buckets[_M_bucket_index(__this_n)] = &_M_before_begin;

	    // Then deal with other nodes.
	    __node_base* __prev_n = __this_n;
	    for (__ht_n = __ht_n->_M_next(); __ht_n; __ht_n = __ht_n->_M_next())
	      {
		__this_n = __node_gen(__ht_n);
		__prev_n->_M_nxt = __this_n;
		this->_M_copy_code(__this_n, __ht_n);
		size_type __bkt = _M_bucket_index(__this_n);
		if (!_M_buckets[__bkt])
		  _M_buckets[__bkt] = __prev_n;
		__prev_n = __this_n;
	      }
	  }
	__catch(...)
	  {
	    clear();
	    if (__buckets)
	      _M_deallocate_buckets();
	    __throw_exception_again;
	  }
      }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    void
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    _M_reset() noexcept
    {
      _M_rehash_policy._M_reset();
      _M_bucket_count = 1;
      _M_single_bucket = nullptr;
      _M_buckets = &_M_single_bucket;
      _M_before_begin._M_nxt = nullptr;
      _M_element_count = 0;
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    void
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    _M_move_assign(_Hashtable&& __ht, geode::stl::true_type)
    {
      this->_M_deallocate_nodes(_M_begin());
      _M_deallocate_buckets();
      __hashtable_base::operator=(std::move(__ht));
      _M_rehash_policy = __ht._M_rehash_policy;
      if (!__ht._M_uses_single_bucket())
	_M_buckets = __ht._M_buckets;
      else
	{
	  _M_buckets = &_M_single_bucket;
	  _M_single_bucket = __ht._M_single_bucket;
	}
      _M_bucket_count = __ht._M_bucket_count;
      _M_before_begin._M_nxt = __ht._M_before_begin._M_nxt;
      _M_element_count = __ht._M_element_count;
      __alloc_on_move(this->_M_node_allocator(), __ht._M_node_allocator());

      // Fix buckets containing the _M_before_begin pointers that can't be
      // moved.
      if (_M_begin())
	_M_buckets[_M_bucket_index(_M_begin())] = &_M_before_begin;
      __ht._M_reset();
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    void
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    _M_move_assign(_Hashtable&& __ht, geode::stl::false_type)
    {
      if (__ht._M_node_allocator() == this->_M_node_allocator())
	_M_move_assign(std::move(__ht), geode::stl::true_type());
      else
	{
	  // Can't move memory, move elements then.
	  __bucket_type* __former_buckets = nullptr;
	  size_type __former_bucket_count = _M_bucket_count;
	  const __rehash_state& __former_state = _M_rehash_policy._M_state();

	  if (_M_bucket_count != __ht._M_bucket_count)
	    {
	      __former_buckets = _M_buckets;
	      _M_buckets = _M_allocate_buckets(__ht._M_bucket_count);
	      _M_bucket_count = __ht._M_bucket_count;
	    }
	  else
	    __builtin_memset(_M_buckets, 0,
			     _M_bucket_count * sizeof(__bucket_type));

	  __try
	    {
	      __hashtable_base::operator=(std::move(__ht));
	      _M_element_count = __ht._M_element_count;
	      _M_rehash_policy = __ht._M_rehash_policy;
	      __reuse_or_alloc_node_type __roan(_M_begin(), *this);
	      _M_before_begin._M_nxt = nullptr;
	      _M_assign(__ht,
			[&__roan](__node_type* __n)
			{ return __roan(std::move_if_noexcept(__n->_M_v())); });
	      __ht.clear();
	    }
	  __catch(...)
	    {
	      if (__former_buckets)
		{
		  _M_deallocate_buckets();
		  _M_rehash_policy._M_reset(__former_state);
		  _M_buckets = __former_buckets;
		  _M_bucket_count = __former_bucket_count;
		}
	      __builtin_memset(_M_buckets, 0,
			       _M_bucket_count * sizeof(__bucket_type));
	      __throw_exception_again;
	    }
	}
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    _Hashtable(const _Hashtable& __ht)
    : __hashtable_base(__ht),
      __map_base(__ht),
      __rehash_base(__ht),
      __hashtable_alloc(
	__node_alloc_traits::_S_select_on_copy(__ht._M_node_allocator())),
      _M_buckets(),
      _M_bucket_count(__ht._M_bucket_count),
      _M_element_count(__ht._M_element_count),
      _M_rehash_policy(__ht._M_rehash_policy)
    {
      _M_assign(__ht,
		[this](const __node_type* __n)
		{ return this->_M_allocate_node(__n->_M_v()); });
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    _Hashtable(_Hashtable&& __ht) noexcept
    : __hashtable_base(__ht),
      __map_base(__ht),
      __rehash_base(__ht),
      __hashtable_alloc(std::move(__ht._M_base_alloc())),
      _M_buckets(__ht._M_buckets),
      _M_bucket_count(__ht._M_bucket_count),
      _M_before_begin(__ht._M_before_begin._M_nxt),
      _M_element_count(__ht._M_element_count),
      _M_rehash_policy(__ht._M_rehash_policy)
    {
      // Update, if necessary, buckets if __ht is using its single bucket.
      if (__ht._M_uses_single_bucket())
	{
	  _M_buckets = &_M_single_bucket;
	  _M_single_bucket = __ht._M_single_bucket;
	}

      // Update, if necessary, bucket pointing to before begin that hasn't
      // moved.
      if (_M_begin())
	_M_buckets[_M_bucket_index(_M_begin())] = &_M_before_begin;

      __ht._M_reset();
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    _Hashtable(const _Hashtable& __ht, const allocator_type& __a)
    : __hashtable_base(__ht),
      __map_base(__ht),
      __rehash_base(__ht),
      __hashtable_alloc(__node_alloc_type(__a)),
      _M_buckets(),
      _M_bucket_count(__ht._M_bucket_count),
      _M_element_count(__ht._M_element_count),
      _M_rehash_policy(__ht._M_rehash_policy)
    {
      _M_assign(__ht,
		[this](const __node_type* __n)
		{ return this->_M_allocate_node(__n->_M_v()); });
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    _Hashtable(_Hashtable&& __ht, const allocator_type& __a)
    : __hashtable_base(__ht),
      __map_base(__ht),
      __rehash_base(__ht),
      __hashtable_alloc(__node_alloc_type(__a)),
      _M_buckets(),
      _M_bucket_count(__ht._M_bucket_count),
      _M_element_count(__ht._M_element_count),
      _M_rehash_policy(__ht._M_rehash_policy)
    {
      if (__ht._M_node_allocator() == this->_M_node_allocator())
	{
	  if (__ht._M_uses_single_bucket())
	    {
	      _M_buckets = &_M_single_bucket;
	      _M_single_bucket = __ht._M_single_bucket;
	    }
	  else
	    _M_buckets = __ht._M_buckets;

	  _M_before_begin._M_nxt = __ht._M_before_begin._M_nxt;
	  // Update, if necessary, bucket pointing to before begin that hasn't
	  // moved.
	  if (_M_begin())
	    _M_buckets[_M_bucket_index(_M_begin())] = &_M_before_begin;
	  __ht._M_reset();
	}
      else
	{
	  _M_assign(__ht,
		    [this](__node_type* __n)
		    {
		      return this->_M_allocate_node(
					std::move_if_noexcept(__n->_M_v()));
		    });
	  __ht.clear();
	}
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    ~_Hashtable() noexcept
    {
      clear();
      if (_M_buckets)
	_M_deallocate_buckets();
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    void
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    swap(_Hashtable& __x)
    noexcept(__node_alloc_traits::_S_nothrow_swap())
    {
      // The only base class with member variables is hash_code_base.
      // We define _Hash_code_base::_M_swap because different
      // specializations have different members.
      this->_M_swap(__x);

      __alloc_on_swap(this->_M_node_allocator(), __x._M_node_allocator());
      swap(_M_rehash_policy, __x._M_rehash_policy);

      // Deal properly with potentially moved instances.
      if (this->_M_uses_single_bucket())
	{
	  if (!__x._M_uses_single_bucket())
	    {
	      _M_buckets = __x._M_buckets;
	      __x._M_buckets = &__x._M_single_bucket;
	    }
	}
      else if (__x._M_uses_single_bucket())
	{
	  __x._M_buckets = _M_buckets;
	  _M_buckets = &_M_single_bucket;
	}
      else
	swap(_M_buckets, __x._M_buckets);

      swap(_M_bucket_count, __x._M_bucket_count);
      swap(_M_before_begin._M_nxt, __x._M_before_begin._M_nxt);
      swap(_M_element_count, __x._M_element_count);
      swap(_M_single_bucket, __x._M_single_bucket);

      // Fix buckets containing the _M_before_begin pointers that can't be
      // swapped.
      if (_M_begin())
	_M_buckets[_M_bucket_index(_M_begin())] = &_M_before_begin;

      if (__x._M_begin())
	__x._M_buckets[__x._M_bucket_index(__x._M_begin())]
	  = &__x._M_before_begin;
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    void
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    __rehash_policy(const _RehashPolicy& __pol)
    {
      auto __do_rehash =
	__pol._M_need_rehash(_M_bucket_count, _M_element_count, 0);
      if (__do_rehash.first)
	_M_rehash(__do_rehash.second, _M_rehash_policy._M_state());
      _M_rehash_policy = __pol;
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
			_H1, _H2, _Hash, _RehashPolicy,
			_Traits>::iterator
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    find(const key_type& __k)
    {
      __hash_code __code = this->_M_hash_code(__k);
      std::size_t __n = _M_bucket_index(__k, __code);
      __node_type* __p = _M_find_node(__n, __k, __code);
      return __p ? iterator(__p) : end();
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
			_H1, _H2, _Hash, _RehashPolicy,
			_Traits>::const_iterator
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    find(const key_type& __k) const
    {
      __hash_code __code = this->_M_hash_code(__k);
      std::size_t __n = _M_bucket_index(__k, __code);
      __node_type* __p = _M_find_node(__n, __k, __code);
      return __p ? const_iterator(__p) : end();
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
			_H1, _H2, _Hash, _RehashPolicy,
			_Traits>::size_type
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    count(const key_type& __k) const
    {
      __hash_code __code = this->_M_hash_code(__k);
      std::size_t __n = _M_bucket_index(__k, __code);
      __node_type* __p = _M_bucket_begin(__n);
      if (!__p)
	return 0;

      std::size_t __result = 0;
      for (;; __p = __p->_M_next())
	{
	  if (this->_M_equals(__k, __code, __p))
	    ++__result;
	  else if (__result)
	    // All equivalent values are next to each other, if we
	    // found a non-equivalent value after an equivalent one it
	    // means that we won't find any new equivalent value.
	    break;
	  if (!__p->_M_nxt || _M_bucket_index(__p->_M_next()) != __n)
	    break;
	}
      return __result;
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    pair<typename _Hashtable<_Key, _Value, _Alloc,
				  _ExtractKey, _Equal, _H1,
				  _H2, _Hash, _RehashPolicy,
				  _Traits>::iterator,
	      typename _Hashtable<_Key, _Value, _Alloc,
				  _ExtractKey, _Equal, _H1,
				  _H2, _Hash, _RehashPolicy,
				  _Traits>::iterator>
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    equal_range(const key_type& __k)
    {
      __hash_code __code = this->_M_hash_code(__k);
      std::size_t __n = _M_bucket_index(__k, __code);
      __node_type* __p = _M_find_node(__n, __k, __code);

      if (__p)
	{
	  __node_type* __p1 = __p->_M_next();
	  while (__p1 && _M_bucket_index(__p1) == __n
		 && this->_M_equals(__k, __code, __p1))
	    __p1 = __p1->_M_next();

	  return make_pair(iterator(__p), iterator(__p1));
	}
      else
	return make_pair(end(), end());
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    pair<typename _Hashtable<_Key, _Value, _Alloc,
				  _ExtractKey, _Equal, _H1,
				  _H2, _Hash, _RehashPolicy,
				  _Traits>::const_iterator,
	      typename _Hashtable<_Key, _Value, _Alloc,
				  _ExtractKey, _Equal, _H1,
				  _H2, _Hash, _RehashPolicy,
				  _Traits>::const_iterator>
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    equal_range(const key_type& __k) const
    {
      __hash_code __code = this->_M_hash_code(__k);
      std::size_t __n = _M_bucket_index(__k, __code);
      __node_type* __p = _M_find_node(__n, __k, __code);

      if (__p)
	{
	  __node_type* __p1 = __p->_M_next();
	  while (__p1 && _M_bucket_index(__p1) == __n
		 && this->_M_equals(__k, __code, __p1))
	    __p1 = __p1->_M_next();

	  return make_pair(const_iterator(__p), const_iterator(__p1));
	}
      else
	return make_pair(end(), end());
    }

  // Find the node whose key compares equal to k in the bucket n.
  // Return nullptr if no node is found.
  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey,
			_Equal, _H1, _H2, _Hash, _RehashPolicy,
			_Traits>::__node_base*
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    _M_find_before_node(size_type __n, const key_type& __k,
			__hash_code __code) const
    {
      __node_base* __prev_p = _M_buckets[__n];
      if (!__prev_p)
	return nullptr;

      for (__node_type* __p = static_cast<__node_type*>(__prev_p->_M_nxt);;
	   __p = __p->_M_next())
	{
	  if (this->_M_equals(__k, __code, __p))
	    return __prev_p;

	  if (!__p->_M_nxt || _M_bucket_index(__p->_M_next()) != __n)
	    break;
	  __prev_p = __p;
	}
      return nullptr;
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    void
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    _M_insert_bucket_begin(size_type __bkt, __node_type* __node)
    {
      if (_M_buckets[__bkt])
	{
	  // Bucket is not empty, we just need to insert the new node
	  // after the bucket before begin.
	  __node->_M_nxt = _M_buckets[__bkt]->_M_nxt;
	  _M_buckets[__bkt]->_M_nxt = __node;
	}
      else
	{
	  // The bucket is empty, the new node is inserted at the
	  // beginning of the singly-linked list and the bucket will
	  // contain _M_before_begin pointer.
	  __node->_M_nxt = _M_before_begin._M_nxt;
	  _M_before_begin._M_nxt = __node;
	  if (__node->_M_nxt)
	    // We must update former begin bucket that is pointing to
	    // _M_before_begin.
	    _M_buckets[_M_bucket_index(__node->_M_next())] = __node;
	  _M_buckets[__bkt] = &_M_before_begin;
	}
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    void
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    _M_remove_bucket_begin(size_type __bkt, __node_type* __next,
			   size_type __next_bkt)
    {
      if (!__next || __next_bkt != __bkt)
	{
	  // Bucket is now empty
	  // First update next bucket if any
	  if (__next)
	    _M_buckets[__next_bkt] = _M_buckets[__bkt];

	  // Second update before begin node if necessary
	  if (&_M_before_begin == _M_buckets[__bkt])
	    _M_before_begin._M_nxt = __next;
	  _M_buckets[__bkt] = nullptr;
	}
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey,
			_Equal, _H1, _H2, _Hash, _RehashPolicy,
			_Traits>::__node_base*
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    _M_get_previous_node(size_type __bkt, __node_base* __n)
    {
      __node_base* __prev_n = _M_buckets[__bkt];
      while (__prev_n->_M_nxt != __n)
	__prev_n = __prev_n->_M_nxt;
      return __prev_n;
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    template<typename... _Args>
      pair<typename _Hashtable<_Key, _Value, _Alloc,
				    _ExtractKey, _Equal, _H1,
				    _H2, _Hash, _RehashPolicy,
				    _Traits>::iterator, bool>
      _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
		 _H1, _H2, _Hash, _RehashPolicy, _Traits>::
      _M_emplace(geode::stl::true_type, _Args&&... __args)
      {
	// First build the node to get access to the hash code
	__node_type* __node = this->_M_allocate_node(std::forward<_Args>(__args)...);
	const key_type& __k = this->_M_extract()(__node->_M_v());
	__hash_code __code;
	__try
	  {
	    __code = this->_M_hash_code(__k);
	  }
	__catch(...)
	  {
	    this->_M_deallocate_node(__node);
	    __throw_exception_again;
	  }

	size_type __bkt = _M_bucket_index(__k, __code);
	if (__node_type* __p = _M_find_node(__bkt, __k, __code))
	  {
	    // There is already an equivalent node, no insertion
	    this->_M_deallocate_node(__node);
	    return make_pair(iterator(__p), false);
	  }

	// Insert the node
	return make_pair(_M_insert_unique_node(__bkt, __code, __node),
			      true);
      }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    template<typename... _Args>
      typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
			  _H1, _H2, _Hash, _RehashPolicy,
			  _Traits>::iterator
      _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
		 _H1, _H2, _Hash, _RehashPolicy, _Traits>::
      _M_emplace(const_iterator __hint, geode::stl::false_type, _Args&&... __args)
      {
	// First build the node to get its hash code.
	__node_type* __node =
	  this->_M_allocate_node(std::forward<_Args>(__args)...);

	__hash_code __code;
	__try
	  {
	    __code = this->_M_hash_code(this->_M_extract()(__node->_M_v()));
	  }
	__catch(...)
	  {
	    this->_M_deallocate_node(__node);
	    __throw_exception_again;
	  }

	return _M_insert_multi_node(__hint._M_cur, __code, __node);
      }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
			_H1, _H2, _Hash, _RehashPolicy,
			_Traits>::iterator
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    _M_insert_unique_node(size_type __bkt, __hash_code __code,
			  __node_type* __node)
    {
      const __rehash_state& __saved_state = _M_rehash_policy._M_state();
      pair<bool, std::size_t> __do_rehash
	= _M_rehash_policy._M_need_rehash(_M_bucket_count, _M_element_count, 1);

      __try
	{
	  if (__do_rehash.first)
	    {
	      _M_rehash(__do_rehash.second, __saved_state);
	      __bkt = _M_bucket_index(this->_M_extract()(__node->_M_v()), __code);
	    }

	  this->_M_store_code(__node, __code);

	  // Always insert at the beginning of the bucket.
	  _M_insert_bucket_begin(__bkt, __node);
	  ++_M_element_count;
	  return iterator(__node);
	}
      __catch(...)
	{
	  this->_M_deallocate_node(__node);
	  __throw_exception_again;
	}
    }

  // Insert node, in bucket bkt if no rehash (assumes no element with its key
  // already present). Take ownership of the node, deallocate it on exception.
  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
			_H1, _H2, _Hash, _RehashPolicy,
			_Traits>::iterator
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    _M_insert_multi_node(__node_type* __hint, __hash_code __code,
			 __node_type* __node)
    {
      const __rehash_state& __saved_state = _M_rehash_policy._M_state();
      pair<bool, std::size_t> __do_rehash
	= _M_rehash_policy._M_need_rehash(_M_bucket_count, _M_element_count, 1);

      __try
	{
	  if (__do_rehash.first)
	    _M_rehash(__do_rehash.second, __saved_state);

	  this->_M_store_code(__node, __code);
	  const key_type& __k = this->_M_extract()(__node->_M_v());
	  size_type __bkt = _M_bucket_index(__k, __code);

	  // Find the node before an equivalent one or use hint if it exists and
	  // if it is equivalent.
	  __node_base* __prev
	    = __builtin_expect(__hint != nullptr, false)
	      && this->_M_equals(__k, __code, __hint)
		? __hint
		: _M_find_before_node(__bkt, __k, __code);
	  if (__prev)
	    {
	      // Insert after the node before the equivalent one.
	      __node->_M_nxt = __prev->_M_nxt;
	      __prev->_M_nxt = __node;
	      if (__builtin_expect(__prev == __hint, false))
	      	// hint might be the last bucket node, in this case we need to
	      	// update next bucket.
	      	if (__node->_M_nxt
	      	    && !this->_M_equals(__k, __code, __node->_M_next()))
	      	  {
	      	    size_type __next_bkt = _M_bucket_index(__node->_M_next());
	      	    if (__next_bkt != __bkt)
	      	      _M_buckets[__next_bkt] = __node;
	      	  }
	    }
	  else
	    // The inserted node has no equivalent in the
	    // hashtable. We must insert the new node at the
	    // beginning of the bucket to preserve equivalent
	    // elements' relative positions.
	    _M_insert_bucket_begin(__bkt, __node);
	  ++_M_element_count;
	  return iterator(__node);
	}
      __catch(...)
	{
	  this->_M_deallocate_node(__node);
	  __throw_exception_again;
	}
    }

  // Insert v if no element with its key is already present.
  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    template<typename _Arg, typename _NodeGenerator>
      pair<typename _Hashtable<_Key, _Value, _Alloc,
				    _ExtractKey, _Equal, _H1,
				    _H2, _Hash, _RehashPolicy,
				    _Traits>::iterator, bool>
      _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
		 _H1, _H2, _Hash, _RehashPolicy, _Traits>::
      _M_insert(_Arg&& __v, const _NodeGenerator& __node_gen, geode::stl::true_type)
      {
	const key_type& __k = this->_M_extract()(__v);
	__hash_code __code = this->_M_hash_code(__k);
	size_type __bkt = _M_bucket_index(__k, __code);

	__node_type* __n = _M_find_node(__bkt, __k, __code);
	if (__n)
	  return make_pair(iterator(__n), false);

	__n = __node_gen(std::forward<_Arg>(__v));
	return make_pair(_M_insert_unique_node(__bkt, __code, __n), true);
      }

  // Insert v unconditionally.
  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    template<typename _Arg, typename _NodeGenerator>
      typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
			  _H1, _H2, _Hash, _RehashPolicy,
			  _Traits>::iterator
      _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
		 _H1, _H2, _Hash, _RehashPolicy, _Traits>::
      _M_insert(const_iterator __hint, _Arg&& __v,
		const _NodeGenerator& __node_gen,
		geode::stl::false_type)
      {
	// First compute the hash code so that we don't do anything if it
	// throws.
	__hash_code __code = this->_M_hash_code(this->_M_extract()(__v));

	// Second allocate new node so that we don't rehash if it throws.
	__node_type* __node = __node_gen(std::forward<_Arg>(__v));

	return _M_insert_multi_node(__hint._M_cur, __code, __node);
      }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
			_H1, _H2, _Hash, _RehashPolicy,
			_Traits>::iterator
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    erase(const_iterator __it)
    {
      __node_type* __n = __it._M_cur;
      std::size_t __bkt = _M_bucket_index(__n);

      // Look for previous node to unlink it from the erased one, this
      // is why we need buckets to contain the before begin to make
      // this search fast.
      __node_base* __prev_n = _M_get_previous_node(__bkt, __n);
      return _M_erase(__bkt, __prev_n, __n);
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
			_H1, _H2, _Hash, _RehashPolicy,
			_Traits>::iterator
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    _M_erase(size_type __bkt, __node_base* __prev_n, __node_type* __n)
    {
      if (__prev_n == _M_buckets[__bkt])
	_M_remove_bucket_begin(__bkt, __n->_M_next(),
	   __n->_M_nxt ? _M_bucket_index(__n->_M_next()) : 0);
      else if (__n->_M_nxt)
	{
	  size_type __next_bkt = _M_bucket_index(__n->_M_next());
	  if (__next_bkt != __bkt)
	    _M_buckets[__next_bkt] = __prev_n;
	}

      __prev_n->_M_nxt = __n->_M_nxt;
      iterator __result(__n->_M_next());
      this->_M_deallocate_node(__n);
      --_M_element_count;

      return __result;
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
			_H1, _H2, _Hash, _RehashPolicy,
			_Traits>::size_type
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    _M_erase(geode::stl::true_type, const key_type& __k)
    {
      __hash_code __code = this->_M_hash_code(__k);
      std::size_t __bkt = _M_bucket_index(__k, __code);

      // Look for the node before the first matching node.
      __node_base* __prev_n = _M_find_before_node(__bkt, __k, __code);
      if (!__prev_n)
	return 0;

      // We found a matching node, erase it.
      __node_type* __n = static_cast<__node_type*>(__prev_n->_M_nxt);
      _M_erase(__bkt, __prev_n, __n);
      return 1;
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
			_H1, _H2, _Hash, _RehashPolicy,
			_Traits>::size_type
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    _M_erase(geode::stl::false_type, const key_type& __k)
    {
      __hash_code __code = this->_M_hash_code(__k);
      std::size_t __bkt = _M_bucket_index(__k, __code);

      // Look for the node before the first matching node.
      __node_base* __prev_n = _M_find_before_node(__bkt, __k, __code);
      if (!__prev_n)
	return 0;

      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 526. Is it undefined if a function in the standard changes
      // in parameters?
      // We use one loop to find all matching nodes and another to deallocate
      // them so that the key stays valid during the first loop. It might be
      // invalidated indirectly when destroying nodes.
      __node_type* __n = static_cast<__node_type*>(__prev_n->_M_nxt);
      __node_type* __n_last = __n;
      std::size_t __n_last_bkt = __bkt;
      do
	{
	  __n_last = __n_last->_M_next();
	  if (!__n_last)
	    break;
	  __n_last_bkt = _M_bucket_index(__n_last);
	}
      while (__n_last_bkt == __bkt && this->_M_equals(__k, __code, __n_last));

      // Deallocate nodes.
      size_type __result = 0;
      do
	{
	  __node_type* __p = __n->_M_next();
	  this->_M_deallocate_node(__n);
	  __n = __p;
	  ++__result;
	  --_M_element_count;
	}
      while (__n != __n_last);

      if (__prev_n == _M_buckets[__bkt])
	_M_remove_bucket_begin(__bkt, __n_last, __n_last_bkt);
      else if (__n_last && __n_last_bkt != __bkt)
	_M_buckets[__n_last_bkt] = __prev_n;
      __prev_n->_M_nxt = __n_last;
      return __result;
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    typename _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
			_H1, _H2, _Hash, _RehashPolicy,
			_Traits>::iterator
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    erase(const_iterator __first, const_iterator __last)
    {
      __node_type* __n = __first._M_cur;
      __node_type* __last_n = __last._M_cur;
      if (__n == __last_n)
	return iterator(__n);

      std::size_t __bkt = _M_bucket_index(__n);

      __node_base* __prev_n = _M_get_previous_node(__bkt, __n);
      bool __is_bucket_begin = __n == _M_bucket_begin(__bkt);
      std::size_t __n_bkt = __bkt;
      for (;;)
	{
	  do
	    {
	      __node_type* __tmp = __n;
	      __n = __n->_M_next();
	      this->_M_deallocate_node(__tmp);
	      --_M_element_count;
	      if (!__n)
		break;
	      __n_bkt = _M_bucket_index(__n);
	    }
	  while (__n != __last_n && __n_bkt == __bkt);
	  if (__is_bucket_begin)
	    _M_remove_bucket_begin(__bkt, __n, __n_bkt);
	  if (__n == __last_n)
	    break;
	  __is_bucket_begin = true;
	  __bkt = __n_bkt;
	}

      if (__n && (__n_bkt != __bkt || __is_bucket_begin))
	_M_buckets[__n_bkt] = __prev_n;
      __prev_n->_M_nxt = __n;
      return iterator(__n);
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    void
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    clear() noexcept
    {
      this->_M_deallocate_nodes(_M_begin());
      __builtin_memset(_M_buckets, 0, _M_bucket_count * sizeof(__bucket_type));
      _M_element_count = 0;
      _M_before_begin._M_nxt = nullptr;
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    void
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    rehash(size_type __n)
    {
      const __rehash_state& __saved_state = _M_rehash_policy._M_state();
      std::size_t __buckets
	= std::max(_M_rehash_policy._M_bkt_for_elements(_M_element_count + 1),
		   __n);
      __buckets = _M_rehash_policy._M_next_bkt(__buckets);

      if (__buckets != _M_bucket_count)
	_M_rehash(__buckets, __saved_state);
      else
	// No rehash, restore previous state to keep a consistent state.
	_M_rehash_policy._M_reset(__saved_state);
    }

  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    void
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    _M_rehash(size_type __n, const __rehash_state& __state)
    {
      __try
	{
	  _M_rehash_aux(__n, __unique_keys());
	}
      __catch(...)
	{
	  // A failure here means that buckets allocation failed.  We only
	  // have to restore hash policy previous state.
	  _M_rehash_policy._M_reset(__state);
	  __throw_exception_again;
	}
    }

  // Rehash when there is no equivalent elements.
  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    void
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    _M_rehash_aux(size_type __n, geode::stl::true_type)
    {
      __bucket_type* __new_buckets = _M_allocate_buckets(__n);
      __node_type* __p = _M_begin();
      _M_before_begin._M_nxt = nullptr;
      std::size_t __bbegin_bkt = 0;
      while (__p)
	{
	  __node_type* __next = __p->_M_next();
	  std::size_t __bkt = __hash_code_base::_M_bucket_index(__p, __n);
	  if (!__new_buckets[__bkt])
	    {
	      __p->_M_nxt = _M_before_begin._M_nxt;
	      _M_before_begin._M_nxt = __p;
	      __new_buckets[__bkt] = &_M_before_begin;
	      if (__p->_M_nxt)
		__new_buckets[__bbegin_bkt] = __p;
	      __bbegin_bkt = __bkt;
	    }
	  else
	    {
	      __p->_M_nxt = __new_buckets[__bkt]->_M_nxt;
	      __new_buckets[__bkt]->_M_nxt = __p;
	    }
	  __p = __next;
	}

      _M_deallocate_buckets();
      _M_bucket_count = __n;
      _M_buckets = __new_buckets;
    }

  // Rehash when there can be equivalent elements, preserve their relative
  // order.
  template<typename _Key, typename _Value,
	   typename _Alloc, typename _ExtractKey, typename _Equal,
	   typename _H1, typename _H2, typename _Hash, typename _RehashPolicy,
	   typename _Traits>
    void
    _Hashtable<_Key, _Value, _Alloc, _ExtractKey, _Equal,
	       _H1, _H2, _Hash, _RehashPolicy, _Traits>::
    _M_rehash_aux(size_type __n, geode::stl::false_type)
    {
      __bucket_type* __new_buckets = _M_allocate_buckets(__n);

      __node_type* __p = _M_begin();
      _M_before_begin._M_nxt = nullptr;
      std::size_t __bbegin_bkt = 0;
      std::size_t __prev_bkt = 0;
      __node_type* __prev_p = nullptr;
      bool __check_bucket = false;

      while (__p)
	{
	  __node_type* __next = __p->_M_next();
	  std::size_t __bkt = __hash_code_base::_M_bucket_index(__p, __n);

	  if (__prev_p && __prev_bkt == __bkt)
	    {
	      // Previous insert was already in this bucket, we insert after
	      // the previously inserted one to preserve equivalent elements
	      // relative order.
	      __p->_M_nxt = __prev_p->_M_nxt;
	      __prev_p->_M_nxt = __p;

	      // Inserting after a node in a bucket require to check that we
	      // haven't change the bucket last node, in this case next
	      // bucket containing its before begin node must be updated. We
	      // schedule a check as soon as we move out of the sequence of
	      // equivalent nodes to limit the number of checks.
	      __check_bucket = true;
	    }
	  else
	    {
	      if (__check_bucket)
		{
		  // Check if we shall update the next bucket because of
		  // insertions into __prev_bkt bucket.
		  if (__prev_p->_M_nxt)
		    {
		      std::size_t __next_bkt
			= __hash_code_base::_M_bucket_index(__prev_p->_M_next(),
							    __n);
		      if (__next_bkt != __prev_bkt)
			__new_buckets[__next_bkt] = __prev_p;
		    }
		  __check_bucket = false;
		}

	      if (!__new_buckets[__bkt])
		{
		  __p->_M_nxt = _M_before_begin._M_nxt;
		  _M_before_begin._M_nxt = __p;
		  __new_buckets[__bkt] = &_M_before_begin;
		  if (__p->_M_nxt)
		    __new_buckets[__bbegin_bkt] = __p;
		  __bbegin_bkt = __bkt;
		}
	      else
		{
		  __p->_M_nxt = __new_buckets[__bkt]->_M_nxt;
		  __new_buckets[__bkt]->_M_nxt = __p;
		}
	    }
	  __prev_p = __p;
	  __prev_bkt = __bkt;
	  __p = __next;
	}

      if (__check_bucket && __prev_p->_M_nxt)
	{
	  std::size_t __next_bkt
	    = __hash_code_base::_M_bucket_index(__prev_p->_M_next(), __n);
	  if (__next_bkt != __prev_bkt)
	    __new_buckets[__next_bkt] = __prev_p;
	}

      _M_deallocate_buckets();
      _M_bucket_count = __n;
      _M_buckets = __new_buckets;
    }

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#endif // _HASHTABLE_H
