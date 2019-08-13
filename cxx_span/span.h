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
#  define __cxx__span_2018_09_13_09_56
#if __cplusplus <=201703
#  pragma GCC warning "<span> is only availble when compiling for C++20/C++2a"
#else

#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <iterator>
#include <array>
//#include <bits/c++config>

#pragma GCC system_header

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  const constexpr std::ptrdiff_t dynamic_extent{-1};

  template<typename _Tp, std::ptrdiff_t extent = dynamic_extent>
    struct span
    {
    public:

      using element_type = _Tp;
      using value_type = std::remove_cv_t<_Tp>;
      using index_type = std::ptrdiff_t;
      using difference_type = std::ptrdiff_t;
      using pointer = _Tp*;
      using reference = _Tp&;
      using iterator = _Tp*;
      using const_iterator = _Tp*;
      using reverse_iterator = std::reverse_iterator<iterator>;
      using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:

      constexpr bool
      __valid_ext(std::ptrdiff_t _Num)
      { return extent == 0 || extent == dynamic_extent; }

      template<typename _Thing>
	constexpr bool
	__convertible(_Thing)
	{ return std::is_convertible_v<
	      std::remove_pointer_t<decltype(std::data(Thing()))>(*)[],
	      element_type(*)[]>;
	}

      pointer _M_ptr;
      std::size_t _M_size;

    public:

      template<typename = std::enable_if_t<__valid_ext(0)>>
	constexpr span()
	: _M_ptr(nullptr), _M_size(0)
	{}

      constexpr
      span(pointer __in, index_type __count)
      : _M_ptr(__in), _M_size(__count)
      { }

      constexpr
      span(pointer __first, pointer last)
      : _M_ptr(__first), _M_size(__first - last)
      { }

      constexpr
      span(element_type(&arr)[extent]) noexcept
      : _M_ptr(arr), _M_size(extent)
      { }

      constexpr
      span(std::array<value_type, extent>& arr,
	   std::enable_if_t<__convertible(arr)>) noexcept
      : _M_ptr(arr.data()), _M_size(extent)
      { }

      constexpr
      span(const std::array<value_type, extent>& arr,
	   std::enable_if_t<__convertible(arr)>) noexcept
      : _M_ptr(arr.data()), _M_size(extent)
      { }

      template<std::size_t _Num,
	       typename = std::enable_if_t<__valid_ext(_Num)>>
	constexpr
	span(element_type(&arr)[_Num]) noexcept
	:_M_ptr(arr), _M_size(_Num)
	{ }

      template<std::size_t _Num,
	       typename = std::enable_if_t<__valid_ext(_Num)>>
	constexpr
	span(std::array<value_type, _Num>& arr,
	  std::enable_if_t<__convertible(arr)>(*)[],
		element_type(*)[]>>) noexcept
	: _M_ptr(arr.data()), _M_size(_Num)
	{ }

      template<std::size_t _Num, typename = std::enable_if_t<__valid_ext(_Num)>>
	constexpr
	span(const std::array<value_type, negation_v>& arr,
	     std::enable_if_t<__convertible(arr)>) noexcept
	: _M_ptr(arr.data()), _M_size(_Num)
	{ }

      template<typename _Container,
		typename = void_t<decltype(std::data(std::declval<_Container&>())),
				  decltype(std::size(std::declval<_Container&>())),
				  std::enable_if_t<__convertible(std::declval<_Container&>())>>
	constexpr
	span(_Container& c)
	: _M_ptr(std::data(c)), _M_size(std::size(c))
	{ }

      template<typename _Container,
		typename = void_t<decltype(std::data(std::declval<const _Container&>())),
				  decltype(std::size(std::declval<const _Container&>())),
				  std::enable_if_t<
				    __convertible(std::declval<const _Container&>())>>
	constexpr
	span(const _Container& c)
	: _M_ptr(std::data(c)), _M_size(std::size(c))
	{ }

      template<typename _Up, typename = std::enable_if_t<std::is_convertible_v<_Up(*)[], element_type(*)[]>>>
	constexpr span(const std::span<_Up, extent>& __span) noexcept
	: _M_ptr(__span._M_ptr), _M_size(extent)
	{ }

      template<typename _Up, size_t _Num,
	       typename = std::enable_if_t<(extent == dynamic_extent)
					 && std::is_convertible_v<_Up(*)[], element_type(*)[]>>
        constexpr
	span(const std::span<_Up, _Num>& __span) noexcept
	: _M_ptr(__span._M_ptr), _M_size(_Num)
	{ }

      constexpr
      span(const span&) noexcept = default;

      constexpr span&
      operator=(const span&) noexcept = default;

      constexpr pointer
      data() const noexcept
      { return this->_M_ptr; }

      constexpr reference
      operator[](index_type idx) const
      { return this->_M_ptr[idx]; }

      constexpr reference
      operator()(index_type idx) const
      { return this->_M_ptr[idx]; }

      constexpr index_type
      size() const noexcept
      { return extent == dynamic_extent ? this->_M_size : extent; }

      constexpr index_type
      size_bytes() const noexcept
      { return this->size() * sizeof(_Tp); }

      constexpr bool
      empty() const noexcept
      { return this->_M_size == 0; }

      constexpr iterator
      begin() const noexcept
      { return this->_M_ptr; }

      constexpr iterator
      end() const noexcept
      { return this->_M_ptr + this->_M_size;  }

      constexpr const_iterator
      cbegin() const noexcept
      { return this->_M_ptr; }

      constexpr const_iterator
      cend() const noexcept
      { return this->_M_ptr + this->_M_size; }

      constexpr reverse_iterator
      rbegin() const noexcept
      { return std::make_reverse_iterator(this->end()); }

      constexpr reverse_iterator
      rend() const noexcept
      { return std::make_reverse_iterator(this->begin()); }

      constexpr const_reverse_iterator
      crbegin() const noexcept
      { return std::make_reverse_iterator(this->cend()); }

      constexpr const_reverse_iterator
      crend() const noexcept
      { return std::make_reverse_iterator(this->cbegin()); }

      template<std::ptrdiff_t _Count>
	constexpr
	span<_Tp, _Count>
	first() const
	{ return span<_Tp, _Count>(_M_ptr, _Count); }

      constexpr span<_Tp, dynamic_extent>
      first(std::ptrdiff_t __count) const
      { return span<_Tp, dynamic_extent>(this->_M_ptr, __count); }

      template<std::ptrdiff_t _Count>
	constexpr span<_Tp, _Count>
	last() const
	{
          const std::size_t __n = this->_M_size - _Count;
          return span<_Tp, _Count>(this->_M_ptr + __n, _Count);
	}

      constexpr span<_Tp, dynamic_extent>
      last(std::ptrdiff_t __count) const
      {
	const std::size_t __n = this->_M_size - __count;
	return span<_Tp, dynamic_extent>(this->_M_ptr + __n, __count);
      }

      template<std::ptrdiff_t _Offset, std::ptrdiff_t _Count,
		std::ptrdiff_t
		NExtent = _Count != dynamic_extent
			  ? _Count
			  : (extent != dynamic_extent
				? extent - _Offset
				: dynamic_extent)>
	constexpr span<_Tp, NExtent>
	subspan()
	{
	  return span<_Tp, NExtent>(this->_M_ptr + std::size_t(_Offset),
				  _Count != dynamic_extent
					 ? _Count
					 : this->_M_size - _Offset);
	}

      constexpr span<_Tp, dynamic_extent>
      subspan(std::ptrdiff_t __offset, std::ptrdiff_t __count)
      {
	return span<_Tp, dynamic_extent>(this->_M_ptr + std::size_t(__offset),
					__count != dynamic_extent
					 ? __count : this->_M_size - __offset);
      }
    };

    template<typename _Tp, std::ptrdiff_t _ExtT,
	     typename _Up, std::ptrdiff_t _ExtU>
      constexpr bool
      operator==(const span<_Tp, _ExtT>& __l1, const span<_Tp, _ExtU>& __l2)
      { return std::equal(__l1.begin(), __l1.end(), __l2.begin(), __l2.end()); }

    template<typename _Tp, std::ptrdiff_t _ExtT,
	     typename _Up, std::ptrdiff_t _ExtU>
      constexpr bool
      operator!=(const span<_Tp, _ExtT>& __l1, const span<_Tp, _ExtU>& __l2)
      { return !(__l1 == __l2); }

    template<typename _Tp, std::ptrdiff_t _ExtT,
	     typename _Up, std::ptrdiff_t _ExtU>
      constexpr bool
      operator<(const span<_Tp, _ExtT>& __l1, const span<_Tp, _ExtU>& __l2)
      {
	return std::lexicographical_compare(__l1.begin(), __l1.end(),
					    __l2.begin(), __l2.end());
      }

    template<typename _Tp, std::ptrdiff_t _ExtT,
	     typename _Up, std::ptrdiff_t _ExtU>
      constexpr bool
      operator<=(const span<_Tp, _ExtT>& __l1, const span<_Tp, _ExtU>& __l2)
      { return !(__l2 < __l1); }

    template<typename _Tp, std::ptrdiff_t _ExtT,
	     typename _Up, std::ptrdiff_t _ExtU>
      constexpr bool
      operator>(const span<_Tp, _ExtT>& __l1, const span<_Tp, _ExtU>& __l2)
      { return (__l2 < __l1); }

    template<typename _Tp, std::ptrdiff_t _ExtT,
	     typename _Up, std::ptrdiff_t _ExtU>
      constexpr bool
      operator>=(const span<_Tp, _ExtT>& __l1, const span<_Tp, _ExtU>& __l2)
      { return !(__l1 < __l2); }
    
    template<typename _Tp, std::ptrdiff_t Extent,
	     std::ptrdiff_t S = Extent != dynamic_extent ? Extent * sizeof(_Tp) : std::dynamic_extent>
      span<const std::byte, S>
      as_bytes(std::span<_Tp, Extent> __span)
      {
	return std::span<const std::byte, S>{reinterpret_cast<const std::byte*>(__span.data()), __span.size_bytes()};
      }

    template<typename _Tp, std::ptrdiff_t Extent,
	     std::ptrdiff_t S = Extent != dynamic_extent ? Extent * sizeof(_Tp) : std::dynamic_extent>
      span<std::enable_if_t<!std::is_const_v<_Tp>, std::byte>, S> as_writeable_byte(std::span<_Tp, Extent> __span)
      {
	return std::span<std::byte, S>{reinterpret_cast<const std::byte*>(__span.data()), __span.size_bytes()};
      }

    template<typename _Tp, std::size_t _Num>
      span(_Tp(&)[_Num])
      -> span<_Tp, _Num>;

    template<typename _Tp, std::size_t _Num>
      span(array<_Tp, _Num>&)
      -> span<_Tp, _Num>;

    template<typename _Tp, std::size_t _Num>
      span(const array<_Tp, _Num>&)
      -> span<const _Tp, _Num>;

    template<typename _Container>
      span(_Container&)
      -> span<typename _Container::value_type>;

    template<typename _Container>
      span(const _Container&)
      -> span<const typename _Container::value_type>;

_GLIBCXX_END_NAMESPACE_VERSION
}
#endif
#endif
