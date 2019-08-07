#ifndef MATRIX_CHOLESKY_DECOMP_H
#define MATRIX_CHOLESKY_DECOMP_H 1

namespace matrix
{

/**
 * This class represents a Cholesky decomposition of a square matrix.
 */
template<typename _HermitianMatrix, typename _Vector>
  class cholesky_decomposition
  {

  public:

    using value_type = decltype(_HermitianMatrix{}[0][0]);

    template<typename _HermitianMatrix2, typename _Vector2>
      cholesky_decomposition(std::size_t n, const _HermitianMatrix2& a, _Vector2& d);

    template<typename _Vector2, typename _VectorOut>
      void backsubstitute(const _Vector2  b, _VectorOut& x) const;

    template<typename InVecIter, typename OutVecIter>
      void
      backsubstitution(InVecIter b_begin, InVecIter b_end,
		       OutVecIter x_begin) const;

    template<typename _HermitianMatrix2>
      void inverse(_HermitianMatrix2& a_inv) const;

  private:

    std::size_t m_n;

    _HermitianMatrix m_a;

    std::vector<value_type> m_d;
  };

/**
 * 
 */
template<typename _HermitianMatrix, typename _Vector>
  void
  cholesky_decomp(std::size_t n, _HermitianMatrix& a, _Vector& d);

/**
 * Solve the system @f$ Ax = b @f$ with a Cholesky decomposition.
 */
template<typename _HermitianMatrix, typename _Vector>
  void
  cholesky_backsub(std::size_t n, const _HermitianMatrix& a,
		   const _Vector& d, const _Vector& b, _Vector& x);

/**
 * 
 */
template<typename _HermitianMatrix, typename _Vector>
  void
  cholesky_invert(std::size_t n, const _HermitianMatrix& a, const _Vector& d,
		  _HermitianMatrix& a_inv);

} // namespace matrix

#include "matrix_cholesky_decomp.tcc"

#endif // MATRIX_CHOLESKY_DECOMP_H

