// Allocator traits -*- C++ -*-

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

/** @file ext/alloc_traits.h
 *  This file is a GNU extension to the Standard C++ Library.
 */

#pragma once

#pragma GCC system_header

#include "../alloc_traits.h"

namespace geode::stl {
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  template<typename _Alloc>
    struct __allocator_always_compares_equal : geode::stl::false_type { };

  template<typename _Tp>
    struct __allocator_always_compares_equal<allocator<_Tp>>
    : geode::stl::true_type { };

  template<typename, typename> struct array_allocator;

  template<typename _Tp, typename _Array>
    struct __allocator_always_compares_equal<array_allocator<_Tp, _Array>>
    : geode::stl::true_type { };

  template<typename> struct bitmap_allocator;

  template<typename _Tp>
    struct __allocator_always_compares_equal<bitmap_allocator<_Tp>>
    : geode::stl::true_type { };

  template<typename> struct malloc_allocator;

  template<typename _Tp>
    struct __allocator_always_compares_equal<malloc_allocator<_Tp>>
    : geode::stl::true_type { };

  template<typename> struct mt_allocator;

  template<typename _Tp>
    struct __allocator_always_compares_equal<mt_allocator<_Tp>>
    : geode::stl::true_type { };

  template<typename> struct new_allocator;

  template<typename _Tp>
    struct __allocator_always_compares_equal<new_allocator<_Tp>>
    : geode::stl::true_type { };

  template<typename> struct pool_allocator;

  template<typename _Tp>
    struct __allocator_always_compares_equal<pool_allocator<_Tp>>
    : geode::stl::true_type { };

/**
 * @brief  Uniform interface to C++98 and C++0x allocators.
 * @ingroup allocators
*/
template<typename _Alloc>
  struct __alloc_traits
  : allocator_traits<_Alloc>
  {
    typedef _Alloc allocator_type;

    typedef allocator_traits<_Alloc>           _Base_type;
    typedef typename _Base_type::value_type         value_type;
    typedef typename _Base_type::pointer            pointer;
    typedef typename _Base_type::const_pointer      const_pointer;
    typedef typename _Base_type::size_type          size_type;
    typedef typename _Base_type::difference_type    difference_type;
    // C++11 allocators do not define reference or const_reference
    typedef value_type&                             reference;
    typedef const value_type&                       const_reference;
    using _Base_type::allocate;
    using _Base_type::deallocate;
    using _Base_type::construct;
    using _Base_type::destroy;
    using _Base_type::max_size;

  private:
    template<typename _Ptr>
      using __is_custom_pointer
	= std::conjunction<std::is_same<pointer, _Ptr>,
		      geode::stl::__not_<is_pointer<_Ptr>>>;

  public:
    // overload construct for non-standard pointer types
    template<typename _Ptr, typename... _Args>
      static typename std::enable_if<__is_custom_pointer<_Ptr>::value>::type
      construct(_Alloc& __a, _Ptr __p, _Args&&... __args)
      {
	_Base_type::construct(__a, std::addressof(*__p),
			      std::forward<_Args>(__args)...);
      }

    // overload destroy for non-standard pointer types
    template<typename _Ptr>
      static typename std::enable_if<__is_custom_pointer<_Ptr>::value>::type
      destroy(_Alloc& __a, _Ptr __p)
      { _Base_type::destroy(__a, std::addressof(*__p)); }

    static _Alloc _S_select_on_copy(const _Alloc& __a)
    { return _Base_type::select_on_container_copy_construction(__a); }

    static void _S_on_swap(_Alloc& __a, _Alloc& __b)
    { __alloc_on_swap(__a, __b); }

    static constexpr bool _S_propagate_on_copy_assign()
    { return _Base_type::propagate_on_container_copy_assignment::value; }

    static constexpr bool _S_propagate_on_move_assign()
    { return _Base_type::propagate_on_container_move_assignment::value; }

    static constexpr bool _S_propagate_on_swap()
    { return _Base_type::propagate_on_container_swap::value; }

    static constexpr bool _S_always_equal()
    { return __allocator_always_compares_equal<_Alloc>::value; }

    static constexpr bool _S_nothrow_move()
    { return _S_propagate_on_move_assign() || _S_always_equal(); }

    static constexpr bool _S_nothrow_swap()
    {
      return !_S_propagate_on_swap()
       	|| noexcept(swap(std::declval<_Alloc&>(), std::declval<_Alloc&>()));
    }

    template<typename _Tp>
      struct rebind
      { typedef typename _Base_type::template rebind_alloc<_Tp> other; };
  };

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std
