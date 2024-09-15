// Pointer Traits -*- C++ -*-

// Copyright (C) 2011-2014 Free Software Foundation, Inc.
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

/** @file bits/ptr_traits.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{memory}
 */

#pragma once

#if __cplusplus >= 201103L
#include "c++config.h"

#include <type_traits>
#define _GLIBCXX_HAS_NESTED_TYPE(_NTYPE)				\
  template<typename _Tp, typename = std::void_t<>>				\
    struct __has_##_NTYPE						\
    : std::false_type							\
    { };								\
  template<typename _Tp>						\
    struct __has_##_NTYPE<_Tp, std::void_t<typename _Tp::_NTYPE>>		\
    : std::true_type								\
    { };

namespace geode::stl {
_GLIBCXX_BEGIN_NAMESPACE_VERSION

_GLIBCXX_HAS_NESTED_TYPE(element_type)
_GLIBCXX_HAS_NESTED_TYPE(difference_type)

  template<typename _Tp, bool = __has_element_type<_Tp>::value>
    struct __ptrtr_elt_type;

  template<typename _Tp>
    struct __ptrtr_elt_type<_Tp, true>
    {
      typedef typename _Tp::element_type __type;
    };

  template<template<typename, typename...> class _SomePtr, typename _Tp,
            typename... _Args>
    struct __ptrtr_elt_type<_SomePtr<_Tp, _Args...>, false>
    {
      typedef _Tp __type;
    };

  template<typename _Tp, bool = __has_difference_type<_Tp>::value>
    struct __ptrtr_diff_type
    {
      typedef typename _Tp::difference_type __type;
    };

  template<typename _Tp>
    struct __ptrtr_diff_type<_Tp, false>
    {
      typedef ptrdiff_t __type;
    };

  template<typename _Ptr, typename _Up>
    class __ptrtr_rebind_helper
    {
      template<typename _Ptr2, typename _Up2>
	static constexpr std::true_type
	_S_chk(typename _Ptr2::template rebind<_Up2>*);

      template<typename, typename>
	static constexpr std::false_type
	_S_chk(...);

    public:
      using __type = decltype(_S_chk<_Ptr, _Up>(nullptr));
    };

  template<typename _Tp, typename _Up,
           bool = __ptrtr_rebind_helper<_Tp, _Up>::__type::value>
    struct __ptrtr_rebind;

  template<typename _Tp, typename _Up>
    struct __ptrtr_rebind<_Tp, _Up, true>
    {
      typedef typename _Tp::template rebind<_Up> __type;
    };

  template<template<typename, typename...> class _SomePtr, typename _Up,
            typename _Tp, typename... _Args>
    struct __ptrtr_rebind<_SomePtr<_Tp, _Args...>, _Up, false>
    {
      typedef _SomePtr<_Up, _Args...> __type;
    };

  template<typename _Tp, typename = typename std::remove_cv<_Tp>::type>
    struct __ptrtr_not_void
    {
      typedef _Tp __type;
    };

  template<typename _Tp>
    struct __ptrtr_not_void<_Tp, void>
    {
      struct __type { };
    };

  template<typename _Ptr>
    class __ptrtr_pointer_to
    {
      typedef typename __ptrtr_elt_type<_Ptr>::__type   __orig_type;
      typedef typename __ptrtr_not_void<__orig_type>::__type __element_type;

    public:
      static _Ptr pointer_to(__element_type& __e)
      { return _Ptr::pointer_to(__e); }
    };

  /**
   * @brief  Uniform interface to all pointer-like types
   * @ingroup pointer_abstractions
  */
  template<typename _Ptr>
    struct pointer_traits : __ptrtr_pointer_to<_Ptr>
    {
      /// The pointer type
      typedef _Ptr                                      pointer;
      /// The type pointed to
      typedef typename __ptrtr_elt_type<_Ptr>::__type   element_type;
      /// Type used to represent the difference between two pointers
      typedef typename __ptrtr_diff_type<_Ptr>::__type  difference_type;

      template<typename _Up>
        using rebind = typename __ptrtr_rebind<_Ptr, _Up>::__type;
    };

  /**
   * @brief  Partial specialization for built-in pointers.
   * @ingroup pointer_abstractions
  */
  template<typename _Tp>
    struct pointer_traits<_Tp*>
    {
      /// The pointer type
      typedef _Tp* pointer;
      /// The type pointed to
      typedef _Tp  element_type;
      /// Type used to represent the difference between two pointers
      typedef ptrdiff_t difference_type;

      template<typename _Up>
        using rebind = _Up*;

      /**
       *  @brief  Obtain a pointer to an object
       *  @param  __r  A reference to an object of type @c element_type
       *  @return @c addressof(__r)
      */
      static pointer
      pointer_to(typename __ptrtr_not_void<element_type>::__type& __r) noexcept
      { return std::addressof(__r); }
    };

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#endif

