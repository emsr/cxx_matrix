#ifndef MATRIX_LU_DECOMP_TCC
#define MATRIX_LU_DECOMP_TCC 1

#include <cstdlib>
#include <vector>
#include <cmath>
#include <limits>

#include "matrix_util.h"

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
	    _Vector& index, _NumTp& parity)
  {
    const _NumTp TINY = _NumTp(1.0e-20L);

    std::vector<_NumTp> scale(n);
    parity = _NumTp{1};

    // Loop over rows to get the implicit scaling information.    
    for (std::size_t i = 0; i < n; ++i)
      {
	_NumTp big{0};
	for (std::size_t j = 0; j < n; ++j)
	  if (const auto temp = std::abs(a[i][j]); temp > big)
	    big = temp;
	if (big == _NumTp{0})
	  std::__throw_logic_error("lu_decomp: singular matrix");

	// Save the scaling for the row.
	scale[i] = _NumTp{1} / big;
      }

    // This is the loop over columns of Crout's method.
    for (std::size_t j = 0; j < n; ++j)
      {
	// Lower triangle.
	for (std::size_t i = 0; i < j; ++i)
	  {
	    auto sum = a[i][j];
	    for (std::size_t k = 0; k < i; ++k)
	      sum -= a[i][k] * a[k][j];
	    a[i][j] = sum;
	  }

	// Initialize for the search for the largest pivot point.
	auto imax = std::numeric_limits<std::size_t>::max();
	auto big = _NumTp{0};
	for (std::size_t i = j; i < n; ++i)
	  {
	    auto sum = a[i][j];
	    for (std::size_t k = 0; k < j; ++k)
	      sum -= a[i][k] * a[k][j];
	    a[i][j] = sum;
	    if (const auto dum = scale[i] * std::abs(sum); dum >= big)
	      {
		big = dum;
		imax = i;
	      }
	  }
	// Interchange rows if required.
	if (j != imax)
	  {
	    for (std::size_t k = 0; k < n; ++k)
	      std::swap(a[imax][k], a[j][k]);

	    // Change parity.
	    parity = -parity;

	    // Interchange the scale factor.
	    std::swap(scale[imax], scale[j]);
	  }
	index[j] = imax;
	if (a[j][j] == _NumTp{0})
	  a[j][j] = TINY;

	// Now finally divide by the pivot element
	if (j != n - 1)
	  {
	    const auto scale = _NumTp{1} / a[j][j];
	    for (std::size_t i = j + 1; i < n; ++i)
	      a[i][j] *= scale;
	  }
      } // Go back for the next column in the reduction.
  }


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
	     _Vector& b)
  {
    using _NumTp = std::remove_reference_t<decltype(a[0][0])>;

    //  When i_start is set to a non-negative value, it will become the index
    //  of the first nonvanishing element of b[0..n-1].
    //  Do the forward substitution unsrambling the permutation as we go.
    int i_start = -1;
    for (std::size_t i = 0; i < n; ++i)
      {
	auto i_perm = index[i];
	auto sum = b[i_perm];
	b[i_perm] = b[i];
	if (i_start > -1)
	  for (std::size_t j = i_start; j <= i - 1; ++j)
	    sum -= a[i][j] * b[j];
	else if (sum != _NumTp{0})
	  i_start = i;
	b[i] = sum;
      }

    //  Now do the backsubstitution.
    for (std::ptrdiff_t i = n - 1; i >= 0; --i)
      {
	auto sum = b[i];
	for (std::size_t j = i + 1; j < n; ++j)
	  sum -= a[i][j] * b[j];
	b[i] = sum / a[i][i];
      }

    return;
  }


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
	     const _VectorInt& index, const _Vector& b, _Vector& x)
  {
    using _NumTp = std::remove_reference_t<decltype(a[0][0])>;

    std::vector<_NumTp> r(n);

    for (std::size_t i = 0; i < n; ++i)
      {
	r[i] = -b[i];
	for (std::size_t j = 0; j < n; ++j)
	  r[i] += a[i][j] * x[j];
      }

    lu_backsub(a_lu, n, index, r);

    for (std::size_t i = 0; i < n; ++i)
      x[i] -= r[i];

    return;
  }


/**
 * Inverts a matrix given the LU decomposed matrix.
 *
 * The inverse matrix is NOT in LU form.
 */
template<typename _SquareMatrix, typename _VectorInt>
  void
  lu_invert(const std::size_t n, const _SquareMatrix& a_lu,
	    const _VectorInt& index, _SquareMatrix& a_inv)
  {
    using _NumTp = std::remove_reference_t<decltype(a_inv[0][0])>;

    for (std::size_t j = 0; j < n; ++j)
      {
	std::vector<_NumTp> col(n);
	col[j] = _NumTp{1};

	lu_backsub(n, a_lu, index, col);

	for (std::size_t i = 0; i < n; ++i)
	  a_inv[i][j] = col[i];
      }

    return;
  }


/**
 * Compute determinant of LU decomposed matrix.
 */
template<typename _NumTp, typename _SquareMatrix>
  _NumTp
  lu_determinant(const std::size_t n, const _SquareMatrix& a_lu, const _NumTp parity)
  {
    auto det = parity;

    for (std::size_t i = 0; i < n; ++i)
      det *= a_lu[i][i];

    return det;
  }


/**
 * Compute trace of LU decomposed matrix.
 */
template<typename _SquareMatrix>
  auto
  lu_trace(const std::size_t n, const _SquareMatrix& a_lu)
  {
    using _NumTp = std::remove_reference_t<decltype(a_lu[0][0])>;

    auto trace = _NumTp{0};

    for (std::size_t i = 0; i < n; ++i)
      {
	trace += a_lu[i][i];
	for (std::ptrdiff_t j = i - 1; j >= 0; --j)
	  trace += a_lu[i][j] * a_lu[j][i];
      }

    return trace;
  }

} // namespace matrix

#endif // MATRIX_LU_DECOMP_TCC
