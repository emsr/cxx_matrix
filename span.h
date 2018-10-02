//<span> C++ 

//Pull Request Header for <span> (C++2a)
//(c) 2018 Connor Horman
//This library is free software; 
//you can redistribute it and/or modify it under the
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



#ifndef __cxx__span_2018_09_13_09_56
#define __cxx__span_2018_09_13_09_56
#if __cplusplus <=201703
#pragma GCC warning "<span> is only availble when compiling for C++20/C++2a"
#else
#include <algorithm>
#include <bits/c++config>
#include <cstddef>
#include <type_traits>
#include <iterator>
#include <array>

#pragma GCC system_header

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  const constexpr std::ptrdiff_t dynamic_extent{-1};

  template<typename T, std::ptrdiff_t extent=dynamic_extent>
    struct span
    {
    public:

      using element_type = T;
      using value_type = std::remove_cv_t<T>;
      using index_type = std::ptrdiff_t;
      using difference_type = std::ptrdiff_t;
      using pointer = T*;
      using reference = T&;
      using iterator = T*;
      using const_iterator = T*;
      using reverse_iterator = std::reverse_iterator<iterator>;
      using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:

      pointer _M_ptr;
      std::size_t _M_size;

    public:

      template<typename = std::enable_if_t<extent == 0
					|| extent == dynamic_extent>>
	constexpr span()
	: _M_ptr(nullptr), _M_size(0)
	{}

      constexpr
      span(pointer in, index_type count)
      : _M_ptr(in), _M_size(count)
      { }

      constexpr
      span(pointer first, pointer last)
      : _M_ptr(first), _M_size(first-last)
      { }

      constexpr
      span(element_type(&arr)[extent]) noexcept
      : _M_ptr(arr), _M_size(extent)
      { }

      constexpr
      span(std::array<value_type, extent>& arr,
           std::enable_if_t<
              std::is_convertible_v<std::remove_pointer_t<decltype(std::data(arr))>(*)[],
              element_type(*)[]>, bool>=false)) noexcept
              :_M_ptr(arr.data()),_M_size(extent){}

      constexpr
      span(const std::array<value_type, extent>& arr,
           std::enable_if_t<
              std::is_convertible_v<std::remove_pointer_t<decltype(std::data(arr))>(*)[],
              element_type(*)[]>, bool>=false)) noexcept
      : _M_ptr(arr.data()), _M_size(extent)
      { }

      template<std::size_t N, typename=std::enable_if_t<extent == dynamic_extent>>
	constexpr
	span(element_type(&arr)[N]) noexcept
	:_M_ptr(arr), _M_size(N)
	{ }

      template<std::size_t N, typename=std::enable_if_t<extent == dynamic_extent>>
	constexpr
	span(std::array<value_type, N>& arr,
          std::enable_if_t<
              std::is_convertible_v<std::remove_pointer_t<decltype(std::data(arr))>(*)[],
              element_type(*)[]>, bool>=false)) noexcept
	: _M_ptr(arr.data()), _M_size(N)
	{ }

      template<std::size_t N, typename=std::enable_if_t<extent == dynamic_extent>>
	constexpr
	span(const std::array<value_type, negation_v>& arr,
          std::enable_if_t<
              std::is_convertible_v<std::remove_pointer_t<decltype(std::data(arr))>(*)[],
              element_type(*)[]>, bool> = false)) noexcept
	: _M_ptr(arr.data()), _M_size(N)
	{ }

      template<typename _Cont,
		typename = void_t<decltype(std::data(std::declval<_Cont&>())),
              decltype(std::size(std::declval<_Cont&>())),
              std::enable_if_t<
              std::is_convertible_v<std::remove_pointer_t<decltype(std::data(std::declval<_Cont&>()))>(*)[],
              element_type(*)[]>>>>
	constexpr
	span(_Cont& c)
	: _M_ptr(std::data(c)), _M_size(std::size(c))
	{ }

      template<typename _Cont,
		typename = void_t<decltype(std::data(std::declval<const _Cont&>())),
				  decltype(std::size(std::declval<const _Cont&>())),
				std::enable_if_t<
          std::is_convertible_v<std::remove_pointer_t<decltype(std::data(std::declval<const _Cont&>()))>(*)[],
          element_type(*)[]>>>>
	constexpr
	span(const _Cont& c)
	: _M_ptr(std::data(c)), _M_size(std::size(c))
	{ }

      template<typename U, typename = std::enable_if_t<std::is_convertible_v<U(*)[], element_type(*)[]>>
	constexpr span(const std::span<U, extent>& s) noexcept
	: _M_ptr(s._M_ptr), _M_size(extent)
	{ }

      template<typename U, size_t N,
	       typename = std::enable_if_t<(extent == dynamic_extent)
					 && std::is_convertible_v<U(*)[], element_type(*)[]>>
        constexpr
	span(const std::span<U, N>& s) noexcept
	: _M_ptr(s._M_ptr), _M_size(N)
	{ }

      constexpr
      span(const span&) noexcept = default;

      constexpr span&
      operator=(const span&) noexcept = default;

      constexpr pointer
      data() const noexcept
      { return _M_ptr; }

      constexpr reference
      operator[](index_type idx) const
      { return _M_ptr[idx]; }

      constexpr reference
      operator()(index_type idx) const
      { return _M_ptr[idx]; }

      constexpr index_type
      size() const noexcept
      { return extent == dynamic_extent ? _M_size:extent; }

      constexpr index_type
      size_bytes() const noexcept
      { return this->size() * sizeof(T); }

      constexpr bool
      empty() const noexcept
      { return _M_size == 0; }

      constexpr iterator
      begin() const noexcept
      { return _M_ptr; }

      constexpr iterator
      end() const noexcept
      { return _M_ptr + _M_size;  }

      constexpr const_iterator
      cbegin() const noexcept
      { return _M_ptr; }

      constexpr const_iterator
      cend() const noexcept
      { return _M_ptr + _M_size; }

      constexpr reverse_iterator
      rbegin() const noexcept
      { return std::make_reverse_iterator(end()); }

      constexpr reverse_iterator
      rend() const noexcept
      { return std::make_reverse_iteratr(begin()); }

      constexpr const_reverse_iterator
      crbegin() const noexcept
      { return std::make_reverse_iterator(cend()) }

      constexpr const_reverse_iterator
      crend() const noexcept
      { return std::make_reverse_iterator(cbegin()); }

      template<std::ptrdiff_t Count>
	constexpr
	span<T, Count> first() const
	{ return span<T, Count>(_M_ptr, Count); }

      constexpr span<T, dynamic_extent>
	first(std::ptrdiff_t Count) const
	{ return span<T, dynamic_extent>(_M_ptr, Count); }

      template<std::ptrdiff_t Count>
	constexpr span<T, Count>
	last() const
	{
          const std::size_t n = _M_size - Count;
          return span<T, Count>(_M_ptr + n, Count);
	}

      constexpr
      span<T, dynamic_extent>
      last(std::ptrdiff_t Count) const
      {
	const std::size_t n = _M_size - Count;
	return span<T, dynamic_extent>(_M_ptr + n, Count);
      }

      template<std::ptrdiff_t Offset, std::ptrdiff_t Count,
		std::ptrdiff_t
		NExtent = Count != dynamic_extent
			  ? Count
			  : (extent != dynamic_extent
				? extent - Offset
				: dynamic_extent)> constexpr
	constexpr span<T, NExtent>
	subspan()
	{
	  return span<T, NExtent>(this->_M_ptr + std::size_t(Offset),
				  Count != dynamic_extent
					 ? Count
					 : this->_M_size - Offset);
	}

      constexpr span<T, dynamic_extent>
      subspan(std::ptrdiff_t Offset, std::ptrdiff_t Count)
      {
	return span<T, dynamic_extent>(this->_M_ptr + std::size_t(Offset),
					Count != dynamic_extent
					 ? Count : this->_M_size - Offset);
      }
    };

    template<typename T, std::ptrdiff_t Extent1,
	     typename U, std::ptrdiff_t Extent2>
      constexpr bool
      operator==(const span<T, Extent1>& l1, const span<T, Extent2>& l2)
      { return std::equal(l1.begin(), l1.end(), l2.begin(), l2.end()); }

    template<typename T, std::ptrdiff_t Extent1,
	     typename U, std::ptrdiff_t Extent2>
      constexpr bool
      operator!=(const span<T, Extent1>& l1, const span<T, Extent2>& l2)
      { return !(l1== l2); }

    template<typename T, std::ptrdiff_t Extent1,
	     typename U, std::ptrdiff_t Extent2>
      constexpr bool
      operator<(const span<T, Extent1>& l1, const span<T, Extent2>& l2)
      {
	return std::lexicographical_compare(l1.begin(), l1.end(),
					    l2.begin(), l2.end());
      }

    template<typename T, std::ptrdiff_t Extent1,
	     typename U, std::ptrdiff_t Extent2>
      constexpr bool
      operator<=(const span<T, Extent1>& l1, const span<T, Extent2>& l2)
      { return l1 < l2 || l1 == l2; }

    template<typename T, std::ptrdiff_t Extent1,
	     typename U, std::ptrdiff_t Extent2>
      constexpr bool
      operator>(const span<T, Extent1>& l1, const span<T, Extent2>& l2)
      { return !(l1 <= l2); }

    template<typename T, std::ptrdiff_t Extent1, typename U, std::ptrdiff_t Extent2>
      constexpr bool
      operator>=(const span<T, Extent1>& l1, const span<T, Extent2>& l2)
      { return !(l1<l2); }
    
    template<typename T, std::ptrdiff_t Extent,
	     std::ptrdiff_t S = Extent != dynamic_extent ? Extent * sizeof(T) : std::dynamic_extent>
      span<const std::byte, S> as_bytes(std::span<T, Extent> s)
      {
	return std::span<const std::byte, S>{reinterpret_cast<const std::byte*>(s.data()), s.size_bytes()};
      }

    template<typename T, std::ptrdiff_t Extent,
	     std::ptrdiff_t S = Extent != dynamic_extent ? Extent * sizeof(T) : std::dynamic_extent>
      span<std::enable_if_t<!std::is_const_v<T>, std::byte>, S> as_writeable_byte(std::span<T, Extent> s)
      {
	return std::span<std::byte, S>{reinterpret_cast<const std::byte*>(s.data()), s.size_bytes()};
      }

    template<typename _Cont>
      span(_Cont&)
      -> span<typename _Cont::value_type>;

    template<typename _Cont>
      span(const _Cont&)
      -> span<std::add_const_t<typename _Cont::value_type>>;

    template<typename T, std::size_t N>
      span(T(&)[N])
      -> span<T, N>;

    template<typename T, std::size_t N>
      span(array<T, N>&)
      -> span<T, N>;

    template<typename T, std::size_t N>
      span(const array<T, N>&)
      -> span<std::add_const_t<T>, N>;

_GLIBCXX_END_NAMESPACE_VERSION
}
#endif
#endif
