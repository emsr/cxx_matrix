#ifndef MATRIX_CHOLESKY_DECOMP_TCC
#define MATRIX_CHOLESKY_DECOMP_TCC 1


#include <cstdlib>
#include <cmath>
#include <stdexcept>
#include <vector>


namespace matrix
{


/**
 * 
 */
template<typename _HermitianMatrix, typename _Vector>
  void
  cholesky_decomp(std::size_t n, _HermitianMatrix& a, _Vector& d)
  {
    for (std::size_t i = 0; i < n; ++i)
      {
	for (std::size_t j = 0; j < n; ++j)
	  {
	    auto sum = a[i][j];
	    for (std::ptrdiff_t k = i - 1; k >= 0; --k)
	      sum -= a[i][k] * a[j][k];
	    if (i == j)
	      {
		if (sum <= 0)
		  std::__throw_logic_error("cholesky_decomp: "
					   "Matrix must be positive definite");
		d[i] = std::sqrt(sum);
	      }
	    else
	      a[j][i] = sum / d[i];
	  }
      }
    for (std::size_t j = 0; j < n; ++j)
      for (std::size_t i = 0; i < j; ++i)
	a[i][j] = 0;
  }


/**
 * Solve the system @f$ Ax = b @f$ with a Cholesky decomposition.
 */
template<typename _HermitianMatrix, typename _Vector>
  void
  cholesky_backsub(std::size_t n, const _HermitianMatrix& a,
		   const _Vector& d, const _Vector& b, _Vector& x)
  {
    for (std::size_t i = 0; i < n; ++i)
      {
	auto sum = b[i];
	for (std::ptrdiff_t k = i - 1; k >= 0; --k)
	  sum -= a[i][k] * x[k];
	x[i] = sum / d[i];
      }
    for (std::ptrdiff_t i = n - 1; i >= 0; --i)
      {
	auto sum = x[i];
	for (std::size_t k = i + 1; k < n; ++k)
	  sum -= a[k][i] * x[k];
	x[i] = sum / d[i];
      }
  }


/**
 * 
 */
template<typename _HermitianMatrix, typename _Vector>
  void
  cholesky_invert(std::size_t n, const _HermitianMatrix& a, const _Vector& d,
		  _HermitianMatrix& a_inv)
  {
    using NumTp = std::decay_t<decltype(a[0][0])>;

    for (std::ptrdiff_t i = 0; i < n; ++i)
      for (std::ptrdiff_t j = 0; j <= i; ++j)
	{
	  auto sum = (j == i ? NumTp{1} : NumTp{0});
	  for (std::ptrdiff_t k = i - 1; k >= j; --k)
	    sum -= a[i][k] * a_inv[j][k];
	  a_inv[j][i] = sum / d[i];
	}

    for (std::ptrdiff_t i = n - 1; i >= 0; --i)
      for (std::ptrdiff_t j = 0; j <= i; ++j)
	{
	  auto sum = (i < j ? NumTp{0} : a_inv[j][i]);
	  for (std::ptrdiff_t k = i + 1; k < n; ++k)
	    sum -= a[k][i] * a_inv[j][k];
	  a_inv[i][j] = a_inv[j][i] = sum / d[i];
	}
  }

} // namespace matrix

#endif // MATRIX_CHOLESKY_DECOMP_TCC



