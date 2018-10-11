#ifndef MATRIX_SV_DECOMP_TCC
#define MATRIX_SV_DECOMP_TCC

namespace matrix
{

/**
 *  This class represents an singular value decomposition of a matrix.
 */
template<typename NumTp, typename _Matrix>
  class sv_decomposition
  {

  public:

    using value_type = std::decay_t<decltype(_Matrix{}[0][0])>;

    template<typename _Matrix2>
      sv_decomposition(std::size_t m_n_rows, std::size_t n_cols,
		       _Matrix2& a);

    template<typename _Vector2, typename _VectorOut>
      void
      backsubstitution(const _Vector2& b, _VectorOut& x) const;

    template<typename _InVecIter, typename _OutVecIter>
      void
      backsubstitution(_InVecIter b_begin, _InVecIter b_end,
		       _OutVecIter x_begin) const;

    template<typename _Matrix2, typename _Vector2, typename _VectorOut>
      void
      improve(const _Matrix2 a_orig,
	      const _Vector2& b, _VectorOut& x) const;

    template<typename _Matrix2, typename InVecIter, typename OutVecIter>
      void
      improve(const _Matrix2 a_orig,
	      InVecIter b_begin, InVecIter b_end,
	      OutVecIter x_begin) const;

  private:

    std::size_t m_n_rows;

    std::size_t m_n_cols;

    _Matrix m_a;

    std::vector<std::size_t> m_w;

    std::vector<std::size_t> m_v;
  };

/**
 *  
 */
template<typename _Matrix, typename _Vector>
  void
  sv_decomp(const std::size_t n_rows, const std::size_t n_cols,
	    _Matrix& a, _Vector& w, _Matrix& v);

/**
 *  
 */
template<typename _Matrix, typename _Vector>
  void
  sv_backsub(std::size_t n_rows, std::size_t n_cols,
	     const _Matrix& u,
	     const _Vector& w, const _Matrix& v,
	     const _Vector& b, _Vector& x);

/**
 *  Improves a solution vector x of the linear set A.x = b.
 *  The _Matrix a and the SV decomposition of a -- u, w, v and the
 *  right-hand side _Vector are input along with the solution vector x.
 *  The solution vector x is improved and modified on output.
 */
template<typename _Matrix, typename _Vector>
  void
  sv_improve(std::size_t n_rows, std::size_t n_cols,
	     const _Matrix& a, const _Matrix& u,
	     const _Vector& w, const _Matrix& v,
	     const _Vector& b, _Vector& x);

} // namespace matrix


#endif // MATRIX_SV_DECOMP_H
