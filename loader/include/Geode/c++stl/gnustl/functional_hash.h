// functional_hash.h header -*- C++ -*-

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

/** @file bits/functional_hash.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{functional}
 */

#ifndef _FUNCTIONAL_HASH_H
#define _FUNCTIONAL_HASH_H 1

#pragma GCC system_header

#include "hash_bytes.h"

namespace geode::stl {
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /** @defgroup hashes Hashes
   *  @ingroup functors
   *
   *   Hashing functors taking a variable type and returning a @c std::size_t.
   *
   *  @{
   */

  template<typename _Result, typename _Arg>
    struct __hash_base
    {
      typedef _Result     result_type;
      typedef _Arg      argument_type;
    };

  /// Primary class template hash.
  template<typename _Tp>
    struct hash;

  /// Partial specializations for pointer types.
  template<typename _Tp>
    struct hash<_Tp*> : public __hash_base<size_t, _Tp*>
    {
      size_t
      operator()(_Tp* __p) const noexcept
      { return reinterpret_cast<size_t>(__p); }
    };

  // Explicit specializations for integer types.
#define _Cxx_hashtable_define_trivial_hash(_Tp) 	\
  template<>						\
    struct hash<_Tp> : public __hash_base<size_t, _Tp>  \
    {                                                   \
      size_t                                            \
      operator()(_Tp __val) const noexcept              \
      { return static_cast<size_t>(__val); }            \
    };

  /// Explicit specialization for bool.
  _Cxx_hashtable_define_trivial_hash(bool)

  /// Explicit specialization for char.
  _Cxx_hashtable_define_trivial_hash(char)

  /// Explicit specialization for signed char.
  _Cxx_hashtable_define_trivial_hash(signed char)

  /// Explicit specialization for unsigned char.
  _Cxx_hashtable_define_trivial_hash(unsigned char)

  /// Explicit specialization for wchar_t.
  _Cxx_hashtable_define_trivial_hash(wchar_t)

  /// Explicit specialization for char16_t.
  _Cxx_hashtable_define_trivial_hash(char16_t)

  /// Explicit specialization for char32_t.
  _Cxx_hashtable_define_trivial_hash(char32_t)

  /// Explicit specialization for short.
  _Cxx_hashtable_define_trivial_hash(short)

  /// Explicit specialization for int.
  _Cxx_hashtable_define_trivial_hash(int)

  /// Explicit specialization for long.
  _Cxx_hashtable_define_trivial_hash(long)

  /// Explicit specialization for long long.
  _Cxx_hashtable_define_trivial_hash(long long)

  /// Explicit specialization for unsigned short.
  _Cxx_hashtable_define_trivial_hash(unsigned short)

  /// Explicit specialization for unsigned int.
  _Cxx_hashtable_define_trivial_hash(unsigned int)

  /// Explicit specialization for unsigned long.
  _Cxx_hashtable_define_trivial_hash(unsigned long)

  /// Explicit specialization for unsigned long long.
  _Cxx_hashtable_define_trivial_hash(unsigned long long)

#undef _Cxx_hashtable_define_trivial_hash

  struct _Hash_impl
  {
    static size_t
    hash(const void* __ptr, size_t __clength,
	 size_t __seed = static_cast<size_t>(0xc70f6907UL))
    { return _Hash_bytes(__ptr, __clength, __seed); }

    template<typename _Tp>
      static size_t
      hash(const _Tp& __val)
      { return hash(&__val, sizeof(__val)); }

    template<typename _Tp>
      static size_t
      __hash_combine(const _Tp& __val, size_t __hash)
      { return hash(&__val, sizeof(__val), __hash); }
  };

  struct _Fnv_hash_impl
  {
    static size_t
    hash(const void* __ptr, size_t __clength,
	 size_t __seed = static_cast<size_t>(2166136261UL))
    { return _Fnv_hash_bytes(__ptr, __clength, __seed); }

    template<typename _Tp>
      static size_t
      hash(const _Tp& __val)
      { return hash(&__val, sizeof(__val)); }

    template<typename _Tp>
      static size_t
      __hash_combine(const _Tp& __val, size_t __hash)
      { return hash(&__val, sizeof(__val), __hash); }
  };

  /// Specialization for float.
  template<>
    struct hash<float> : public __hash_base<size_t, float>
    {
      size_t
      operator()(float __val) const noexcept
      {
	// 0 and -0 both hash to zero.
	return __val != 0.0f ? _Hash_impl::hash(__val) : 0;
      }
    };

  /// Specialization for double.
  template<>
    struct hash<double> : public __hash_base<size_t, double>
    {
      size_t
      operator()(double __val) const noexcept
      {
	// 0 and -0 both hash to zero.
	return __val != 0.0 ? _Hash_impl::hash(__val) : 0;
      }
    };

  /// Specialization for long double.
  template<>
    struct hash<long double>
    : public __hash_base<size_t, long double>
    {
      _GLIBCXX_PURE size_t
      operator()(long double __val) const noexcept;
    };

  // @} group hashes

  // Hint about performance of hash functor. If not fast the hash based
  // containers will cache the hash code.
  // Default behavior is to consider that hasher are fast unless specified
  // otherwise.
  template<typename _Hash>
    struct __is_fast_hash : public geode::stl::true_type
    { };

  template<>
    struct __is_fast_hash<hash<long double>> : public geode::stl::false_type
    { };

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#endif // _FUNCTIONAL_HASH_H
