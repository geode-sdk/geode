// <tuple> -*- C++ -*-

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

/** @file include/tuple
 *  This is a Standard C++ Library header.
 */

#ifndef _GLIBCXX_TUPLE
#define _GLIBCXX_TUPLE 1

#pragma GCC system_header

#include "uses_allocator.h"

#include "utility.h"
#include "array.h"

namespace geode::stl
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   *  @addtogroup utilities
   *  @{
   */

  // Adds a const reference to a non-reference type.
  template<typename _Tp>
    struct __add_c_ref
    { typedef const _Tp& type; };

  template<typename _Tp>
    struct __add_c_ref<_Tp&>
    { typedef _Tp& type; };

  // Adds a reference to a non-reference type.
  template<typename _Tp>
    struct __add_ref
    { typedef _Tp& type; };

  template<typename _Tp>
    struct __add_ref<_Tp&>
    { typedef _Tp& type; };

  // Adds an rvalue reference to a non-reference type.
  template<typename _Tp>
    struct __add_r_ref
    { typedef _Tp&& type; };

  template<typename _Tp>
    struct __add_r_ref<_Tp&>
    { typedef _Tp& type; };

  template<std::size_t _Idx, typename _Head, bool _IsEmptyNotFinal>
    struct _Head_base;

  template<std::size_t _Idx, typename _Head>
    struct _Head_base<_Idx, _Head, true>
    : public _Head
    {
      constexpr _Head_base()
      : _Head() { }

      constexpr _Head_base(const _Head& __h)
      : _Head(__h) { }

      constexpr _Head_base(const _Head_base&) = default;
      constexpr _Head_base(_Head_base&&) = default;

      template<typename _UHead>
        constexpr _Head_base(_UHead&& __h)
    : _Head(std::forward<_UHead>(__h)) { }

      _Head_base(allocator_arg_t, __uses_alloc0)
      : _Head() { }

      template<typename _Alloc>
    _Head_base(allocator_arg_t, __uses_alloc1<_Alloc> __a)
    : _Head(allocator_arg, *__a._M_a) { }

      template<typename _Alloc>
    _Head_base(allocator_arg_t, __uses_alloc2<_Alloc> __a)
    : _Head(*__a._M_a) { }

      template<typename _UHead>
    _Head_base(__uses_alloc0, _UHead&& __uhead)
    : _Head(std::forward<_UHead>(__uhead)) { }

      template<typename _Alloc, typename _UHead>
    _Head_base(__uses_alloc1<_Alloc> __a, _UHead&& __uhead)
    : _Head(allocator_arg, *__a._M_a, std::forward<_UHead>(__uhead)) { }

      template<typename _Alloc, typename _UHead>
    _Head_base(__uses_alloc2<_Alloc> __a, _UHead&& __uhead)
    : _Head(std::forward<_UHead>(__uhead), *__a._M_a) { }

      static constexpr _Head&
      _M_head(_Head_base& __b) noexcept { return __b; }

      static constexpr const _Head&
      _M_head(const _Head_base& __b) noexcept { return __b; }
    };

  template<std::size_t _Idx, typename _Head>
    struct _Head_base<_Idx, _Head, false>
    {
      constexpr _Head_base()
      : _M_head_impl() { }

      constexpr _Head_base(const _Head& __h)
      : _M_head_impl(__h) { }

      constexpr _Head_base(const _Head_base&) = default;
      constexpr _Head_base(_Head_base&&) = default;

      template<typename _UHead>
        constexpr _Head_base(_UHead&& __h)
    : _M_head_impl(std::forward<_UHead>(__h)) { }

      _Head_base(allocator_arg_t, __uses_alloc0)
      : _M_head_impl() { }

      template<typename _Alloc>
    _Head_base(allocator_arg_t, __uses_alloc1<_Alloc> __a)
    : _M_head_impl(allocator_arg, *__a._M_a) { }

      template<typename _Alloc>
    _Head_base(allocator_arg_t, __uses_alloc2<_Alloc> __a)
    : _M_head_impl(*__a._M_a) { }

      template<typename _UHead>
    _Head_base(__uses_alloc0, _UHead&& __uhead)
    : _M_head_impl(std::forward<_UHead>(__uhead)) { }

      template<typename _Alloc, typename _UHead>
    _Head_base(__uses_alloc1<_Alloc> __a, _UHead&& __uhead)
    : _M_head_impl(allocator_arg, *__a._M_a, std::forward<_UHead>(__uhead))
    { }

      template<typename _Alloc, typename _UHead>
    _Head_base(__uses_alloc2<_Alloc> __a, _UHead&& __uhead)
    : _M_head_impl(std::forward<_UHead>(__uhead), *__a._M_a) { }

      static constexpr _Head&
      _M_head(_Head_base& __b) noexcept { return __b._M_head_impl; }

      static constexpr const _Head&
      _M_head(const _Head_base& __b) noexcept { return __b._M_head_impl; }

      _Head _M_head_impl;
    };

  /**
   * Contains the actual implementation of the @c tuple template, stored
   * as a recursive inheritance hierarchy from the first element (most
   * derived class) to the last (least derived class). The @c Idx
   * parameter gives the 0-based index of the element stored at this
   * point in the hierarchy; we use it to implement a constant-time
   * get() operation.
   */
  template<std::size_t _Idx, typename... _Elements>
    struct _Tuple_impl; 

  /**
   * Zero-element tuple implementation. This is the basis case for the 
   * inheritance recursion.
   */
  template<std::size_t _Idx>
    struct _Tuple_impl<_Idx>
    {
      template<std::size_t, typename...> friend class _Tuple_impl;

      _Tuple_impl() = default;

      template<typename _Alloc>
        _Tuple_impl(allocator_arg_t, const _Alloc&) { }

      template<typename _Alloc>
        _Tuple_impl(allocator_arg_t, const _Alloc&, const _Tuple_impl&) { }

      template<typename _Alloc>
        _Tuple_impl(allocator_arg_t, const _Alloc&, _Tuple_impl&&) { }

    protected:
      void _M_swap(_Tuple_impl&) noexcept { /* no-op */ }
    };

  template<typename _Tp>
    struct __is_empty_non_tuple : is_empty<_Tp> { };

  // Using EBO for elements that are tuples causes ambiguous base errors.
  template<typename _El0, typename... _El>
    struct __is_empty_non_tuple<tuple<_El0, _El...>> : false_type { };

  // Use the Empty Base-class Optimization for empty, non-final types.
  template<typename _Tp>
    using __empty_not_final
    = typename conditional<__is_final(_Tp), false_type,
               __is_empty_non_tuple<_Tp>>::type;

  /**
   * Recursive tuple implementation. Here we store the @c Head element
   * and derive from a @c Tuple_impl containing the remaining elements
   * (which contains the @c Tail).
   */
  template<std::size_t _Idx, typename _Head, typename... _Tail>
    struct _Tuple_impl<_Idx, _Head, _Tail...>
    : public _Tuple_impl<_Idx + 1, _Tail...>,
      private _Head_base<_Idx, _Head, __empty_not_final<_Head>::value>
    {
      template<std::size_t, typename...> friend class _Tuple_impl;

      typedef _Tuple_impl<_Idx + 1, _Tail...> _Inherited;
      typedef _Head_base<_Idx, _Head, __empty_not_final<_Head>::value> _Base;

      static constexpr _Head&  
      _M_head(_Tuple_impl& __t) noexcept { return _Base::_M_head(__t); }

      static constexpr const _Head&
      _M_head(const _Tuple_impl& __t) noexcept { return _Base::_M_head(__t); }

      static constexpr _Inherited&
      _M_tail(_Tuple_impl& __t) noexcept { return __t; }

      static constexpr const _Inherited&
      _M_tail(const _Tuple_impl& __t) noexcept { return __t; }

      constexpr _Tuple_impl()
      : _Inherited(), _Base() { }

      explicit 
      constexpr _Tuple_impl(const _Head& __head, const _Tail&... __tail)
      : _Inherited(__tail...), _Base(__head) { }

      template<typename _UHead, typename... _UTail, typename = typename
               enable_if<sizeof...(_Tail) == sizeof...(_UTail)>::type> 
        explicit
        constexpr _Tuple_impl(_UHead&& __head, _UTail&&... __tail)
    : _Inherited(std::forward<_UTail>(__tail)...),
      _Base(std::forward<_UHead>(__head)) { }

      constexpr _Tuple_impl(const _Tuple_impl&) = default;

      constexpr
      _Tuple_impl(_Tuple_impl&& __in)
      noexcept(__and_<is_nothrow_move_constructible<_Head>,
                  is_nothrow_move_constructible<_Inherited>>::value)
      : _Inherited(std::move(_M_tail(__in))), 
    _Base(std::forward<_Head>(_M_head(__in))) { }

      template<typename... _UElements>
        constexpr _Tuple_impl(const _Tuple_impl<_Idx, _UElements...>& __in)
    : _Inherited(_Tuple_impl<_Idx, _UElements...>::_M_tail(__in)),
      _Base(_Tuple_impl<_Idx, _UElements...>::_M_head(__in)) { }

      template<typename _UHead, typename... _UTails>
        constexpr _Tuple_impl(_Tuple_impl<_Idx, _UHead, _UTails...>&& __in)
    : _Inherited(std::move
             (_Tuple_impl<_Idx, _UHead, _UTails...>::_M_tail(__in))),
      _Base(std::forward<_UHead>
        (_Tuple_impl<_Idx, _UHead, _UTails...>::_M_head(__in))) { }

      template<typename _Alloc>
    _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a)
    : _Inherited(__tag, __a),
          _Base(__tag, __use_alloc<_Head>(__a)) { }

      template<typename _Alloc>
    _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a,
            const _Head& __head, const _Tail&... __tail)
    : _Inherited(__tag, __a, __tail...),
          _Base(__use_alloc<_Head, _Alloc, _Head>(__a), __head) { }

      template<typename _Alloc, typename _UHead, typename... _UTail,
               typename = typename enable_if<sizeof...(_Tail)
                         == sizeof...(_UTail)>::type>
    _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a,
                _UHead&& __head, _UTail&&... __tail)
    : _Inherited(__tag, __a, std::forward<_UTail>(__tail)...),
          _Base(__use_alloc<_Head, _Alloc, _UHead>(__a),
            std::forward<_UHead>(__head)) { }

      template<typename _Alloc>
        _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a,
                const _Tuple_impl& __in)
    : _Inherited(__tag, __a, _M_tail(__in)), 
          _Base(__use_alloc<_Head, _Alloc, _Head>(__a), _M_head(__in)) { }

      template<typename _Alloc>
    _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a,
                _Tuple_impl&& __in)
    : _Inherited(__tag, __a, std::move(_M_tail(__in))), 
      _Base(__use_alloc<_Head, _Alloc, _Head>(__a),
            std::forward<_Head>(_M_head(__in))) { }

      template<typename _Alloc, typename... _UElements>
    _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a,
                const _Tuple_impl<_Idx, _UElements...>& __in)
    : _Inherited(__tag, __a,
             _Tuple_impl<_Idx, _UElements...>::_M_tail(__in)),
      _Base(__use_alloc<_Head, _Alloc, _Head>(__a),
        _Tuple_impl<_Idx, _UElements...>::_M_head(__in)) { }

      template<typename _Alloc, typename _UHead, typename... _UTails>
    _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a,
                _Tuple_impl<_Idx, _UHead, _UTails...>&& __in)
    : _Inherited(__tag, __a, std::move
             (_Tuple_impl<_Idx, _UHead, _UTails...>::_M_tail(__in))),
      _Base(__use_alloc<_Head, _Alloc, _UHead>(__a),
                std::forward<_UHead>
        (_Tuple_impl<_Idx, _UHead, _UTails...>::_M_head(__in))) { }

      _Tuple_impl&
      operator=(const _Tuple_impl& __in)
      {
    _M_head(*this) = _M_head(__in);
    _M_tail(*this) = _M_tail(__in);
    return *this;
      }

      _Tuple_impl&
      operator=(_Tuple_impl&& __in)
      noexcept(__and_<is_nothrow_move_assignable<_Head>,
                  is_nothrow_move_assignable<_Inherited>>::value)
      {
    _M_head(*this) = std::forward<_Head>(_M_head(__in));
    _M_tail(*this) = std::move(_M_tail(__in));
    return *this;
      }

      template<typename... _UElements>
        _Tuple_impl&
        operator=(const _Tuple_impl<_Idx, _UElements...>& __in)
        {
      _M_head(*this) = _Tuple_impl<_Idx, _UElements...>::_M_head(__in);
      _M_tail(*this) = _Tuple_impl<_Idx, _UElements...>::_M_tail(__in);
      return *this;
    }

      template<typename _UHead, typename... _UTails>
        _Tuple_impl&
        operator=(_Tuple_impl<_Idx, _UHead, _UTails...>&& __in)
        {
      _M_head(*this) = std::forward<_UHead>
        (_Tuple_impl<_Idx, _UHead, _UTails...>::_M_head(__in));
      _M_tail(*this) = std::move
        (_Tuple_impl<_Idx, _UHead, _UTails...>::_M_tail(__in));
      return *this;
    }

    protected:
      void
      _M_swap(_Tuple_impl& __in)
      noexcept(noexcept(swap(std::declval<_Head&>(),
                 std::declval<_Head&>()))
           && noexcept(_M_tail(__in)._M_swap(_M_tail(__in))))
      {
    using std::swap;
    swap(_M_head(*this), _M_head(__in));
    _Inherited::_M_swap(_M_tail(__in));
      }
    };

  /// Primary class template, tuple
  template<typename... _Elements> 
    class tuple : public _Tuple_impl<0, _Elements...>
    {
      typedef _Tuple_impl<0, _Elements...> _Inherited;

    public:
      constexpr tuple()
      : _Inherited() { }

      explicit
      constexpr tuple(const _Elements&... __elements)
      : _Inherited(__elements...) { }

      template<typename... _UElements, typename = typename
        enable_if<__and_<is_convertible<_UElements,
                    _Elements>...>::value>::type>
    explicit
        constexpr tuple(_UElements&&... __elements)
    : _Inherited(std::forward<_UElements>(__elements)...) {	}

      constexpr tuple(const tuple&) = default;

      constexpr tuple(tuple&&) = default; 

      template<typename... _UElements, typename = typename
        enable_if<__and_<is_convertible<const _UElements&,
                    _Elements>...>::value>::type>
        constexpr tuple(const tuple<_UElements...>& __in)
        : _Inherited(static_cast<const _Tuple_impl<0, _UElements...>&>(__in))
        { }

      template<typename... _UElements, typename = typename
        enable_if<__and_<is_convertible<_UElements,
                    _Elements>...>::value>::type>
        constexpr tuple(tuple<_UElements...>&& __in)
        : _Inherited(static_cast<_Tuple_impl<0, _UElements...>&&>(__in)) { }

      // Allocator-extended constructors.

      template<typename _Alloc>
    tuple(allocator_arg_t __tag, const _Alloc& __a)
    : _Inherited(__tag, __a) { }

      template<typename _Alloc>
    tuple(allocator_arg_t __tag, const _Alloc& __a,
          const _Elements&... __elements)
    : _Inherited(__tag, __a, __elements...) { }

      template<typename _Alloc, typename... _UElements, typename = typename
           enable_if<sizeof...(_UElements)
             == sizeof...(_Elements)>::type>
    tuple(allocator_arg_t __tag, const _Alloc& __a,
          _UElements&&... __elements)
    : _Inherited(__tag, __a, std::forward<_UElements>(__elements)...)
        { }

      template<typename _Alloc>
    tuple(allocator_arg_t __tag, const _Alloc& __a, const tuple& __in)
    : _Inherited(__tag, __a, static_cast<const _Inherited&>(__in)) { }

      template<typename _Alloc>
    tuple(allocator_arg_t __tag, const _Alloc& __a, tuple&& __in)
    : _Inherited(__tag, __a, static_cast<_Inherited&&>(__in)) { }

      template<typename _Alloc, typename... _UElements, typename = typename
           enable_if<sizeof...(_UElements)
             == sizeof...(_Elements)>::type>
    tuple(allocator_arg_t __tag, const _Alloc& __a,
          const tuple<_UElements...>& __in)
    : _Inherited(__tag, __a,
                 static_cast<const _Tuple_impl<0, _UElements...>&>(__in))
    { }

      template<typename _Alloc, typename... _UElements, typename = typename
           enable_if<sizeof...(_UElements)
             == sizeof...(_Elements)>::type>
    tuple(allocator_arg_t __tag, const _Alloc& __a,
          tuple<_UElements...>&& __in)
    : _Inherited(__tag, __a,
                 static_cast<_Tuple_impl<0, _UElements...>&&>(__in))
    { }

      tuple&
      operator=(const tuple& __in)
      {
    static_cast<_Inherited&>(*this) = __in;
    return *this;
      }

      tuple&
      operator=(tuple&& __in)
      noexcept(is_nothrow_move_assignable<_Inherited>::value)
      {
    static_cast<_Inherited&>(*this) = std::move(__in);
    return *this;
      }

      template<typename... _UElements, typename = typename
           enable_if<sizeof...(_UElements)
             == sizeof...(_Elements)>::type>
        tuple&
        operator=(const tuple<_UElements...>& __in)
        {
      static_cast<_Inherited&>(*this) = __in;
      return *this;
    }

      template<typename... _UElements, typename = typename
           enable_if<sizeof...(_UElements)
             == sizeof...(_Elements)>::type>
        tuple&
        operator=(tuple<_UElements...>&& __in)
        {
      static_cast<_Inherited&>(*this) = std::move(__in);
      return *this;
    }

      void
      swap(tuple& __in)
      noexcept(noexcept(__in._M_swap(__in)))
      { _Inherited::_M_swap(__in); }
    };

  // Explicit specialization, zero-element tuple.
  template<>  
    class tuple<>
    {
    public:
      void swap(tuple&) noexcept { /* no-op */ }
    };

  /// Partial specialization, 2-element tuple.
  /// Includes construction and assignment from a pair.
  template<typename _T1, typename _T2>
    class tuple<_T1, _T2> : public _Tuple_impl<0, _T1, _T2>
    {
      typedef _Tuple_impl<0, _T1, _T2> _Inherited;

    public:
      constexpr tuple()
      : _Inherited() { }

      explicit
      constexpr tuple(const _T1& __a1, const _T2& __a2)
      : _Inherited(__a1, __a2) { }

      template<typename _U1, typename _U2, typename = typename
           enable_if<__and_<is_convertible<_U1, _T1>,
                is_convertible<_U2, _T2>>::value>::type>
        explicit
        constexpr tuple(_U1&& __a1, _U2&& __a2)
    : _Inherited(std::forward<_U1>(__a1), std::forward<_U2>(__a2)) { }

      constexpr tuple(const tuple&) = default;

      constexpr tuple(tuple&&) = default;

      template<typename _U1, typename _U2, typename = typename
    enable_if<__and_<is_convertible<const _U1&, _T1>,
             is_convertible<const _U2&, _T2>>::value>::type>
        constexpr tuple(const tuple<_U1, _U2>& __in)
    : _Inherited(static_cast<const _Tuple_impl<0, _U1, _U2>&>(__in)) { }

      template<typename _U1, typename _U2, typename = typename
           enable_if<__and_<is_convertible<_U1, _T1>,
                is_convertible<_U2, _T2>>::value>::type>
        constexpr tuple(tuple<_U1, _U2>&& __in)
    : _Inherited(static_cast<_Tuple_impl<0, _U1, _U2>&&>(__in)) { }

      template<typename _U1, typename _U2, typename = typename
    enable_if<__and_<is_convertible<const _U1&, _T1>,
             is_convertible<const _U2&, _T2>>::value>::type>
        constexpr tuple(const pair<_U1, _U2>& __in)
    : _Inherited(__in.first, __in.second) { }

      template<typename _U1, typename _U2, typename = typename
           enable_if<__and_<is_convertible<_U1, _T1>,
                is_convertible<_U2, _T2>>::value>::type>
        constexpr tuple(pair<_U1, _U2>&& __in)
    : _Inherited(std::forward<_U1>(__in.first),
             std::forward<_U2>(__in.second)) { }

      // Allocator-extended constructors.

      template<typename _Alloc>
    tuple(allocator_arg_t __tag, const _Alloc& __a)
    : _Inherited(__tag, __a) { }

      template<typename _Alloc>
    tuple(allocator_arg_t __tag, const _Alloc& __a,
          const _T1& __a1, const _T2& __a2)
    : _Inherited(__tag, __a, __a1, __a2) { }

      template<typename _Alloc, typename _U1, typename _U2>
    tuple(allocator_arg_t __tag, const _Alloc& __a, _U1&& __a1, _U2&& __a2)
    : _Inherited(__tag, __a, std::forward<_U1>(__a1),
                 std::forward<_U2>(__a2)) { }

      template<typename _Alloc>
    tuple(allocator_arg_t __tag, const _Alloc& __a, const tuple& __in)
    : _Inherited(__tag, __a, static_cast<const _Inherited&>(__in)) { }

      template<typename _Alloc>
    tuple(allocator_arg_t __tag, const _Alloc& __a, tuple&& __in)
    : _Inherited(__tag, __a, static_cast<_Inherited&&>(__in)) { }

      template<typename _Alloc, typename _U1, typename _U2>
    tuple(allocator_arg_t __tag, const _Alloc& __a,
          const tuple<_U1, _U2>& __in)
    : _Inherited(__tag, __a,
                 static_cast<const _Tuple_impl<0, _U1, _U2>&>(__in))
    { }

      template<typename _Alloc, typename _U1, typename _U2>
    tuple(allocator_arg_t __tag, const _Alloc& __a, tuple<_U1, _U2>&& __in)
    : _Inherited(__tag, __a, static_cast<_Tuple_impl<0, _U1, _U2>&&>(__in))
    { }

      template<typename _Alloc, typename _U1, typename _U2>
        tuple(allocator_arg_t __tag, const _Alloc& __a,
          const pair<_U1, _U2>& __in)
    : _Inherited(__tag, __a, __in.first, __in.second) { }

      template<typename _Alloc, typename _U1, typename _U2>
        tuple(allocator_arg_t __tag, const _Alloc& __a, pair<_U1, _U2>&& __in)
    : _Inherited(__tag, __a, std::forward<_U1>(__in.first),
             std::forward<_U2>(__in.second)) { }

      tuple&
      operator=(const tuple& __in)
      {
    static_cast<_Inherited&>(*this) = __in;
    return *this;
      }

      tuple&
      operator=(tuple&& __in)
      noexcept(is_nothrow_move_assignable<_Inherited>::value)
      {
    static_cast<_Inherited&>(*this) = std::move(__in);
    return *this;
      }

      template<typename _U1, typename _U2>
        tuple&
        operator=(const tuple<_U1, _U2>& __in)
        {
      static_cast<_Inherited&>(*this) = __in;
      return *this;
    }

      template<typename _U1, typename _U2>
        tuple&
        operator=(tuple<_U1, _U2>&& __in)
        {
      static_cast<_Inherited&>(*this) = std::move(__in);
      return *this;
    }

      template<typename _U1, typename _U2>
        tuple&
        operator=(const pair<_U1, _U2>& __in)
        {
      this->_M_head(*this) = __in.first;
      this->_M_tail(*this)._M_head(*this) = __in.second;
      return *this;
    }

      template<typename _U1, typename _U2>
        tuple&
        operator=(pair<_U1, _U2>&& __in)
        {
      this->_M_head(*this) = std::forward<_U1>(__in.first);
      this->_M_tail(*this)._M_head(*this) = std::forward<_U2>(__in.second);
      return *this;
    }

      void
      swap(tuple& __in)
      noexcept(noexcept(__in._M_swap(__in)))
      { _Inherited::_M_swap(__in); }
    };


  /// Gives the type of the ith element of a given tuple type.
  template<std::size_t __i, typename _Tp>
    struct tuple_element;

  /**
   * Recursive case for tuple_element: strip off the first element in
   * the tuple and retrieve the (i-1)th element of the remaining tuple.
   */
  template<std::size_t __i, typename _Head, typename... _Tail>
    struct tuple_element<__i, tuple<_Head, _Tail...> >
    : tuple_element<__i - 1, tuple<_Tail...> > { };

  /**
   * Basis case for tuple_element: The first element is the one we're seeking.
   */
  template<typename _Head, typename... _Tail>
    struct tuple_element<0, tuple<_Head, _Tail...> >
    {
      typedef _Head type;
    };

  template<std::size_t __i, typename _Tp>
    struct tuple_element<__i, const _Tp>
    {
      typedef typename
      add_const<typename tuple_element<__i, _Tp>::type>::type type;
    };

  template<std::size_t __i, typename _Tp>
    struct tuple_element<__i, volatile _Tp>
    {
      typedef typename
      add_volatile<typename tuple_element<__i, _Tp>::type>::type type;
    };

  template<std::size_t __i, typename _Tp>
    struct tuple_element<__i, const volatile _Tp>
    {
      typedef typename
      add_cv<typename tuple_element<__i, _Tp>::type>::type type;
    };

#if __cplusplus > 201103L
  template<std::size_t __i, typename _Tp>
    using tuple_element_t = typename tuple_element<__i, _Tp>::type;
#endif

  /// Finds the size of a given tuple type.
  template<typename _Tp>
    struct tuple_size;

  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // 2313. tuple_size should always derive from integral_constant<size_t, N>
  template<typename _Tp>
    struct tuple_size<const _Tp>
    : public integral_constant<size_t, tuple_size<_Tp>::value> { };

  template<typename _Tp>
    struct tuple_size<volatile _Tp>
    : public integral_constant<size_t, tuple_size<_Tp>::value> { };

  template<typename _Tp>
    struct tuple_size<const volatile _Tp>
    : public integral_constant<size_t, tuple_size<_Tp>::value> { };

  /// class tuple_size
  template<typename... _Elements>
    struct tuple_size<tuple<_Elements...>>
    : public integral_constant<std::size_t, sizeof...(_Elements)> { };

  template<std::size_t __i, typename _Head, typename... _Tail>
    constexpr typename __add_ref<_Head>::type
    __get_helper(_Tuple_impl<__i, _Head, _Tail...>& __t) noexcept
    { return _Tuple_impl<__i, _Head, _Tail...>::_M_head(__t); }

  template<std::size_t __i, typename _Head, typename... _Tail>
    constexpr typename __add_c_ref<_Head>::type
    __get_helper(const _Tuple_impl<__i, _Head, _Tail...>& __t) noexcept
    { return _Tuple_impl<__i, _Head, _Tail...>::_M_head(__t); }

  /// Return a reference to the ith element of a tuple.
  template<std::size_t __i, typename... _Elements>
    constexpr typename __add_ref<
                      typename tuple_element<__i, tuple<_Elements...>>::type
                    >::type
    get(tuple<_Elements...>& __t) noexcept
    { return __get_helper<__i>(__t); }

  /// Return a const reference to the ith element of a const tuple.
  template<std::size_t __i, typename... _Elements>
    constexpr typename __add_c_ref<
                      typename tuple_element<__i, tuple<_Elements...>>::type
                    >::type
    get(const tuple<_Elements...>& __t) noexcept
    { return __get_helper<__i>(__t); }

  /// Return an rvalue reference to the ith element of a tuple rvalue.
  template<std::size_t __i, typename... _Elements>
    constexpr typename __add_r_ref<
                      typename tuple_element<__i, tuple<_Elements...>>::type
                    >::type
    get(tuple<_Elements...>&& __t) noexcept
    { return std::forward<typename tuple_element<__i,
    tuple<_Elements...>>::type&&>(get<__i>(__t)); }

#if __cplusplus > 201103L

  template<typename _Head, size_t __i, typename... _Tail>
    constexpr typename __add_ref<_Head>::type
    __get_helper2(_Tuple_impl<__i, _Head, _Tail...>& __t) noexcept
    { return _Tuple_impl<__i, _Head, _Tail...>::_M_head(__t); }

  template<typename _Head, size_t __i, typename... _Tail>
    constexpr typename __add_c_ref<_Head>::type
    __get_helper2(const _Tuple_impl<__i, _Head, _Tail...>& __t) noexcept
    { return _Tuple_impl<__i, _Head, _Tail...>::_M_head(__t); }

  /// Return a reference to the unique element of type _Tp of a tuple.
  template <typename _Tp, typename... _Types>
    constexpr _Tp&
    get(tuple<_Types...>& __t) noexcept
    { return __get_helper2<_Tp>(__t); }

  /// Return a reference to the unique element of type _Tp of a tuple rvalue.
  template <typename _Tp, typename... _Types>
    constexpr _Tp&&
    get(tuple<_Types...>&& __t) noexcept
    { return std::forward<_Tp&&>(__get_helper2<_Tp>(__t)); }

  /// Return a const reference to the unique element of type _Tp of a tuple.
  template <typename _Tp, typename... _Types>
    constexpr const _Tp&
    get(const tuple<_Types...>& __t) noexcept
    { return __get_helper2<_Tp>(__t); }
#endif


  // This class helps construct the various comparison operations on tuples
  template<std::size_t __check_equal_size, std::size_t __i, std::size_t __j,
       typename _Tp, typename _Up>
    struct __tuple_compare;

  template<std::size_t __i, std::size_t __j, typename _Tp, typename _Up>
    struct __tuple_compare<0, __i, __j, _Tp, _Up>
    {
      static constexpr bool 
      __eq(const _Tp& __t, const _Up& __u)
      {
    return (get<__i>(__t) == get<__i>(__u) &&
        __tuple_compare<0, __i + 1, __j, _Tp, _Up>::__eq(__t, __u));
      }
     
      static constexpr bool 
      __less(const _Tp& __t, const _Up& __u)
      {
    return ((get<__i>(__t) < get<__i>(__u))
        || !(get<__i>(__u) < get<__i>(__t)) &&
        __tuple_compare<0, __i + 1, __j, _Tp, _Up>::__less(__t, __u));
      }
    };

  template<std::size_t __i, typename _Tp, typename _Up>
    struct __tuple_compare<0, __i, __i, _Tp, _Up>
    {
      static constexpr bool 
      __eq(const _Tp&, const _Up&) { return true; }
     
      static constexpr bool 
      __less(const _Tp&, const _Up&) { return false; }
    };

  template<typename... _TElements, typename... _UElements>
    constexpr bool
    operator==(const tuple<_TElements...>& __t,
           const tuple<_UElements...>& __u)
    {
      typedef tuple<_TElements...> _Tp;
      typedef tuple<_UElements...> _Up;
      return bool(__tuple_compare<tuple_size<_Tp>::value - tuple_size<_Up>::value,
          0, tuple_size<_Tp>::value, _Tp, _Up>::__eq(__t, __u));
    }

  template<typename... _TElements, typename... _UElements>
    constexpr bool
    operator<(const tuple<_TElements...>& __t,
          const tuple<_UElements...>& __u)
    {
      typedef tuple<_TElements...> _Tp;
      typedef tuple<_UElements...> _Up;
      return bool(__tuple_compare<tuple_size<_Tp>::value - tuple_size<_Up>::value,
          0, tuple_size<_Tp>::value, _Tp, _Up>::__less(__t, __u));
    }

  template<typename... _TElements, typename... _UElements>
    constexpr bool
    operator!=(const tuple<_TElements...>& __t,
           const tuple<_UElements...>& __u)
    { return !(__t == __u); }

  template<typename... _TElements, typename... _UElements>
    constexpr bool
    operator>(const tuple<_TElements...>& __t,
          const tuple<_UElements...>& __u)
    { return __u < __t; }

  template<typename... _TElements, typename... _UElements>
    constexpr bool
    operator<=(const tuple<_TElements...>& __t,
           const tuple<_UElements...>& __u)
    { return !(__u < __t); }

  template<typename... _TElements, typename... _UElements>
    constexpr bool
    operator>=(const tuple<_TElements...>& __t,
           const tuple<_UElements...>& __u)
    { return !(__t < __u); }

  // NB: DR 705.
  template<typename... _Elements>
    constexpr tuple<typename __decay_and_strip<_Elements>::__type...>
    make_tuple(_Elements&&... __args)
    {
      typedef tuple<typename __decay_and_strip<_Elements>::__type...>
    __result_type;
      return __result_type(std::forward<_Elements>(__args)...);
    }

  template<typename... _Elements>
    tuple<_Elements&&...>
    forward_as_tuple(_Elements&&... __args) noexcept
    { return tuple<_Elements&&...>(std::forward<_Elements>(__args)...); }

  template<typename>
    struct __is_tuple_like_impl : false_type
    { };

  template<typename... _Tps>
    struct __is_tuple_like_impl<tuple<_Tps...>> : true_type
    { };

  template<typename _T1, typename _T2>
    struct __is_tuple_like_impl<pair<_T1, _T2>> : true_type
    { };

  template<typename _Tp, std::size_t _Nm>
    struct __is_tuple_like_impl<array<_Tp, _Nm>> : true_type
    { };

  // Internal type trait that allows us to sfinae-protect tuple_cat.
  template<typename _Tp>
    struct __is_tuple_like
    : public __is_tuple_like_impl<typename std::remove_cv
            <typename std::remove_reference<_Tp>::type>::type>::type
    { };

  template<std::size_t, typename, typename, std::size_t>
    struct __make_tuple_impl;

  template<std::size_t _Idx, typename _Tuple, typename... _Tp,
           std::size_t _Nm>
    struct __make_tuple_impl<_Idx, tuple<_Tp...>, _Tuple, _Nm>
    {
      typedef typename __make_tuple_impl<_Idx + 1, tuple<_Tp...,
    typename tuple_element<_Idx, _Tuple>::type>, _Tuple, _Nm>::__type
      __type;
    };

  template<std::size_t _Nm, typename _Tuple, typename... _Tp>
    struct __make_tuple_impl<_Nm, tuple<_Tp...>, _Tuple, _Nm>
    {
      typedef tuple<_Tp...> __type;
    };

  template<typename _Tuple>
    struct __do_make_tuple
    : public __make_tuple_impl<0, tuple<>, _Tuple,
                               tuple_size<_Tuple>::value>
    { };

  // Returns the std::tuple equivalent of a tuple-like type.
  template<typename _Tuple>
    struct __make_tuple
    : public __do_make_tuple<typename std::remove_cv
            <typename std::remove_reference<_Tuple>::type>::type>
    { };

  // Combines several std::tuple's into a single one.
  template<typename...>
    struct __combine_tuples;

  template<>
    struct __combine_tuples<>
    {
      typedef tuple<> __type;
    };

  template<typename... _Ts>
    struct __combine_tuples<tuple<_Ts...>>
    {
      typedef tuple<_Ts...> __type;
    };

  template<typename... _T1s, typename... _T2s, typename... _Rem>
    struct __combine_tuples<tuple<_T1s...>, tuple<_T2s...>, _Rem...>
    {
      typedef typename __combine_tuples<tuple<_T1s..., _T2s...>,
                    _Rem...>::__type __type;
    };

  // Computes the result type of tuple_cat given a set of tuple-like types.
  template<typename... _Tpls>
    struct __tuple_cat_result
    {
      typedef typename __combine_tuples
        <typename __make_tuple<_Tpls>::__type...>::__type __type;
    };

  // Helper to determine the index set for the first tuple-like
  // type of a given set.
  template<typename...>
    struct __make_1st_indices;

  template<>
    struct __make_1st_indices<>
    {
      typedef _Index_tuple<> __type;
    };

  template<typename _Tp, typename... _Tpls>
    struct __make_1st_indices<_Tp, _Tpls...>
    {
      typedef typename _Build_index_tuple<tuple_size<
    typename std::remove_reference<_Tp>::type>::value>::__type __type;
    };

  // Performs the actual concatenation by step-wise expanding tuple-like
  // objects into the elements,  which are finally forwarded into the
  // result tuple.
  template<typename _Ret, typename _Indices, typename... _Tpls>
    struct __tuple_concater;

  template<typename _Ret, std::size_t... _Is, typename _Tp, typename... _Tpls>
    struct __tuple_concater<_Ret, _Index_tuple<_Is...>, _Tp, _Tpls...>
    {
      template<typename... _Us>
        static constexpr _Ret
        _S_do(_Tp&& __tp, _Tpls&&... __tps, _Us&&... __us)
        {
      typedef typename __make_1st_indices<_Tpls...>::__type __idx;
      typedef __tuple_concater<_Ret, __idx, _Tpls...>      __next;
      return __next::_S_do(std::forward<_Tpls>(__tps)...,
                   std::forward<_Us>(__us)...,
                   get<_Is>(std::forward<_Tp>(__tp))...);
    }
    };

  template<typename _Ret>
    struct __tuple_concater<_Ret, _Index_tuple<>>
    {
      template<typename... _Us>
    static constexpr _Ret
    _S_do(_Us&&... __us)
        {
      return _Ret(std::forward<_Us>(__us)...);
    }
    };

  /// tuple_cat
  template<typename... _Tpls, typename = typename
           enable_if<__and_<__is_tuple_like<_Tpls>...>::value>::type>
    constexpr auto
    tuple_cat(_Tpls&&... __tpls)
    -> typename __tuple_cat_result<_Tpls...>::__type
    {
      typedef typename __tuple_cat_result<_Tpls...>::__type __ret;
      typedef typename __make_1st_indices<_Tpls...>::__type __idx;
      typedef __tuple_concater<__ret, __idx, _Tpls...> __concater;
      return __concater::_S_do(std::forward<_Tpls>(__tpls)...);
    }

  /// tie
  template<typename... _Elements>
    inline tuple<_Elements&...>
    tie(_Elements&... __args) noexcept
    { return tuple<_Elements&...>(__args...); }

  /// swap
  template<typename... _Elements>
    inline void 
    swap(tuple<_Elements...>& __x, tuple<_Elements...>& __y)
    noexcept(noexcept(__x.swap(__y)))
    { __x.swap(__y); }

  // A class (and instance) which can be used in 'tie' when an element
  // of a tuple is not required
  struct _Swallow_assign
  {
    template<class _Tp>
      const _Swallow_assign&
      operator=(const _Tp&) const
      { return *this; }
  };

  const _Swallow_assign ignore{};

  /// Partial specialization for tuples
  template<typename... _Types, typename _Alloc>
    struct uses_allocator<tuple<_Types...>, _Alloc> : true_type { };

  // See stl_pair.h...
  template<class _T1, class _T2>
    template<typename... _Args1, typename... _Args2>
      inline
      pair<_T1, _T2>::
      pair(piecewise_construct_t,
       tuple<_Args1...> __first, tuple<_Args2...> __second)
      : pair(__first, __second,
         typename _Build_index_tuple<sizeof...(_Args1)>::__type(),
         typename _Build_index_tuple<sizeof...(_Args2)>::__type())
      { }

  template<class _T1, class _T2>
    template<typename... _Args1, std::size_t... _Indexes1,
             typename... _Args2, std::size_t... _Indexes2>
      inline
      pair<_T1, _T2>::
      pair(tuple<_Args1...>& __tuple1, tuple<_Args2...>& __tuple2,
       _Index_tuple<_Indexes1...>, _Index_tuple<_Indexes2...>)
      : first(std::forward<_Args1>(get<_Indexes1>(__tuple1))...),
        second(std::forward<_Args2>(get<_Indexes2>(__tuple2))...)
      { }

  /// @}

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#endif // C++11
