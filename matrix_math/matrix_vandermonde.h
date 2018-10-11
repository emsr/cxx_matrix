#ifndef MATRIX_VANDERMONDE_H
#define MATRIX_VANDERMONDE_H 1

namespace matrix
{

template<typename _Tp>
  void
  vandermonde(std::size_t n, const _Tp* x, const _Tp* q, _Tp* w);

} // namespace matrix

#include "matrix_vandermonde.tcc"

#endif // MATRIX_VANDERMONDE_H
