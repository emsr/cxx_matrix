
#include <ext/cmath>
#include <limits>
#include <vector>
#include <iostream>
#include <iomanip>

#include <ext/matrix.h>
#include <bits/quadrature_point.h>

#include <ext/float128_limits.h>
#include <ext/float128_io.h>

  /**
   * @brief  Return the Legendre polynomial by upward recursion
   * 	     on degree @f$ l @f$.
   *
   * The Legendre function of degree @f$ l @f$ and argument @f$ x @f$,
   * @f$ P_l(x) @f$, is defined by:
   * @f[
   *   P_l(x) = \frac{1}{2^l l!}\frac{d^l}{dx^l}(x^2 - 1)^{l}
   * @f]
   * This can be expressed as a series:
   * @f[
   *   P_l(x) = \frac{1}{2^l l!}\sum_{k=0}^{\lfloor l/2 \rfloor}
   *            \frac{(-1)^k(2l-2k)!}{k!(l-k)!(l-2k)!}x^{l-2k}
   * @f]
   *
   * @param  __l  The degree of the Legendre polynomial.  @f$ l >= 0 @f$.
   * @param  __x  The argument of the Legendre polynomial.
   */
  template<typename _Tp>
    _Tp
    __legendre_p_norm(unsigned int __l, _Tp __x)
    {
      const auto norm = std::sqrt(_Tp(2 * __l + 1) / _Tp{2});

      if (__x == _Tp{+1})
	return norm * _Tp{+1};
      else if (__x == _Tp{-1})
	return norm * (__l % 2 == 1 ? _Tp{-1} : _Tp{+1});
      else
	{
	  auto _P_lm2 = _Tp{1};
	  if (__l == 0)
	    return norm * _P_lm2;

	  auto _P_lm1 = __x;
	  if (__l == 1)
	    return norm * _P_lm1;

	  auto _P_l = _Tp{2} * __x * _P_lm1 - _P_lm2
		    - (__x * _P_lm1 - _P_lm2) / _Tp{2};
	  for (unsigned int __ll = 3; __ll <= __l; ++__ll)
	    {
	      _P_lm2 = _P_lm1;
	      _P_lm1 = _P_l;
	      // This arrangement is supposed to be better for roundoff
	      // protection, Arfken, 2nd Ed, Eq 12.17a.
	      _P_l = _Tp{2} * __x * _P_lm1 - _P_lm2
		    - (__x * _P_lm1 - _P_lm2) / _Tp(__ll);
	    }

	  return norm * _P_l;
	}
    }

  /**
   * Return a Chebyshev polynomial of non-negative order @f$ n @f$
   * and real argument @f$ x @f$ by the recursion
   * @f[
   *    C_n(x) = 2xC_{n-1} - C_{n-2}
   * @f]
   *
   * @tparam _Tp The real type of the argument
   * @param __n The non-negative integral order
   * @param __x The real argument @f$ -1 <= x <= +1 @f$
   * @param _C0 The value of the zeroth-order Chebyshev polynomial at @f$ x @f$
   * @param _C1 The value of the first-order Chebyshev polynomial at @f$ x @f$
   */
  template<typename _Tp>
    std::tuple<_Tp, _Tp, _Tp>
    __chebyshev_recur(unsigned int __n, _Tp __x, _Tp _C0, _Tp _C1)
    {
      auto _Ck = _Tp{2} * __x * _C1 - _C0;
      for (unsigned int __j = 2; __j < __n; ++__j)
      {
	_C0 = _C1;
	_C1 = _Ck;
	_Ck = _Tp{2} * __x * _C1 - _C0;
      }
      return std::make_tuple(_Ck, _C1, _C0);
    }

  /**
   * Return the Chebyshev polynomial of the first kind @f$ T_n(x) @f$
   * of non-negative order @f$ n @f$ and real argument @f$ x @f$.
   *
   * The Chebyshev polynomial of the first kind is defined by:
   * @f[
   *    T_n(x) = \cos(n \theta)
   * @f]
   * where @f$ \theta = \arccos(x) @f$, @f$ -1 <= x <= +1 @f$.
   *
   * @tparam _Tp The real type of the argument
   * @param __n The non-negative integral order
   * @param __x The real argument @f$ -1 <= x <= +1 @f$
   */
  template<typename _Tp>
    _Tp
    __chebyshev_t(unsigned int __n, _Tp __x)
    {
      auto _T0 = _Tp{1};
      if (__n == 0)
	return _T0;

      auto _T1 = __x;
      if (__n == 1)
	return _T1;

      auto _Ts = __chebyshev_recur(__n, __x, _T0, _T1);
      return std::get<0>(_Ts);
    }

  /**
   * Return the Chebyshev polynomial of the second kind @f$ U_n(x) @f$
   * of non-negative order @f$ n @f$ and real argument @f$ x @f$.
   *
   * The Chebyshev polynomial of the second kind is defined by:
   * @f[
   *    U_n(x) = \frac{\sin \left[(n + 1)\theta \right]}{\sin(\theta)}
   * @f]
   * where @f$ \theta = \arccos(x) @f$, @f$ -1 <= x <= +1 @f$.
   *
   * @tparam _Tp The real type of the argument
   * @param __n The non-negative integral order
   * @param __x The real argument @f$ -1 <= x <= +1 @f$
   */
  template<typename _Tp>
    _Tp
    __chebyshev_u(unsigned int __n, _Tp __x)
    {
      auto _U0 = _Tp{1};
      if (__n == 0)
	return _U0;

      auto _U1 = _Tp{2} * __x;
      if (__n == 1)
	return _U1;

      auto _Us = __chebyshev_recur(__n, __x, _U0, _U1);
      return std::get<0>(_Us);
    }

  /**
   * *** stole this from tr29124/quadrature/build_*
   *
   * Return a vector of zeros of the Chebyshev function of the second kind
   * of order @f$ n @f$, @f$ U_n(x) @f$.
   * The zeros are given by:
   * @f[
   *   x_k = \cos\left(\frac{k\pi}{n + 1}\right), k \elem {1, ..., n}
   * @f]
   */
  template<typename _Tp>
    std::vector<__gnu_cxx::__quadrature_point_t<_Tp>>
    __chebyshev_u_zeros(unsigned int __n)
    {
      const auto _S_pi = _Tp{3.141592653589793238462643383279502884195Q};
      std::vector<__gnu_cxx::__quadrature_point_t<_Tp>> __pt(__n);
      for (unsigned int __k = 1; __k <= __n; ++__k)
	{
	  auto __arg = _Tp(__k) / _Tp(__n + 1);
	  auto __half = (__arg == _Tp{0.5Q});
	  auto __z = (__half ? _Tp{0} : std::cos(_S_pi * __arg));
	  auto __w = _S_pi * (_Tp{1} - __z * __z) / _Tp(__n + 1);
	  __pt[__k - 1].__point = __z;
	  __pt[__k - 1].__weight = __w;
	}
      return __pt;
    }

/**
 * *** stole this from tr29124/quadrature/build_*
 *
 * @f[
 *    w_k = frac{c_k}{n}\left[1-\sum_{j=1}^{[n/2]}\frac{b_k}{4j^2-1}
 *            \cos\left(\frac{2jk\pi}{n}\right)\right]
 *    \mbox{   } k = 0, 1, ..., n
 * @f]
 * 
 * @see Fast Construction of the Fejer and Clenshaw-Curtis Quadrature Rules
 */
template<typename _Tp>
  std::vector<__gnu_cxx::__quadrature_point_t<_Tp>>
  build_clenshaw_curtis_sum(std::size_t __n)
  {
    std::vector<__gnu_cxx::__quadrature_point_t<_Tp>> __out(__n + 1);
    if (__n == 1)
      {
	__out[0].__point = _Tp{0};
	__out[0].__weight = _Tp{2};
	return __out;
      }
    else
      {
	const auto _S_pi = _Tp{3.141592653589793238462643383279502884195Q};
	auto uz = __chebyshev_u_zeros<_Tp>(__n - 1);
	__out[0].__point = _Tp{+1};
	__out[0].__weight = _Tp{1} / (__n * __n - 1 + __n % 2);
	for (auto __k = 1u; __k <= uz.size(); ++__k)
	  {
	    __out[__k].__point = uz[__k - 1].__point;

	    auto __sum = _Tp{0};
	    for (auto __j = 1u; __j <= __n / 2; ++__j)
	      {
		auto __b = _Tp(__j == __n / 2 ? 1 : 2);
		__sum += __b * std::cos(2 * __j * __k * _S_pi / __n)
		       / _Tp(4 * __j * __j - 1);
	      }
	    auto __w = _Tp{2} * (_Tp{1} - __sum) / _Tp(__n);
	    __out[__k].__weight = __w;
	  }
	__out[__n].__point = _Tp{-1};
	__out[__n].__weight = __out[0].__weight;
	return __out;
      }
  }

template<typename _Tp>
  void
  test_vandermonde()
  {
    std::cout.precision(std::numeric_limits<_Tp>::digits10);
    auto w = 8 + std::cout.precision();

    std::cout << "\n\nCQUAD Rules\n";
    for (const auto& n : {4, 8, 16, 32, 64})
      {
	std::cout << "\nClenshaw-Curtis " << n << "\n";
	auto ccvec = build_clenshaw_curtis_sum<long double>(n);
	std::reverse(ccvec.begin(), ccvec.end());
	for (const auto& cc : ccvec)
	  {
	    std::cout << std::setw(w) << cc.__point << ' '
		      << std::setw(w) << cc.__weight << ' '
		      << '\n';
	  }

	// The Vandermonde-like matrices use polynomial values not monomial powers.
        std::vector<std::vector<_Tp>> vdm(n + 1, std::vector<_Tp>(n + 1));
	for (int i = 0; i <= n; ++i)
	  for (int j = 0; j <= n; ++j)
	    vdm[i][j] = __legendre_p_norm(j, ccvec[i].__point);
	using matrix_t = std::vector<std::vector<_Tp>>;
	matrix_t inv(n + 1, std::vector<_Tp>(n + 1));
	std::vector<int> index(n + 1);
	_Tp parity;
	matrix::lu_decomp(n + 1, vdm, index, parity);
	matrix::lu_invert(n + 1, vdm, index, inv);
	std::cout << '\n';
	for (int i = 0; i <= n; ++i)
	  {
	    for (int j = 0; j <= n; ++j)
	      {
		if (std::abs(inv[i][j]) < 50 * std::numeric_limits<_Tp>::epsilon())
		  inv[i][j] = _Tp{};
		std::cout << ' ' << std::setw(w) << inv[i][j];
	      }
	    std::cout << '\n';
	  }
	std::cout << '\n';
      }
  }

int
main()
{
  test_vandermonde<long double>();

  test_vandermonde<__float128>();
}

