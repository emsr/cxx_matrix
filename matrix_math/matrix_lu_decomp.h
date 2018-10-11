#ifndef MATRIX_LU_DECOMP_H
#define MATRIX_LU_DECOMP_H 1

#include <vector>

namespace matrix
{

/**
 * This class represents an lower-upper decomposition of a square matrix.
 */
template<typename _NumTp, typename _SquareMatrix>
  class lu_decomposition
  {

  public:

    using value_type = std::decay_t<decltype(_SquareMatrix{}[0][0])>;

    template<typename _SquareMatrix2>
      lu_decomposition(std::size_t n, const _SquareMatrix2& a);

    template<typename _Vector, typename _VectorOut>
      void backsubstitute(const _Vector& b, _VectorOut& x) const;

    template<typename InVecIter, typename OutVecIter>
      void
      backsubstitution(InVecIter b_begin, InVecIter b_end,
		      OutVecIter x_begin) const;

    template<typename _SquareMatrix2, typename _Vector, typename _VectorOut>
      void
      improve(const _SquareMatrix2& a_orig,
	     const _Vector& b, _VectorOut& x) const;

    template<typename _SquareMatrix2, typename InVecIter, typename OutVecIter>
      void
      improve(const _SquareMatrix2& a_orig,
	     InVecIter b_begin, InVecIter b_end,
	     OutVecIter x_begin) const;

    template<typename _SquareMatrix2>
      void
      inverse(_SquareMatrix2& a_inv) const;

    _NumTp determinant() const;

    _NumTp trace() const;

  private:

    std::size_t m_n;

    _SquareMatrix m_a;

    std::vector<std::size_t> m_index;

    int m_parity;
  };

/**
 * Given an n*n matrix a[0..n-1][0..n-1], this routine replaces it
 * by the LU (Lower-triangular Upper-triangular)  decomposition
 * of a rowwise permutation of itself.
 * The matrix size n and a[][] are input.  a[][] is output, index[]
 * is an output vector which row permutation effected by the partial pivoting;
 * d is output as the parity of the row permutation
 */
template<typename _NumTp, typename _SquareMatrix, typename _Vector>
  void
  lu_decomp(std::size_t n, _SquareMatrix& a,
	    _Vector& index, _NumTp& parity);

/**
 * Solve the set of n linear equations a.x = b.  Here a[0..n-1][0..n-1] is input, not as the original matrix a but as 
 * its LU decomposition, determined by the routine lu_decomp().  b[0..n-1] is input as the right hand side vector b 
 * and returns with the left-hand solution vector x.  a, n, and index are not modified by this routine and can be left 
 * in place for successive calls with different right hand sides b[0..n-1].  This routine takes into account the 
 * possibility that b will begin with a lot of zeros so that it is efficient for use in matrix inversion.
 */
template<typename _SquareMatrix, typename _VectorInt, typename _Vector>
  void
  lu_backsub(const std::size_t n,
	     const _SquareMatrix& a,
	     const _VectorInt& index,
	     _Vector& b);

/**
 * Improves a solution vector x of the linear set A.x = b.  The matrix a and the
 * LU decomposition of a a_lu (with its row permutation vector index) and the
 * right-hand side vector are input along with the solution vector x.  
 * The solution vector x is improved and modified on output.
 */
template<typename _SquareMatrix, typename _VectorInt, typename _Vector>
  void
  lu_improve(const std::size_t n, const _SquareMatrix& a,
	     const _SquareMatrix& a_lu,
	     const _VectorInt& index, const _Vector& b, _Vector& x);

/**
 * Inverts a matrix given the LU decomposed matrix.
 *
 * The inverse matrix is NOT in LU form.
 */
template<typename _SquareMatrix, typename _VectorInt>
  void
  lu_invert(const std::size_t n, const _SquareMatrix& a_lu,
	    const _VectorInt& index, _SquareMatrix& a_inv);

/**
 * Compute determinant of LU decomposed matrix.
 */
template<typename _NumTp, typename _SquareMatrix>
  _NumTp
  lu_determinant(const std::size_t n, const _SquareMatrix& a_lu, const _NumTp parity);

/**
 * Compute trace of LU decomposed matrix.
 */
template<typename _SquareMatrix>
  auto
  lu_trace(const std::size_t n, const _SquareMatrix& a_lu);

} // namespace matrix

#include "matrix_lu_decomp.tcc"

#endif // MATRIX_LU_DECOMP_H
