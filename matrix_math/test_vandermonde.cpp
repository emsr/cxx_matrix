
/*
$HOME/bin/bin/g++ -std=gnu++17 -Wno-psabi -I../tr29124_test -o test_vandermonde test_vandermonde.cpp -lquadmath
./test_vandermonde > test_vandermonde.txt
*/

#include <ext/cmath>
#include <limits>
#include <vector>
#include <iostream>
#include <iomanip>
#include "matrix_math/matrix.h"

  /**
   * *** stole this from tr29124/quadrature/build_*
   *
   * Return a vector of zeros of the Chebyshev function of the second kind
   * of order @f$ n @f$, @f$ U_n(x) @f$.
   * The zeros are given by:
   * @f[
   *   x_k = \cos\left(\frac{k\pi}{n+1}\right), k \elem {1, ..., n}
   * @f]
   */
  template<typename _Tp>
    std::vector<__gnu_cxx::__quadrature_point_t<_Tp>>
    __chebyshev_u_zeros(unsigned int __n)
    {
      const auto _S_pi = __gnu_cxx::__const_pi<_Tp>();
      std::vector<__gnu_cxx::__quadrature_point_t<_Tp>> __pt(__n);
      for (unsigned int __k = 1; __k <= __n; ++__k)
	{
	  auto __arg = _Tp(__k) / _Tp(__n + 1);
	  auto __half = __gnu_cxx::__fp_is_equal<_Tp>(__arg, _Tp{0.5L});
	  auto __z = (__half ? _Tp{0} : __gnu_cxx::cos_pi(__arg));
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
	const auto _S_pi = __gnu_cxx::__const_pi<_Tp>();
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
    std::cout.precision(__gnu_cxx::__digits10<_Tp>());
    auto w = 8 + std::cout.precision();

    std::cout << "\n\nCQUAD Rules\n";
    for (const auto& n : {4, 8, 16, 32, 64})
      {
	std::cout << "\nClenshaw-Curtis " << n << "\n";
	auto ccvec = build_clenshaw_curtis_sum<long double>(n);
	for (const auto& cc : ccvec)
	  {
	    std::cout << std::setw(w) << cc.__point << ' '
		      << std::setw(w) << cc.__weight << ' '
		      << '\n';
	  }

        std::vector<std::vector<_Tp>> vdm(n, std::vector<_Tp>(n));
	for (int i = 0; i < n; ++i)
	  {
	    auto xk = _Tp{1};
	    for (int j = 0; j < n; ++j)
	      {
		vdm[i][j] = xk;
		xk *= ccvec[i].__point;
	      }
	  }
	//vandermonde(std::size_t n, const _Tp* x, const _Tp* q, _Tp* w);
	using matrix_t = std::vector<std::vector<_Tp>>;
	//matrix::lu_decomposition<_Tp, matrix_t> lu(n, vdm);
	//lu.inverse(inv);
	matrix_t inv(n, std::vector<_Tp>(n));
	std::vector<int> index(n);
	_Tp parity;
	matrix::lu_decomp(n, vdm, index, parity);
	matrix::lu_invert(n, vdm, index, inv);
	for (int i = 0; i < n; ++i)
	  {
	    for (int j = 0; j < n; ++j)
	      {
		if (std::abs(inv[i][j]) < 50 * std::numeric_limits<_Tp>::epsilon())
		  inv[i][j] = _Tp{};
		std::cout << ' ' << inv[i][j];
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
}

