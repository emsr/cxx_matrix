#ifndef MATRIX_TRIDIAG_H
#define MATRIX_TRIDIAG_H 1

namespace matrix
{

/**
 * Solves a tridiagonal set of equations where a[1..n] is the subdiagonal vector,
 * b[1..n] is the diagonal vector, and c[1..n] is the superdiagonal vector, and
 * r[1..n] is the right hand side vector.  The solution is u[1..n].
 * a, b, c, and r are not modified.
 */
template<typename _Tp>
  void
  tridiagonal(const _Tp* a, const _Tp* b, const _Tp* c,
	      const _Tp* r, _Tp* u, std::size_t n);

/**
 * Solves for a vector x[1..n] the cyclic set of linear equations.
 * a[[1..n], b[1..n], c[1..n], and r[1..n] are input vectors of the three diagonal rows and the
 * right side respectively.  alpha and beta are the lower and upper corner entries respectively.
 * The input is not modified.
 */
template<typename _Tp>
  void
  cyclic(const _Tp* a, const _Tp* b, const _Tp* c,
	 _Tp alpha, _Tp beta,
	 const _Tp* r, _Tp* x, std::size_t n);

} // namespace matrix

#include "matrix_tridiag.tcc"

#endif // MATRIX_TRIDIAG_H
