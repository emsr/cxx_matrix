#ifndef MATRIX_GAUSS_JORDAN_H
#define MATRIX_GAUSS_JORDAN_H 1

namespace matrix
{

/**
 * Linear equation solution by Gauss-Jordan elimination. a[1..n][1..n] is an input matrix.
 * b[1..n][1..m] is the input matrix of m right-hand side vectors.
 * On output, a is replaced by its inverse, and b is replaced by the corresponding set of
 *solution vectors.
 */
template<typename NumTp, typename _SquareMatrix, typename _Matrix>
  void
  gauss_jordan(_SquareMatrix& a, std::size_t n, _Matrix& b, std::size_t m);

} // namespace matrix

#include "matrix_gauss_jordan.tcc"

#endif // MATRIX_GAUSS_JORDAN_TCC
