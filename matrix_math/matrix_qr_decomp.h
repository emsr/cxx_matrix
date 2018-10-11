#ifndef MATRIX_QR_DECOMP_H
#define MATRIX_QR_DECOMP_H 1


namespace matrix
{

/**
 *  This class represents an QR decomposition.
 */
template<typename _NumTp, typename _Matrix>
  class qr_decomposition
  {

  public:

    using value_type = std::decay_t<decltype(_Matrix{}[0][0])>;

    template<typename _Matrix2>
      qr_decomposition(std::size_t m_n_rows, std::size_t n_cols,
		       _Matrix2& a);

    template<typename _Vector2, typename _VectorOut>
      void backsubstitution(_Vector2& b, _VectorOut& x);

    template<typename _InVecIter, typename _OutVecIter>
      void
      backsubstitution(_InVecIter b_begin, _InVecIter b_end,
		       _OutVecIter x_begin) const;

    template<typename _Matrix2>
      void inverse(_Matrix2& a_inv);

    template<typename _Matrix2, typename _Vector2>
      void update(_Matrix2& r, _Matrix2& qt,
		  _Vector2& u, _Vector2& v);

  private:

    std::size_t m_n_rows;

    std::size_t m_n_cols;

    _Matrix m_a;

    std::vector<std::size_t> m_c;

    std::vector<std::size_t> m_d;

    bool m_singular;
  };

/**
 * Constructs the QR decomposition of a[0..n_rows - 1][0..n_cols - 1].  The upper triangular matrix R
 * is returned in the upper triangle of a except the diagonal elements of R which are returned
 * in d[0..n_cols - 1].  The orthogonal matrix Q is represented as a product of n - 1 Householder
 * matrices Q_0...Q_n-2 where Q_j = 1 - u_j x u_j/c_j.  The ith component of u_j is zero for
 * i = 
 */
template<typename _Matrix, typename _Vector>
  void
  qr_decomp(std::size_t n_rows, std::size_t n_cols,
	    _Matrix& a,
	    _Vector& c, _Vector& d, bool& singular);

/**
 * This routine solves the set of equations Rx = b where R is the upper triangular
 * matrix stored in a[0..n_rows - 1][0..n_cols - 1] and d[0..n_cols - 1].
 * Here n_rows >= n_cols.
 */
template<typename _Matrix, typename _Vector, typename _VectorB>
  void
  r_backsub(std::size_t n_rows, std::size_t n_cols,
	    const _Matrix& a, const _Vector& d,
	    _VectorB& b);

/**
 * This routine solves the set of equations Ax = b.
 * The inputs are the QR decomposition of the matrix in a[0..n_rows - 1][0..n_cols - 1],
 * c[0..n_cols - 1], and d[0..n_cols - 1].
 * The vector b[0..n_rows - 1] is input as the "RHS" and output and the solution.
 * Here n_rows >= n_cols.
 */
template<typename _Matrix, typename _Vector, typename _VectorB>
  void
  qr_backsub(const std::size_t n_rows, const std::size_t n_cols,
	     const _Matrix& a, const _Vector& c, const _Vector& d,
	     _VectorB& b);

/**
 * Inverts a matrix given the QR decomposed matrix.
 * The inverse matrix is allocated in this routine so make sure the pointer is freed first.
 *
 * The inverse matrix is NOT in QR form.
 */
template<typename _Matrix, typename _Vector>
  void
  qr_invert(std::size_t n_rows, std::size_t n_cols,
	    const _Matrix& a_qr, const _Vector& c, const _Vector& d,
	    _Matrix& a_inv);

/**
 *  Update the QR decomposition.
 */
template<typename _Matrix, typename _Vector>
  void
  qr_update(std::size_t n_rows, std::size_t n_cols,
	    _Matrix& r, _Matrix& qt,
	    _Vector& u, _Vector& v);

/**
 *  Do a Jacobi rotation on rows i and i+1 of the matrices r[0..n_cols - 1][0..n_cols - 1]
 *  and qt[0..n_cols - 1][0..n_rows - 1].
 *  The angle is specified with a and b such that
 *    cos(theta) = a/sqrt(a^2 + b^2)
 *    sin(theta) = b/sqrt(a^2 + b^2).
 */
template<typename _NumTp, typename _Matrix, typename _Vector>
  void
  jacobi_rotate(const int i, const int n_rows, const int n_cols,
		_Matrix& r, _Matrix& qt,
		_NumTp a, _NumTp b);


} // namespace matrix

#include "matrix_qr_decomp.tcc"

#endif // MATRIX_QR_DECOMP_H

