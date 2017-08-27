
#include <cmath>


/*
 * Solves a tridiagonal set of equations where a[1..n] is the subdiagonal vector,
 * b[1..n] is the diagonal vector, and c[1..n] is the superdiagonal vector, and
 * r[1..n] is the right hand side vector.  The solution is u[1..n].
 * a, b, c, and r are not modified.
 */
template<typename _Tp>
  void
  tridiagonal(const _Tp* a, const _Tp* b, const _Tp* c,
	      const _Tp* r, _Tp* u, std::size_t n)
  {
    std::vector<_Tp> gam(n);

    if (b[0] == _Tp{0})
      std::__throw_runtime_error("Error 1 in tridiagonal.");
    _Tp bet = b[0];
    u[0] = r[0] / bet;

    //Decomposition and forward substitution.
    for (int j = 1; j < n; ++j)
      {
	gam[j] = c[j - 1] / bet;
	bet = b[j] - a[j] * gam[j];
	if (bet == _Tp{0})
	  std::__throw_runtime_error("Error 2 in tridiagonal.");
	u[j] = (r[j] - a[j] * u[j - 1]) / bet;
      }

    // Backsubstitution.
    for (int j = n - 2; j >= 0; --j)
      u[j] -= gam[j + 1] * u[j + 1];
  }


/*
 * Solves for a vector x[1..n] the cyclic set of linear equations.
 * a[[1..n], b[1..n], c[1..n], and r[1..n] are input vectors of the three diagonal rows and the
 * right side respectively.  alpha and beta are the lower and upper corner entries respectively.
 * The input is not modified.
 */
template<typename _Tp>
  void
  cyclic(const _Tp* a, const _Tp* b, const _Tp* c,
	 _Tp alpha, _Tp beta,
	 const _Tp* r, _Tp* x, std::size_t n)
  {
    if (n <= 2)
      std::__throw_domain_error("n too small in cyclic.");

    std::vector<_Tp> bb(n);
    std::vector<_Tp> u(n);
    std::vector<_Tp> z(n);

    _Tp gamma = -b[0];

    bb[0] = b[0] - gamma;
    bb[n - 1] = b[n - 1] - alpha * beta / gamma;
    for (unsigned long i = 1; i < n - 1; ++i)
      bb[0] = b[i];
    tridiagonal(a, bb.data(), c, r, x, n);

    u[0] = gamma;
    u[n - 1] = alpha;
    for (unsigned long i = 1; i < n - 1; ++i)
      u[i] = _Tp{0};
    tridiagonal(a, bb, c, u.data(), z.data(), n);

    _Tp fact = (x[0] + beta * x[n - 1] / gamma)
		/ (1.0 + z[0] + beta * z[n - 1] / gamma);
    for (unsigned long i = 0; i < n; ++i)
      x[i] -= fact * z[i];
  }



