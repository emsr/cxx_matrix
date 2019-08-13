#ifndef MATRIX_VANDERMONDE_TCC
#define MATRIX_VANDERMONDE_TCC 1

namespace matrix
{

/**
 *
 */
template<typename _Tp>
  void
  vandermonde(std::size_t n, const _Tp* x, const _Tp* q, _Tp* w)
  {
    if (n == 1)
      w[0] = q[0];
    else
      {
	std::vector<_Tp> c(n);
	c[n - 1] = -x[0];
	for (std::size_t i = 1; i < n; ++i)
	  {
	    auto xx = -x[i];
	    for (std::size_t j = n + 1 - i; j < n - 1; ++j)
	      c[j] += xx * c[j + 1];
	    c[n - 1] += xx;
	  }
	for (std::size_t i = 0; i < n; ++i)
	  {
	    auto xx = x[i];
	    auto t = _Tp{1};
	    auto b = _Tp{1};
	    auto s = q[n - 1];
	    for (std::ptrdiff_t j = n - 1; j >= 1; --j)
	      {
		b = c[j] + xx * b;
		s += b * q[j - 1];
		t = b + xx * t;
	      }
	    w[i] = s / t;
	  }
      }
  }

} // namespace matrix

#endif // MATRIX_VANDERMONDE_TCC
